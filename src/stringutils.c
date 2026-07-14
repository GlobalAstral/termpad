#include <stringutils.h>

#define BASE_CAP 16

string newString(char* init) {
  if (!init)
    return (string) {
      .buffer = (char*)malloc(BASE_CAP),
      .capacity = BASE_CAP,
      .size = 0
    };

  size_t needed_cap = (strlen(init) + BASE_CAP - 1) / BASE_CAP;
  needed_cap *= BASE_CAP;
  char* buf = (char*)malloc(needed_cap);
  memcpy(buf, init, strlen(init));

  return (string) {
    .buffer = buf,
    .capacity = needed_cap,
    .size = strlen(init) + 1,
  };
}

string newStringFromChar(char ch) {
  char temp[] = {ch, 0};
  return newString(temp);
}

int reallocate(string* this, size_t new_cap) {
  if (this->capacity == new_cap)
    return 0;

  char* new_ptr = (char*) realloc(this->buffer, new_cap);
  if (!new_cap)
    return 1;
  this->capacity = new_cap;
  this->buffer = new_ptr;
  return 0;
}

int insertString(string* this, string* other, size_t index) {
  if (index > this->size)
    return 1;
  
  size_t needed_size = this->size + other->size - 1;
  if (this->capacity < needed_size) {
    size_t needed_cap = (needed_size + BASE_CAP - 1) / BASE_CAP;
    needed_cap *= BASE_CAP;
    if (reallocate(this, needed_cap))
      return 1;
  }

  if (index == this->size) {
    memcpy(this->buffer + this->size, other->buffer, other->size);
    this->size += other->size - 1;
    this->buffer[this->size-1] = 0;
    return 0;
  }

  memmove(this->buffer + index + other->size - 1, this->buffer + index, this->size - index);
  memcpy(this->buffer + index, other->buffer, other->size - 1);
  this->size += other->size - 1;
  this->buffer[this->size-1] = 0;
  return 0;
}

void deleteString(string* s) {
  free(s->buffer);
  free(s);
}
