// Build with:
// $ gcc -ggdb -std=c99 debug.c tests/debug_test.c -Iinclude -Wno-int-to-pointer-cast
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "debug.h"

void uart_puts(char const* p) {
}

void uart_putc(char const c) {
}

int str_parse_int(char const* str) {
    return 0;
}

char const uart_newline[] = "\n";

int main() {
    assert(0x42 == dbg_parse_hex("42"));
    assert(0x42 == dbg_parse_hex("0x42"));
    assert(0xff == dbg_parse_hex("ff"));
    assert(0xff == dbg_parse_hex("fF"));
    assert(0xabcdef03 == dbg_parse_hex("0xaBcDeF03"));
    assert(-1 == dbg_parse_hex("0xaBcDeF037"));
    assert(0x13 == dbg_parse_hex("00000013"));
    assert(-2 == dbg_parse_hex("zoo"));

    printf("OK\n");
}
