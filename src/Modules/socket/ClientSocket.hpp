#ifndef CLIENTSOCKET_HPP_
#define CLIENTSOCKET_HPP_

#include "Socket.hpp"

class ClientSocket : public Socket {
 public:
  // constructors & destructor : no meaning in this class
  ClientSocket();
  ~ClientSocket();
  // Initiation
  void InitClientSocket(int server_socket_fd);

 protected:
 private:
  // functions
  void AcceptSocket(int server_socket_fd);
};

#endif
