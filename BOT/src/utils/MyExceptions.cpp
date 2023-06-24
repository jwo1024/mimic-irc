#include "MyExceptions.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

MyExceptions::MyExceptions() : type_(kDefault), msg_(kDEFAULT_EXCEPTION_MSG) {}

MyExceptions::MyExceptions(const char *msg) : type_(kDefault), msg_(msg) {}

MyExceptions::MyExceptions(const std::string &msg)
    : type_(kDefault), msg_(msg.c_str()) {}

MyExceptions::MyExceptions(ExceptionType type, const char *msg)
    : type_(type), msg_(msg) {}

MyExceptions::MyExceptions(ExceptionType type, const std::string &msg)
    : type_(type), msg_(msg.c_str()) {}

MyExceptions::MyExceptions(const MyExceptions &src)
    : type_(src.type_), msg_(src.msg_) {
  *this = src;
}

//  destructor
MyExceptions::~MyExceptions() throw() {}

//  operators ------------------------------------------------------------------

//  = operator overloading
MyExceptions &MyExceptions::operator=(const MyExceptions &rhs) {
  if (this == &rhs) {
    return *this;
  }
  type_ = rhs.type_;
  msg_ = rhs.msg_;
  return *this;
}

//  functions ------------------------------------------------------------------

/**
 * @brief Returns the error message which contains [%Time] [%Type] %Message
 *
 * @return std::string
 */
std::string MyExceptions::GetErrorMessage() const {
  std::string error_msg;
  error_msg += "[";
  error_msg += MyString::GetPreciseTime();
  error_msg += "] [";
  switch (type_) {
    case kDefault:
      error_msg += "Default";
      break;
    case kChannel:
      error_msg += "Channel";
      break;
    case kClient:
      error_msg += "Client";
      break;
    case kCommand:
      error_msg += "Command";
      break;
    case kConnectionManager:
      error_msg += "ConnectionManager";
      break;
    case kServer:
      error_msg += "Server";
      break;
    case kSocket:
      error_msg += "Socket";
      break;
    default:
      error_msg += "Unknown";
      break;
  }
  error_msg += "] ";
  error_msg += msg_;
  if (kColorErrorMsg) {
    error_msg =
        MyString::ColorString(error_msg, MyString::kRed, MyString::kBlack);
  }
  return error_msg;
}

// private *********************************************************************