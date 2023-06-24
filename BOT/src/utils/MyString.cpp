#include "MyString.hpp"

// public **********************************************************************

/**
 * @brief Returns the colored string.
 *
 * @param str
 * @param foreground_color
 * @param background_color
 * @return std::string
 */
std::string MyString::ColorString(
    const std::string &str, enum StringColor foreground_color = kPreset,
    enum StringColor background_color = kPreset) {
  std::string color_prefix = "\033[";
  std::string color_suffix = "m";
  std::string color_end = "\033[0m";

  std::string foreground_color_data, background_color_data;
  std::stringstream ss;
  ss << foreground_color;
  ss >> foreground_color_data;
  ss.clear();
  ss << background_color;
  ss >> background_color_data;
  if (foreground_color != kPreset)
    foreground_color_data = "3" + foreground_color_data;
  if (background_color != kPreset)
    background_color_data = "4" + background_color_data;
  std::string color_start;
  color_start += color_prefix;
  color_start += foreground_color_data;
  if (background_color_data.empty() == false) {
    if (foreground_color_data.empty() == false) color_start += ";";
    color_start += background_color_data;
  }
  color_start += color_suffix;
  return color_start + str + color_end;
}

std::string MyString::ColorString(
    const char *str, enum StringColor foreground_color = kPreset,
    enum StringColor background_color = kPreset) {
  std::string converted_str = str;
  return ColorString(converted_str, foreground_color, background_color);
}

/**
 * @brief If you want to get a time stamp, use this function. It will return the
 * current time.
 *
 * @return std::string
 */
std::string MyString::GetPreciseTime() {
  // Get current time
  timeval current_time;
  gettimeofday(&current_time, NULL);
  int micro = current_time.tv_usec;

  // Convert timeval to tm
  std::time_t raw_time = current_time.tv_sec;
  std::tm *time_info = std::localtime(&raw_time);

  // Prepare a string stream
  std::ostringstream oss;

  // Output the date, time, and microseconds into the string stream
  oss << (time_info->tm_year + 1900) << "." << std::setfill('0') << std::setw(2)
      << (time_info->tm_mon + 1) << "." << std::setfill('0') << std::setw(2)
      << time_info->tm_mday << " " << std::setfill('0') << std::setw(2)
      << time_info->tm_hour << ":" << std::setfill('0') << std::setw(2)
      << time_info->tm_min << ":" << std::setfill('0') << std::setw(2)
      << time_info->tm_sec << "." << std::setfill('0') << std::setw(6) << micro;
  // Return the string
  return oss.str();
}

/**
 * @brief Splits string by delimiter and returns it by string vector.
 *
 * @param str
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> MyString::Split(const std::string &str,
                                         const char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::string::size_type start = 0;
  std::string::size_type end = 0;
  for (start = 0; start < str.size(); start++) {
    if (str[start] != delimiter) break;
  }
  while (start < str.size()) {
    if (str[start] != delimiter) {
      end = str.find(delimiter, start);
      token = str.substr(start, end - start);
      tokens.push_back(token);
      if (end == std::string::npos) break;
      start = end + 1;
    } else {
      start++;
    }
  }
  return tokens;
}

/**
 * @brief Returns the first word splitted by the delimiter.
 *
 * @param str
 * @param delimiter
 * @return std::string
 */
std::string MyString::GetFirstWord(const std::string &str,
                                   const char delimiter) {
  std::string::size_type start;
  std::string::size_type end;
  for (start = 0; start < str.size(); start++) {
    if (str[start] != delimiter) break;
  }
  end = str.find(delimiter, start);
  return str.substr(start, end - start);
}

// Trims white spaces from both ends of the string
std::string MyString::TrimWhiteSpace(const std::string &str) {
  std::string::size_type start;
  std::string::size_type end;
  for (start = 0; start < str.size(); start++) {
    if (str[start] != ' ') break;
  }
  for (end = str.size() - 1; end > 0; end--) {
    if (str[end] != ' ') break;
  }
  return str.substr(start, end - start + 1);
}