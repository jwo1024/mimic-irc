#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <deque>
#include <map>
#include <string>

#include "Channel.hpp"
#include "ClientSocket.hpp"
#include "IRCServer.hpp"
#include "IRCServerManager.hpp"
#include "Message.hpp"
#include "MessageManager.hpp"
#include "MyUtils.hpp"

class Channel;
class IRCServerManager;
class ClientSocket;
class IRCServer;

class Client {
  friend class IRCServerManager;

 public:
  // constructors & destructor
  Client(IRCServer &server);
  Client(const Client &src);
  ~Client();
  // operators
  Client &operator=(const Client &rhs);
  // getters
  const ClientSocket &GetClientSocket() const { return client_socket_; }
  int GetSocketFD() const { return client_socket_.GetSocketFD(); }
  const std::string GetSocketIP() const { return client_socket_.GetIPString(); }
  const std::string& GetNickname() const { return nickname_; }
  const std::string& GetUserName() const { return username_; }
  const std::string& GetRealName() const { return realname_; }
  const Message GetMessages() const { return message_; }
  const IRCServer& GetServer() { return server_; }
//  const std::string& GetServerPassword() const { return server_.GetPassword(); }
  const std::string &GetMessageToClient() { return message_.GetMessageToClient(); }
  const std::string &GetMessageFromClient() { return message_.GetMessageFromClient(); }
  std::map<std::string, Channel *>& GetClientChannels() { return channels_; } // for compile
//  const std::string& GetServerName() const { return server_.GetName(); }
//  const std::string& GetServerVersion() const { return server_.GetVersion(); }

  // setters
  void SetMessageFromClient(char *buff) { MessageManager::AppendMessageFromClient(message_, buff); }
  void SetMessageToClient(const std::string &str);
  void SetNickName(std::string nickname) { nickname_ = nickname; }
  void SetNames(const std::string &nickname, const std::string &username);
  void SetUserName(const std::string &username) { username_ = username; }
  void SetRealName(const std::string &realname) { realname_ = realname; }
  void SetRegistered(bool is_registered) { is_registered_ = is_registered; }
  void SetHasPassword(bool on) { has_password_ = on; }

  // functions
  bool IsNoChannel() const { return channels_.empty(); }
  bool IsInChannel(const Channel &channel) const;
  bool IsInChannel(const std::string &channel_name) const;
  bool IsRegistered() const { return is_registered_; }  
  bool HasPassword() const { return has_password_; }

  void RemoveChannelFromClient(const Channel& channel);
  void RemoveChannelFromClient(const std::string& target_channel);

  // void Clean() {};
  void ClearMessageFromClient() { message_.ClearMessageFromClient(); }
  void ClearMessageToClient() { message_.ClearMessageToClient(); }
  void ClearMessages(){ message_.ClearMessages(); };

  void UpdateTimeout() { last_activity_time_ = time(NULL); }
  void SetTimeouted() { is_timeouted_ = true; }
  bool IsTimeout(time_t time_out) { return (time(NULL) - last_activity_time_) > time_out; } // kTimeOut = 120
  bool IsTimeouted() { return is_timeouted_; }

 private:
  // variables
  IRCServer &server_; // delete const
  ClientSocket client_socket_;
  Message message_;
  std::map<std::string, Channel *> channels_;  // channel name, channel *
  
  std::string nickname_;
  std::string username_;
  std::string realname_;

  bool has_password_;
  bool is_registered_;

  bool is_timeouted_;
  time_t  last_activity_time_; // timeout

  // functions
  void AddChannel(Channel *channel);
  void RemoveChannel(Channel *channel);
};

#endif
