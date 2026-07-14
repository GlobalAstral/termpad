#include <logger.h>

static Logger logger;

void initLogger(char* fname) {
  FILE* f = fopen(fname, "w");
  if (f == NULL)
    logger = (Logger) { NULL, 1 };
  logger = (Logger) { f, 0 };
}

void __log(char* fmt, va_list list) {
  vfprintf(logger.f, fmt, list);
  fflush(logger.f);
}

void logformat(char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  __log(fmt, list);
  fprintf(logger.f, "\n");
  va_end(list);
}

int failsafe(int flag, char* init, char* success, char* fail, ...) {
  logformat(init);
  if (flag) {
    va_list list;
    va_start(list, fail);
    fprintf(logger.f, "\t");
    __log(fail, list);
    fprintf(logger.f, "\n");
    va_end(list);
    return 1;
  }
  fprintf(logger.f, "\t");
  logformat(success);
  return 0;
}

int loggerfail() {
  fclose(logger.f);
  return 1;
}

int loggerok() {
  fclose(logger.f);
  return 0;
}
