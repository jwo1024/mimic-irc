#include "MyUtils.hpp"

bool  MyUtils::is_empty(const char* str)
{
  return static_cast<bool>(str == NULL);
}

bool MyUtils::IsChannelNameValid(const std::string& channelName) {
  if (channelName.length() < 2 || channelName[0] != '#' || !isalpha(channelName[1]))
	return false; // Channel name must start with '#' and have at least one letter after it

  for (std::string::size_type i = 1; i < channelName.length(); ++i) {
	char c = channelName[i];
	if (c == ' ' || c == '\a' || c == ',')
	  return false; // Channel name contains invalid character(s)
  }
  return true; // Channel name is valid
}

bool MyUtils::IsNickNameValid(const std::string &given_nick) {
  char *str = const_cast<char *>(given_nick.c_str());
  if (*str == '#' || *str == ':' || *str == ' ')
	return false;
  if (isdigit(*str))
	return false;
  str++;
  while (*str != '\0') {
	if (isalpha(*str) == false && isdigit(*str) == false) {
	  if (*str != '[' && *str != ']' && *str != '{' && *str != '}'
		  && *str != '\\' && *str != '|')
		return false;
	}
	str++;
  }
  return true;
}

std::string MyUtils::GetCurrentRawTimeString() {
  std::stringstream ss;
  ss << GetCurrentRawTime();
  return ss.str();
}

bool MyUtils::IsKeyValid(const std::string &given_key) {
  for (std::string::size_type i = 0; i < given_key.length(); ++i) {
	char c = given_key[i];
	if (!isalnum(c))
	  return false; // Channel name contains invalid character(s)
  }
  return true;
}

int MyUtils::ConvertStringToNumber(const std::string& str) {
  for (std::string::size_type i = 1; i < str.length(); ++i) {
	char c = str[i];
	if (isdigit(c) == false)
	  return -1;
  }
  std::stringstream ss(str);
  int ret;
  ss >> ret;
  if (ss.fail())
	return -1;
  if (ret < 1 || ret > 10)
	return kChannelMaxClientCnt;
  return ret;
}
