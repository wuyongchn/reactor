#include "reactor/epoll.h"
#include <unistd.h>
#include "utils/logging.h"

namespace reactor {
Epoll::Epoll(const int init_events) {
  if ((epfd_ = ::epoll_create(1)) == -1) {
    LOG_FATAL("epoll_create() error, %s", strerror(errno));
  }
  ready_events_.resize(init_events);
}

Epoll::~Epoll() {
  if (epfd_ >= 0) {
    ::close(epfd_);
  }
}

int Epoll::AddToRegistry(const Handle handle, const Events events) {
  struct epoll_event ep_event = {0};
  ep_event.data.fd = handle;
  if (events & WATCH_READ) {
    ep_event.events |= EPOLLIN;
  }
  if (events & WATCH_WRITE) {
    ep_event.events |= EPOLLOUT;
  }
  if (events & WATCH_ERROR) {
    ep_event.events |= EPOLLERR;
  }
  return ::epoll_ctl(epfd_, EPOLL_CTL_ADD, handle, &ep_event);
}

int Epoll::UpdateInRegistry(const Handle handle, const Events events) {
  struct epoll_event ep_event = {0};
  ep_event.data.fd = handle;
  if (events & WATCH_READ) {
    ep_event.events |= EPOLLIN;
  }
  if (events & WATCH_WRITE) {
    ep_event.events |= EPOLLOUT;
  }
  if (events & WATCH_ERROR) {
    ep_event.events |= EPOLLERR;
  }
  return ::epoll_ctl(epfd_, EPOLL_CTL_MOD, handle, &ep_event);
}

int Epoll::RemoveFromRegistry(const Handle handle) {
  return ::epoll_ctl(epfd_, EPOLL_CTL_DEL, handle, NULL);
}

std::vector<Epoll::ReadyEvents> Epoll::WaitEvents() {
  int ready =
      ::epoll_wait(epfd_, ready_events_.data(), ready_events_.size(), 0);
  if (ready == -1) {
    return {};
  }
  std::vector<ReadyEvents> vec;
  for (int i = 0; i < ready; ++i) {
    Events events = 0;
    if (ready_events_[i].events & EPOLLIN) {
      events |= WATCH_READ;
    }
    if (ready_events_[i].events & EPOLLOUT) {
      events |= WATCH_WRITE;
    }
    if (ready_events_[i].events & EPOLLERR) {
      events |= WATCH_ERROR;
    }
    Handle handle = ready_events_[i].data.fd;
    vec.emplace_back(handle, events);
  }
  if (ready == ready_events_.size()) {
    ready_events_.resize(ready * 2);
  }
  return vec;
}

}  // namespace reactor