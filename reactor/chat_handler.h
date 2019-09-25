#ifndef REACTOR_CHAT_HANDLER_H
#define REACTOR_CHAT_HANDLER_H

#include "reactor/event_handler.h"
namespace reactor {
class ChatHandler : public EventHandler {
#define BUFFER_SIZE 1024
 public:
  ChatHandler(const Handle handle, EventDispatcher* EventDispatcher)
      : EventHandler(handle, EventDispatcher) {}
  virtual void HandleEvents(const Events events, void* arg) override;

 private:
  void HandleRead();
  void HandleWrite(void*arg);
  unsigned char buffer_[BUFFER_SIZE];
};
}  // namespace reactor

#endif