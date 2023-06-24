#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <deque>
#include <string>

class Message {
 public:
  Message();
  ~Message();

  // setters : append message
  void AppendMessageFromClient(const char* buff);
  void AppendMessageToClient(const std::string& message);

  // getters
  const std::string& GetMessageToClient() const { return to_client_; }
  const std::string& GetMessageFromClient() const { return from_client_; }

  // clear
  void ClearMessages();
  void ClearMessageFromClient() { from_client_.clear(); }
  void ClearMessageToClient() { to_client_.clear(); }

 private:
  std::string from_client_;
  std::string to_client_;
};

#endif