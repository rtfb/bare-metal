/* debug.c - various debugging helpers */

#include <stdint.h>

#include "uart.h"
#include "strutil.h"

#define MIN(x,y) (((x) < (y)) ? (x) : (y))

char* hexbyte(uint8_t b, char *buff) {
    char const* hex = "0123456789abcdef";
    *buff = hex[b >> 4];
    ++buff;
    *buff = hex[b & 0x0f];
    ++buff;
    *buff = '\0';
    return buff - 2;
}

void puthexint(uint32_t i) {
    char buff[32];
    uart_puts(hexbyte(i >> 24, buff));
    uart_puts(hexbyte((i & 0x00ff0000) >> 16, buff));
    uart_puts(hexbyte((i & 0x0000ff00) >> 8, buff));
    uart_puts(hexbyte(i & 0x000000ff, buff));
}

void putint(int i) {
    char buff[32];
    uart_puts(hexbyte(i, buff));
    uart_putc(' ');
}

void puthexrun(uint8_t* ptr, int len) {
    uint8_t* ptr2 = ptr;
    int num = MIN(len, 8);
    for (int i = 0; i < num; ++i) {
        putint(*ptr++);
    }
    for (int i = num; i < 8; ++i) {
        uart_puts("   ");
    }
    uart_putc(' ');
    if (len > 8) {
        num = MIN(len - 8, 8);
        for (int i = 0; i < num; ++i) {
            putint(*ptr++);
        }
        for (int i = num; i < 8; ++i) {
            uart_puts("   ");
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            uart_puts("   ");
        }
    }
    uart_puts(" | ");
    num = MIN(len, 16);
    for (int i = 0; i < num; ++i) {
        if (*ptr2 > 31 && *ptr2 < 128) {
            uart_putc(*ptr2);
        } else {
            uart_putc('.');
        }
        ++ptr2;
    }
}

void inspect_memory(char const* straddr) {
    int addr = str_parse_int(straddr);
    uint8_t *ptr = (uint8_t*) addr;
    int num = 37;
    while (num > 0) {
        puthexint(addr);
        uart_puts(": ");
        puthexrun(ptr, MIN(num, 16));
        uart_puts(uart_newline);
        addr += 16;
        ptr += 16;
        num -= 16;
    }
}
