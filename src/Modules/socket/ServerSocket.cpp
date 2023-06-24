#include "ServerSocket.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------
ServerSocket::ServerSocket() {}   //  default constructor
ServerSocket::~ServerSocket() {}  //  destructor

// Initiation
void ServerSocket::InitServerSocket(uint32_t addr, unsigned short port) {
  const int option_value = static_cast<int>(true);

  CreateSocket(PF_INET, SOCK_STREAM);
  SetSocketOption(SOL_SOCKET, SO_REUSEADDR, &option_value);
  memset(&sa_in_, 0, sizeof(sa_in_));
  sa_in_.sin_family = AF_INET;
  sa_in_.sin_addr.s_addr = htonl(addr);
  sa_in_.sin_port = htons(port);
  BindSocket();
  ListenSocket();
  NonBlockSocket();
}

// private *********************************************************************

// functions
void ServerSocket::CreateSocket(int domain, int type) {
  if ((socket_fd_ = socket(domain, type, 0)) == -1)
    throw MyExceptions(MyExceptions::kSocket, "socket() fail");
}

void ServerSocket::BindSocket() const {
  if (bind(socket_fd_, (struct sockaddr *)&sa_in_, socket_len_) == -1)
    throw MyExceptions(MyExceptions::kSocket, "bind() fail");
}

void ServerSocket::ListenSocket() const {
  if (listen(socket_fd_, kBacklog) == -1)
    throw MyExceptions(MyExceptions::kSocket, "listen() fail");
}
