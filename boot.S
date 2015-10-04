/* boot.S - assembly startup code */

#include <arm.h>

// To keep this in the first portion of the binary.
.section ".text.boot"

// Make Start global.
.globl Start
Start:
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
swi_addr:        .word reset_handler
prefetch_addr:   .word reset_handler
data_abort_addr: .word reset_handler
unused_addr:     .word reset_handler
irq_addr:        .word irq_handler
fiq_addr:        .word reset_handler

/*
reset_handler:
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}


    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    mov sp,#0x4000

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0
    mov sp,#0x8000000

    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
    ;@mov r0,#0x53
    ;@msr cpsr_c, r0

    ;@bl notmain
    bl kernel_main
*/

reset_handler:
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)
    // CPSID: CPS=Change Processor State, ID=interrupt disable
    // if: i=IRQ interrupts, f=FIQ interrupts
    // #CPSR_MODE_SYSTEM: switch to SYS mode
    cpsid if, #CPSR_MODE_SYSTEM

    // Install interrupt vectors. Copy 16 words from 0x8000, where they are
    // stored in our binary, to 0x0000 where ARM calls them
    mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!, {r2-r9}
    stmia   r1!, {r2-r9}
    ldmia   r0!, {r2-r9}
    stmia   r1!, {r2-r9}

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    mov sp, #0x8000

    // Switch back to supervisor mode (our application mode) and
    // set the stack pointer towards the end of RAM. Remember that the
    // stack works its way down memory, our heap will work it's way
    // up memory toward the application stack.
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0

    // Set the stack pointer at the end of RAM.
    // Keep it within the limits and also keep it aligned to a 32-bit
    // boundary!
    mov     sp, #0x4000
    // mov     sp, #(128 * (1024 * 1024)) - 4
    //mov     sp, #(256 * (1024 * 1024)) - 4
    //mov     sp, #(512 * (1024 * 1024)) - 4

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

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

irq_handler:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4
