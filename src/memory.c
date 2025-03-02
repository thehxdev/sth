#include <mimalloc.h>
#include <mimalloc-override.h>

#include <sth/memory.h>

typedef unsigned char byte_t;
typedef unsigned long long word_t;

#define WORD_SIZE             (sizeof(word_t) * 8u)
#define SHIFT_SIZE_BY         ((WORD_SIZE >> 5u) + 1u)
#define LAST_BYTES_ADDR_MASK  ((WORD_SIZE >> 3u) - 1u)

#define aligned_access_info(buff, n)                                        \
	word_t *buff##_word = (word_t*)buff;                                    \
	word_t *buff##_word_end = buff##_word + ((n) >> SHIFT_SIZE_BY);         \
	byte_t *buff##_byte = (byte_t*)buff##_word_end ;                        \
	byte_t *buff##_byte_end = buff##_byte + ((n) & LAST_BYTES_ADDR_MASK);


void *__sth_malloc(size_t size) {
	return malloc(size);
}

void *__sth_calloc(size_t count, size_t size) {
	return calloc(count, size);
}

void *__sth_realloc(void *p, size_t size) {
	return realloc(p, size);
}

void __sth_free(void *p) {
	free(p);
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
	return strndup(s, n);
}

char *__sth_strdup(const char *s) {
	return strdup(s);
}

size_t __sth_strlen(const char *s) {
	size_t c = 0;
	while (*s) {
		s++;
		c++;
	}
	return c;
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
