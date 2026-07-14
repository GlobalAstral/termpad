#pragma once

#include <stdlib.h>
#include <memory.h>

typedef struct string {
  char* buffer;
  size_t size;
  size_t capacity;
} string;

string newString(char* init);
string newStringFromChar(char ch);

int reallocate(string* this, size_t new_cap);
int insertString(string* this, string* other, size_t index);

void deleteString(string* s);
