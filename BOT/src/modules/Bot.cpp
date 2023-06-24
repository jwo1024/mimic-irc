#include "Bot.hpp"

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

//  default constructor
Bot::Bot() {
  std::cout << "Bot Created!!" << std::endl;
  // Initialize variables.
  socket_fd_ = -1;
  nick_ = kDefaultNick;
  user_name_ = kDefaultUserName;
  host_name_ = kDefaultHostName;
  server_name_ = kDefaultServerName;
  real_name_ = kDefaultRealName;
  server_.ip_ = kDefaultServerIP;
  server_.port_ = kDefaultServerPort;
  server_.password_ = kDefaultServerPassword;
}

//  destructor
Bot::~Bot() {
}

//  functions ------------------------------------------------------------------

// Start the bot.
void Bot::Start() {
  MyUtils::SlowTalker("Starting bot...");
  // Ask user for server IP, port and password.
  if (!kUseDefaultServerInfo) {
    AskUserForServerInfo();
  }

  // Ask user for nick and user.
  if (!kUseDefaultBotInfo) {
    AskUserForBotInfo();
  }

  // Connect to server.
  try {
    ConnectToServer();
  } catch (MyExceptions &e) {
    std::cout << e.What() << std::endl;
    Stop(1);
  }

  // main loop for the bot. Use select() to check for input from stdin and bot
  // socket.
  PrintBotMenu();
  while (true) {
    // set up the file descriptor set
    fd_set read_fds;
    int max_fd = SetReadFDs(read_fds);
    int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if (activity < 0) {
      throw MyExceptions("select() error");
    }

    // Check if there is input from stdin.
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      // get user input
      char input[kMaxMessageLength];
      std::cin.getline(input, kMaxMessageLength);
      std::string input_str(input);
      if (input_str == "0") {
        Stop();
      } else if (input_str == "1") {
        SendPingToServer();
      } else if (input_str == "2") {
        AskUserToJoinChannel();
      } else if (input_str == "3") {
        SendCurrentTimeToChannels();
      } else if (input_str == "4") {
        AskUserToExitChannel();
      } else {
        std::cout << "Invalid input." << std::endl;
      }
      PrintBotMenu();
    }

    // Check if there is input from the bot socket.
    if (FD_ISSET(socket_fd_, &read_fds)) {
      // Read input from the bot socket.
      std::string server_input = Socket::Receive(socket_fd_);
      // If the server closes the connection, exit the program.
      if (server_input.empty()) {
        std::cout << "Server closed the connection." << std::endl;
        break;
      }
      // received PING message, send a PONG message back.
      Message message(server_input);
      if (message.GetCommand() == "PING") {
        HandlePing(message);
      }
      if (message.GetCommand() == "PRIVMSG") {
        HandlePRIVMSG(message);
      }
    }
  }
  Stop();
}

void Bot::Stop(int exit_code) {
  std::cout << "Stopping bot..." << std::endl;
  std::string quit_message;
  if (exit_code == 0) {
    quit_message += "Bot stopped successfully.";
  } else {
    quit_message += "Bot stopped with error code ";
  }
  quit_message += "\nI WILL BE BACK......!";
  MyUtils::SlowTalker(quit_message);
  std::cout << std::endl;
  Socket::Close(socket_fd_);
  exit(exit_code);
}

// private *********************************************************************

//  functions ------------------------------------------------------------------

// Ask the user for the server IP, port and password.
void Bot::AskUserForServerInfo() {
  // Ask user for server IP. If the user enters an invalid IP, ask again.
  // If the user enters "quit", exit the program.
  while (true) {
    std::cout << "Enter the server IP: ";
    std::string server_ip;
    std::getline(std::cin, server_ip);
    if (server_ip == "quit") {
      Stop(1);
    }
    if (MyUtils::IsValidIP(server_ip)) {
      server_.ip_ = server_ip;
      break;
    }
    std::cout << "Invalid IP address." << std::endl;
  }

  // Ask user for server port. If the user enters an invalid port, ask again.
  // If the user enters "quit", exit the program.
  while (true) {
    std::cout << "Enter the server port: ";
    std::string server_port;
    std::getline(std::cin, server_port);
    if (server_port == "quit") {
      Stop(1);
    }
    int port = MyUtils::IsValidPort(server_port.c_str());
    if (port != -1) {
      server_.port_ = port;
      break;
    }
    std::cout << "Invalid port number format." << std::endl;
  }

  // Ask user for server password. If the user enters an invalid password, ask
  // again. If the user enters "quit", exit the program.
  while (true) {
    std::cout << "Enter the server password: ";
    std::string server_password;
    std::getline(std::cin, server_password);
    if (server_password == "quit") {
      Stop(1);
    }
    if (MyUtils::IsValidPassword(server_password.c_str())) {
      server_.password_ = server_password;
      break;
    }
    std::cout << "Invalid password format." << std::endl;
  }
}

// Ask the user for the nick and user.
void Bot::AskUserForBotInfo() {
  // Ask user for bot nick. If the user enters an invalid nick, ask again.
  // If the user enters "quit", exit the program.
  while (true) {
    std::cout << "Enter the nick for bot: ";
    std::string bot_nick;
    std::getline(std::cin, bot_nick);
    if (bot_nick == "quit") {
      Stop(1);
    }
    if (MyUtils::IsValidNick(bot_nick)) {
      nick_ = bot_nick;
      break;
    }
    std::cout << "Invalid nick." << std::endl;
  }

  // Ask user for bot real name. If the user enters an invalid real name, ask
  // again. If the user enters "quit", exit the program.
  while (true) {
    std::cout << "Enter your real name: ";
    std::string bot_real_name;
    std::getline(std::cin, bot_real_name);
    if (bot_real_name == "quit") {
      Stop(1);
    }
    if (MyUtils::IsValidRealName(bot_real_name)) {
      real_name_ = bot_real_name;
      break;
    }
    std::cout << "Invalid real name." << std::endl;
  }
}

// Connect to the server.
void Bot::ConnectToServer() {
  // Create a socket
  socket_fd_ = Socket::CreateSocket();
  // Connect to the server
  Socket::Connect(socket_fd_, server_.port_, server_.ip_.c_str());
  // Send the password to the server
  Socket::Send(socket_fd_, Message::BuildMessage(kPASS, server_.password_));
  // Send the nick to the server
  Socket::Send(socket_fd_, Message::BuildMessage(kNICK, nick_));
  // Send the user to the server
  Socket::Send(socket_fd_,
               Message::BuildMessage(kUSER, user_name_ + " 0 *", real_name_));
}

// Ask the user for the channel to join.
void Bot::AskUserToJoinChannel() {
  while (true) {
    std::cout << "Enter the channel to join: ";
    std::string channel;
    std::getline(std::cin, channel);
    if (channel == "quit") {
      Stop(1);
    }
    if (MyUtils::IsValidChannel(channel)) {
      if (JoinChannel(channel)) {
        channels_.push_back(channel);
        break;
      }
    }
    std::cout << "Invalid channel." << std::endl;
  }
}

// Join a channel.
bool Bot::JoinChannel(const std::string &channel) {
  try {
    std::string channel_name = "#" + channel;
    // Send the join message to the server
    Socket::Send(socket_fd_, Message::BuildMessage(kJOIN, channel_name));
    // Receive the join message from the server
    std::string join_message = Socket::Receive(socket_fd_);
    // Check if the join message is valid
    if (MyUtils::IsJoinFailedMessage(join_message)) {
      std::cout << "Failed to join the channel." << std::endl;
      return false;
    }
    return true;
  } catch (MyExceptions &e) {
    std::cout << e.What() << std::endl;
    return false;
  }
}

void Bot::SendMessageToChannel(const std::string &channel,
                               const std::string &message) {
  // Check if the channel is valid
  for (std::vector<std::string>::iterator it = channels_.begin();
       it != channels_.end(); ++it) {
    if (*it == channel) {
      break;
    }
    if (it == channels_.end() - 1) {
      std::cout << "Invalid channel." << std::endl;
      return;
    }
  }
  // Check if the message is too long
  if (message.length() > kMaxMessageLength) {
    std::cout << "Message is too long." << std::endl;
    return;
  }
  try {
    // Build the message and send it to the server
    std::string prefix = nick_ + "!" + user_name_ + "@" + server_.ip_;
    std::string message_to_send =
        Message::BuildMessage(kPRIVMSG, "#" + channel, message);
    Socket::Send(socket_fd_, message_to_send);
  } catch (MyExceptions &e) {
    std::cout << e.What() << std::endl;
  }
}

// Initialize the read_fds and add all client sockets to it. Returns the max_fd.
int Bot::SetReadFDs(fd_set &read_fds) {
  // Initialize the read set of file descriptors for select(). The bot
  // socket should always be in the read set.
  FD_ZERO(&read_fds);
  FD_SET(STDIN_FILENO, &read_fds);
  FD_SET(socket_fd_, &read_fds);
  int max_fd = socket_fd_;
  return max_fd;
}

void Bot::HandlePing(Message &message) {
  // Send the pong message to the server
  message.SetCommand("PONG");
  Socket::Send(socket_fd_, message.BuildMessage());
  message.Clear();
}

void Bot::HandlePRIVMSG(Message &message) {
  std::vector<std::string> params = message.GetParams();
  std::string msg = message.GetTrailing();
  if (params.size() == 0)
    ;
  else if (msg == "help\r\n") {
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :" + "Enter message like" + "\r\n");
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :" + "\"Hi, jiyun\"" + "\r\n");
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :" + "\"Hi, jchoi\"" + "\r\n");
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :" + "\"Hi, jiwolee\"" + "\r\n");
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :" + "\"Hi, seoyoo\"" + "\r\n");
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :" + "\"Hi, jayoon\"" + "\r\n");
  }
  else if (msg == "Hi, jiyun\r\n") {
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :MuckGoZa !\r\n");
  }
  else if (msg == "Hi, jchoi\r\n") {
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :Take care of your BLACKHOLE !\r\n");
  }
  else if (msg == "Hi, jiwolee\r\n") {
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :Ocksang u ro ddara wa !\r\n");
  }
  else if (msg == "Hi, seoyoo\r\n") {
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :I'm going to Gym !\r\n");
  }
  else if (msg == "Hi, jayoon\r\n") {
    Socket::Send(socket_fd_, "PRIVMSG " + params[0] + " :La lalalala la~ !\r\n");
  }
  message.Clear();
}

void Bot::PrintBotMenu() {
  MyUtils::ClearTerminal();
  std::stringstream ss;
  ss << "----------------------------------------" << std::endl;
  ss << "Bot Menu:" << std::endl;
  ss << "0. Quit" << std::endl;
  ss << "1. Send PING to a channels" << std::endl;
  ss << "2. Join channel" << std::endl;
  ss << "3. Send current time to channels" << std::endl;
  ss << "4. Exit channel" << std::endl;
  ss << "----------------------------------------" << std::endl;
  ss << "Joined Channels: \n";
  int channel_count = 1;
  for (std::vector<std::string>::iterator it = channels_.begin();
       it != channels_.end(); ++it) {
    ss << "[" << channel_count++ << "] " << *it << std::endl;
  }
  ss << "----------------------------------------" << std::endl;
  ss << "Enter your choice: ";
  MyUtils::SlowTalker(ss.str(), kMenuSpeed);
}

void Bot::SendPingToServer() {
  Socket::Send(socket_fd_, Message::BuildMessage(kPING, server_.ip_));
  Message message(Socket::Receive(socket_fd_));
  if (message.GetCommand() == "PONG") {
    std::cout << "PONG from server!\nSuccessfully sent PING to the server."
              << std::endl;
    // Ask user to press enter to continue
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.get();
  } else {
    std::cout << "Failed to receive PONG from the server." << std::endl;
    // Ask user to press enter to continue
    std::cout << "Do you want to continue...[Y/N]" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    if (input == "Y" || input == "y") {
      return;
    } else if (input == "N" || input == "n") {
      Stop();
    } else {
      std::cout << "Invalid input. Exiting..." << std::endl;
      Stop();
    }
  }
}

void Bot::SendCurrentTimeToChannels() {
  // Get the current time
  time_t raw_time;
  struct tm *time_info;
  time(&raw_time);
  time_info = localtime(&raw_time);
  std::string time_str(asctime(time_info));
  // Send the current time to the channels
  for (std::vector<std::string>::iterator it = channels_.begin();
       it != channels_.end(); ++it) {
    SendMessageToChannel(*it, time_str);
  }
}

void Bot::AskUserToExitChannel() {
  // Ask user to enter the channel
  std::cout << "Which channel do you want to exit?: ";
  std::string channel;
  std::getline(std::cin, channel);
  // Check if the channel is valid
  for (std::vector<std::string>::iterator it = channels_.begin();
       it != channels_.end(); ++it) {
    if (*it == channel) {
      break;
    }
    if (it == channels_.end() - 1) {
      std::cout << "Invalid channel." << std::endl;
      return;
    }
  }
  // Send the part message to the server
  std::string prefix = nick_ + "!" + user_name_ + "@" + server_.ip_;
  std::string message_to_send =
      Message::BuildMessage(prefix, kPART, "#" + channel, "Bye humans!");
  Socket::Send(socket_fd_, message_to_send);
  // Remove the channel from the channels_ vector
  std::vector<std::string>::iterator it;
  for (it = channels_.begin(); it != channels_.end(); ++it) {
    if (*it == channel) {
      break;
    }
  }
  if (it != channels_.end()) {
    channels_.erase(it);
    std::cout << "Successfully exited the channel." << std::endl;
  } else
    std::cout << "Failed to exit the channel." << std::endl;
}
