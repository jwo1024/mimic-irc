#include "MessageManager.hpp"

// constructor & destructor
MessageManager::MessageManager() : prefix_(""), command_(""), params_() { ; }

MessageManager::~MessageManager() { ; }

// public

// functions
bool MessageManager::ParseMessages(const std::string& recv_buff) {
  splited_str_ = MyString::Split(recv_buff, "\r\n");
  if (splited_str_.empty() == true) return false;
  return true;
}

bool MessageManager::ParseMessage(const std::string& message) {
  std::stringstream ss(message);
  std::string token;

  ss >> token;
  if (token.empty()) return false;
  if (token[0] == ':') {
    prefix_ = token;
    prefix_.erase(0, 1);
    ss >> command_;
  } else {
    command_ = token;
  }
  while (ss >> token) {
    if (token[0] == ':') {
      std::string rest;
      std::getline(ss, rest);
      token.erase(0, 1);  // delete ':' in token
      params_.push_back(token + rest);
      break;
    }
    params_.push_back(token);
  }
  // if '/r' is exist in params_.back(), delete '/r'
  if (params_.empty() == false && params_.back().empty() == false &&
      params_.back().substr(params_.back().size() - 1, 1) == "\r")
    params_.back().erase(params_.back().size() - 1, 1);
  
  // make command_ uppercase
  std::transform(command_.begin(), command_.end(), command_.begin(),
                 ::toupper);

  return true;
}

// setters : set message in client
void MessageManager::AppendMessageFromClient(Message& message, char* buff) {
  if (MyUtils::is_empty(buff) == true) return;
  message.AppendMessageFromClient(buff);
}

void MessageManager::AppendMessageToClient(
    Message& message, const std::string& message_to_client) {
  message.AppendMessageToClient(message_to_client);
}

// getters
const std::string& MessageManager::GetParam(int i) const {
  unsigned long idx = i;
  if (idx < params_.size()) return params_[idx];
  return params_.back();
}

bool MessageManager::IsLastCharCRLF(const std::string &str) {
  size_t str_length = str.length();
  if (str_length >= 2)
    return ((int)str[str_length - 2] == '\r' && (int)str[str_length - 1] == '\n');
  return false;
}
