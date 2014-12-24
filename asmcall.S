
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
