#ifndef BOT_HPP_
#define BOT_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "Message.hpp"
#include "MyUtils.hpp"
#include "Socket.hpp"
#include "defines.hpp"

class Bot {
  struct IRCServer {
    std::string ip_;
    int port_;
    std::string password_;
  };

 public:
  // constructors & destructor
  Bot();
  ~Bot();
  // functions
  void Start();
  void Stop(int exit_code = 0);

 protected:
 private:
  // variables
  int socket_fd_;
  sockaddr_in socket_addr_;
  std::string nick_;
  std::string user_name_;    // use kDefaultUserName
  std::string host_name_;    // use kDefaultHostName
  std::string server_name_;  // use kDefaultServerName
  std::string real_name_;    // use kDefaultRealName
  IRCServer server_;
  std::vector<std::string> channels_;
  // functions
  void AskUserForServerInfo();
  void AskUserForBotInfo();
  void ConnectToServer();

  // features
  void PrintBotMenu();
  void AskUserToJoinChannel();
  bool JoinChannel(const std::string &channel);
  void SendMessageToChannel(const std::string &channel,
                            const std::string &message);
  void HandlePing(Message &message);
  void HandlePRIVMSG(Message &message);
  int SetReadFDs(fd_set &read_fds);
  void HandleUserInput();
  void SendPingToServer();
  void SendCurrentTimeToChannels();
  void AskUserToExitChannel();
};

#endif