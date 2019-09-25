#ifndef REACTOR_ACCEPT_HANDLER_H
#define REACTOR_ACCEPT_HANDLER_H
#include "reactor/event_handler.h"

namespace reactor {
class AcceptHandler : public EventHandler {
 public:
  AcceptHandler(const Handle handle, EventDispatcher* EventDispatcher)
      : EventHandler(handle, EventDispatcher) {}
  virtual void HandleEvents(const Events events, void* arg) override;
  private:
};
}  // namespace reactor

#endif