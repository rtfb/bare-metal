
/*
 * A simple unconditional branch to a specified address, presumably, to a newly
 * uploaded binary.
 */
.globl BRANCHTO
BRANCHTO:
    bx r0

/*
 * Call a function with address in r0, forward it a param in r1.
 */
.globl CALL_1
CALL_1:
    mov r10, r0
    mov r0, r1
    bx r10
    mov pc, lr

.globl bootstrap_decoded_binary
bootstrap_decoded_binary:
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

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

.globl irq_handler
irq_handler:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4
