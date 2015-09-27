#ifndef  _ARM_H_
#define  _ARM_H_

// From the ARM ARM (Architecture Reference Manual). Make sure you get the
// ARMv5 documentation which includes the ARMv6 documentation which is the
// correct processor type for the Broadcom BCM2835. The ARMv6-M manuals
// available on the ARM website are for Cortex-M parts only and are very
// different.
//
// See ARM section A2.2 (Processor Modes)
#define CPSR_MODE_USER         0x10
#define CPSR_MODE_FIQ          0x11
#define CPSR_MODE_IRQ          0x12
#define CPSR_MODE_SVR          0x13
#define CPSR_MODE_ABORT        0x17
#define CPSR_MODE_UNDEFINED    0x1B
#define CPSR_MODE_SYSTEM       0x1F

// See ARM section A2.5 (Program status registers)
#define CPSR_IRQ_INHIBIT       0x80
#define CPSR_FIQ_INHIBIT       0x40
#define CPSR_THUMB             0x20

#endif /* _ARM_H_ */
