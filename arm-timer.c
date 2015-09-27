#include "arm-timer.h"

static volatile arm_timer_t* timer = (volatile arm_timer_t*)ARM_TIMER_BASE;

volatile arm_timer_t* arm_timer() {
    return timer;
}
