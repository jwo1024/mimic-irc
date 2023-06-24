#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "defines.hpp"
#include "MyUtils.hpp"

// Socket namespace: functions for socket operations
namespace Socket {

// functions
int CreateSocket();
void Bind(int port, const char *ip, int server_fd,
          struct sockaddr_in &server_addr);
void Bind(int port, int server_fd, struct sockaddr_in &server_addr);
void Listen(int server_fd);
int Accept(int server_fd);
void SetNonBlocking(int fd);
void SetBlocking(int fd);
void Connect(int fd, int port, const char *ip);
void Send(int fd, const char *message);
void Send(int fd, const std::string &message);
std::string Receive(int fd);
void Close(int fd);

}  // namespace Socket

#endif