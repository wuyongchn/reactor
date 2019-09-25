#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "reactor/accept_handler.h"
#include "reactor/epoll.h"
#include "reactor/event_dispatcher.h"
#include "reactor/handle.h"
#include "utils/logging.h"
#include "utils/net_ops.h"

#define INIT_EVENTS 10
using namespace std;
using namespace reactor;

Handle CreateAndListen(in_port_t port);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    LOG_FATAL("Usage %s <port>", argv[0]);
  }
  Handle serv_handle = CreateAndListen(atoi(argv[1]));
  EventDispatcher EventDispatcher(new Epoll(INIT_EVENTS));
  EventDispatcher.RegisterHandler(
      new AcceptHandler(serv_handle, &EventDispatcher), WATCH_READ);
  EventDispatcher.EventLoop();
  return 0;
}

Handle CreateAndListen(in_port_t port) {
  Handle handle = net::CreateNonBlockingSocket();
  net::EndPoint local_size(net::IP_ANY, htons(port));
  net::Bind(handle, local_size);
  net::Listen(handle, 10);
  return handle;
}