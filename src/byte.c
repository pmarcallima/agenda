#include <byte.h>
#include <stdlib.h>

unsigned char *int_to_byte(int integer) {
  unsigned char *byte_array =
      (unsigned char *)malloc(sizeof(unsigned char) * 4);
  byte_array[0] = (unsigned char)(integer & 0xFF);
  byte_array[1] = (unsigned char)((integer >> 8) & 0xFF);
  byte_array[2] = (unsigned char)((integer >> 16) & 0xFF);
  byte_array[3] = (unsigned char)((integer >> 24) & 0xFF);

  return byte_array;
}
unsigned char *short_to_byte(short number) {
  unsigned char *byte_array =
      (unsigned char *)malloc(sizeof(unsigned char) * 2);
  byte_array[0] = (unsigned char)(number & 0xFF);
  byte_array[1] = (unsigned char)((number >> 8) & 0xFF);

  return byte_array;
}
unsigned char *pointer_to_byte(__intptr_t pointer) {
  unsigned char *byte_array =
      (unsigned char *)malloc(sizeof(unsigned char) * 8);
  byte_array[0] = (unsigned char)(pointer & 0xFF);
  byte_array[1] = (unsigned char)((pointer >> 8) & 0xFF);
  byte_array[2] = (unsigned char)((pointer >> 16) & 0xFF);
  byte_array[3] = (unsigned char)((pointer >> 24) & 0xFF);
  byte_array[4] = (unsigned char)((pointer >> 32) & 0xFF);
  byte_array[5] = (unsigned char)((pointer >> 40) & 0xFF);
  byte_array[6] = (unsigned char)((pointer >> 48) & 0xFF);
  byte_array[7] = (unsigned char)((pointer >> 54) & 0xFF);

  return byte_array;
}

int byte_to_int(unsigned char *byte_array) {

  return (int)((byte_array[3] << 24 | byte_array[2] << 16 | byte_array[1] << 8 |
                byte_array[0]));
}
intptr_t byte_to_intptr(unsigned char *byte_array) {
  intptr_t value = 0;
  size_t size = sizeof(intptr_t);

  // Combine bytes into the intptr_t value
  for (size_t i = 0; i < size; i++) {
    value |= (intptr_t)byte_array[i] << (i * 8);
  }

  return value;
}
short byte_to_short(unsigned char *byte_array) {
  return (short)((byte_array[1] << 8 | byte_array[0]));
}
