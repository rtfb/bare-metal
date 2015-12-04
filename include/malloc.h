#ifndef MALLOC_H
#define MALLOC_H

#include "common.h"

void init_heap(uint32_t start, uint32_t end);
void* kmalloc(uint32_t nbytes);
void kfree(void *ptr);

// private
void print_heap_range();

#endif // MALLOC_H
