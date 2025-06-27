#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef STH_WITH_MIMALLOC
	#include <mimalloc.h>
	#include <mimalloc-override.h>
#else
	#include <stdlib.h>
#endif

#include <sth/malloc.h>
#include <sth/mutex.h>

#define LOG_PREFIX "[STH-MEM-DEBUG] %s:%d: "

#ifdef STH_MEM_DBG_LOG
	#define log_dbg(fmt, ...) fprintf(stderr, LOG_PREFIX fmt, __VA_ARGS__);
#else
	#define log_dbg(fmt, ...)
#endif

#define exit_if_null(ptr, msg) \
	do {	\
		if ((ptr) == NULL) {	\
			fprintf(stderr, LOG_PREFIX msg, __FILE__, __LINE__);	\
			exit(EXIT_FAILURE);	\
		}	\
	} while (0)

typedef struct allocation {
	void *ptr;
	size_t size;
	struct allocation *next, *prev;
	char *file;
	int line;
} allocation_t;

static struct {
	sth_mutex_t mu;
	allocation_t *head, *tail;
} meminfo;

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

void __sth_mem_dbg_init(void) {
	memset(&meminfo, 0, sizeof(meminfo));
	sth_mutex_init(&meminfo.mu);
}

void __sth_mem_dbg_destroy(void) {
	sth_mutex_destroy(&meminfo.mu);
	allocation_t *lhs = meminfo.head, *rhs = meminfo.head;
	while (1) {
		if (lhs == NULL)
			break;
		rhs = lhs->next;
		__sth_free(lhs->file);
		__sth_free(lhs);
		lhs = rhs;
	}
}

static void sth_mem_dbg_alloc_add(void *p, size_t size, __STH_MEM_DBG_PARAMS) {
	allocation_t *a = __sth_malloc(sizeof(*a));
	exit_if_null(a, "failed to allocate memory for memory debugger\n");

	*a = (allocation_t){
		.ptr  = p,
		.size = size,
		.file = sth_strdup(file),
		.line = line,
		.next = NULL,
		.prev = meminfo.tail,
	};
	exit_if_null(a->file, "failed to allocate memory for allocation file location");

	if (meminfo.head == NULL) {
		meminfo.head = a;
		meminfo.tail = a;
		return;
	}

	meminfo.tail->next = a;
	meminfo.tail = a;
}

static allocation_t *sth_mem_dbg_alloc_find(void *p) {
	allocation_t *tmp = meminfo.head;
	while (tmp) {
		if (tmp->ptr == p)
			break;
		tmp = tmp->next;
	}
	return tmp;
}

static int sth_mem_dbg_alloc_remove(void *p) {
	allocation_t *tmp = sth_mem_dbg_alloc_find(p);
	if (tmp == NULL)
		return 1;

	__sth_free(tmp->file);

	if (tmp->prev != NULL)
		tmp->prev->next = tmp->next;
	else
		meminfo.head = tmp->next;

	if (tmp->next != NULL)
		tmp->next->prev = tmp->prev;
	else
		meminfo.tail = tmp->prev;

	__sth_free(tmp);

	return 0;
}

static int sth_mem_dbg_alloc_change(void *old, void *new, size_t size,
									 __STH_MEM_DBG_PARAMS)
{
	allocation_t *tmp = sth_mem_dbg_alloc_find(old);
	if (tmp == NULL)
		return 1;

	__sth_free(tmp->file);

	tmp->ptr  = new;
	tmp->size = size;
	tmp->line = line;

	tmp->file = __sth_strdup(file);
	exit_if_null(tmp->file, "failed to allocate memory for allocation file location");

	return 0;
}

void __sth_mem_dbg_print(void) {
	sth_mutex_lock(&meminfo.mu);
	allocation_t *tmp = meminfo.head;
	while (tmp) {
		fprintf(stderr, LOG_PREFIX "(addr: %p) (size: %zu)\n",
		  tmp->file, tmp->line, tmp->ptr, tmp->size);
		tmp = tmp->next;
	}
	sth_mutex_unlock(&meminfo.mu);
}

void *__sth_malloc_dbg(size_t size, __STH_MEM_DBG_PARAMS) {
	sth_mutex_lock(&meminfo.mu);
	void *rp = __sth_malloc(size);
	log_dbg("sth_malloc(%zu) = %p\n", file, line, size, rp);
	sth_mem_dbg_alloc_add(rp, size, file, line);
	sth_mutex_unlock(&meminfo.mu);
	return rp;
}

void *__sth_calloc_dbg(size_t count, size_t size, __STH_MEM_DBG_PARAMS) {
	sth_mutex_lock(&meminfo.mu);
	void *rp = __sth_calloc(count, size);
	log_dbg("sth_calloc(%zu, %zu) = %p\n", file, line, count, size, rp);
	sth_mem_dbg_alloc_add(rp, size, file, line);
	sth_mutex_unlock(&meminfo.mu);
	return rp;
}

void *__sth_realloc_dbg(void *p, size_t size, __STH_MEM_DBG_PARAMS) {
	sth_mutex_lock(&meminfo.mu);
	void *rp = __sth_realloc(p, size);
	log_dbg("sth_realloc(%p, %zu) = %p\n", file, line, p, size, rp);
	sth_mem_dbg_alloc_change(p, rp, size, file, line);
	sth_mutex_unlock(&meminfo.mu);
	return rp;
}

void __sth_free_dbg(void *p, __STH_MEM_DBG_PARAMS) {
	(void)file; (void)line;
	sth_mutex_lock(&meminfo.mu);
	__sth_free(p);
	log_dbg("sth_free(%p)\n", file, line, p);
	sth_mem_dbg_alloc_remove(p);
	sth_mutex_unlock(&meminfo.mu);
}

#ifdef __cplusplus
}
#endif
