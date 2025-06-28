#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <string.h>

#include <sth/malloc.h>
#include <sth/string.h>

typedef unsigned char byte_t;
typedef unsigned long long word_t;

#define WORD_SIZE				(sizeof(word_t) * 8u)
#define SHIFT_SIZE_BY			((WORD_SIZE >> 5u) + 1u)
#define LAST_BYTES_ADDR_MASK	((WORD_SIZE >> 3u) - 1u)

#define aligned_access_info(buf, n)									\
	word_t *buf##_word = (word_t*)buf;								\
	word_t *buf##_word_end = buf##_word + ((n) >> SHIFT_SIZE_BY);	\
	byte_t *buf##_byte = (byte_t*)buf##_word_end;					\
	byte_t *buf##_byte_end = buf##_byte + ((n) & LAST_BYTES_ADDR_MASK);

void *__sth_bzero_fast(void *dest, size_t n) {
	aligned_access_info(dest, n);
	while (dest_word != dest_word_end) {
		*dest_word = 0;
		dest_word++;
	}
	while (dest_byte != dest_byte_end) {
		*dest_byte = 0;
		dest_byte++;
	}
	return dest;
}

void *__sth_memcpy_fast(void *dest, const void *src, size_t n) {
	aligned_access_info(src, n);
	aligned_access_info(dest, n);
	(void)dest_byte_end;

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

void *__sth_memmove_fast(void *dest, const void *src, size_t n) {
	if (dest < src)
		return sth_memcpy_fast(dest, src, n);

	aligned_access_info(src, n);
	aligned_access_info(dest, n);

	dest_byte_end--;
	dest_word_end--;
	src_byte_end--;
	src_word_end--;

	while (src_byte < src_byte_end) {
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

char *__sth_strndup(const char *s, size_t n) {
	char *buf = sth_malloc(n + 1);
	if (buf == NULL)
		return NULL;
	buf[n] = 0;
	return memcpy(buf, s, n);
}

char *__sth_strdup(const char *s) {
	return sth_strndup(s, sth_strlen(s));
}

size_t __sth_strlen(const char *s) {
	const char *p = s;
	while (*p++);
	return (p-s-1);
}

char *__sth_strndup_fast(const char *s, size_t n) {
	if (!s || !n)
		return NULL;
	char *buf = sth_malloc(n + 1);
	if (!buf)
		return NULL;
	buf[n] = 0;
	return sth_memcpy_fast(buf, s, n);
}

char *__sth_strdup_fast(const char *s) {
	return sth_strndup_fast(s, sth_strlen(s));
}

char *__sth_strndup_dbg(const char *s, size_t n, __STH_MEM_DBG_PARAMS) {
	char *buf = __sth_malloc_dbg(n + 1, file, line);
	if (buf == NULL)
		return NULL;
	buf[n] = 0;
	return memcpy(buf, s, n);
}

char *__sth_strdup_dbg(const char *s, __STH_MEM_DBG_PARAMS) {
	return __sth_strndup_dbg(s, sth_strlen(s), file, line);
}

char *__sth_strndup_fast_dbg(const char *s, size_t n, __STH_MEM_DBG_PARAMS) {
	if (!s || !n)
		return NULL;
	char *buf = __sth_malloc_dbg(n + 1, file, line);
	if (!buf)
		return NULL;
	buf[n] = 0;
	return sth_memcpy_fast(buf, s, n);
}

char *__sth_strdup_fast_dbg(const char *s, __STH_MEM_DBG_PARAMS) {
	return __sth_strndup_fast_dbg(s, sth_strlen(s), file, line);
}

#ifdef __cplusplus
}
#endif
