#include "Message.hpp"

#include "MyUtils.hpp"

Message::Message() { ; }
Message::~Message() { ; }

// setters : append message
void Message::AppendMessageFromClient(const char* buff) {
  if (MyUtils::is_empty(buff) == true) return;
  from_client_ += buff;
}

void Message::AppendMessageToClient(const std::string& message) {
  to_client_ += message;
}

// clear
void Message::ClearMessages() {
  from_client_.clear();
  to_client_.clear();
}
