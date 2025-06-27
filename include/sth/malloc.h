#ifndef _STH_MEMORY_H_
#define _STH_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "debug.h"

void *__sth_malloc(size_t size);
void *__sth_calloc(size_t count, size_t size);
void *__sth_realloc(void *p, size_t size);
void  __sth_free(void *p);

char *__sth_strndup(const char *s, size_t n);
char *__sth_strdup(const char* s);
char *__sth_strndup_fast(const char *s, size_t n);
char *__sth_strdup_fast(const char *s);

void  __sth_mem_dbg_init(void);
void  __sth_mem_dbg_print(void);
void  __sth_mem_dbg_destroy(void);

void *__sth_malloc_dbg(size_t size, __STH_MEM_DBG_PARAMS);
void *__sth_calloc_dbg(size_t count, size_t size, __STH_MEM_DBG_PARAMS);
void *__sth_realloc_dbg(void *p, size_t size, __STH_MEM_DBG_PARAMS);
void  __sth_free_dbg(void *p, __STH_MEM_DBG_PARAMS);

char *__sth_strndup_dbg(const char *s, size_t n, __STH_MEM_DBG_PARAMS);
char *__sth_strdup_dbg(const char* s, __STH_MEM_DBG_PARAMS);
char *__sth_strndup_fast_dbg(const char *s, size_t n, __STH_MEM_DBG_PARAMS);
char *__sth_strdup_fast_dbg(const char *s, __STH_MEM_DBG_PARAMS);

#ifdef STH_MEM_DBG
	#define sth_mem_dbg_init()		__sth_mem_dbg_init()
	#define sth_mem_dbg_print()		__sth_mem_dbg_print()
	#define sth_mem_dbg_destroy()	__sth_mem_dbg_destroy()

	#define sth_malloc(size)		__sth_malloc_dbg(size, __FILE__, __LINE__)
	#define sth_calloc(count, size)	__sth_calloc_dbg(count, size, __FILE__, __LINE__)
	#define sth_realloc(p, size)	__sth_realloc_dbg(p, size, __FILE__, __LINE__)
	#define sth_free(p)				__sth_free_dbg(p, __FILE__, __LINE__)

	#define sth_strndup(s, n)	__sth_strndup_dbg(s, n, __FILE__, __LINE__)
	#define sth_strdup(s)		__sth_strdup_dbg(s, __FILE__, __LINE__)
	#define sth_strndup_fast(s, n)	__sth_strndup_fast_dbg(s, n, __FILE__, __LINE__)
	#define sth_strdup_fast(s)		__sth_strdup_fast_dbg(s, __FILE__, __LINE__)
#else
	#define sth_mem_dbg_init()
	#define sth_mem_dbg_print()
	#define sth_mem_dbg_destroy()

	#define sth_malloc(size)		__sth_malloc(size)
	#define sth_calloc(count, size)	__sth_calloc(count, size)
	#define sth_realloc(p, size)	__sth_realloc(p, size)
	#define sth_free(p)				__sth_free(p)

	#define sth_strndup(s, n)	__sth_strndup(s, n)
	#define sth_strdup(s)		__sth_strdup(s)
	#define sth_strndup_fast(s, n)	__sth_strndup_fast(s, n)
	#define sth_strdup_fast(s)		__sth_strdup_fast(s)
#endif // STH_MEM_DBG

#ifdef __cplusplus
}
#endif

#endif // _STH_MEMORY_H_
