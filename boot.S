/* boot.S - assembly startup code */

#include <arm.h>

// To keep this in the first portion of the binary.
.section ".text.boot"

// Make Start global.
.globl Start
Start:
    b reset_handler

irq_vectors:
    ldr pc, reset_addr
    ldr pc, undefined_addr
    ldr pc, swi_addr
    ldr pc, prefetch_addr
    ldr pc, data_abort_addr
    ldr pc, unused_addr
    ldr pc, irq_addr
    ldr pc, fiq_addr

reset_addr:      .word reset_handler
undefined_addr:  .word reset_handler
swi_addr:        .word swi_handler
prefetch_addr:   .word reset_handler
data_abort_addr: .word reset_handler
unused_addr:     .word reset_handler
irq_addr:        .word irq_handler
fiq_addr:        .word reset_handler

ram_size:        .word 0x8000000 // TODO: detect that, but for now pretend it's
                                 // 128megs
irq_stack_size:  .word 0x100000  // 1meg for each stack...
fiq_stack_size:  .word 0x100000
user_stack_size: .word 0x100000
svr_stack_size:  .word 0x100000
sys_stack_size:  .word 0x100000

reset_handler:
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)
    // CPSID: CPS=Change Processor State, ID=interrupt disable
    // if: i=IRQ interrupts, f=FIQ interrupts
    // #CPSR_MODE_SYSTEM: switch to SYS mode
    cpsid if, #CPSR_MODE_SYSTEM

    // Install interrupt vectors. Copy 16 words from 0x8000, where they are
    // stored in our binary, to 0x0000 where ARM calls them
    ldr     r0, =irq_vectors
    mov     r1, #0x0000
    ldmia   r0!, {r2-r9}
    stmia   r1!, {r2-r9}
    ldmia   r0!, {r2-r9}
    stmia   r1!, {r2-r9}

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0
    ldr r1, ram_size    // Top of memory
    mov sp, r1          // Set IRQ stack bottom to the very end of RAM (stack grows downward)

    // Subtract IRQ stack size
    ldr r2, irq_stack_size
    sbc r1, r1, r2

    // Similarly, set up FIQ stack:
    mov r0, #(CPSR_MODE_FIQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0
    mov sp, r1          // Set FIQ stack bottom below IRQ stack (stack grows downward)

    // Subtract FIQ stack size
    ldr r2, fiq_stack_size
    sbc r1, r1, r2

    // Similarly, set up stack for supervisor mode:
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0
    mov sp, r1          // Set SVR stack bottom below FIQ stack (stack grows downward)

    // Subtract SVR stack size
    ldr r2, svr_stack_size
    sbc r1, r1, r2

    // Finally, set the stack for ordinary system mode operation:
    mov r0, #(CPSR_MODE_SYSTEM | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0
    mov sp, r1          // Set SYS stack bottom below SVR stack (stack grows downward)

    // Subtract SYS stack size
    ldr r2, sys_stack_size
    sbc r1, r1, r2

    // Remember that the stack works its way down memory, our heap will work
    // it's way up memory toward the system stack. r1 now contains end of RAM,
    // minus all the stacks. Heap can grow from bss_end to r1. Leave core in
    // SYS mode.
    ldr r0, =_bss_end
    // r0 == start_of_heap
    // r1 == end_of_heap

    // Clear out bss.
    ldr r4, =_bss_start
    ldr r9, =_bss_end
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f

1:
    // store multiple at r4.
    stmia   r4!, {r5-r8}

    // If we are still below bss_end, loop.
2:
    cmp r4, r9
    blo 1b

    // The c-startup function which we never return from. This function will
    // initialise the ro data section (most things that have the const
    // declaration) and initialise the bss section variables to 0 (generally
    // known as automatics). It'll then call main, which should never return.
    bl      kernel_main
