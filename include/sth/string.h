#ifndef _STH_STRING_H_
#define _STH_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void *__sth_bzero_fast(void *dest, size_t n);
void *__sth_memcpy_fast(void *dest, const void *src, size_t n);
void *__sth_memmove_fast(void *dest, const void *src, size_t n);

size_t __sth_strlen(const char* s);
#define sth_bzero_fast(dest, n)			__sth_bzero_fast(dest, n)
#define sth_memcpy_fast(dest, src, n)	__sth_memcpy_fast(dest, src, n)
#define sth_memmove_fast(dest, src, n)	__sth_memmove_fast(dest, src, n)
#define sth_strlen(s)	__sth_strlen(s)

#ifdef __cplusplus
}
#endif

#endif // _STH_STRING_H_
