#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sth/memory.h>

int main(void) {
	const char *s = "Hello World From sth!";
	char *tmp = sth_strdup_fast(s);
	assert(!strcmp(s, tmp));
	printf("ALL TESTS PASSED!\n");
	sth_free(tmp);
	return 0;
}
