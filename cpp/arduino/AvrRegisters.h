#pragma once

#include <stdint.h>

// ADCSRA is defined in the CPU specific header files
// like iom328p.h.
// It is liked to _SFR_MEM8 what does not exists in the test environment.
// Therefore we define _SFR_MEM8 here and provide it a storage
// location so that the test code can read/write on it.
// While at it, also cover the 16 and 32 bit macros.

extern uint8_t  sfr8_store;
extern uint16_t sfr16_store;
extern uint32_t sfr32_store;

#define _SFR_MEM8(mem_addr)  sfr8_store
#define _SFR_MEM16(mem_addr) sfr16_store
#define _SFR_MEM32(mem_addr) sfr32_store

// Notice that the above will map every single 8 bit register defined
// with _SFR_MEM8 into the same memory location. For some testing that is
// perfectly fine, but if you for instance are testing that a function
// updates a register a certain way, that has a high chance of breaking
// when more than one register is being used.
//
// Long term it would be better to provide a separate storage for
// each register. In the real avr header files, _SFR_MEM8(mem_addr)
// is wrapping _MMIO_BYTE(mem_addr) which expands to volatile memory
// access of the memory address. The ADCSRA register might be defined as
// _SFR_MEM8(0x7A). Other registers use higher address values like 0x80B,
// so there is too much variation and too high values to use this as an
// index in an array.
//
// In theory it would be possible to use macro token concatination and
// generate unique variables like sfr_store_0X7A but it would fail for
// hex char casing differences (0x7a vs 0X7A) and would either require
// predeclaring a huge amount variables for all the possible addresses
// that could be used, or require some tool to parse the relevant header
// files and generate code.
//
// In my oppinion the most viable option to support distinct storage for
// each register would be to lazily create register storages as they are
// requested and store them in a linked list with the address as a key. So
// the _SFR_MEM8 macro would then wrap to a function ala
//
//     find_or_create_storage(mem_addr) {
//         node = find_in_list(mem_addr)
//         if (!node) {
//             node = create_node()
//             add_to_list(node)
//         }
//         return node
//     }

