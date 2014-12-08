// Build with:
// $ gcc -ggdb -std=c99 strutil.c tests/strutil_test.c -Iinclude
#include <stdio.h>
#include <assert.h>

#include "strutil.h"

int main() {
    assert(0 == str_startswith(NULL, NULL));
    assert(0 == str_startswith("foo", NULL));
    assert(0 == str_startswith(NULL, "foo"));
    assert(1 == str_startswith("foo", "foo"));
    assert(1 == str_startswith("fooz", "foo"));
    assert(0 == str_startswith("fooz", "goo"));
    assert(0 == str_startswith("foo", "fooz"));
    printf("OK\n");
}
