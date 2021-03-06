/* debug.c - various debugging helpers */

#include "uart.h"
#include "common.h"
#include "asmcall.h"

#define MIN(x,y) (((x) < (y)) ? (x) : (y))

int dbg_parse_hex(char const* str) {
    while (*str && *str == '0') {
        ++str;
    }
    if (*str && (*str == 'x' || *str == 'X')) {
        ++str;
    }
    char const* start = str;
    while (*str && ((*str >= '0' && *str <= '9') ||
                    (*str >= 'a' && *str <= 'f') ||
                    (*str >= 'A' && *str <= 'F'))) {
        ++str;
    }
    if (str - start > 8) {
        return -1;
    }
    if (str - start == 0) {
        return -2;
    }
    --str;
    --start;
    int result = 0;
    int iter = 0;
    while (str != start) {
        int nibble = 0;
        if (*str >= '0' && *str <= '9') {
            nibble = *str - '0';
        } else if (*str >= 'a' && *str <= 'f') {
            nibble = *str - 'a' + 10;
        } else if (*str >= 'A' && *str <= 'F') {
            nibble = *str - 'A' + 10;
        } else {
            return -3; // Should never get here
        }
        result |= nibble << (iter * 4);
        ++iter;
        --str;
    }
    return result;
}

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
    uart_puts("0x");
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
    int addr = dbg_parse_hex(straddr);
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

uint32_t get_reg(char const* regname) {
    if (regname[0] == 'c' && regname[1] == 'p' && regname[2] == 's' && regname[3] == 'r') {
        return dumpreg_cpsr();
    }
    if (regname[0] == 'p' && regname[1] == 'c') {
        return dumpreg_pc();
    }
    if (regname[0] == 's' && regname[1] == 'p') {
        return dumpreg_sp();
    }
    if (regname[0] == 'l' && regname[1] == 'r') {
        return dumpreg_lr();
    }
    if (regname[0] == 'r') {
        switch (regname[1]) {
            case '1':
                if (regname[2] == '0') {
                    return dumpreg_r10();
                }
                if (regname[2] == '1') {
                    return dumpreg_r11();
                }
                if (regname[2] == '2') {
                    return dumpreg_r12();
                }
            case '0':
                return dumpreg_r0();
            case '2':
                return dumpreg_r2();
            case '3':
                return dumpreg_r3();
            case '4':
                return dumpreg_r4();
            case '5':
                return dumpreg_r5();
            case '6':
                return dumpreg_r6();
            case '7':
                return dumpreg_r7();
            case '8':
                return dumpreg_r8();
            case '9':
                return dumpreg_r9();
        }
    }
    return 0;
}

void inspect_reg(char const* regname) {
    puthexint(get_reg(regname));
}
