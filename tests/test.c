#include <stdio.h>
#include <string.h>

#define STH_IMPL
#include "../sth.h"

#include "test_memmove_fast.c"

int main(void) {
    test_memmove_fast();
    puts("OK!");
}
