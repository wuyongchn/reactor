#ifndef REACTOR_LOGGING_H
#define REACTOR_LOGGING_H

#include <errno.h>
#include <stdarg.h>
#include <string.h>

namespace reactor {
enum LogLevel {
  LOG_INFO = 0,
  LOG_WARNING = 1,
  LOG_ERROR = 2,
  LOG_FATAL = 3,
  NUM_LEVELS = 4
};
typedef __gnuc_va_list va_list;
const int kBufferSize = 1024;

namespace internal {
LogLevel get_log_level();
void set_log_level(const LogLevel level);
void log_handler(const LogLevel level, const char* filename, const int lineno,
                 const char* fmt, ...);
}  // namespace internal

#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)
#define REACTOR_SET_LOG_LEVEL(level) \
  ::reactor::internal::set_log_level(::reactor::LOG_##level);
#define LOG(level, fmt, arg...)                                             \
  (::reactor::LOG_##level < ::reactor::internal::get_log_level())           \
      ? (void)0                                                             \
      : (::reactor::internal::log_handler(::reactor::LOG_##level, __FILE__, \
                                          __LINE__, fmt, ##arg))
#define LOG_IF(condition, level, fmt, arg...)       \
  !(condition) ? (void)0                            \
               : (::reactor::internal::log_handler( \
                     ::reactor::LOG_##level, __FILE__, __LINE__, fmt, ##arg))
#define LOG_INFO(fmt, arg...) LOG(INFO, fmt, ##arg)
#define LOG_WARNING(fmt, arg...) LOG(WARNING, fmt, ##arg)
#define LOG_ERROR(fmt, arg...) LOG(ERROR, fmt, ##arg)
#define LOG_FATAL(fmt, arg...) LOG(FATAL, fmt, ##arg)
#define LOG_ASSERT(x)                  \
  do {                                 \
    if (UNLIKELY(!(x))) {              \
      LOG_FATAL("ASSERT FAILED: " #x); \
    }                                  \
  } while (0)
#define CHECK(expression) \
  LOG_IF(!(expression), FATAL, "CHECK FAILED: " #expression)
#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_NE(a, b) CHECK((a) !(b))
#define CHECK_LT(a, b) CHECK((a) < (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))
#define CHECK_GT(a, b) CHECK((a) > (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))
}  // namespace reactor

#endif