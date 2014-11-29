/* main.c - the entry point for the kernel */

#include <stdint.h>

#include <uart.h>
#include <timer.h>

#define UNUSED(x) (void)(x)

const char hello[] = "\r\nHello World\r\n";
const char _1[] = "\r\n1\r\n";
const char _2[] = "\r\n2\r\n";
const char _3[] = "\r\n3\r\n";
const char halting[] = "\r\n*** system halting ***";
const char newline[] = "\r\n";

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    char buff[256];
    uint32_t len = 256;
    int status = 0;
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);

    uart_init();

    // Wait a bit
    Wait(1000000);

    uart_puts(hello);

    Wait(1000000);
    uart_puts(_1);
    Wait(1000000);
    uart_puts(_2);
    Wait(1000000);
    uart_puts(_3);

    while (1) {
        status = uart_getln(buff, len);
        if (status == 0) {
            uart_puts(buff);
        }
        uart_puts(newline);
    }

    // Wait a bit
    for(volatile int i = 0; i < 10000000; ++i) { }

    uart_puts(halting);
}
