#ifndef REACTOR_EPOLL_H
#define REACTOR_EPOLL_H
#include <sys/epoll.h>
#include "reactor/io_multiplexer.h"

namespace reactor {
class Epoll : public IOMultiplexer {
 public:
  explicit Epoll(const int init_events);
  ~Epoll();
  virtual int AddToRegistry(const Handle handle, const Events events) override;
  virtual int RemoveFromRegistry(const Handle handle) override;
  virtual int UpdateInRegistry(const Handle handle, const Events events) override;
  virtual std::vector<ReadyEvents> WaitEvents() override;

 private:
  int epfd_;
  std::vector<epoll_event> ready_events_;
};
}  // namespace reactor

#endif