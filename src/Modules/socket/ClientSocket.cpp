#include "ClientSocket.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

ClientSocket::ClientSocket() {}   //  default constructor
ClientSocket::~ClientSocket() {}  //  destructor

//  operators ------------------------------------------------------------------

// Initiation
void ClientSocket::InitClientSocket(int server_socket_fd) {
  AcceptSocket(server_socket_fd);
  NonBlockSocket();
}

// private *********************************************************************

// functions
void ClientSocket::AcceptSocket(int server_socket_fd) {
  if ((socket_fd_ = accept(server_socket_fd, (struct sockaddr *)&sa_in_,
                           &socket_len_)) == -1)
    throw MyExceptions(MyExceptions::kSocket, "accept() fail");
}
