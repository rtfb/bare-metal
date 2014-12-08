/* main.c - the entry point for the kernel */

#include <stdint.h>

#include <uart.h>
#include <timer.h>
#include "b64.h"
#include "strutil.h"

#define UNUSED(x) (void)(x)

const char halting[] = "\r\n*** system halting ***";
const char newline[] = "\r\n";
const char ready[] = "ready\r\n";
const char yoo[] = "yoo!";

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    char buff[256];
    uint32_t len = 256;
    char decodebuff[256];
    uint32_t decodebufflen = ARR_LEN(decodebuff);
    int status = 0;
    int strlen = 0;
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
            } else if (str_startswith(buff, "icky")) {
                uart_puts(yoo);
            } else {
                for (strlen = 0; buff[strlen] != 0; ++strlen);
                --strlen;
                int j = 0;
                for (; strlen != -1; --strlen, ++j) {
                    decodebuff[j] = buff[strlen];
                }
                decodebuff[j+1] = 0;
                uart_puts(decodebuff);
                for (strlen = 0; decodebuff[strlen] != '\0'; ++strlen) {
                    decodebuff[strlen] = '\0';
                }
                //uart_puts(buff);
            }
        }
        uart_puts(newline);
    }

    // Wait a bit
    for(volatile int i = 0; i < 10000000; ++i) { }

    uart_puts(halting);
}
