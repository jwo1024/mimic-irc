#include "IRCServerManager.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

//  default constructor
IRCServerManager::IRCServerManager(IRCServer &server) : server_(server) {}

//  destructor
IRCServerManager::~IRCServerManager() {}

//  functions ------------------------------------------------------------------

//  Create functions : create a new channel or client and add it to the server

// Create a new channel and add it to the server.
void IRCServerManager::CreateChannel(const std::string &channel_name) {
  // Create a new channel
  Channel *new_channel = new Channel(server_, channel_name);
  // Check if new fails
  if (new_channel == NULL) {
    throw MyExceptions(MyExceptions::kIRCServerManager,
                       "CreateChannel(const std::string &channel_name) failed. "
                       "new Channel() failed.");
  }
  // Add the new channel to server's channels_
  AddChannelToServer(new_channel);
}

// Create a new client and add it to the server.
int IRCServerManager::CreateClient(void) {
  // Create a new client
  Client *new_client = new Client(server_);
  // Check if new fails
  if (new_client == NULL) {
    throw MyExceptions(MyExceptions::kIRCServerManager,
                       "CreateClient(int client_fd) failed. new Client() "
                       "failed.");
  }
  // Add the new client to server's clients_
  AddClientToServer(new_client);
  return new_client->GetSocketFD();
}

//  Add functions : add a channel or client to the server or add client to
//  channel.

// Add a client to channel.
void IRCServerManager::AddClientToChannel(Client &client, Channel &channel) {
  AddClientToChannel(&client, &channel);
}

void IRCServerManager::AddClientToChannel(Client &client,
                                          const std::string &channel_name) {
  AddClientToChannel(&client, &GetChannel(channel_name));
}

void IRCServerManager::AddClientToChannel(int client_fd, Channel &channel) {
  AddClientToChannel(&GetClient(client_fd), &channel);
}

void IRCServerManager::AddClientToChannel(int client_fd,
                                          const std::string &channel_name) {
  AddClientToChannel(&GetClient(client_fd), &GetChannel(channel_name));
}

void IRCServerManager::AddClientToChannel(std::string nickname,
                                          Channel &channel) {
  AddClientToChannel(&GetClient(nickname), &channel);
}

void IRCServerManager::AddClientToChannel(std::string nickname,
                                          const std::string &channel_name) {
  AddClientToChannel(&GetClient(nickname), &GetChannel(channel_name));
}

//  Remove functions : remove a client from channel
// Remove a client from channel.

void IRCServerManager::RemoveClientFromChannel(Client &client,
                                               Channel &channel) {
  RemoveClientFromChannel(&client, &channel);
}

void IRCServerManager::RemoveClientFromChannel(
    int client_fd, const std::string &channel_name) {
  RemoveClientFromChannel(GetClient(client_fd), GetChannel(channel_name));
}

void IRCServerManager::RemoveClientFromChannel(int client_fd,
                                               Channel &channel) {
  RemoveClientFromChannel(GetClient(client_fd), channel);
}

void IRCServerManager::RemoveClientFromChannel(
    Client &client, const std::string &channel_name) {
  RemoveClientFromChannel(client, GetChannel(channel_name));
}

void IRCServerManager::RemoveClientFromChannel(
    std::string nickname, const std::string &channel_name) {
  RemoveClientFromChannel(GetClient(nickname), GetChannel(channel_name));
}

void IRCServerManager::RemoveClientFromChannel(std::string nickname,
                                               Channel &channel) {
  RemoveClientFromChannel(GetClient(nickname), channel);
}

//  Delete functions : delete a channel or client from the server
void IRCServerManager::DeleteChannel(const std::string &channel_name) {
  RemoveChannelFromServer(&GetChannel(channel_name));
}

void IRCServerManager::DeleteChannel(Channel &channel) {
  RemoveChannelFromServer(&channel);
}

void IRCServerManager::DeleteClient(int client_fd) {
  RemoveClientFromServer(&GetClient(client_fd));
}

void IRCServerManager::DeleteClient(Client &client) {
  RemoveClientFromServer(&client);
}

void IRCServerManager::DeleteClient(const std::string &nickname) {
  RemoveClientFromServer(&GetClient(nickname));
}

// Get functions : get a channel or client from the server. If the channel or
// client doesn't exist, throw an exception.

// Get a client reference from server's clients_. If the client doesn't exist,
// throw an exception.
Client &IRCServerManager::GetClient(int client_fd) {
  std::map<int, Client *>::iterator it = server_.clients_.find(client_fd);
  if (it == server_.clients_.end()) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "GetClient(int client_fd) failed. Client doesn't exist.");
  }
  return *(it->second);
}

// Get a client reference from server's clients_. If the client doesn't exist,
// throw an exception.
Client &IRCServerManager::GetClient(const std::string &nickname) {
  std::map<int, Client *>::iterator it;
  for (it = server_.clients_.begin(); it != server_.clients_.end(); ++it) {
    if (it->second->GetNickname() == nickname) {
      return *(it->second);
    }
  }
  // If client doesn't exist, throw an exception.
  throw MyExceptions(MyExceptions::kIRCServerManager,
                     "GetClient(const std::string &nickname) failed. Client "
                     "doesn't exist.");
}

// Get a channel reference from server's channels_. If the channel doesn't
// exist, throw an exception.
Channel &IRCServerManager::GetChannel(const std::string &channel_name) {
  std::map<std::string, Channel *>::iterator it =
      server_.channels_.find(channel_name);
  if (it == server_.channels_.end()) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "GetChannel(const std::string &channel_name) failed. Channel doesn't "
        "exist.");
  }
  return *(it->second);
}

//  Check functions ------------------------------------------------------------

// If channel exists in server, return true. Otherwise, return false.
bool IRCServerManager::IsChannelInServer(
    const std::string &channel_name) const {
  // Check if channel exists in server's channels_
  if (server_.channels_.find(channel_name) == server_.channels_.end()) {
    return false;
  }
  return true;
}

// If client exists in server, return true. Otherwise, return false.
bool IRCServerManager::IsClientInServer(int client_fd) const {
  // Check if client exists in server's clients_
  if (server_.clients_.find(client_fd) == server_.clients_.end()) {
    return false;
  }
  return true;
}

// If client exists in server, return true. Otherwise, return false.
bool IRCServerManager::IsClientInServer(const std::string &nickname) const {
  // Check if client exists in server's clients_
  std::map<int, Client *>::iterator it;
  for (it = server_.clients_.begin(); it != server_.clients_.end(); ++it) {
    if (it->second->GetNickname() == nickname) {
      return true;
    }
  }
  return false;
}

// If client is in channel, return true. Otherwise, return false.
bool IRCServerManager::IsClientInChannel(
    int client_fd, const std::string &channel_name) const {
  // Check if channel exists in server's channels_
  if (!IsChannelInServer(channel_name)) {
    return false;
  }
  // Check if client exists in server's clients_
  if (!IsClientInServer(client_fd)) {
    return false;
  }
  // Check if client is in channel
  if (server_.channels_[channel_name]->clients_.find(client_fd) ==
      server_.channels_[channel_name]->clients_.end()) {
    return false;
  }
  return true;
}

// If client is in channel, return true. Otherwise, return false.
bool IRCServerManager::IsClientInChannel(
    Client &client, const std::string &channel_name) const {
  return IsClientInChannel(client.GetSocketFD(), channel_name);
}

// If channel is full, return true. Otherwise, return false.
// If channel doesn't exist, throw an exception.
bool IRCServerManager::IsChannelFull(const std::string &channel_name) {
  if (!IsChannelInServer(channel_name)) {
    throw MyExceptions(MyExceptions::kIRCServerManager,
                       "IsChannelFull(const std::string &channel_name) failed. "
                       "Channel doesn't exist.");
  }
  if (server_.channels_[channel_name]->clients_.size() >=
      server_.channels_[channel_name]->kMaxClientsPerChannel) {
    return true;
  }
  return false;
}

// private *********************************************************************

//  Core functions for this class ----------------------------------------------

void IRCServerManager::AddClientToServer(Client *client) {
  // Check NULL
  if (client == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddClientToServer(Client *client) failed. client is NULL.");
  }
  // Check if client already exists
  if (IsClientInServer(client->GetSocketFD())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddClientToServer(Client *client) failed. Client already exists.");
  }
  // Add client pointer to server's clients_(std::map<int, Client *>)
  server_.clients_.insert(
      std::pair<int, Client *>(client->GetSocketFD(), client));
}

void IRCServerManager::AddChannelToServer(Channel *channel) {
  // Check NULL
  if (channel == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddChannelToServer(Channel *channel) failed. channel is NULL.");
  }
  // Check if channel already exists
  if (IsChannelInServer(channel->GetName())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddChannelToServer(Channel *channel) failed. Channel already exists.");
  }
  // Add channel pointer to server's channels_(std::map<std::string, Channel *>)
  server_.channels_.insert(
      std::pair<std::string, Channel *>(channel->GetName(), channel));
}

void IRCServerManager::AddClientToChannel(Client *client, Channel *channel) {
  // Check NULL
  if (client == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddClientToChannel(Client *client, Channel *channel) failed. client "
        "is NULL.");
  }
  if (channel == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddClientToChannel(Client *client, Channel *channel) failed. channel "
        "is NULL.");
  }
  // Check if client already exists in channel
  if (IsClientInChannel(client->GetSocketFD(), channel->GetName())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddClientToChannel(Client *client, Channel *channel) failed. Client "
        "already exists in channel.");
  }
  // Check if channel is full
  if (IsChannelFull(channel->GetName())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "AddClientToChannel(Client *client, Channel *channel) failed. Channel "
        "is full.");
  }

  // Add client pointer to channel's clients_(std::map<int, Client *>)
  channel->clients_.insert(
      std::pair<int, Client *>(client->GetSocketFD(), client));
  // Add channel pointer to client's channels_(std::map<std::string, Channel *>)
  client->channels_.insert(
      std::pair<std::string, Channel *>(channel->GetName(), channel));
}

void IRCServerManager::RemoveClientFromChannel(Client *client,
                                               Channel *channel) {
  // Check NULL
  if (client == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveClientFromChannel(Client *client, Channel *channel) failed. "
        "client is NULL.");
  }
  if (channel == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveClientFromChannel(Client *client, Channel *channel) failed. "
        "channel is NULL.");
  }
  // Check if client exists in channel
  if (!IsClientInChannel(client->GetSocketFD(), channel->GetName())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveClientFromChannel(Client *client, Channel *channel) failed. "
        "Client doesn't exist in channel.");
  }
  // Remove client pointer from channel's clients_.(Removing channel -> client)
  channel->clients_.erase(client->GetSocketFD());
  // Remove channel pointer from client's channels_.(Removing client -> channel)
  client->channels_.erase(channel->GetName());
  // If channel is empty, delete channel
  if (channel->clients_.size() == 0) {
    RemoveChannelFromServer(channel);
  }
}

void IRCServerManager::RemoveClientFromServer(Client *client) {
  // Check NULL
  if (client == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveClientFromServer(Client *client) failed. client is NULL.");
  }
  // Check if client exists in server
  if (!IsClientInServer(client->GetSocketFD())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveClientFromServer(Client *client) failed. Client doesn't exist "
        "in server.");
  }
  int client_socket_fd = client->GetSocketFD();
  // Remove client from channels which client is in
  for (std::map<std::string, Channel *>::iterator it =
           client->channels_.begin();
       it != client->channels_.end(); it++) {
    it->second->clients_.erase(client_socket_fd);
  }
  // Remove client pointer from server's clients_.(Removing server -> client)
  server_.clients_.erase(client_socket_fd);
  // Delete client
  delete client;
  DeleteEmptyChannels();
}

void IRCServerManager::RemoveChannelFromServer(Channel *channel) {
  // Check NULL
  if (channel == NULL) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveChannelFromServer(Channel *channel) failed. channel is NULL.");
  }
  // Check if channel exists in server
  if (!IsChannelInServer(channel->GetName())) {
    throw MyExceptions(
        MyExceptions::kIRCServerManager,
        "RemoveChannelFromServer(Channel *channel) failed. Channel doesn't "
        "exist in server.");
  }
  // Remove the channel from all clients
  for (std::map<int, Client *>::iterator it = channel->clients_.begin();
       it != channel->clients_.end(); it++) {
    it->second->channels_.erase(channel->GetName());
  }
  // Remove channel pointer from server's channels_.(Removing server -> channel)
  server_.channels_.erase(channel->GetName());
  // Delete channel
  delete channel;
}

void IRCServerManager::DeleteEmptyChannels() {
  // Delete empty channels
  for (std::map<std::string, Channel *>::iterator it =
           server_.channels_.begin();
       it != server_.channels_.end();) {
    if (it->second->clients_.size() == 0) {
      delete it->second;
      server_.channels_.erase(it++);
    } else {
      ++it;
    }
  }
}