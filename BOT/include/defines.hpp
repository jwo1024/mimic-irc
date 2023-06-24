#ifndef DEFINES_HPP_
#define DEFINES_HPP_

#include <iostream>
#include <string>

enum Command {
  kUnknown,
  kQUIT,
  kPASS,
  kNICK,
  kUSER,
  kJOIN,
  kPRIVMSG,
  kPONG,
  kPING,
  kPART,
};

// constants
static const int kMaxMessageLength = 512;
static const int kBacklog = 10;
static const int kBufferSize = 1024;
static const int kDefaultPort = 6667;
static const int kDefaultServerPort = 6667;
static const double kMenuSpeed = 0.02;

// constants for testing
static const bool kUseDefaultBotInfo = true;
static const bool kUseDefaultServerInfo = true;

// constant strings

#ifndef kDefaultPassword
#define kDefaultPassword "password"
#endif

#ifndef kDefaultHostName
#define kDefaultHostName "localhost"
#endif

#ifndef kDefaultServerName
#define kDefaultServerName "localhost"
#endif

#ifndef kDefaultNick
#define kDefaultNick "T800"
#endif

#ifndef kDefaultUserName
#define kDefaultUserName "SkyNet"
#endif

#ifndef kDefaultRealName
#define kDefaultRealName "Terminator"
#endif

#ifndef kDefaultServerIP
#define kDefaultServerIP "127.0.0.1"
#endif

#ifndef kDefaultServerPassword
#define kDefaultServerPassword "password"
#endif

#endif
