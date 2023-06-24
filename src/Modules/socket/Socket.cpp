#include "Socket.hpp"

// static variables ------------------------------------------------------------

socklen_t Socket::socket_len_ = sizeof(struct sockaddr_in);

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

Socket::Socket() {}  //  default constructor

Socket::~Socket() {
  if (close(socket_fd_) == -1)
    throw MyExceptions(MyExceptions::kSocket, "close() fail");
}  //  destructor

//  operators ------------------------------------------------------------------


// protected *******************************************************************

// functions

void Socket::NonBlockSocket() const {
  if (fcntl(socket_fd_, F_SETFL, O_NONBLOCK) == -1)
    throw MyExceptions(MyExceptions::kSocket, "fcntl() fail");
}

void Socket::SetSocketOption(int level, int option_name,
                             const int *option_value) const {
  socklen_t option_len = sizeof(*option_value);
  if (setsockopt(socket_fd_, level, option_name, option_value, option_len) ==
      -1)
    throw MyExceptions(MyExceptions::kSocket, "setsockopt() fail");
}
