/* uart.h - UART initialization & communication */

#ifndef UART_H
#define UART_H

#include <stdint.h>

#define EPARAM -1
#define ENOMEM -2

extern const char uart_newline[];

/*
 * Initialize UART0.
 */
void uart_init();

/*
 * Transmit a byte via UART0.
 * uint8_t Byte: byte to send.
 */
void uart_putc(uint8_t byte);

/*
 * Receive a byte via UART0.
 *
 * Returns:
 * uint8_t: byte received.
 */
uint8_t uart_getc();

/*
 * print a string to the UART one character at a time
 * const char *str: 0-terminated string
 */
void uart_puts(const char *str);

int uart_getln(char *buff, uint32_t len);

#endif // #ifndef UART_H
