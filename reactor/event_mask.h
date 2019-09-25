#ifndef REACTOR_EVENT_MASK_H
#define REACTOR_EVENT_MASK_H

namespace reactor {
enum EventMask { WATCH_READ = 0x01, WATCH_WRITE = 0x02, WATCH_ERROR = 0x04 };
}
typedef unsigned char Events;

#endif