#ifndef REACTOR_EVENT_DISPATCHER_H
#define REACTOR_EVENT_DISPATCHER_H
#include <memory>
#include <unordered_map>
#include "reactor/event_handler.h"
#include "reactor/event_mask.h"
#include "reactor/handle.h"
#include "reactor/noncopyable.h"

namespace reactor {
class IOMultiplexer;
class EventDispatcher : NonCopyable {
 public:
  using HandleMap = std::unordered_map<Handle, EventHandler*>;

 public:
  EventDispatcher(IOMultiplexer* iomplexer);
  ~EventDispatcher();
  void RegisterHandler(EventHandler* handler, Events events);
  void UnregisterHandler(EventHandler* handler);
  void EventLoop();
  const HandleMap& registered_handlers() const { return registered_handlers_; }
  IOMultiplexer* iomplexer() const { return iomplexer_; }

 private:
  HandleMap registered_handlers_;
  IOMultiplexer* iomplexer_;
};
}  // namespace reactor
#endif