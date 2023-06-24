#ifndef MYEXCEPTIONS_HPP_
#define MYEXCEPTIONS_HPP_

#include <exception>

#include "MyString.hpp"

#ifndef kDEFAULT_EXCEPTION_MSG
#define kDEFAULT_EXCEPTION_MSG "Default exception"
#endif

class MyExceptions : public std::exception {
 public:
  // enum for exception types
  enum ExceptionType {
    kDefault,
    kChannel,
    kClient,
    kCommand,
    kConnectionManager,
    kServer,
    kSocket
  };

  // constructors & destructor
  MyExceptions();
  MyExceptions(const char *msg);
  MyExceptions(const std::string &msg);
  MyExceptions(ExceptionType type, const char *msg);
  MyExceptions(ExceptionType type, const std::string &msg);
  MyExceptions(const MyExceptions &src);
  virtual ~MyExceptions() throw();

  // operators
  MyExceptions &operator=(const MyExceptions &rhs);

  // functions
  virtual const char *what() const throw() { return msg_; }
  std::string GetErrorMessage() const;
  std::string What() const { return GetErrorMessage(); }

 private:
  // constants
  static const bool kColorErrorMsg = true;

  // variables
  ExceptionType type_;
  const char *msg_;
};

#endif