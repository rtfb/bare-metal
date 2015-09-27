#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "rpi-base.h"

#define IRQ_CONTROLLER_BASE (PERIPHERALS_BASE + 0xb200)

// Bits in the Enable_Basic_IRQs register to enable various interrupts.
// See the BCM2835 ARM Peripherals manual, section 7.5
#define IRQ_BASIC_ARM_TIMER       (1 << 0)
#define IRQ_BASIC_ARM_MAILBOX     (1 << 1)
#define IRQ_BASIC_ARM_DOORBELL_0  (1 << 2)
#define IRQ_BASIC_ARM_DOORBELL_1  (1 << 3)
#define IRQ_BASIC_GPU_0_HALTED    (1 << 4)
#define IRQ_BASIC_GPU_1_HALTED    (1 << 5)
#define IRQ_BASIC_ACCESS_ERROR_1  (1 << 6)
#define IRQ_BASIC_ACCESS_ERROR_0  (1 << 7)

typedef struct {
    volatile uint32_t irq_basic_pending;
    volatile uint32_t irq_pending_1;
    volatile uint32_t irq_pending_2;
    volatile uint32_t fiq_control;
    volatile uint32_t enable_irqs_1;
    volatile uint32_t enable_irqs_2;
    volatile uint32_t enable_basic_irqs;
    volatile uint32_t disable_irqs_1;
    volatile uint32_t disable_irqs_2;
    volatile uint32_t disable_basic_irqs;
} irq_controller_t;

extern volatile irq_controller_t* irq_controller();

#endif // IRQ_H
