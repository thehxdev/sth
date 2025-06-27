#ifndef _STH_MUTEX_H_
#define _STH_MUTEX_H_

#ifdef __cplusplus
extern "C" {
#endif

// Code below copied from https://github.com/cgaebel/pipe

// The number of spins to do before performing an expensive kernel-mode context
// switch. This is a nice easy value to tweak for your application's needs. Set
// it to 0 if you want the implementation to decide, a low number if you are
// copying many objects into pipes at once (or a few large objects), and a high
// number if you are coping small or few objects into pipes at once.
#define STH_MUTEX_SPINS 8192

// Standard threading stuff. This lets us support simple synchronization
// primitives on multiple platforms painlessly.

#if defined(_WIN32) || defined(_WIN64) // use the native win32 API on windows

#include <windows.h>

// On vista+, we have native condition variables and fast locks. Yay.
#if defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x0600

#define sth_mutex_t             SRWLOCK

#define sth_mutex_init          InitializeSRWLock
#define sth_mutex_lock          AcquireSRWLockExclusive
#define sth_mutex_unlock        ReleaseSRWLockExclusive
#define sth_mutex_destroy(m)

#define sth_cond_t              CONDITION_VARIABLE

#define sth_cond_init           InitializeConditionVariable
#define sth_cond_signal         WakeConditionVariable
#define sth_cond_broadcast      WakeAllConditionVariable
#define sth_cond_wait(c, m)     SleepConditionVariableSRW((c), (m), INFINITE, 0)
#define sth_cond_destroy(c)

// Oh god. Microsoft has slow locks and lacks native condition variables on
// anything lower than Vista. Looks like we're rolling our own today.
#else // vista+

#define sth_mutex_t         CRITICAL_SECTION

#define sth_mutex_init(m)   InitializeCriticalSectionAndSpinCount((m), STH_MUTEX_SPINS)
#define sth_mutex_lock      EnterCriticalSection
#define sth_mutex_unlock    LeaveCriticalSection
#define sth_mutex_destroy   DeleteCriticalSection

// This Condition variable implementation is stolen from:
// http://www.cs.wustl.edu/~schmidt/win32-cv-1.html (section 3.3)

typedef struct sth_cond {
    // Count of the number of waiters, with a critical section to serialize
    // accesses to it.
    int waiters_count;
    CRITICAL_SECTION waiters_count_lock;

    // Number of threads to release via a cond_broadcast or a cond_signal.
    int release_count;

    // Keeps track of the current "generation" so that a single thread can't
    // steal all the resources from a broadcast.
    int wait_generation_count;

    // A manual-reset event that's used to block and release waiting threads.
    HANDLE event;
} sth_cond_t;

static void sth_cond_init(sth_cond_t *c)
{
    c->waiters_count = 0;
    InitializeCriticalSection(&c->waiters_count_lock);
    c->release_count = 0;
    c->wait_generation_count = 0;
    c->event = CreateEvent(NULL, true, false, NULL);
}

static void sth_cond_signal(sth_cond_t *c)
{
    EnterCriticalSection(&c->waiters_count_lock);
    if(c->waiters_count > c->release_count)
    {
        SetEvent(c->event);
        c->release_count++;
        c->wait_generation_count++;
    }
    LeaveCriticalSection(&c->waiters_count_lock);
}

static void sth_cond_broadcast(sth_cond_t *c)
{
    EnterCriticalSection(&c->waiters_count_lock);
    if(c->waiters_count > 0)
    {
        SetEvent(c->event);

        // Release all the threads in this generation.
        c->release_count = c->waiters_count;

        // Start a new generation.
        c->wait_generation_count++;
    }
    LeaveCriticalSection(&c->waiters_count_lock);
}

static void sth_cond_wait(sth_cond_t *c, sth_mutex_t *m)
{
    EnterCriticalSection(&c->waiters_count_lock);

    c->waiters_count++;

    int my_generation = c->wait_generation_count;

    LeaveCriticalSection(&c->waiters_count_lock);
    mutex_unlock(m);

    bool wait_done;

    do
    {
        WaitForSingleObject(c->event, INFINITE);

        EnterCriticalSection(&c->waiters_count_lock);
        int release_count = c->release_count;
        int wait_generation_count = c->wait_generation_count;
        LeaveCriticalSection(&c->waiters_count_lock);

        wait_done = release_count > 0
                 && wait_generation_count != my_generation;
    }
    while(!wait_done);

    mutex_lock(m);
    EnterCriticalSection(&c->waiters_count_lock);
    c->waiters_count--;
    int release_count = --c->release_count;
    LeaveCriticalSection(&c->waiters_count_lock);

    if(release_count == 0) // we're the last waiter
        ResetEvent(c->event);
}

static void sth_cond_destroy(sth_cond_t *c)
{
    DeleteCriticalSection(&c->waiters_count_lock);
    CloseHandle(c->event);
}

#endif // vista+

// Fall back on pthreads if we haven't special-cased the current OS.
#else

#include <pthread.h>

#define sth_mutex_t pthread_mutex_t
#define sth_cond_t  pthread_cond_t

#define sth_mutex_init(m)  pthread_mutex_init((m), NULL)

#define sth_mutex_lock     pthread_mutex_lock
#define sth_mutex_unlock   pthread_mutex_unlock
#define sth_mutex_destroy  pthread_mutex_destroy

#define sth_cond_init(c)   pthread_cond_init((c), NULL)
#define sth_cond_signal    pthread_cond_signal
#define sth_cond_broadcast pthread_cond_broadcast
#define sth_cond_wait      pthread_cond_wait
#define sth_cond_destroy   pthread_cond_destroy

#endif

#ifdef __cplusplus
}
#endif

#endif // _STH_MUTEX_H_
