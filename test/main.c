#include <stdio.h>
#include <string.h>
#include <assert.h>

#define STH_MALLOC
#include "malloc.h"

int main(void) {
	sth_mem_dbg_init();

	int *x = malloc(sizeof(*x));

	const char s[] = "Hello World From sth!";
	char *tmp = strdup_fast(s);
	assert(strcmp(s, tmp) == 0);
	printf("tmp = %s\n", tmp);

	free(x);
	free(tmp);

	sth_mem_dbg_print();
	sth_mem_dbg_destroy();
	return 0;
}
