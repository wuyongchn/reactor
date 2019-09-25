#include "reactor/accept_handler.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "reactor/chat_handler.h"
#include "reactor/event_dispatcher.h"
#include "utils/logging.h"
#include "utils/net_ops.h"

namespace reactor {
void AcceptHandler::HandleEvents(const Events events, void* arg) {
  if (events & WATCH_READ) {
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addrlen = sizeof(clnt_addr);
    int clnt_sock =
        ::accept(handle_, (struct sockaddr*)&clnt_addr, &clnt_addrlen);
    if (clnt_sock == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return;
    } else if (clnt_sock == -1) {
      LOG_FATAL("accept() error, %s", strerror(errno));
    }
    net::SetNonBlocking(clnt_sock, true);
    event_dispatcher_->RegisterHandler(
        new ChatHandler(clnt_sock, event_dispatcher_), WATCH_READ);
  }
}
}  // namespace reactor