static void __test_memmove_fast_int_array(void) {
    size_t i;
    int src[5] = {1, 2, 3, 4, 5}, dest[5];

    sth_memmove_fast(dest, src, sizeof(src));
    for (i = 0; i < STH_ARRAY_LEN(dest); i++)
        STH_ASSERT(dest[i] == src[i]);
}

static void __test_memmove_fast_string(void) {
    char dest[128];
    const char src[] = "This is a test string!";

    sth_memmove_fast(dest, src, sizeof(src));
    dest[sizeof(src)] = '\0';

    STH_ASSERT(strcmp(dest, src) == 0);
}

void test_memmove_fast(void) {
    __test_memmove_fast_int_array();
    __test_memmove_fast_string();
}
