#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#define LOCAL_HOST "127.0.0.1"

#include "Channel.hpp"
#include "Client.hpp"
#include "IRCServerManager.hpp"
#include "Message.hpp"
#include "MessageMaker.hpp"
#include "MessageManager.hpp"

class Command {
 public:
  static void Execute(Client& client, IRCServerManager& server_manager);
  static void ExecuteCommand(Client& client, MessageManager& msg_manager,
                             IRCServerManager& server_manager);

 private:
  // connection registration
  static void PASS(Client& client, MessageManager& msg_manager);
  static void NICK(Client& client, MessageManager& msg_manager,
                   IRCServerManager& server_manager);
  static void USER(Client& client, MessageManager& msg_manager);
  static void QUIT(Client& client, MessageManager& msg_manager,
                   IRCServerManager& server_manager);

  // channel operator
  static void JOIN(Client& client, MessageManager& msg_manager,
                   IRCServerManager& server_manager);
  static void PART(Client& client, MessageManager& msg_manager,
                   IRCServerManager& server_manager);
  static void TOPIC(Client& client, MessageManager& msg_manager,
                    IRCServerManager& server_manager);
  static void INVITE(Client& client, MessageManager& msg_manager,
                     IRCServerManager& server_manager);
  static void KICK(Client& client, MessageManager& msg_manager,
                   IRCServerManager& server_manager);
  static void PRIVMSG(Client& client, MessageManager& msg_manager,
                      IRCServerManager& server_manager);
  static void MODE(Client& client, MessageManager& msg_manager, IRCServerManager& server_manager);
  static void MODE_Request(Client& client, Channel& channel);
  static bool MODE_I(Channel& channel, bool on);
  static bool MODE_T(Channel& channel, bool on);
  static bool MODE_L(Channel& channel, std::string param, bool on);
  static bool MODE_O(Client& client, Channel& channel, IRCServerManager& server_manager, std::string param, bool on); 
  static bool MODE_K(Client& client, Channel& channel, std::string param, bool on);
  
  // utils
  static void PONG(Client& client, MessageManager& msg_manager); // jiwolee
  static void UnknownCMD(Client& client, MessageManager& msg_manager);  // jiyun

  // connetcion registration utils
  static void ConnectionRegistration(Client& client);
  static bool CanClientRegister(Client& client);
  static void RegisterClient(Client& client);
};

#endif
