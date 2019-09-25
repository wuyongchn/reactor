#include "utils/endpoint.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace reactor {
namespace net {
int Str2IP(const char* ip_str, ip_t& ip) {
  while (isspace(*ip_str)) {
    ++ip_str;
  }
  if (inet_pton(AF_INET, ip_str, &ip)) {  // ip is big endian
    return 0;
  }
  return -1;
}

IPStr IP2Str(const ip_t ip) {  // ip is big endian
  IPStr ipstr;
  if (inet_ntop(AF_INET, &ip, ipstr.buf, INET_ADDRSTRLEN) == NULL) {
    return IP2Str(IP_NONE);
  }
  return ipstr;
}

struct sockaddr_in EndPoint::ToSockaddrIn() const {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr = this->ip;
  addr.sin_port = this->port;
  return addr;
}

struct sockaddr EndPoint::ToSockaddr() const {
  struct sockaddr_in addr = ToSockaddrIn();
  return *static_cast<struct sockaddr*>(static_cast<void*>(&addr));
}

EndPointStr EndPoint2Str(const EndPoint& point) {
  // default, point.ip and point.port is big endian
  EndPointStr point_str;
  if (inet_ntop(AF_INET, &point.ip, point_str.buf, INET_ADDRSTRLEN) == NULL) {
    return EndPoint2Str(EndPoint(IP_NONE, 0));
  }
  char* buf = point_str.buf + strlen(point_str.buf);
  snprintf(buf, PORT_STRLEN, ":%d", ntohs(point.port));
  return point_str;
}

int Str2EndPoint(const char* str, EndPoint& point) {
  char buf[64];
  size_t i = 0;
  for (; i < sizeof(buf) && str[i] != '\0' && str[i] != ':'; ++i) {
    buf[i] = str[i];
  }
  if (i > sizeof(buf) || str[i] != ':') {
    return -1;
  }
  buf[i] = '\0';
  if (Str2IP(buf, point.ip)) {
    return -1;
  }
  ++i;
  char* end = NULL;
  in_port_t tmp = strtol(str + i, &end, 10);
  point.port = htons(tmp);
  if (end == str + i) {
    return -1;
  } else if (*end) {
    for (++end; isspace(*end); ++end) {
    }
    if (*end) {
      return -1;
    }
  }
  if (point.port < 0 || point.port > 65535) {
    return -1;
  }
  return 0;
}

int Str2EndPoint(const char* ip_str, const in_port_t port, EndPoint& point) {
  if (Str2IP(ip_str, point.ip)) {
    return -1;
  }
  if (port < 0 || port > 65535) {
    return -1;
  }
  point.port = htons(port);
  return 0;
}
}  // namespace net
}  // namespace reactor
