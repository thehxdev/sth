#include <stdio.h>

#define STH_IMPL
#include "../sth.h"

void test_int_array(void) {
    size_t i;
    int src[5] = {1, 2, 3, 4, 5}, dest[5];
    sth_memmove_fast(dest, src, sizeof(src));
    for (i = 0; i < STH_ARRAY_LEN(dest); i++)
        sth_assert(dest[i] == src[i]);
}

int main(void) {
    test_int_array();
}
