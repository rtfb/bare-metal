#include <asm.h>
#include <arm.h>

sys_stack_ptr:   .word 0x0

/*
 * A simple unconditional branch to a specified address, presumably, to a newly
 * uploaded binary.
 */
FUNC BRANCHTO
    bx r0

/*
 * Call a function with address in r0, forward it a param in r1.
 */
FUNC CALL_1
    mov r10, r0
    mov r0, r1
    bx r10
    mov pc, lr

FUNC bootstrap_decoded_binary
    mov r0, #0x20000 // src
    mov r1, #0x8000  // size
    mov r3, #0x8000  // dest
    .loop:
        ldrb  r2, [r0]
        strb  r2, [r3]
        add   r0, r0, #1
        add   r3, r3, #1
        subs  r1, r1, #1
        bne .loop

    mov r0, #0x8000
    bx  r0

// Take a process_t* in r0 and transfer control to it
FUNC switch_to_user_process
    // save state of system process in system stack
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    // switch to userland, inhibit FIQ, but allow IRQ:
    mov r1, #(CPSR_MODE_USER | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r1

    // store system's sp for later
    ldr r2, =sys_stack_ptr
    str sp, [r2]

    // set userlad sp from the process struct
    ldr sp, [r0, #8]

    // now here's a trick: process structure has a stack prepopulated so we can
    // bx lr here and we'll end up in the processes' code
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bx lr

FUNC sys_exit
    swi #13

FUNC enable_irq
    mrs r0, cpsr
    bic r0, r0, #CPSR_IRQ_INHIBIT
    msr cpsr_c, r0
    bx lr

FUNC irq_handler
    // Read processor state so we could later determine what mode we were in
    // when the interrupt occurred. r0 will be passed as param to irq_dispatch()
    mrs r0, cpsr

    sub lr, lr, #4
    srsdb #CPSR_MODE_SYSTEM!
    cpsid if, #CPSR_MODE_SYSTEM
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    // According to the document "Procedure Call Standard for the ARM
    // Architecture", the stack pointer is 4-byte aligned at all times, but it
    // must be 8-byte aligned when calling an externally visible function.
    // This is important because this code is reached from an IRQ and therefore
    // the stack currently may only be 4-byte aligned.  If this is the case,
    // the stack must be padded to an 8-byte boundary before calling
    // irq_dispatch().
    and r4, sp, #4
    sub sp, sp, r4
    bl irq_dispatch
    // Restore the original stack alignment (see note about 8-byte alignment
    // above).
    add sp, sp, r4
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    rfeia sp!

FUNC swi_handler
    // switch to systemland, inhibit FIQ, but allow IRQ:
    mov r1, #(CPSR_MODE_SYSTEM | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r1

    // restore system's sp
    ldr r0, =sys_stack_ptr
    ldr sp, [r0]

    // now when the stack is back, restore previously saved system state
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bx lr

FUNC dumpreg_r0
    bx lr

FUNC dumpreg_r1
    mov r0, r1
    bx lr

FUNC dumpreg_r2
    mov r0, r2
    bx lr

FUNC dumpreg_r3
    mov r0, r3
    bx lr

FUNC dumpreg_r4
    mov r0, r4
    bx lr

FUNC dumpreg_r5
    mov r0, r5
    bx lr

FUNC dumpreg_r6
    mov r0, r6
    bx lr

FUNC dumpreg_r7
    mov r0, r7
    bx lr

FUNC dumpreg_r8
    mov r0, r8
    bx lr

FUNC dumpreg_r9
    mov r0, r9
    bx lr

FUNC dumpreg_r10
    mov r0, r10
    bx lr

FUNC dumpreg_r11
    mov r0, r11
    bx lr

FUNC dumpreg_r12
    mov r0, r12
    bx lr

FUNC dumpreg_sp
    mov r0, sp
    bx lr

FUNC dumpreg_lr
    mov r0, lr
    bx lr

FUNC dumpreg_pc
    mov r0, pc
    bx lr

FUNC dumpreg_cpsr
    mrs r0, cpsr
    bx lr
