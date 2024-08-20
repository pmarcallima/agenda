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

int byte_to_int(unsigned char *byte_array) {
  return (int)((byte_array[3] << 24 | byte_array[2] << 16 | byte_array[1] << 8 |
                byte_array[0]));
}
short byte_to_short(unsigned char *byte_array) {
  return (short)((byte_array[1] << 8 | byte_array[0]));
}
