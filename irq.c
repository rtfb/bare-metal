#include "irq.h"

static volatile irq_controller_t* controller =
    (volatile irq_controller_t*)IRQ_CONTROLLER_BASE;

volatile irq_controller_t* irq_controller() {
    return controller;
}
