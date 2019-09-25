#include "utils/logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

namespace reactor {
namespace internal {
enum TextColor {
  ANSI_BLACK = 30,
  ANSI_RED,
  ANSI_GREEN2,
  ANSI_YELLOW,
  ANSI_BLUE,
  ANSI_MAGENTA,
  ANSI_CYAN,
  ANSI_WHITE
};
static LogLevel reactor_log_level = ::reactor::LOG_INFO;
static const char* levelnames[] = {"INFO", "WARNING", "ERROR", "FATAL"};
static const TextColor text_color[] = {ANSI_WHITE, ANSI_YELLOW, ANSI_RED,
                                       ANSI_RED};
static char buf[kBufferSize];

LogLevel get_log_level() { return reactor_log_level; }
void set_log_level(const LogLevel level) { reactor_log_level = level; }
void default_log_handler(const LogLevel level, const char* filename,
                         const int lineno, const char* fmt, va_list ap) {
  vsnprintf(buf, kBufferSize, fmt, ap);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  struct tm* p = localtime(&tv.tv_sec);
  fprintf(stdout, "\033[1;%dm%c%d%02d%02d %02d%02d%02d.%06ld %s:%d] %s\n\033[0m",
          text_color[level], levelnames[level][0], 1900 + p->tm_year,
          1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec,
          tv.tv_usec, filename, lineno, buf);
  fflush(stdout);
  if (level > ::reactor::LOG_ERROR) {
    abort();
  }
}
void log_handler(const LogLevel level, const char* filename, const int lineno,
                 const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  default_log_handler(level, strrchr(filename, '/') + 1, lineno, fmt, ap);
  va_end(ap);
}
}  // namespace internal
}  // namespace reactor