#ifndef _STH_H_
#define _STH_H_

#define STH_VERSION_MAJOR 0
#define STH_VERSION_MINOR 0
#define STH_VERSION_PATCH 1
#define STH_VERSION_STRING "0.0.1"

#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS (1)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

/*
 ************************************************************
 Declarations
 ***********************************************************
 */

#define STH_MALLOC(s)       malloc((s))
#define STH_CALLOC(n, s)    calloc((n), (s))
#define STH_REALLOC(p, s)   realloc((p), (s))
#define STH_FREE(p)         free((p))
#define STH_ASSERT(c)       assert((c))

#define STH_UNUSED(v)   ((void)(v))
#define STH_ARRAY_LEN(a) (sizeof((a)) / sizeof((a)[0]))

#define STH_TIME_SPENT_MS(__elapsed, __block) \
    do { \
        clock_t __clock_start, __clock_end; \
        __clock_start = clock(); \
        do { __block; } while (0); \
        __clock_end = clock(); \
        __elapsed = ((double)(__clock_end - __clock_start)) * 1000.0 / CLOCKS_PER_SEC; \
    } while (0)

// Branch prediction hints
#if defined(__clang__) || defined(__GNUC__)
    #define STH_EXPECT(expr, val) __builtin_expect((expr), (val))
#else
    #define STH_EXPECT(expr, val) (expr)
#endif
#define STH_LIKELY(expr)    STH_EXPECT(expr, 1)
#define STH_UNLIKELY(expr)  STH_EXPECT(expr, 0)


// Compiler specific macros
#if defined(__GNUC__) || defined(__clang__)
    #define STH_TRAP() __builtin_trap()
    #define STH_PACKED(__decl__) __decl__ __attribute__((packed))
#elif defined (_MSC_VER)
    #define STH_TRAP() __debugbreak()
    #define STH_PACKED(__decl__) __pragma(pack(push, 1)) __decl__ __pragma(pack(pop))
#else
    #define STH_TRAP() (*(volatile char*)0)
#endif


#if defined(NDEBUG)
    #define sth_assert(cond)
    #define sth_trap_assert(cond)
#else
    #define sth_assert(cond) \
        do { \
            if (!(cond)) { \
                fprintf(stderr, "%s(%d): sth_assert(" #cond ") failed\n", __FILE__, __LINE__); \
                abort(); \
            } \
        } while (0)

    #define sth_trap_assert(cond) \
        do { \
            if (!(cond)) { \
                fprintf(stderr, "%s(%d): sth_trap_assert(" #cond ") failed\n", __FILE__, __LINE__); \
                STH_TRAP(); \
            } \
        } while (0)
#endif

#define __sth_concat_(A,B) A##B
#define __sth_concat(A,B) __sth_concat_(A,B)
#define sth_static_assert(condition, id) \
    char __sth_concat(id, __LINE__)[ ((condition)) ? 1 : -1 ]

// Change byte order of a numeric value
#define sth_bswap16(x) ((((x) << 8) & 0xff00)  | (((x) >> 8) & 0x00ff))
#define sth_bswap32(x) (sth_bswap16(x) << 16 | sth_bswap16((x) >> 16))
#define sth_bswap64(x) (sth_bswap32(x) << 32 | sth_bswap32((x) >> 32))

#ifdef __cplusplus
    #define STH_DECLTYPE_CAST(T) (decltype(T))
#else
    #define STH_DECLTYPE_CAST(T)
#endif


/*
 * Every type with these fields can treated as a dynamic array:
 *  - T *items
 *  - size_t len
 *  - size_t cap
 */

#define STH_DA_INIT_CAP (256)

#define sth_da_init(da, _cap) \
    do { \
        (da)->len = 0; \
        (da)->cap = ((_cap) <= 0) ? STH_DA_INIT_CAP : (_cap); \
        (da)->items = STH_MALLOC(((da)->cap) * sizeof(*(da)->items)); \
        STH_ASSERT((da)->items); \
    } while (0)

#define sth_da_reserve(da, expected_cap) \
    do { \
        if ((expected_cap) > (da)->cap) { \
            if ((da)->cap == 0) { \
                (da)->cap = STH_DA_INIT_CAP; \
            } \
            while ((da)->cap < (expected_cap)) { \
                (da)->cap <<= 1; \
            } \
            (da)->items = STH_DECLTYPE_CAST((da)->items) STH_REALLOC((da)->items, (da)->cap * sizeof(*(da)->items)); \
            STH_ASSERT((da)->items); \
        } \
    } while (0)

#define sth_da_append(da, item) \
    do { \
        sth_da_reserve((da), (da)->len + 1); \
        memcpy(&(da)->items[(da)->len], item, sizeof(*(da)->items)); \
        (da)->len++; \
    } while (0)

#define sth_da_append_many(da, new_items, new_items_count) \
    do { \
        sth_da_reserve((da), (da)->len + (new_items_count)); \
        memcpy(&(da)->items[(da)->len], (new_items), (new_items_count) * sizeof(*(da)->items)); \
        (da)->len += (new_items_count); \
    } while (0)

#define sth_da_resize(da, new_size) \
    do { \
        sth_da_reserve((da), (new_size)); \
        (da)->len = (new_size); \
    } while (0)

#define sth_da_free(da) STH_FREE((da)->items)


void *sth_memcpy_fast(void *dest, const void *src, size_t n);
void *sth_memmove_fast(void *dest, const void *src, size_t n);


#ifdef STH_IMPL
/*
 ************************************************************
 Implementations
 ***********************************************************
 */

typedef unsigned long long __sth_word_t;

#define __STH_WORD_BIT_SIZE         (sizeof(__sth_word_t) << 3U)
#define __STH_SHIFT_N_BY            ((__STH_WORD_BIT_SIZE >> 5U) + 1U)
#define __STH_LAST_BYTES_ADDR_MASK  ((__STH_WORD_BIT_SIZE >> 3U) - 1U)

#define __sth_aligned_mem_info(__buffer, __n) \
    __sth_word_t *__buffer##_word = (__sth_word_t*) __buffer; \
    __sth_word_t *__buffer##_word_end = __buffer##_word + ((__n) >> __STH_SHIFT_N_BY); \
    uint8_t *__buffer##_byte = (uint8_t*) __buffer##_word_end; \
    uint8_t *__buffer##_byte_end = __buffer##_byte + ((__n) & __STH_LAST_BYTES_ADDR_MASK);


void *sth_memcpy_fast(void *dest, const void *src, size_t n) {
    __sth_aligned_mem_info(src, n);
    __sth_aligned_mem_info(dest, n);
    STH_UNUSED(dest_byte_end);

    while (src_word != src_word_end) {
        *dest_word = *src_word;
        dest_word++;
        src_word++;
    }
    while (src_byte != src_byte_end) {
        *dest_byte = *src_byte;
        dest_byte++;
        src_byte++;
    }

    return dest;
}

void *sth_memmove_fast(void *dest, const void *src, size_t n) {
    if (dest < src)
        return sth_memcpy_fast(dest, src, n);

    __sth_aligned_mem_info(src, n);
    __sth_aligned_mem_info(dest, n);

    dest_byte_end--;
    dest_word_end--;
    src_byte_end--;
    src_word_end--;

    while (src_byte <= src_byte_end) {
        *dest_byte_end = *src_byte_end;
        dest_byte_end--;
        src_byte_end--;
    }
    while (src_word <= src_word_end) {
        *dest_word_end = *src_word_end;
        dest_word_end--;
        src_word_end--;
    }

    return dest;
}
#endif // STH_IMPL

#ifdef __cplusplus
}
#endif

#endif // _STH_H_
