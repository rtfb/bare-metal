/* main.c - the entry point for the kernel */

#include <uart.h>
#include <timer.h>
#include "b64.h"
#include "strutil.h"
#include "debug.h"
#include "asmcall.h"
#include "common.h"
#include "mmio.h"
#include "led.h"
#include "arm-timer.h"
#include "irq.h"
#include "version.h"

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

volatile unsigned int icount;
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;

//-------------------------------------------------------------------------
//void __attribute__((interrupt("IRQ"))) c_irq_handler (void) {
void c_irq_handler (void) {
    arm_timer()->irq_clear = 1;
    icount++;
    if (icount & 1) {
        gpio[LED_GPCLR] = 1 << LED_GPIO_BIT;
    } else {
        gpio[LED_GPSET] = 1 << LED_GPIO_BIT;
    }
}

void setup_timer() {
    gpio[LED_GPFSEL] |= (1 << LED_GPFBIT);
    irq_controller()->enable_basic_irqs = IRQ_BASIC_ARM_TIMER;
    arm_timer()->load = 0x400;
    arm_timer()->control = ARM_TIMER_CTRL_23BIT
                         | ARM_TIMER_CTRL_ENABLE
                         | ARM_TIMER_CTRL_INT_ENABLE
                         | ARM_TIMER_CTRL_PRESCALE_256;
    enable_irq();
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
            } else if (str_startswith(buff, "r ")) {
                inspect_reg(buff+2);
            } else if (str_startswith(buff, "icky")) {
                uart_puts(yoo);
            } else if (str_startswith(buff, "freloc")) {
                char tmp[32];
                uint32_t tmplen = ARR_LEN(tmp);
                uint32_t func_len = ((uint32_t) str_parse_int) - ((uint32_t) str_len);
                mem_cpy((uint32_t)str_len, 0x30000, func_len);
                uart_puts(str_int_to_str(CALL_1(0x30000, "xyz"), tmp, tmplen));
            } else if (str_startswith(buff, "version")) {
                uart_puts(version);
                uart_puts("\r\n");
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

    uart_puts(halting);
}
