#ifndef REACTOR_NET_OPS_H
#define REACTOR_NET_OPS_H

#include "reactor/handle.h"
#include "utils/endpoint.h"

namespace reactor {
namespace net {

Handle CreateSocket();
Handle CreateNonBlockingSocket();
void Bind(const Handle handle, const EndPoint& local_side);
void Listen(const Handle handle, const int backlog);
int Connect(const Handle handle, const EndPoint& remote_side);
void SetNonBlocking(const Handle handle, bool on);
}  // namespace net

}  // namespace reactor

#endif