#include "MyUtils.hpp"

namespace MyUtils {

// Checks if its a valid port number. If it is valid, returns actual port
// number. If not, it returns -1.
int IsValidPort(const char *port) {
  if (port == NULL) {
    return -1;
  }
  int i = 0;
  while (port[i] != '\0') {
    if (port[i] < '0' || port[i] > '9') {
      return -1;
    }
    i++;
  }
  char *endptr;
  long port_number = strtol(port, &endptr, 10);
  // Checks if strtol fails
  if (endptr == port || *endptr != '\0') {
    return -1;
  }
  if (port_number < 0 || port_number > 65535) {
    return -1;
  }
  return static_cast<int>(port_number);
}

// Checks if its a valid password. If it is valid, returns true. If not, it
// return false.
bool IsValidPassword(const char *password) {
  if (password == NULL) {
    return false;
  }
  int i = 0;
  while (password[i] != '\0') {
    // Only numbers and letters are allowed
    if ((password[i] < '0' || password[i] > '9') &&
        (password[i] < 'A' || password[i] > 'Z') &&
        (password[i] < 'a' || password[i] > 'z')) {
      return false;
    }
    i++;
  }
  return true;
}

// Checks if its a valid IP address. If it is valid, returns true. If not, it
// return false.
bool IsValidIP(const std::string &ip) {
  if (ip.empty()) {
    return false;
  }
  int i = 0;
  int dot_count = 0;
  while (ip[i] != '\0') {
    if (ip[i] == '.') {
      dot_count++;
    }
    // Only numbers and dots are allowed
    if ((ip[i] < '0' || ip[i] > '9') && ip[i] != '.') {
      return false;
    }
    i++;
  }
  if (dot_count != 3) {
    return false;
  }
  return true;
}

// Checks if its a valid nickname. If it is valid, returns true. If not, it
// return false.
bool IsValidNick(const std::string &nick) {
  if (nick.empty()) {
    return false;
  }
  int i = 0;
  while (nick[i] != '\0') {
    // Only numbers, letters, and underscore are allowed
    if ((nick[i] < '0' || nick[i] > '9') && (nick[i] < 'A' || nick[i] > 'Z') &&
        (nick[i] < 'a' || nick[i] > 'z') && nick[i] != '_') {
      return false;
    }
    i++;
  }
  return true;
}

// Checks if its a valid Real Name. If it is valid, returns true. If not, it
// return false.
bool IsValidRealName(const std::string &real_name) {
  if (real_name.empty()) {
    return false;
  }
  int i = 0;
  while (real_name[i] != '\0') {
    // Only numbers, letters, space and underscore are allowed
    if ((real_name[i] < '0' || real_name[i] > '9') &&
        (real_name[i] < 'A' || real_name[i] > 'Z') &&
        (real_name[i] < 'a' || real_name[i] > 'z') && real_name[i] != '_' &&
        real_name[i] != ' ') {
      return false;
    }
    i++;
  }
  return true;
}

// Checks if its a valid channel name. If it is valid, returns true. If not, it
// return false.
bool IsValidChannel(const std::string &channel) {
  if (channel.empty()) {
    return false;
  }
  int i = 0;
  while (channel[i] != '\0') {
    // Only numbers, letters, and underscore are allowed
    if ((channel[i] < '0' || channel[i] > '9') &&
        (channel[i] < 'A' || channel[i] > 'Z') &&
        (channel[i] < 'a' || channel[i] > 'z') && channel[i] != '_') {
      return false;
    }
    i++;
  }
  return true;
}

bool IsValidWelcomeMessage(const std::string &message) {
  // checks numeric 001
  if (message.find("001") != std::string::npos) {
    return true;
  }
  return false;
}

bool IsJoinFailedMessage(const std::string &message) {
  // checks numeric 473 475 471 476
  if (message.find("473") != std::string::npos ||
      message.find("475") != std::string::npos ||
      message.find("471") != std::string::npos ||
      message.find("476") != std::string::npos) {
    return true;
  }
  return false;
}

void Timer(int seconds, bool print) {
  clock_t start_time = clock();
  clock_t end_time = start_time + seconds * CLOCKS_PER_SEC;
  while (clock() < end_time) {
    if (print) {
      std::cout << end_time - clock() << " " << std::flush;
    }
  }
}

void Timer(double seconds) {
  clock_t start_time = clock();
  clock_t end_time = start_time + seconds * CLOCKS_PER_SEC;
  while (clock() < end_time) {
  }
}

void ClearTerminal() { std::cout << "\033[2J\033[1;1H"; }

void SlowTalker(const std::string &message, double speed) {
  for (unsigned long i = 0; i < message.size(); i++) {
    std::cout << message[i] << std::flush;
    Timer(speed);
  }
}

}  // namespace MyUtils