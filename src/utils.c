#include <utils.h>

int countlines(char* buffer) {
  int acc = 1;
  while (*buffer) {
    if (*buffer == '\n')
      acc++;
    buffer++;
  }
  return acc;
}
