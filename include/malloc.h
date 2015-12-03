#ifndef MALLOC_H
#define MALLOC_H

#include "common.h"

void* kmalloc(uint32_t nbytes);
void kfree(void *ptr);

// private
void print_heap_range();

#endif // MALLOC_H
