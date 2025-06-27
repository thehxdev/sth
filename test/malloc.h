#ifndef _MALLOC_H_
#define _MALLOC_H_

// Example of a wrapper header file to switch between libc memory allocation
// functions and sth provided ones.

#ifdef STH_MALLOC
	// #define STH_MEM_DBG
	#include <sth/malloc.h>

	#define malloc(s) sth_malloc(s)
	#define calloc(c, s) sth_calloc(c, s)
	#define realloc(p, s) sth_realloc(p, s)
	#define free(p) sth_free(p)

	#define strndup(s, n) sth_strndup(s, n)
	#define strdup(s) sth_strdup(s)

	#define strndup_fast(s, n) sth_strndup_fast(s, n)
	#define strdup_fast(s) sth_strdup_fast(s)
#else
	#include <stdlib.h>
#endif // STH_MALLOC

#endif // _MALLOC_H_
