#ifndef MESSAGEMANAGER_HPP_
#define MESSAGEMANAGER_HPP_

#include <sstream>
#include <string>
#include <vector>

#include "Message.hpp"
#include "MyUtils.hpp"
#include "MyString.hpp"

class MessageManager {
 public:
  MessageManager();
  ~MessageManager();

  // functions
  bool ParseMessages(const std::string& recv_buff);
  bool ParseMessage(const std::string& message);
  static bool IsLastCharCRLF(const std::string &str);

  // setters : set message in client
  static void AppendMessageFromClient(Message& message, char* buff);
  static void AppendMessageToClient(Message& message, const std::string& message_to_client);  

  // getters
  const std::string& GetPrefix() const { return prefix_; };
  const std::string& GetCommand() const { return command_; };
  const std::vector<std::string>& GetParams() const { return params_; };
  const std::string& GetParam(int i) const;
  int GetParamSize() const { return params_.size(); };
  const std::vector<std::string> &GetSplitedStr() const { return splited_str_; };

  // clear
  void ClearCommand() { command_.clear(); };
  void ClearParams() { params_.clear(); };
  void Clear() { prefix_.clear(); command_.clear(); params_.clear(); };

 private:
  std::vector<std::string> splited_str_;
  std::string prefix_;
  std::string command_;
  std::vector<std::string> params_;

};

#endif