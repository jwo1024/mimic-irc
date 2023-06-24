#ifndef MYSTRING_HPP_
#define MYSTRING_HPP_

#include <sys/time.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// This class in an abstract class.
class MyString {
 public:
  // enum to color strings
  enum StringColor {
    kPreset = -1,
    kBlack = 0,
    kRed,
    kGreen,
    kYellow,
    kBlue,
    kMagenta,
    kCyan,
    kWhite
  };
  // functions
  static std::string ColorString(const std::string &str,
                                 enum StringColor foreground_color,
                                 enum StringColor background_color);
  static std::string ColorString(const char *str,
                                 enum StringColor foreground_color,
                                 enum StringColor background_color);
  static std::string GetPreciseTime();
  static std::vector<std::string> Split(const std::string &str,
                                        const char delimiter = ' ');
  static std::vector<std::string> Split(const std::string &str,
                                        const std::string &delimiter); // added
  static std::string GetFirstWord(const std::string &str,
                                  const char delimiter = ' ');

 private:
  // constructor to make this class abstract.
  MyString(){};
};

#endif