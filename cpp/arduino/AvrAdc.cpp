#include "AvrAdc.h"

// mock storage to allow access to ADCSRA and other registers accessed through the _SFR_MEM8/16/32 macros.
uint8_t  sfr8_store;
uint16_t sfr16_store;
uint32_t sfr32_store;

