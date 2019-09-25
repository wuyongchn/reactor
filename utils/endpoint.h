#ifndef REACTOR_ENDPOINT_H
#define REACTOR_ENDPOINT_H

#include <netinet/in.h>

namespace reactor {
namespace net {

#define PORT_STRLEN 16
typedef struct in_addr ip_t;
const ip_t IP_ANY = {INADDR_ANY};
const ip_t IP_NONE = {INADDR_NONE};

inline in_addr_t IP2Int(ip_t ip) { return ip.s_addr; }
inline ip_t Int2IP(in_addr_t ip_value) { return {ip_value}; }

struct IPStr {
  const char* c_str() const { return buf; }
  char buf[INET_ADDRSTRLEN];
};

int Str2IP(const char* ip_str, ip_t& ip);
IPStr IP2Str(const ip_t ip);

struct EndPoint {
  /// user ensures that passed value is big (or network) endian
  EndPoint() : ip(IP_ANY), port(0) {}
  EndPoint(ip_t ip2, in_port_t port2) : ip(ip2), port(port2) {}
  explicit EndPoint(const struct sockaddr_in& in)
      : ip(in.sin_addr), port(in.sin_port) {}
  struct sockaddr_in ToSockaddrIn() const;
  struct sockaddr ToSockaddr() const;
  ip_t ip;
  in_port_t port;
};

struct EndPointStr {
  const char* c_str() const { return buf; }
  char buf[INET_ADDRSTRLEN + PORT_STRLEN];
};

EndPointStr EndPoint2Str(const EndPoint& point);
int Str2EndPoint(const char* ip_and_port, EndPoint& point);
int Str2EndPoint(const char* ip, const in_port_t port, EndPoint& point);

}  // namespace net
}  // namespace reactor

#endif