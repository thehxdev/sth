#ifndef _STH_MEMORY_H_
#define _STH_MEMORY_H_

#include <stddef.h>

void *__sth_malloc(size_t size);
void *__sth_calloc(size_t count, size_t size);
void *__sth_realloc(void *p, size_t size);
void  __sth_free(void *p);

void *__sth_memcpy_fast(void *dest, const void *src, size_t n);
void *__sth_memmove_fast(void *dest, const void *src, size_t n);

size_t __sth_strlen(const char* s);
char *__sth_strndup(const char *s, size_t n);
char *__sth_strdup(const char* s);
char *__sth_strndup_fast(const char *s, size_t n);
char *__sth_strdup_fast(const char *s);

#define sth_malloc(size)		__sth_malloc(size)
#define sth_calloc(count, size)	__sth_calloc(size)
#define sth_realloc(p, size)	__sth_realloc(size)
#define sth_free(p)				__sth_free(p)

#define sth_memcpy_fast(dest, src, n)	__sth_memcpy_fast(dest, src, n)
#define sth_memmove_fast(dest, src, n)	__sth_memmove_fast(dest, src, n)

#define sth_strlen(s)		__sth_strlen(s)
#define sth_strndup(s, n)	__sth_strndup(s, n)
#define sth_strdup(s)		__sth_strdup(s)

#define sth_strndup_fast(s, n)	__sth_strndup_fast(s, n)
#define sth_strdup_fast(s)		__sth_strdup_fast(s)

#endif // _STH_MEMORY_H_