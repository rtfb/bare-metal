// Build with:
// $ gcc -ggdb -std=c99 strutil.c tests/strutil_test.c -Iinclude
#include <stdio.h>
#include <assert.h>

#define uint32_t unsigned long
#include "strutil.h"

int main() {
    assert(0 == str_startswith(NULL, NULL));
    assert(0 == str_startswith("foo", NULL));
    assert(0 == str_startswith(NULL, "foo"));
    assert(1 == str_startswith("foo", "foo"));
    assert(1 == str_startswith("fooz", "foo"));
    assert(0 == str_startswith("fooz", "goo"));
    assert(0 == str_startswith("foo", "fooz"));

    assert(0 == str_len(NULL));
    assert(0 == str_len(""));
    assert(1 == str_len("a"));
    assert(4 == str_len("fofo"));
    printf("OK\n");
}
