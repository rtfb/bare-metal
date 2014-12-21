/* main.c - the entry point for the kernel */

#include <stdint.h>

#include <uart.h>
#include <timer.h>
#include "b64.h"
#include "strutil.h"

#define UNUSED(x) (void)(x)
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

const char halting[] = "\r\n*** system halting ***";
const char newline[] = "\r\n";
const char ready[] = "ready\r\n";
const char yoo[] = "yoo!";

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
    int num = MIN(len, 8);
    for (int i = 0; i < num; ++i) {
        putint(*ptr++);
    }
    if (len > 8) {
        uart_putc(' ');
        num = MIN(len - 8, 8);
        for (int i = 0; i < num; ++i) {
            putint(*ptr++);
        }
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
        uart_puts(newline);
        addr += 16;
        ptr += 16;
        num -= 16;
    }
}

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    char buff[256];
    uint32_t len = ARR_LEN(buff);
    char decodebuff[256];
    uint32_t decodebufflen = ARR_LEN(decodebuff);
    int status = 0;
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);

    uart_init();

    // Wait a bit
    Wait(1000000);

    uart_puts(ready);

    while (1) {
        status = uart_getln(buff, len);
        if (status == 0) {
            uart_puts(newline);
            if (str_startswith(buff, "b64 ")) {
                /*int bytes_decoded =*/ b64_decode(buff+4, decodebuff, decodebufflen);
                uart_puts(decodebuff);
            } else if (str_startswith(buff, "m ")) {
                inspect_memory(buff+2);
            } else if (str_startswith(buff, "icky")) {
                uart_puts(yoo);
            } else {
                int strlen = str_len(buff) - 1;
                int j = 0;
                for (; strlen != -1; --strlen, ++j) {
                    decodebuff[j] = buff[strlen];
                }
                decodebuff[j] = 0;
                uart_puts(decodebuff);
            }
        }
        uart_puts(newline);
    }

    // Wait a bit
    for(volatile int i = 0; i < 10000000; ++i) { }

    uart_puts(halting);
}
