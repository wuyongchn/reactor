#include "reactor/event_dispatcher.h"
#include <vector>
#include "reactor/io_multiplexer.h"
#include "utils/logging.h"

namespace reactor {
EventDispatcher::EventDispatcher(IOMultiplexer* iomplexer)
    : iomplexer_(iomplexer) {}

EventDispatcher::~EventDispatcher() {
  delete iomplexer_;
  for (auto& p : registered_handlers_) {
    ::close(p.first);
    delete p.second;
  }
}

void EventDispatcher::RegisterHandler(EventHandler* handler, Events events) {
  CHECK(handler != nullptr);
  Handle handle = handler->handle();
  if (registered_handlers_.insert(std::make_pair(handle, handler)).second) {
    if (iomplexer_->AddToRegistry(handle, events)) {
      LOG_FATAL("ResigterHandler Error");
    }
  } else {
    if (iomplexer_->UpdateInRegistry(handle, events)) {
      LOG_FATAL("UpdateInRegistry Error");
    }
  }
}

void EventDispatcher::UnregisterHandler(EventHandler* handler) {
  CHECK(handler != nullptr);
  if (iomplexer_->RemoveFromRegistry(handler->handle())) {
    LOG_FATAL("UnregisterHandler Error");
  }
  Handle handle = handler->handle();
  delete registered_handlers_[handle];
  registered_handlers_.erase(handle);
}

void EventDispatcher::EventLoop() {
  while (1) {
    std::vector<IOMultiplexer::ReadyEvents> readys = iomplexer_->WaitEvents();
    for (auto& ready : readys) {
      registered_handlers_[ready.handle]->HandleEvents(ready.events);
    }
  }
}

}  // namespace reactor