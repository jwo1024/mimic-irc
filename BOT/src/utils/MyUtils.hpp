#ifndef MYUTILS_HPP_
#define MYUTILS_HPP_

#include <iostream>
#include <thread>

#include "MyExceptions.hpp"
#include "MyString.hpp"

namespace MyUtils {
int IsValidPort(const char *port);
bool IsValidPassword(const char *password);
bool IsValidIP(const std::string &ip);
bool IsValidNick(const std::string &nick);
bool IsValidRealName(const std::string &user);
bool IsValidChannel(const std::string &channel);
bool IsValidWelcomeMessage(const std::string &message);
bool IsJoinFailedMessage(const std::string &message);
void Timer(int seconds, bool print);
void Timer(double seconds);
void ClearTerminal();
void SlowTalker(const std::string &message, double speed = 0.1);
}  // namespace MyUtils

#endif