#include "Client.hpp"
// public **********************************************************************

//  constructors & destructor --------------------------------------------------

//  default constructor
Client::Client(IRCServer &server)
    : server_(server),
      nickname_("*"),
      has_password_(false),
      is_registered_(false),
      is_timeouted_(false) {
  // Initialize client_socket_ with server's socket
  client_socket_.InitClientSocket(server_.GetServerSocket().GetSocketFD());
  UpdateTimeout();
}

//  destructor
Client::~Client() {}

//  operators ------------------------------------------------------------------

//  = operator overloading
Client &Client::operator=(const Client &rhs) {
  if (this == &rhs) {
    return *this;
  }
  // copy variables
  nickname_ = rhs.nickname_;
  username_ = rhs.username_;
  channels_ = rhs.channels_;
  // add more when variables are added
  return *this;
}

//  functions ------------------------------------------------------------------

void Client::SetMessageToClient(const std::string &str) {
  MessageManager::AppendMessageToClient(message_, str);
  server_.AddWriteEventOfClient(client_socket_.GetSocketFD());
}

void Client::SetNames(const std::string &nickname,
                      const std::string &username) {
  nickname_ = nickname;
  username_ = username;
}

bool Client::IsInChannel(const Channel &channel) const {
  // Loop through the channels_ and check if the channel is in the list
  for (std::map<std::string, Channel *>::const_iterator it = channels_.begin();
       it != channels_.end(); ++it) {
    if (it->second == &channel) {
      return true;
    }
  }
  return false;
}

bool Client::IsInChannel(const std::string &channel_name) const {
  // Loop through the channels_ and check if the channel is in the list
  for (std::map<std::string, Channel *>::const_iterator it = channels_.begin();
       it != channels_.end(); ++it) {
    if (it->first == channel_name) {
      return true;
    }
  }
  return false;
}

// private *********************************************************************

//  functions ------------------------------------------------------------------

//// Add channel to client's channels_. This function will only be called from
///the / IRCServerManager class
// void Client::AddChannel(Channel *channel) {
//   channels_.insert(
//       std::pair<std::string, Channel *>(channel->GetName(), channel));
// }
//
//// Remove channel from client's channels_. This function will only be called
//// from the IRCServerManager class
// void Client::RemoveChannel(Channel *channel) {
//   channels_.erase(channel->GetName());
// }

void Client::RemoveChannelFromClient(const Channel &channel) {
  IRCServerManager &server_manager = server_.GetServerManager();
  server_manager.RemoveClientFromChannel(this->GetSocketFD(),
                                         channel.GetName());
}

void Client::RemoveChannelFromClient(const std::string &channel_name) {
  IRCServerManager &server_manager = server_.GetServerManager();
  server_manager.RemoveClientFromChannel(this->GetSocketFD(), channel_name);
}
