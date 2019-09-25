#ifndef REACTOR_NONCOPYABLE_H
#define REACTOR_NONCOPYABLE_H

namespace reactor {

class NonCopyable {
 protected:
  NonCopyable() = default;
  virtual ~NonCopyable() = default;

  NonCopyable(const NonCopyable& rhs) = delete;
  NonCopyable& operator=(const NonCopyable& rhs) = delete;
};
}  // namespace reactor

#endif