#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <iostream>
#include <map>
#include <string>

#include "Client.hpp"
#include "IRCServer.hpp"
#include "IRCServerManager.hpp"
#include "MyUtils.hpp"
#include "Socket.hpp"
#include "defines.hpp"

class Client;

class Channel {
  friend class IRCServerManager;

 public:
  // enums
  enum Type { kPublic, kPrivate, kSecret };
  enum Mode {
    kModeI = 1 << 0,  // i: Set/remove Invite-only channel
    kModeT = 1 << 1,  // t: Set/remove the restrictions of the TOPIC command to
    kModeK = 1 << 2,  // k: Set/remove the channel key (password)
    kModeL = 1 << 3   // l: Set/remove the user limit to channel
  };

  // constructors & destructor
  Channel(const IRCServer &server, const std::string &channel_name);
  Channel(const Channel &src);
  ~Channel();
  // operators
  Channel &operator=(const Channel &rhs);

  // getters & setters
  const std::string &GetName() const { return kName; }
  std::string GetTopic() const { return topic_; }
  std::string GetTopicSetter() const { return topic_setter_; }
  std::string GetTopicSetTime() const { return topic_set_time_; }
  std::string GetChannelTime() const { return channel_create_time_; }
  std::vector<int> &GetInvitedClients() { return invited_clients_; }
  int GetMaxClientCntInChannel() const { return max_client_cnt_; }
  void SetTopic(const std::string &topic, Client &client);
  void SetPassword(std::string password) { password_ = password; }
  void SetMaxClientCnt(int count) { max_client_cnt_ = count; }
  void SetModeOn(Mode mode);
  void SetModeOff(Mode mode);

  // function
  bool IsInChannel(const Client &client) const;
  bool IsInChannel(const std::string &nickname) const;
  bool IsInChannel(int client_fd) const;
  bool IsEmpty() const { return clients_.empty(); }
  bool IsOperator(int client_fd) const;
  bool IsOperator(const Client &client) const;
  bool IsModeOn(Mode mode) const;
  bool IsPasswordMatch(const std::string &given_password);
  // or const std::string &GetPassWord() const { return password_; }
  bool HaveChannelPassword();
  bool IsTopicSet() const { return !topic_.empty(); }
  bool IsInvitedClient(int client_fd);

  void AddInvitedClient(int client_fd) {
    invited_clients_.push_back(client_fd);
  }
  void RemoveInvitedClient(int client_fd);
  void AddOperator(int client_fd) {
    operators_.push_back(client_fd);
  }  // for compile
  void RemoveOperator(int client_fd);
  void BroadcastMessage(const std::string broadcast);
  void BroadcastMessage(const std::string broadcast, int sender_fd);
  const std::map<int, Client *> &GetClientsInChannel() { return clients_; }

 private:
  // constants
  const IRCServer &server_;
  static const int kMaxClientsPerChannel = 100;
  const std::string kName;
  // variables
  Type type_;
  char mode_;  // mode flag using bitwise operation
  int max_client_cnt_;
  std::string password_;
  std::string topic_;
  std::string topic_setter_;
  std::string topic_set_time_;
  std::string channel_create_time_;

  // need to define operators of channel
  std::vector<int> operators_;       // client fd,
  std::map<int, Client *> clients_;  // client fd, clients *
  std::vector<int> invited_clients_;
};

#endif
