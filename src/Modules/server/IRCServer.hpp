#ifndef IRCSERVER_HPP_
#define IRCSERVER_HPP_

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "IRCServerManager.hpp"
#include "KqueuePoller.hpp"
#include "MyUtils.hpp"
#include "ServerSocket.hpp"
#include "Command.hpp"

class Client;
class Channel;
class IRCServerManager;

class IRCServer {
  friend class IRCServerManager;

 public:
  // constructors & destructor
  IRCServer(int port, const std::string& password);
  ~IRCServer();
  // static functions
  static int CheckPort(const char* port);
  static std::string CheckPassword(const char* password);
  // functions
  void Set();
  void Run();
  void ManageReadEvent(struct kevent* current_event);
  void ManageWriteEvent(struct kevent* current_event);
  void AddWriteEventOfClient(int client_fd) { poller_.AddEvent(client_fd, EVFILT_WRITE);}

  // getters & setters
  const ServerSocket& GetServerSocket() const { return server_socket_; }
  const std::map<std::string, Channel*>& GetChannels() const {
    return channels_;
  }
  const std::map<int, Client*>& GetClients() const { return clients_; }

  const std::string& GetPassword() const { return password_; }
  IRCServerManager& GetServerManager() const { return *manager_; }
  const std::string& GetServerName() const { return kServerName; }
  const std::string& GetServerVersion() const { return kServerVersion; }
  const std::string& GetServerInitTime() const { return kServerInitTime; }
  const std::string& GetUserModes() const { return kUserModes; }
  const std::string& GetChanModes() const { return kChanModes; };

 private:
  // constants
  const static int kPortMin = 0;
  const static int kPortMax = 65535;
  const static int kIrcPortDefault = 6667;
  const static int kPasswordMinLength = 4;
  const static int kPasswordMaxLength = 32;
  const static int kEventMax = 100;
  const static int kBufferSize = 1024;
  const static int kMaxClients = 100;
  const static int kMaxChannels = 100;
  const static time_t kTimeout = 120; // 120 ?
  const static std::string kServerName;
  const static std::string kServerVersion;
  const static std::string kServerInitTime;
  const static std::string kUserModes;
  const static std::string kChanModes;
  // variables
  static int server_count_;
  int port_;
  std::string password_;
  ServerSocket server_socket_;
  KqueuePoller poller_;
  IRCServerManager* manager_;

  std::map<std::string, Channel*> channels_;  // channel name, channel pointer
  std::map<int, Client*> clients_;            // client fd, Client pointer

  void CheckTimeout();
};

#endif
