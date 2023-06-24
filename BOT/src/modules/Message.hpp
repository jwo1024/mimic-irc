#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>

#include "MyUtils.hpp"
#include "defines.hpp"

class Message {
 public:
  // constructors & destructor
  Message();
  Message(const std::string &raw);
  Message(const Message &src);
  ~Message();
  // operators
  Message &operator=(const Message &rhs);
  // functions
  Command GetCommandFromString(const std::string &command);
  static std::string GetStringFromCommand(Command command);
  void ParseReceivedMessage(const std::string &data);
  static std::string BuildMessage(const std::string &prefix, Command command,
                                  const std::string &message,
                                  const std::string &trailing);
  static std::string BuildMessage(Command command, const std::string &message);
  static std::string BuildMessage(Command command, const std::string &message,
                                  const std::string &trailing);
  static std::string BuildMessage(Command command);
  std::string BuildMessage();
  void Clear();

  // getters & setters
  const std::string &GetRaw() { return raw_; }
  const std::string &GetPrefix() { return prefix_; }
  const std::string &GetCommand() { return command_; }
  const std::vector<std::string> &GetParams() { return params_; }
  const std::string &GetTrailing() { return trailing_; }
  void SetRaw(const std::string &raw) { raw_ = raw; }
  void SetPrefix(const std::string &prefix) { prefix_ = prefix; }
  void SetCommand(const std::string &command) { command_ = command; }
  void SetParams(const std::vector<std::string> &params) { params_ = params; }
  void SetTrailing(const std::string &trailing) { trailing_ = trailing; }

 private:
  // variables
  std::string raw_;
  std::string prefix_;
  std::string command_;
  std::vector<std::string> params_;
  std::string trailing_;
};

#endif