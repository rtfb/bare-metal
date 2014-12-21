// Build with:
// $ gcc -ggdb -std=c99 strutil.c tests/strutil_test.c -Iinclude
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "strutil.h"
#include "b64.h" // for ARR_LEN

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

    assert(42 == str_parse_int("42"));
    assert(13 == str_parse_int("13"));
    assert(42 == str_parse_int("42x"));
    assert(-42 == str_parse_int("-42"));
    assert(42 == str_parse_int("+42"));
    assert(42 == str_parse_int("   +42"));
    assert(42 == str_parse_int("   +42x666"));
    assert(42 == str_parse_int("   +42    "));
    assert(42 == str_parse_int("   +42    666"));

    char buff[32];
    assert(!strcmp(str_int_to_str(42, buff, ARR_LEN(buff)), "42"));
    assert(!strcmp(str_int_to_str(666, buff, ARR_LEN(buff)), "666"));
    assert(!strcmp(str_int_to_str(-666, buff, ARR_LEN(buff)), "-666"));
    assert(!strcmp(str_int_to_str(0, buff, ARR_LEN(buff)), "0"));
    assert(!strcmp(str_int_to_str(1, buff, ARR_LEN(buff)), "1"));

    printf("OK\n");
}
