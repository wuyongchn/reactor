#ifndef REACTOR_MULTIPLEXER_H
#define REACTOR_MULTIPLEXER_H

#include <vector>
#include "reactor/event_mask.h"
#include "reactor/handle.h"
#include "reactor/noncopyable.h"

namespace reactor {
class IOMultiplexer : NonCopyable {
 public:
  struct ReadyEvents {
    ReadyEvents(const Handle hl, const Events ev) : handle(hl), events(ev) {}
    Handle handle;
    Events events;
  };

 public:
  virtual int AddToRegistry(const Handle handle, const Events events) = 0;
  virtual int UpdateInRegistry(const Handle handle, const Events events) = 0;
  virtual int RemoveFromRegistry(const Handle handle) = 0;
  virtual std::vector<ReadyEvents> WaitEvents() = 0;
};
}  // namespace reactor
#endif