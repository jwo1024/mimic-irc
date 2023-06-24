#include "KqueuePoller.hpp"

// public **********************************************************************
//  no constructors & destructor & operators -----------------------------------

const struct timespec KqueuePoller::kTimeoutEvent = {60, 0};

// functions
void KqueuePoller::InitPoller(int server_socket_fd) {
  if ((kq_fd_ = kqueue()) == -1)
    throw MyExceptions(MyExceptions::kKqueuePoller, "kqueue() fail");
  AddChangeList(server_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
}

void KqueuePoller::SafeKevent() {
  cnt_event_ = kevent(kq_fd_, &change_list_[0], change_list_.size(),
                      event_list_, kNevents, &kTimeoutEvent);
  if (cnt_event_ == -1)
    throw MyExceptions(MyExceptions::kKqueuePoller, "kevent() fail");
  ClearChangeList();
  idx_event_ = 0;
}

struct kevent* KqueuePoller::GetCurrentEvent() {
  struct kevent* current_event = NULL;

  if (idx_event_ < cnt_event_) current_event = &event_list_[idx_event_++];
  return (current_event);
}

void KqueuePoller::CheckErrorFlags(struct kevent* current_event) {
  if (current_event->flags & EV_ERROR) {
    throw MyExceptions(MyExceptions::kKqueuePoller, "EV_ERROR occur");
    // maybe we need more error handling here
  }
}

bool KqueuePoller::IsReadEvent(int16_t filter) const {
  return (filter == EVFILT_READ);
}

bool KqueuePoller::IsWriteEvent(int16_t filter) const {
  return (filter == EVFILT_WRITE);
}

void KqueuePoller::AddEvent(int socket_fd, int16_t filter) {
  AddChangeList(socket_fd, filter, EV_ADD, 0, 0, NULL);
}

void KqueuePoller::DeleteEvent(int socket_fd, int16_t filter) {
  AddChangeList(socket_fd, filter, EV_DELETE, 0, 0, NULL);
}

// private *********************************************************************
// functions -------------------------------------------------------------------

void KqueuePoller::AddChangeList(uintptr_t ident, int16_t filter,
                                 uint16_t flags, uint32_t fflags, intptr_t data,
                                 void* udata) {
  struct kevent new_event;

  EV_SET(&new_event, ident, filter, flags, fflags, data, udata);
  change_list_.push_back(new_event);
}

void KqueuePoller::ClearChangeList(uintptr_t ident) {
  for (std::vector<struct kevent>::iterator it = change_list_.begin();it != change_list_.end();) {
    if (it->ident == ident)
      it = change_list_.erase(it);
    else {
      ++it;
    }
  }
}
void KqueuePoller::ClearChangeList() { change_list_.clear(); }
