#ifndef KQUEUEPOLLER_HPP_
#define KQUEUEPOLLER_HPP_

#include <sys/event.h>
#include <sys/time.h>

#include <iostream>

#include "MyUtils.hpp"

class KqueuePoller {
 public:  // no constructors & destructor & operators
  // functions
  void InitPoller(int server_socket_fd);
  void SafeKevent();
  struct kevent* GetCurrentEvent();
  void CheckErrorFlags(struct kevent* current_event);
  bool IsReadEvent(int16_t filter) const;
  bool IsWriteEvent(int16_t filter) const;

  void AddEvent(int socket_fd, int16_t filter);
  void DeleteEvent(int socket_fd, int16_t filter);
  void ClearChangeList(uintptr_t ident);

 private:
  // const
  static const int kNevents = 8;
  static const struct timespec kTimeoutEvent; // timeout
  // variables
  int kq_fd_;
  int cnt_event_;
  int idx_event_;
  struct kevent event_list_[kNevents];
  std::vector<struct kevent> change_list_;

  // functions
  void ClearChangeList();
  void AddChangeList(uintptr_t ident, int16_t filter, uint16_t flags,
                     uint32_t fflags, intptr_t data, void* udata);
};

#endif
