#include "Socket.hpp"

int Socket::CreateSocket() {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to create socket.");
  }
  return socket_fd;
}

void Socket::Bind(int port, const char *ip, int socket_fd,
                  struct sockaddr_in &socket_addr) {
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_port = htons(port);
  if (ip == NULL) {
    socket_addr.sin_addr.s_addr = INADDR_ANY;
  } else {
    socket_addr.sin_addr.s_addr = inet_addr(ip);
  }
  if (bind(socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) ==
      -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to bind socket.");
  }
}

void Socket::Bind(int port, int socket_fd, struct sockaddr_in &socket_addr) {
  Bind(port, NULL, socket_fd, socket_addr);
}

void Socket::Listen(int socket_fd) {
  if (listen(socket_fd, kBacklog) == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to listen socket.");
  }
}

int Socket::Accept(int socket_fd) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  int client_fd =
      accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
  if (client_fd == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to accept socket.");
  }
  return client_fd;
}

void Socket::SetNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to get flags.");
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to set non-blocking.");
  }
}

void Socket::SetBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to get flags.");
  }
  if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to set blocking.");
  }
}

void Socket::Connect(int fd, int port, const char *ip) {
  struct sockaddr_in socket_addr;
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_port = htons(port);
  socket_addr.sin_addr.s_addr = inet_addr(ip);
  if (connect(fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to connect socket.");
  }
}

void Socket::Send(int fd, const char *message) {
  if (send(fd, message, strlen(message), 0) == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to send message.");
  }
}

void Socket::Send(int fd, const std::string &message) {
  Send(fd, message.c_str());
}

std::string Socket::Receive(int fd) {
  char buffer[kBufferSize];
  int bytes_read = recv(fd, buffer, kBufferSize, 0);
  if (bytes_read == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to receive message.");
  }
  return std::string(buffer, bytes_read);
}

void Socket::Close(int fd) {
  if (close(fd) == -1) {
    throw MyExceptions(MyExceptions::kSocket, "Failed to close socket.");
  }
}

// private *********************************************************************
