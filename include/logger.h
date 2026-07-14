#pragma once

#include <stdio.h>
#include <stdarg.h>

typedef struct Logger {
  FILE* f;
  int err;
} Logger;

void initLogger(char* fname);
void logformat(char* fmt, ...);
int failsafe(int flag, char* init, char* success, char* fail, ...);
int loggerfail();
int loggerok();
