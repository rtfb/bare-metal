#include "malloc.h"
#include "debug.h"
#include "uart.h"

uint32_t end_of_heap = 0;
uint32_t start_of_heap = 0;

void init_heap(uint32_t start, uint32_t end) {
    start_of_heap = start;
    end_of_heap = end;
}

void* kmalloc(uint32_t nbytes) {
    if (start_of_heap + nbytes >= end_of_heap)
        return NULL;
    uint32_t temp = start_of_heap;
    start_of_heap += nbytes;
    return (void*)temp;
}

void kfree(void *ptr) {
    UNUSED(ptr);
    // TODO: does nothing for now
}

void print_heap_range() {
    uart_puts("Beginning of heap: 0x");
    puthexint(start_of_heap);
    uart_puts(uart_newline);
    uart_puts("End of heap: 0x");
    puthexint(end_of_heap);
    uart_puts(uart_newline);
}
