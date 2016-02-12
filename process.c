#include "process.h"
#include "malloc.h"

static uint32_t next_pid = 1;

uint32_t get_next_pid() {
    return next_pid++;
}

process_t* new_process(process_entry_point_t func) {
    uint32_t user_stack_size = 0x100000; // XXX: this constant is already defined in boot.S, reuse
    process_t *p = kmalloc(sizeof(process_t));
    void *stack = kmalloc(user_stack_size);
    uint32_t *sp = (uint32_t*)(stack) + user_stack_size/4;
    p->stack_bottom = sp;
    p->pid = get_next_pid();

    void *env = NULL;
    //void *exit_fn = NULL;

    //*(--sp) = 0x00000010;             // CPSR (user mode with interrupts enabled)
    *(--sp) = (uint32_t)func;         // 'return' address (i.e. where we come in)
    *(--sp) = 0x0c0c0c0c;             // r12
    *(--sp) = 0x0b0b0b0b;             // r11
    *(--sp) = 0x0a0a0a0a;             // r10
    *(--sp) = 0x09090909;             // r9
    *(--sp) = 0x08080808;             // r8
    *(--sp) = 0x07070707;             // r7
    *(--sp) = 0x06060606;             // r6
    *(--sp) = 0x05050505;             // r5
    *(--sp) = 0x04040404;             // r4
    *(--sp) = 0x03030303;             // r3
    *(--sp) = 0x02020202;             // r2
    *(--sp) = 0x01010101;             // r1
    *(--sp) = (uint32_t)env;          // r0, i.e. arg to entry function

    /*
    if ((uint32_t)sp & 0x07) {
        *(--sp) = 0xdeadc0de;         // Stack filler
        *(--sp) = (uint32_t)exit_fn;  // lr, where we go on exit
        *(--sp) = 0x00000004;         // Stack Adjust
    } else {
        *(--sp) = (uint32_t)exit_fn;  // lr, where we go on exit
        *(--sp) = 0x00000000;         // Stack Adjust
    }
    */

    p->stack_ptr = sp;
    return p;
}
