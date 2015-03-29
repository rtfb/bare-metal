/* main.c - the entry point for the kernel */

#include <uart.h>
#include <timer.h>
#include "b64.h"
#include "strutil.h"
#include "debug.h"
#include "asmcall.h"
#include "common.h"
#include "mmio.h"

#define UNUSED(x) (void)(x)

const char halting[] = "\r\n*** system halting ***";
const char ready[] = "ready\r\n";
const char yoo[] = "yoo!";

void mem_cpy(uint32_t from, uint32_t to, uint32_t len) {
    uint8_t *p_from = (uint8_t*) from;
    uint8_t *p_to = (uint8_t*) to;
    for (int i = 0; i < len; ++i) {
        *p_to++ = *p_from++;
    }
}

extern void enable_irq ( void );


#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CLI 0x2000B40C
#define ARM_TIMER_RIS 0x2000B410
#define ARM_TIMER_MIS 0x2000B414
#define ARM_TIMER_RLD 0x2000B418
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420

#define SYSTIMERCLO 0x20003004
#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028

#define IRQ_BASIC 0x2000B200
#define IRQ_PEND1 0x2000B204
#define IRQ_PEND2 0x2000B208
#define IRQ_FIQ_CONTROL 0x2000B210
#define IRQ_ENABLE_BASIC 0x2000B218
#define IRQ_DISABLE_BASIC 0x2000B224

volatile unsigned int icount;

//-------------------------------------------------------------------------
void __attribute__((interrupt("IRQ"))) c_irq_handler (void) {
    mmio_write(ARM_TIMER_CLI, 1);
    icount++;
    if (icount & 1) {
        mmio_write(GPCLR0, 1 << 16);
        uart_puts("LED OFF");
        uart_puts(uart_newline);
    } else {
        mmio_write(GPSET0, 1 << 16);
        uart_puts("LED ON");
        uart_puts(uart_newline);
    }
    //mmio_write(ARM_TIMER_CLI,0);
}

void setup_timer() {
    uart_puts("1");
    mmio_write(GPFSEL1, 1 << 18);
    uart_puts("2");
    mmio_write(IRQ_ENABLE_BASIC, 1 << 0);
    uart_puts("3");
    mmio_write(ARM_TIMER_LOD, 0x400);
    uart_puts("4");
    mmio_write(ARM_TIMER_CTL, (1 << 1) | (1 << 7) | (1 << 5) | (2 << 2));
    uart_puts("5");
    enable_irq();
    uart_puts("6");
}

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    char *buff = (char*) 0x10000;
    uint32_t len = 0x20000 - 0x10000;
    char *decodebuff = (char*) 0x20000;
    uint32_t decodebufflen = 0x30000 - 0x20000;
    int status = 0;
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);

    uart_init();

    // Wait a bit
    Wait(1000000);

    uart_puts(ready);

    setup_timer();

    while (1) {
        status = uart_getln(buff, len);
        if (status == 0) {
            uart_puts(uart_newline);
            if (str_startswith(buff, "b64 ")) {
                uint32_t bytes_decoded = b64_decode(buff+4, decodebuff, decodebufflen);
                uart_puts("base64 decoded #bytes: ");
                char tmp[32];
                uint32_t tmplen = ARR_LEN(tmp);
                uart_puts(str_int_to_str(bytes_decoded, tmp, tmplen));
                uart_puts(uart_newline);
                // Copy the code of bootstrap_decoded_binary somewhere safe.
                uint32_t func_len = 64; // wild guess
                mem_cpy((uint32_t)bootstrap_decoded_binary, 0x30000, func_len);
                // Call bootstrap_decoded_binary from that safe location
                BRANCHTO(0x30000);
            } else if (str_startswith(buff, "m ")) {
                inspect_memory(buff+2);
            } else if (str_startswith(buff, "icky")) {
                uart_puts(yoo);
            } else if (str_startswith(buff, "freloc")) {
                char tmp[32];
                uint32_t tmplen = ARR_LEN(tmp);
                uint32_t func_len = ((uint32_t) str_parse_int) - ((uint32_t) str_len);
                mem_cpy((uint32_t)str_len, 0x30000, func_len);
                uart_puts(str_int_to_str(CALL_1(0x30000, "xyz"), tmp, tmplen));
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
        uart_puts(uart_newline);
    }

    // Wait a bit
    for(volatile int i = 0; i < 10000000; ++i) { }

    uart_puts(halting);
}
