#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "MyUtils.hpp"

class Socket {
 public:
  // constructors & destructor
  Socket();
  Socket(const Socket &src);
  virtual ~Socket() = 0;
  // operators
  Socket &operator=(const Socket &rhs);
  // getters & setters

  // getters & setters
  int GetSocketFD() const { return socket_fd_; }
  unsigned short GetPort() const { return sa_in_.sin_port; }
  uint32_t GetIP() const { return sa_in_.sin_addr.s_addr; }
  std::string GetIPString() const { return inet_ntoa(sa_in_.sin_addr); }

 protected:
  // static variables
  static socklen_t socket_len_;
  // variables
  int socket_fd_;
  struct sockaddr_in sa_in_;
  // functions
  void NonBlockSocket() const;
  void SetSocketOption(int level, int option_name,
                       const int *option_value) const;

 private:
};

#endif
