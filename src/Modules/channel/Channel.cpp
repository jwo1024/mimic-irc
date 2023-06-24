#include "Channel.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

//  default constructor
Channel::Channel(const IRCServer &server, const std::string &channel_name)
    : server_(server),
      kName(channel_name),
      max_client_cnt_(kChannelMaxClientCnt),
      channel_create_time_(MyUtils::GetCurrentRawTimeString()) {}

//  copy constructor
Channel::Channel(const Channel &src) : server_(src.server_), kName(src.kName) {
  *this = src;
}

//  destructor
Channel::~Channel() {}

//  operators ------------------------------------------------------------------

//  = operator overloading
Channel &Channel::operator=(const Channel &rhs) {
  if (this == &rhs) {
    return *this;
  }

  return *this;
}

//  functions ------------------------------------------------------------------

bool Channel::IsModeOn(Mode mode) const { return (mode_ & mode) == mode; }

void Channel::SetModeOn(Mode mode) { mode_ |= mode; }

void Channel::SetModeOff(Mode mode) { mode_ &= ~mode; }

bool Channel::HaveChannelPassword() {
  if (!password_.empty())
    return true;
  else
    return false;
}

bool Channel::IsPasswordMatch(const std::string &given_password) {
  if (given_password == password_)
    return true;
  else
    return false;
}

// Check if the client is in the channel's clients_
bool Channel::IsInChannel(const Client &client) const {
  // Loop through the client_ and check if the client is in the list
  for (std::map<int, Client *>::const_iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    if (it->second == &client) {
      return true;
    }
  }
  return false;
}

bool Channel::IsInChannel(const std::string &nickname) const {
  // Loop through the client_ and check if the client is in the list
  for (std::map<int, Client *>::const_iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    if (it->second->GetNickname() == nickname) {
      return true;
    }
  }
  return false;
}

bool Channel::IsInChannel(int client_fd) const {
  // Loop through the client_ and check if the client is in the list
  for (std::map<int, Client *>::const_iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    if (it->second->GetSocketFD() == client_fd) {
      return true;
    }
  }
  return false;
}

void Channel::SetTopic(const std::string &topic, Client &client) {
  topic_ = topic;
  topic_setter_ = client.GetNickname() + "!" + client.GetUserName() + "@" +
                  client.GetSocketIP();
  topic_set_time_ = MyUtils::GetCurrentRawTimeString();  // return (std::string)
                                                         // 요청해야할 것임
}

void Channel::BroadcastMessage(const std::string broadcast) {
  for (std::map<int, Client *>::const_iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    it->second->SetMessageToClient(broadcast);
  }
}
void Channel::BroadcastMessage(const std::string broadcast, int sender_fd) {
  for (std::map<int, Client *>::const_iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    if (it->first != sender_fd) it->second->SetMessageToClient(broadcast);
  }
}
//
bool Channel::IsOperator(int client_fd) const {
  if (std::find(operators_.begin(), operators_.end(), client_fd) !=
      operators_.end())
    return true;
  else
    return false;
}

bool Channel::IsOperator(const Client &client) const {
  return IsOperator(client.GetSocketFD());
}

void Channel::RemoveInvitedClient(int client_fd) {
  invited_clients_.erase(
      std::remove(operators_.begin(), operators_.end(), client_fd),
      operators_.end());
}

void Channel::RemoveOperator(int client_fd) {
  operators_.erase(std::remove(operators_.begin(), operators_.end(), client_fd),
                   operators_.end());
}

bool Channel::IsInvitedClient(int client_fd) {
  std::vector<int>::iterator iter = invited_clients_.begin();
  for (; iter != invited_clients_.end(); iter++) {
    if (*iter == client_fd) return true;
  }
  return false;
}
