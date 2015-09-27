#ifndef ARM_TIMER_H
#define ARM_TIMER_H

#include <stdint.h>
#include "rpi-base.h"

#define ARM_TIMER_BASE (PERIPHERALS_BASE + 0xb400)

#define ARM_TIMER_CTRL_23BIT        (1 << 1)
#define ARM_TIMER_CTRL_PRESCALE_1   (0 << 2)
#define ARM_TIMER_CTRL_PRESCALE_16  (1 << 2)
#define ARM_TIMER_CTRL_PRESCALE_256 (2 << 2)
#define ARM_TIMER_CTRL_INT_ENABLE   (1 << 5)
#define ARM_TIMER_CTRL_INT_DISABLE  (0 << 5)
#define ARM_TIMER_CTRL_ENABLE       (1 << 7)
#define ARM_TIMER_CTRL_DISABLE      (0 << 7)

typedef struct {
    volatile uint32_t load;
    volatile uint32_t value;
    volatile uint32_t control;
    volatile uint32_t irq_clear;
    volatile uint32_t raw_irq;
    volatile uint32_t masked_irq;
    volatile uint32_t reload;
    volatile uint32_t pre_divider;
    volatile uint32_t counter;
} arm_timer_t;

extern volatile arm_timer_t* arm_timer();

#endif // ARM_TIMER_H
