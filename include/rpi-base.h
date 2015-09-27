#ifndef RPI_BASE_H
#define RPI_BASE_H

#ifdef RPI_2
    #define PERIPHERALS_BASE 0x3F000000UL
#else
    #define PERIPHERALS_BASE 0x20000000UL
#endif

#endif // RPI_BASE_H
