#ifndef REACTOR_EVENT_HANDLER_H
#define REACTOR_EVENT_HANDLER_H

#include <unistd.h>
#include "reactor/event_mask.h"
#include "reactor/handle.h"
#include "utils/logging.h"

namespace reactor {
class EventDispatcher;
class EventHandler {
 public:
  EventHandler(const Handle handle, EventDispatcher* EventDispatcher)
      : handle_(handle), event_dispatcher_(EventDispatcher) {}
  ~EventHandler() {
    if (handle_ >= 0) {
      ::close(handle_);
    }
  }
  Handle handle() const { return handle_; }
  EventDispatcher* event_dispatcher() const { return event_dispatcher_; }
  virtual void HandleEvents(Events events, void* arg = nullptr) = 0;

 protected:
  Handle handle_;
  EventDispatcher* event_dispatcher_;
};
}  // namespace reactor

#endif