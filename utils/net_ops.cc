#include "utils/net_ops.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "utils/logging.h"

namespace reactor {
namespace net {
int CreateSocket() {
  Handle handle = ::socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
  if (handle == -1) {
    LOG_FATAL("socket() system call error, %s", strerror(errno));
  }
  return handle;
}

int CreateNonBlockingSocket() {
  Handle handle =
      ::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (handle == -1) {
    LOG_FATAL("socket() system call error, %s", strerror(errno));
  }
  return handle;
}

void Bind(const Handle handle, const EndPoint& local_side) {
  struct sockaddr addr = local_side.ToSockaddr();
  if (::bind(handle, &addr, sizeof(addr)) == -1) {
    EndPointStr str = EndPoint2Str(local_side);
    LOG_FATAL("failed to bind socket %d to the adress %s, %s", handle,
              str.c_str(), strerror(errno));
  }
}

void Listen(const Handle handle, const int backlog) {
  if (::listen(handle, backlog) == -1) {
    LOG_FATAL("listen() systen error, %s", strerror(errno));
  }
}

int Connect(const Handle handle, const EndPoint& remote_side) {
  struct sockaddr addr = remote_side.ToSockaddr();
  return ::connect(handle, &addr, sizeof(addr));
}

void SetNonBlocking(int sockfd, bool on) {
  int flag = fcntl(sockfd, F_GETFL, 0);
  if (on) {
    flag |= O_NONBLOCK;
  } else {
    flag &= ~O_NONBLOCK;
  }
  if (flag == -1 || fcntl(sockfd, F_SETFL, flag) == -1) {
    LOG_ERROR("fcntl() error, %s", sockfd, strerror(errno));
  }
}

}  // namespace net
}  // namespace reactor