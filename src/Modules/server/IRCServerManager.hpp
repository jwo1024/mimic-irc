#ifndef IRCSERVERMANAGER_HPP_
#define IRCSERVERMANAGER_HPP_

#include "IRCServer.hpp"
#include "MyUtils.hpp"

class IRCServer;
class Client;
class Channel;

class IRCServerManager {
 public:
  // constructors & destructor
  IRCServerManager(IRCServer &server);
  IRCServerManager(const IRCServerManager &src);
  ~IRCServerManager();
  // operators
  IRCServerManager &operator=(const IRCServerManager &rhs);
  // functions

  //  Create functions : create a new channel or client and add it to the server
  void CreateChannel(const std::string &channel_name);
  int CreateClient(void);

  //  Add client to channel. It will be used when client joins the channel.
  void AddClientToChannel(Client &client, Channel &channel);
  void AddClientToChannel(Client &client, const std::string &channel_name);
  void AddClientToChannel(int client_fd, Channel &channel);
  void AddClientToChannel(int client_fd, const std::string &channel_name);
  void AddClientToChannel(std::string nickname, Channel &channel);
  void AddClientToChannel(std::string nickname,
                          const std::string &channel_name);

  // Remove Client from a Channel.
  void RemoveClientFromChannel(Client &client, Channel &channel);  // core
  void RemoveClientFromChannel(int client_fd, const std::string &channel_name);
  void RemoveClientFromChannel(int client_fd, Channel &channel);
  void RemoveClientFromChannel(Client &client, const std::string &channel_name);
  void RemoveClientFromChannel(std::string nickname,
                               const std::string &channel_name);
  void RemoveClientFromChannel(std::string nickname, Channel &channel);

  //  Delete functions : delete a channel or client from the server. Connection
  //  between client and channel will be deleted too.
  void DeleteChannel(const std::string &channel_name);
  void DeleteChannel(Channel &channel);
  void DeleteClient(int client_fd);
  void DeleteClient(Client &client);
  void DeleteClient(const std::string &nickname);

  // Get functions : get a channel or client from the server. If the channel or
  // client doesn't exist, throw an exception.
  Client &GetClient(int client_fd);
  Client &GetClient(const std::string &nickname);
  Channel &GetChannel(const std::string &channel_name);
  IRCServer &GetServer() const { return server_; };

  // Check functions
  bool IsChannelInServer(const std::string &channel_name) const;
  bool IsClientInServer(int client_fd) const;
  bool IsClientInServer(const std::string &nickname) const;
  bool IsClientInChannel(int client_fd, const std::string &channel_name) const;
  bool IsClientInChannel(Client &client, const std::string &channel_name) const; //jayoon
  bool IsChannelFull(const std::string &channel_name);

 protected:
 private:
  // constants
  // variables
  IRCServer &server_;
  // functions

  //  Core functions for this class.
  //  WARNING!! Do not use it directly. This function will only be used in
  //  IRCServerManager class.
  void AddClientToServer(Client *client);
  void AddChannelToServer(Channel *channel);
  void AddClientToChannel(Client *client, Channel *channel);
  void RemoveClientFromChannel(Client *client, Channel *channel);
  void RemoveClientFromServer(Client *client);
  void RemoveChannelFromServer(Channel *channel);
  void DeleteEmptyChannels(void);
};

#endif