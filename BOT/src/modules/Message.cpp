#include "Message.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

//  default constructor
Message::Message() {}

// Create a message from a raw string.
Message::Message(const std::string &raw) : raw_(raw) {
  ParseReceivedMessage(raw_);
}

//  copy constructor
Message::Message(const Message &src) { *this = src; }

//  destructor
Message::~Message() {}

//  operators ------------------------------------------------------------------

//  = operator overloading
Message &Message::operator=(const Message &rhs) {
  if (this == &rhs) {
    return *this;
  }
  raw_ = rhs.raw_;
  prefix_ = rhs.prefix_;
  command_ = rhs.command_;
  params_ = rhs.params_;
  trailing_ = rhs.trailing_;
  return *this;
}

//  functions ------------------------------------------------------------------

// Get command(enum) from a string.
Command Message::GetCommandFromString(const std::string &command) {
  if (command == "QUIT") return kQUIT;
  if (command == "PASS") return kPASS;
  if (command == "NICK") return kNICK;
  if (command == "USER") return kUSER;
  if (command == "JOIN") return kJOIN;
  if (command == "PRIVMSG") return kPRIVMSG;
  if (command == "PONG") return kPONG;
  if (command == "PING") return kPING;
  return kUnknown;
}

// Get command(string) from an enum.
std::string Message::GetStringFromCommand(Command command) {
  if (command == kQUIT) return "QUIT";
  if (command == kPASS) return "PASS";
  if (command == kNICK) return "NICK";
  if (command == kUSER) return "USER";
  if (command == kJOIN) return "JOIN";
  if (command == kPRIVMSG) return "PRIVMSG";
  if (command == kPONG) return "PONG";
  if (command == kPING) return "PING";
  if (command == kPART) return "PART";
  return "";
}

// Parse a message from a raw string.
void Message::ParseReceivedMessage(const std::string &data) {
  raw_ = data;
  std::string temp_str = MyString::TrimWhiteSpace(data);

  // Parse the prefix. If the message has a prefix, it will start with a colon.\
  // If not, the prefix will be empty.
  if (temp_str[0] == ':') {
    size_t prefix_end = temp_str.find(' ');
    prefix_ = temp_str.substr(1, prefix_end - 1);
    temp_str = MyString::TrimWhiteSpace(temp_str.substr(prefix_end + 1));
  }

  // Parse the command. The command is the first word after the prefix.
  size_t command_end = temp_str.find(' ');
  command_ = temp_str.substr(0, command_end);
  temp_str = MyString::TrimWhiteSpace(temp_str.substr(command_end + 1));

  // Parse the rest of the message. The rest of the message is the
  // parameters\trailing.
  while (temp_str.size() > 0) {
    // If the next word starts with a colon, it is the trailing.
    if (temp_str[0] == ':') {
      trailing_ = temp_str.substr(1);
      break;
    }
    // Otherwise, it is a parameter.
    size_t param_end = temp_str.find(' ');
    params_.push_back(temp_str.substr(0, param_end));
    temp_str = MyString::TrimWhiteSpace(temp_str.substr(param_end + 1));
  }
}

// Build a message to send to the server.
std::string Message::BuildMessage(const std::string &prefix, Command command,
                                  const std::string &message,
                                  const std::string &trailing) {
  std::stringstream ss;
  if (!prefix.empty()) {
    ss << ":" << prefix << " ";
  }
  ss << GetStringFromCommand(command) << " ";
  if (!message.empty()) {
    ss << message << " ";
  }
  if (!trailing.empty()) {
    ss << ":" << trailing;
  }
  ss << "\r\n";
  return ss.str();
}

std::string Message::BuildMessage(Command command, const std::string &message) {
  return BuildMessage("", command, message, "");
}

std::string Message::BuildMessage(Command command, const std::string &message,
                                  const std::string &trailing) {
  return BuildMessage("", command, message, trailing);
}

std::string Message::BuildMessage(Command command) {
  return BuildMessage("", command, "", "");
}

std::string Message::BuildMessage() {
  Command command = GetCommandFromString(command_);
  std::string params;
  for (std::vector<std::string>::iterator it = params_.begin();
       it != params_.end(); ++it) {
    params += *it + " ";
  }
  return BuildMessage(prefix_, command, params, trailing_);
}

// Clear the message.
void Message::Clear() {
  raw_.clear();
  prefix_.clear();
  command_.clear();
  params_.clear();
  trailing_.clear();
}