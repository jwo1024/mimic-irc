#ifndef MYUTILS_HPP_
#define MYUTILS_HPP_

#include "MyString.hpp"
#include "MyExceptions.hpp"
#include "defines.hpp"

class MyUtils {
public:
  static bool  is_empty(const char* str);
  static bool IsKeyValid(const std::string &given_key);
  static bool IsChannelNameValid(const std::string& channel_name);
  static bool IsNickNameValid(const std::string& given_nick);
  static time_t GetCurrentRawTime() { return time(NULL); }
  static std::string GetCurrentRawTimeString();
  static int ConvertStringToNumber(const std::string& str);
};

#endif
