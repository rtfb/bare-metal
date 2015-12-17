#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"

typedef void(*process_entry_point_t)(void * environment);

typedef struct process_s {
    uint32_t pid;
    void *stack_bottom; // bottom of the stack; but since stack grows downward,
                        // it's actually the end of a buffer allocated for
                        // the stack
    void *stack_ptr;    // current stack top
} process_t;

process_t* new_process(process_entry_point_t func);
extern void switch_to_user_process(process_t *p); // implemented in asmcall.S
extern void sys_exit(uint32_t status); // implemented in asmcall.S

#endif // PROCESS_H
