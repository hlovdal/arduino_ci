/*
   This header file defines the macros required for the production
   code for AVR CPUs to declare ISRs in the test environment.
   See for more details
   https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
*/
#pragma once

#include "arduino_ci/AvrRegisters.h"

// Macros for disable/enable/return from interrupts
#define cli()
#define sei()
#define reti()

// Allows the production code to define an ISR method.
// These definitions come from the original avr/interrupt.h file
// https://www.nongnu.org/avr-libc/user-manual/interrupt_8h_source.html
#define _VECTOR(N) __vector_ ## N
#define ISR(vector, ...)            \
     extern "C" void vector (void)  __VA_ARGS__; \
     void vector (void)

// Possible arguments to the ISR macro
#define ISR_BLOCK
#define ISR_NOBLOCK
#define ISR_NAKED
#define ISR_ALIASOF(v)

