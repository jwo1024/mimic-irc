#include "IRCServer.hpp"

// initialize variables
const std::string IRCServer::kServerName = "Was_WebServ";
const std::string IRCServer::kServerVersion = "1.0.0";
const std::string IRCServer::kServerInitTime =
    MyUtils::GetCurrentRawTimeString();
const std::string IRCServer::kUserModes = "";
const std::string IRCServer::kChanModes = "itkol";

// public **********************************************************************

//  constructors & destructor --------------------------------------------------

//  default constructor
IRCServer::IRCServer(int port, const std::string& password)
    : port_(port), password_(password) {
  // Create server manager
  manager_ = new IRCServerManager(*this);

  // print server created! -> erase it if you want!!
  std::stringstream ss;
  ss << "Server created!!\n port: " << port_ << "\n password: " << password_
     << std::endl;
  std::cout << MyString::ColorString(ss.str(), MyString::kCyan,
                                     MyString::kPreset)
            << std::endl;
}

//  destructor
IRCServer::~IRCServer() { delete manager_; }

//  static functions -----------------------------------------------------------

// check port : return port if valid, throw exception if invalid
int IRCServer::CheckPort(const char* port) {
  std::string port_str(port);
  // check port_str length
  if (port_str.length() > 5)
    throw MyExceptions(MyExceptions::kIRCServer, "Invalid port");
  // check port_str characters : allows only digits
  for (size_t i = 0; i < port_str.length(); ++i) {
    if (!std::isdigit(port_str[i]))
      throw MyExceptions(MyExceptions::kIRCServer, "Invalid port");
  }
  int port_int;
  port_int = std::atoi(port);
  if (port_int == 0)
    throw MyExceptions(MyExceptions::kIRCServer, "Invalid port");
  // check port_int range
  if (port_int < kPortMin || port_int > kPortMax)
    throw MyExceptions(MyExceptions::kIRCServer, "Invalid port");
  return port_int;
}

// check password : return password if valid, throw exception if invalid
std::string IRCServer::CheckPassword(const char* password) {
  std::string password_str(password);
  // check password length
  if (password_str.length() < kPasswordMinLength)
    throw MyExceptions(MyExceptions::kIRCServer, "Password too short");
  if (password_str.length() > kPasswordMaxLength)
    throw MyExceptions(MyExceptions::kIRCServer, "Password too long");
  // check password characters : allows alphanumeric characters, underscore and
  // !@#$%^&*()-+ characters
  for (size_t i = 0; i < password_str.length(); ++i) {
    if (!std::isalnum(password_str[i]) && password_str[i] != '_' &&
        password_str[i] != '!' && password_str[i] != '@' &&
        password_str[i] != '#' && password_str[i] != '$' &&
        password_str[i] != '%' && password_str[i] != '^' &&
        password_str[i] != '&' && password_str[i] != '*' &&
        password_str[i] != '(' && password_str[i] != ')' &&
        password_str[i] != '-' && password_str[i] != '+')
      throw MyExceptions(MyExceptions::kIRCServer, "Invalid password");
  }
  return password_str;
}

//  functions ------------------------------------------------------------------

void IRCServer::Set() {
  try {
    server_socket_.InitServerSocket(INADDR_ANY, port_);
    poller_.InitPoller(server_socket_.GetSocketFD());
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    throw MyExceptions(MyExceptions::kIRCServer, "IRCServer::Set() fail");
  }
}

void IRCServer::Run() {
  // start of while loop
  try {
    while (1) {
      poller_.SafeKevent();
      struct kevent* current_event;
      while ((current_event = poller_.GetCurrentEvent())) {
        poller_.CheckErrorFlags(current_event);  // throw internal exception
        if (poller_.IsReadEvent(current_event->filter)) {
          ManageReadEvent(current_event);
        } else if (poller_.IsWriteEvent(current_event->filter)) {
          ManageWriteEvent(current_event);
        }
      }
      CheckTimeout();
    }
  } catch (std::exception& e) {
    throw MyExceptions(MyExceptions::kIRCServer, "IRCServer::Run() fail");
  }
}

void IRCServer::ManageReadEvent(struct kevent* current_event) {
  // server_sock에서-> 새 client 등장
  if (current_event->ident == (uintptr_t)server_socket_.GetSocketFD()) {
    int client_socket_fd = manager_->CreateClient();
    poller_.AddEvent(client_socket_fd, EVFILT_READ);
  } else {  // Client에서 메시지가 왔을떄..
    char buff[4096] = {
        0,
    };
    Client& client = manager_->GetClient(current_event->ident);
    ssize_t n = recv(current_event->ident, buff, sizeof(buff), 0);
    client.UpdateTimeout();
    if (n < 0)
      throw MyExceptions(MyExceptions::kIRCServer, "IRCServer::recv() fail");
    else if (n == 0) {
      poller_.ClearChangeList(current_event->ident);
      manager_->DeleteClient(current_event->ident);
    } else {
      client.SetMessageFromClient(buff);
      Command::Execute(client, *manager_);
    }
  }
}

void IRCServer::ManageWriteEvent(struct kevent* current_event) {
  try {
    Client& client = manager_->GetClient(current_event->ident);
    size_t len = client.GetMessageToClient().size();
    ssize_t result =
        send(current_event->ident, client.GetMessageToClient().c_str(), len, 0);
    if ((size_t)result != len)
      throw MyExceptions(MyExceptions::kIRCServer, "IRCServer::send() fail");
    if (client.IsTimeouted()) {
      poller_.ClearChangeList(current_event->ident);
      manager_->DeleteClient(current_event->ident);
      return;
    }
    client.ClearMessages();
    poller_.DeleteEvent(current_event->ident, EVFILT_WRITE);
  } catch (MyExceptions& e) {
    return;
  }
}

// private *********************************************************************

void IRCServer::CheckTimeout() {
  // check all clients timeout
  // unregistered clients
  for (std::map<int, Client*>::iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    if (it->second->IsTimeout(kTimeout)) {
      it->second->SetTimeouted();
      it->second->SetMessageToClient(MessageMaker::ERR_ClosingLink(
          it->second->GetUserName(), it->second->GetSocketIP(), "Timeout"));
    }
  }
}
