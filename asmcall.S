#include <asm.h>

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

FUNC enable_irq
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

FUNC irq_handler
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

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
