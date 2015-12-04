#ifndef ASMCALL_H
#define ASMCALL_H

extern void BRANCHTO(unsigned int);
extern uint32_t CALL_1(uint32_t addr, char const* param);
extern void bootstrap_decoded_binary();
extern void enable_irq();
extern uint32_t dumpreg_r0();
extern uint32_t dumpreg_r1();
extern uint32_t dumpreg_r2();
extern uint32_t dumpreg_r3();
extern uint32_t dumpreg_r4();
extern uint32_t dumpreg_r5();
extern uint32_t dumpreg_r6();
extern uint32_t dumpreg_r7();
extern uint32_t dumpreg_r8();
extern uint32_t dumpreg_r9();
extern uint32_t dumpreg_r10();
extern uint32_t dumpreg_r11();
extern uint32_t dumpreg_r12();
extern uint32_t dumpreg_sp();
extern uint32_t dumpreg_lr();
extern uint32_t dumpreg_pc();
extern uint32_t dumpreg_cpsr();

#endif
