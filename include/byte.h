#ifndef BYTE_H
#define BYTE_H
#include <stdint.h>
unsigned char *int_to_byte(int integer);

unsigned char *short_to_byte(short number);

unsigned char *pointer_to_byte(intptr_t pointer);

int byte_to_int(unsigned char *byte_array);

intptr_t byte_to_intptr(unsigned char *byte_array);

short byte_to_short(unsigned char *byte_array);

#endif // BYTE_H
