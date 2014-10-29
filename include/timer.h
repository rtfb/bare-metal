#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void GetSystemTimerBase();

/*
* GetTimeStamp gets the current timestamp of the system timer, and returns it
* in registers r0 and r1, with r1 being the most significant 32 bits.
* C++ Signature: u64 GetTimeStamp()
*/
uint64_t GetTimeStamp();

/*
* Wait waits at least a specified number of microseconds before returning.
* The duration to wait is given in r0.
* C++ Signature: void Wait(u32 delayInMicroSeconds)
*/
void Wait(uint32_t microseconds);

#endif // #ifndef TIMER_H
