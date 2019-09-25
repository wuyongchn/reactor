#include "reactor/chat_handler.h"
#include <unistd.h>
#include "reactor/event_dispatcher.h"
#include "utils/logging.h"

namespace reactor {
void ChatHandler::HandleEvents(const Events events, void* arg) {
  if (events & WATCH_READ) {
    HandleRead();
  } else if (events & WATCH_WRITE) {
    HandleWrite(arg);
  }
}
void ChatHandler::HandleRead() {
  int recv_cnt = 0, nr;
  while ((nr = ::read(handle_, buffer_ + recv_cnt, BUFFER_SIZE - recv_cnt)) >
         0) {
    recv_cnt += nr;
  }
  if (recv_cnt > 0) {
    buffer_[recv_cnt] = 0;
    for (auto& p : event_dispatcher_->registered_handlers()) {
      if (p.first != handle_) {
        p.second->HandleEvents(WATCH_WRITE, (void*)buffer_);
      }
    }
  }
  if (nr == 0) {
    EventDispatcher* tmp = event_dispatcher_;
    tmp->UnregisterHandler(this);
  }
}

void ChatHandler::HandleWrite(void* arg) {
  const char* ptr = (const char*)arg;
  int left = strlen(ptr), nw, written = 0;
  while ((nw = ::write(handle_, ptr + written, left)) > 0) {
    left -= nw;
    written += nw;
  }
  if (nw == 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  }
  if (nw == -1) {
    LOG_FATAL("write error %d, %s", nw, strerror(errno));
  }
}
}  // namespace reactor