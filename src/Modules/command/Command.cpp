#include "Command.hpp"

void Command::Execute(Client &client, IRCServerManager &server_manager) {
  MessageManager msg_manager;

  if (msg_manager.IsLastCharCRLF(client.GetMessageFromClient()) == false) {
    return;
  }
  msg_manager.ParseMessages(client.GetMessageFromClient());
  for (size_t i = 0; i < msg_manager.GetSplitedStr().size(); i++) {
    msg_manager.ParseMessage(msg_manager.GetSplitedStr()[i]);
    ExecuteCommand(client, msg_manager, server_manager);
    msg_manager.Clear();
  }
  client.ClearMessageFromClient();
}

void Command::ExecuteCommand(Client &client, MessageManager &msg_manager,
                             IRCServerManager &server_manager) {
  std::string command = msg_manager.GetCommand();
  if (command.empty()) return;
  if (command == "PASS") {
    PASS(client, msg_manager);
  } else if (command == "USER") {
    USER(client, msg_manager);
  } else if (command == "NICK") {
    NICK(client, msg_manager, server_manager);
  } else if (client.IsRegistered() == false) {
    ;  // if client is not registered, ignore message
  } else if (command == "JOIN") {
    JOIN(client, msg_manager, server_manager);
  } else if (command == "INVITE") {
    INVITE(client, msg_manager, server_manager);
  } else if (command == "KICK") {
    KICK(client, msg_manager, server_manager);
  } else if (command == "PRIVMSG") {
    PRIVMSG(client, msg_manager, server_manager);
  } else if (command == "PART") {
    PART(client, msg_manager, server_manager);
  } else if (command == "QUIT") {
    QUIT(client, msg_manager, server_manager);
  } else if (command == "TOPIC") {
    TOPIC(client, msg_manager, server_manager);
  } else if (command == "MODE") {
    MODE(client, msg_manager, server_manager);
  } else if (command == "PING") {
    PONG(client, msg_manager);
  } else if (command == "PONG") {
    ;  // ignore
  } else if (command == "WHO") {
    ;
  } else {
    UnknownCMD(client, msg_manager);
  }
}

// connection registration
void Command::USER(Client &client, MessageManager &msg_manager) {
  if (msg_manager.GetParamSize() < 4) {
    client.SetMessageToClient(MessageMaker::ERR_NEEDMOREPARAMS_461(
        client.GetNickname(), msg_manager.GetCommand()));
  } else if (client.IsRegistered()) {
    client.SetMessageToClient(
        MessageMaker::ERR_ALREADYREGISTRED_462(client.GetNickname()));
  } else {
    // hostname and servername are normally ignored by the IRC server
    // when the USER command comes from a directly connected client
    // (for security reasons),
    // but they are used in server to server communication. //RFC 1459
    std::string username = msg_manager.GetParam(0);
    std::string hostname = msg_manager.GetParam(1);
    std::string servername = msg_manager.GetParam(2);
    std::string realname = msg_manager.GetParam(3);

    client.SetUserName(username);
    client.SetRealName(realname);
    ConnectionRegistration(client);
  }
}

void Command::TOPIC(Client &client, MessageManager &msg_manager,
                    IRCServerManager &server_manager) {
  if (msg_manager.GetParamSize() < 1) {
    client.SetMessageToClient(MessageMaker::ERR_NEEDMOREPARAMS_461(
        client.GetNickname(), msg_manager.GetCommand()));
    return;
  }
  std::string channel_name = msg_manager.GetParam(0);
  Channel *channel;
  try {
    channel = &server_manager.GetChannel(channel_name);
  } catch (std::exception &e) {
    client.SetMessageToClient(MessageMaker::ERR_NOSUCHCHANNEL_403(
        client.GetNickname(), channel_name));
    return;
  }
  if (msg_manager.GetParamSize() == 1) {  // Topic을 요청하는 경우
    if (!channel->IsTopicSet()) {         // Topic이 없는 경우
      client.SetMessageToClient(
          MessageMaker::RPL_NOTOPIC_331(client.GetNickname(), channel_name));
      return;
    } else {
      client.SetMessageToClient(  // Topic이 있는 경우
          MessageMaker::RPL_TOPIC_332(client.GetNickname(), channel_name,
                                      channel->GetTopic()) +
          MessageMaker::RPL_TOPICWHOTIME_333(client.GetNickname(), channel_name,
                                             channel->GetTopicSetter(),
                                             channel->GetTopicSetTime()));
      return;
    }
  } else {  // Topic을 설정하는 경우
    if (client.IsInChannel(*channel) ==
        false) {  // Channel에 속해있지 않은 경우
      client.SetMessageToClient(MessageMaker::ERR_NOTONCHANNEL_442(
          client.GetNickname(), channel_name));
      return;
    } else if (channel->IsModeOn(Channel::kModeT) &&
               channel->IsOperator(client) ==
                   false) {  // Channel의 Operator가 아닌 경우
      client.SetMessageToClient(MessageMaker::ERR_CHANOPRIVSNEEDED_482(
          client.GetNickname(), channel_name));
      return;
    } else {  // Topic을 설정하는 경우
      std::string topic = msg_manager.GetParam(1);
      channel->SetTopic(topic, client);
      channel->BroadcastMessage(MessageMaker::RPL_SET_TOPIC_SUCCESS(
          channel->GetTopicSetter(), channel_name, topic));
    }
  }
}

void Command::MODE(Client &client, MessageManager &msg_manager,
                   IRCServerManager &server_manager) {
  // channel mode i should code [i, l, k, t / o]
  if (msg_manager.GetParamSize() < 1) {  // channel name이 없는 경우
    client.SetMessageToClient(MessageMaker::ERR_NEEDMOREPARAMS_461(
        client.GetNickname(), msg_manager.GetCommand()));
    return;
  }
  std::string channel_name = msg_manager.GetParam(0);
  Channel *channel;
  try {
    channel = &server_manager.GetChannel(channel_name);
  } catch (std::exception &e) {  // 채널이 존재하지 않는 경우
    if (MyUtils::IsNickNameValid(channel_name))
      return;
    else
      client.SetMessageToClient(MessageMaker::ERR_NOSUCHCHANNEL_403(
          client.GetNickname(), channel_name));
    return;
  }
  if (msg_manager.GetParamSize() == 1) {  // channel mode를 요청하는 경우
    MODE_Request(client, *channel);
    return;
  }
  std::string mode_setter = client.GetNickname() + "!" + client.GetUserName() +
                            "@" + client.GetSocketIP();
  std::string mode = msg_manager.GetParam(1);
  std::string mode_param = "";
  bool sign = true, success = false;
  if (mode[0] == '-')  // mode가 -로 시작하는 경우
    sign = false;
  if (mode[0] == '+' || mode[0] == '-')  // mode가 + 또는 -로 시작하는 경우
    mode = mode.substr(1);
  if (mode[0] != 'i' && mode[0] != 't' && mode[0] != 'k' && mode[0] != 'l' &&
      mode[0] != 'o') {
    client.SetMessageToClient(
        MessageMaker::ERR_UNKNOWNMODE_472(client.GetNickname(), mode[0]));
    return;  // mode가 존재하지 않는 경우
  } else if (!channel->IsOperator(client.GetSocketFD())) {
    client.SetMessageToClient(MessageMaker::ERR_CHANOPRIVSNEEDED_482(
        client.GetNickname(), channel_name, mode[0]));
    return;  // channel의 operator가 아닌 경우
  }
  if (mode[0] == 'i')  // i mode를 설정하는 경우
    success = MODE_I(*channel, sign);
  else if (mode[0] == 't')  // t mode를 설정하는 경우
    success = MODE_T(*channel, sign);
  else if (mode[0] == 'l' && sign == false)  // l mode를 해제하는 경우
    success = MODE_L(*channel, "", sign);
  else if (msg_manager.GetParamSize() <
           3) {  // mode를 설정하는데 parameter가 부족한 경우
    client.SetMessageToClient(MessageMaker::ERR_NEEDMOREPARAMS_461(
        client.GetNickname(), msg_manager.GetCommand()));
    return;
  } else {
    mode_param = msg_manager.GetParam(2);
    if (mode[0] == 'l' && sign == true)  // l mode를 설정하는 경우
      success = MODE_L(*channel, mode_param, sign);
    if (mode[0] == 'k')  // k mode를 설정하는 경우
      success = MODE_K(client, *channel, mode_param, sign);
    else if (mode[0] == 'o')  // o mode를 설정하는 경우
      success = MODE_O(client, *channel, server_manager, mode_param, sign);
  }
  mode = (sign ? "+" : "-") + std::string(1, mode[0]);
  if (success)
    channel->BroadcastMessage(MessageMaker::RPL_MODE_SUCCESS(
        mode_setter, channel_name, mode, mode_param));
}

void Command::MODE_Request(Client &client,
                           Channel &channel) {  // channel mode를 요청하는 경우
  std::string mode_format = "+";

  if (channel.IsModeOn(Channel::kModeI)) mode_format += "i";
  if (channel.IsModeOn(Channel::kModeT)) mode_format += "t";
  if (channel.IsModeOn(Channel::kModeK)) mode_format += "k";
  if (channel.IsModeOn(Channel::kModeL)) mode_format += "l";
  client.SetMessageToClient(
      MessageMaker::RPL_CHANNELMODEIS_324(client.GetNickname(),
                                          channel.GetName(), mode_format) +
      MessageMaker::RPL_CREATIONTIME_329(
          client.GetNickname(), channel.GetName(), channel.GetChannelTime()));
  return;
}

bool Command::MODE_I(Channel &channel, bool on) {
  if (on == true && channel.IsModeOn(Channel::kModeI) == false) {
    channel.SetModeOn(Channel::kModeI);
    return true;
  } else if (on == false && channel.IsModeOn(Channel::kModeI) == true) {
    channel.SetModeOff(Channel::kModeI);
    return true;
  }
  return false;
}

bool Command::MODE_T(Channel &channel, bool on) {
  if (on == true && channel.IsModeOn(Channel::kModeT) == false) {
    channel.SetModeOn(Channel::kModeT);
    return true;
  } else if (on == false && channel.IsModeOn(Channel::kModeT) == true) {
    channel.SetModeOff(Channel::kModeT);
    return true;
  }
  return false;
}

bool Command::MODE_L(Channel &channel, std::string param, bool on) {
  if (on == false && channel.IsModeOn(Channel::kModeL)) {
    channel.SetModeOff(Channel::kModeL);
    return true;
  } else if (on == true && !channel.IsModeOn(Channel::kModeL)) {
    int count = MyUtils::ConvertStringToNumber(param);
    if (0 < count) {
      channel.SetModeOn(Channel::kModeL);
      channel.SetMaxClientCnt(count);
      return true;
    }
  }
  return false;
}

bool Command::MODE_K(Client &client, Channel &channel, std::string param,
                     bool on) {
  if (on == true && !channel.IsModeOn(Channel::kModeK)) {
    if (MyUtils::IsKeyValid(param) == false)  // key가 유효하지 않은 경우
      return false;
    channel.SetModeOn(Channel::kModeK);
    channel.SetPassword(param);
    return true;
  } else if (on == false && channel.IsModeOn(Channel::kModeK)) {
    if (channel.IsPasswordMatch(param) == true) {  // key가 일치하는 경우
      channel.SetModeOff(Channel::kModeK);
      channel.SetPassword("");
      return true;
    }
    client.SetMessageToClient(MessageMaker::ERR_KEYSET_467(
        client.GetNickname(), channel.GetName()));  // key가 일치하지 않는 경우
  }
  return false;
}

bool Command::MODE_O(Client &client, Channel &channel,
                     IRCServerManager &server_manager, std::string param,
                     bool on) {
  Client *target_client;
  try {
    target_client = &server_manager.GetClient(param);
  } catch (std::exception &e) {  // 존재하지 않는 client인 경우
    client.SetMessageToClient(
        MessageMaker::ERR_NOSUCHNICK_401(client.GetNickname(), param));
    return false;
  }
  if (target_client->IsInChannel(channel) ==
      false) {  // client가 channel에 속해있지 않은 경우
    client.SetMessageToClient(
        MessageMaker::ERR_NOTONCHANNEL_442(param, channel.GetName()));
    return false;
  }
  if (on && channel.IsOperator(*target_client) == false) {
    channel.AddOperator(target_client->GetSocketFD());
    return true;
  } else if (on == false && channel.IsOperator(*target_client) == true) {
    channel.RemoveOperator(target_client->GetSocketFD());
    return true;
  }
  return false;
}

void Command::PASS(Client &client, MessageManager &msg_manager) {
  /*
  if registered == false
  * Password 가 일치하지 않아 464 ERR_PASSWDMISMATCH 보내고
  if registered == true
  * 462 ERR_ALREADYREGISTERED
  */
  if (msg_manager.GetParamSize() == 0) {
    client.SetMessageToClient(
        MessageMaker::ERR_NEEDMOREPARAMS_461(client.GetNickname(), "PASS"));
    return;
  }
  const std::string &client_nick = client.GetNickname();
  if (client.IsRegistered() == false) {
    if (client.GetServer().GetPassword() != msg_manager.GetParam(0)) {
      client.SetMessageToClient(
          MessageMaker::ERR_PASSWDMISMATCH_464(client_nick));
    } else {
      client.SetHasPassword(true);
    }
  } else {
    client.SetMessageToClient(
        MessageMaker::ERR_ALREADYREGISTRED_462(client_nick));
  }
}

void Command::NICK(Client &client, MessageManager &msg_manager,
                   IRCServerManager &server_manager) {
  std::string client_nick = client.GetNickname();
  if (msg_manager.GetParamSize() == 0) {
    client.SetMessageToClient(
        MessageMaker::ERR_NEEDMOREPARAMS_461(client.GetNickname(), "NICK"));
    return;
  }
  const std::string &given_nick = msg_manager.GetParam(0);
  if (client_nick != "*" &&
      client_nick == given_nick) {  // 닉이 이미 있는 유저가 같은 이름으로
                                    // 수정을 시도한 경우 (무시)
    return;
  }
  // 닉이 이상할 경우
  if (MyUtils::IsNickNameValid(given_nick) == false) {
    client.SetMessageToClient(MessageMaker::ERR_ERRONEUSNICKNAME_432(
        client_nick, msg_manager.GetParam(0)));
    return;
  }
  // 서버에 있는 닉으로 시도 할 경우
  if (server_manager.IsClientInServer(given_nick) == true) {
    client.SetMessageToClient(MessageMaker::ERR_NICKNAMEINUSE_433(
        client_nick, msg_manager.GetParam(0)));
    return;
  }
  client.SetNickName(given_nick);
  if (client.IsRegistered() == true) {
    if (client.GetClientChannels().size() > 0) {
      std::vector<int> client_fds;
      std::map<std::string, Channel *>::iterator iter =
          client.GetClientChannels().begin();
      for (; iter != client.GetClientChannels().end(); iter++) {
        const std::map<int, Client *> &clients_in_channel =
            iter->second->GetClientsInChannel();
        std::map<int, Client *>::const_iterator iter2 =
            clients_in_channel.begin();
        for (; iter2 != clients_in_channel.end(); iter2++) {
          if (std::find(client_fds.begin(), client_fds.end(), iter2->first) ==
              client_fds.end())
            client_fds.push_back(iter2->first);
        }
      }
      std::vector<int>::iterator iter3 = client_fds.begin();
      for (; iter3 != client_fds.end(); iter3++) {
        Client &uniq_client = server_manager.GetClient(*iter3);
        uniq_client.SetMessageToClient(MessageMaker::RPL_CHANGE_NICK_SUCCESS(
            client_nick, client.GetUserName(), client.GetSocketIP(),
            given_nick));
      }
    } else {
      client.SetMessageToClient(MessageMaker::RPL_CHANGE_NICK_SUCCESS(
          client_nick, client.GetUserName(), client.GetSocketIP(), given_nick));
    }
  } else {
    ConnectionRegistration(client);
  }
}

void Command::QUIT(Client &client, MessageManager &msg_manager,
                   IRCServerManager &server_manager) {
  // 클라이언트가 속한 채널 모두 돌면서
  std::map<std::string, Channel *> &client_channels =
      client.GetClientChannels();
  std::map<std::string, Channel *>::iterator iter = client_channels.begin();
  std::vector<int> client_fds;
  for (; iter != client_channels.end(); iter++) {
    Channel &channel = *(iter->second);
    if (channel.IsInvitedClient(client.GetSocketFD()))
      channel.RemoveInvitedClient(client.GetSocketFD());
    if (channel.IsOperator(client)) {
      channel.RemoveOperator(client.GetSocketFD());
    }
    const std::map<int, Client *> &clients_in_channel =
        channel.GetClientsInChannel();
    for (std::map<int, Client *>::const_iterator iter =
             clients_in_channel.begin();
         iter != clients_in_channel.end(); iter++) {
      int client_fd = iter->first;
      if (client_fd != client.GetSocketFD() &&
          std::find(client_fds.begin(), client_fds.end(), client_fd) ==
              client_fds.end())
        client_fds.push_back(client_fd);
    }
  }
  std::string comment = "Quit :leaving";
  if (msg_manager.GetParamSize() > 0) comment = msg_manager.GetParam(0);
  for (std::vector<int>::iterator iter = client_fds.begin();
       iter != client_fds.end(); iter++) {
    Client &target_client = server_manager.GetClient(*iter);
    target_client.SetMessageToClient(MessageMaker::RPL_QUIT_SUCCESS(
        client.GetNickname(), client.GetUserName(), client.GetSocketIP(),
        comment));
  }
  client.SetMessageToClient(MessageMaker::ERR_ClosingLink(
      client.GetUserName(), client.GetSocketIP(), comment));
  client.SetTimeouted();
}

void Command::JOIN(Client &client, MessageManager &msg_manager,
                   IRCServerManager &server_manager) {
  // JOIN #channel (key)
  // channel이 있/없 : server의 channel을 조회하여 이름이 일치하는 채널이 있는지
  // 확인한다
  if (client.IsRegistered() == false) return;
  if (msg_manager.GetParamSize() < 1) {
    client.SetMessageToClient(
        MessageMaker::ERR_NEEDMOREPARAMS_461(client.GetNickname(), "JOIN"));
    return;
  }
  const std::string &client_nickname = client.GetNickname();
  const std::string &channel_name = msg_manager.GetParam(0);
  if (MyUtils::IsChannelNameValid(channel_name) == false) {
    client.SetMessageToClient(
        MessageMaker::ERR_BADCHANMASK_476(client_nickname, channel_name));
    return;
  }
  if (!server_manager.IsChannelInServer(
          channel_name)) {  // channel이 없는 경우 : 만들어서 쪼인
    server_manager.CreateChannel(channel_name);
    Channel &channel = server_manager.GetChannel(channel_name);
    server_manager.AddClientToChannel(client, channel_name);
    channel.AddOperator(client.GetSocketFD());

    channel.BroadcastMessage(
        MessageMaker::RPL_JOIN_SUCCESS(client_nickname, client.GetUserName(),
                                       client.GetSocketIP(), channel_name));
    client.SetMessageToClient(
        MessageMaker::RPL_NAMREPLY_353(client_nickname, channel));
    client.SetMessageToClient(
        MessageMaker::RPL_ENDOFNAMES_366(client_nickname, channel_name));

  } else {
    Channel &channel = server_manager.GetChannel(channel_name);
    if (server_manager.IsClientInChannel(client, channel_name) == true) return;
    if (channel.IsModeOn(Channel::kModeL) &&
        (channel.GetClientsInChannel().size() >=
         (size_t)channel.GetMaxClientCntInChannel())) {
      client.SetMessageToClient(
          MessageMaker::ERR_CHANNELISFULL_471(client_nickname, channel_name));
      return;
    }
    if (channel.IsModeOn(Channel::kModeK)) {
      if (msg_manager.GetParamSize() == 1 ||
          !channel.IsPasswordMatch(msg_manager.GetParam(1))) {
        client.SetMessageToClient(
            MessageMaker::ERR_BADCHANNELKEY_475(client_nickname, channel_name));
        return;
      }
    }
    if (channel.IsModeOn(Channel::kModeI)) {  // invite-only OOOO
      if (channel.IsInvitedClient(client.GetSocketFD()) ==
          false) {  // invite 안됨
        client.SetMessageToClient(MessageMaker::ERR_INVITEONLYCHAN_473(
            client_nickname, channel_name));
        return;
      }
    }
    server_manager.AddClientToChannel(client, channel);

    channel.BroadcastMessage(
        MessageMaker::RPL_JOIN_SUCCESS(client_nickname, client.GetUserName(),
                                       client.GetSocketIP(), channel_name));
    client.SetMessageToClient(
        MessageMaker::RPL_NAMREPLY_353(client_nickname, channel));
    client.SetMessageToClient(
        MessageMaker::RPL_ENDOFNAMES_366(client_nickname, channel_name));
  }
}

void Command::PART(Client &client, MessageManager &msg_manager,
                   IRCServerManager &server_manager) {
  if (msg_manager.GetParamSize() < 1) {
    client.SetMessageToClient(
        MessageMaker::ERR_NEEDMOREPARAMS_461(client.GetNickname(), "PART"));
    return;
  }
  const std::string &channel_name = msg_manager.GetParam(0);
  Channel *channel;
  try {
    channel = &server_manager.GetChannel(channel_name);
  } catch (MyExceptions &e) {
    client.SetMessageToClient(MessageMaker::ERR_NOSUCHCHANNEL_403(
        client.GetNickname(), channel_name));
    return;
  }
  if (server_manager.IsClientInChannel(client.GetSocketFD(),
                                       channel->GetName()) == false) {
    client.SetMessageToClient(MessageMaker::ERR_NOTONCHANNEL_442(
        client.GetNickname(), channel->GetName()));
    return;
  }
  std::string comment = "";
  if (msg_manager.GetParamSize() > 1) comment = msg_manager.GetParam(1);
  channel->BroadcastMessage(MessageMaker::RPL_PART_SUCCESS(
      client.GetNickname(), client.GetUserName(), client.GetSocketIP(),
      channel->GetName(), comment));
  if (channel->IsInvitedClient(client.GetSocketFD()))
    channel->RemoveInvitedClient(client.GetSocketFD());
  if (channel->IsOperator(client.GetSocketFD()))
    channel->RemoveOperator(client.GetSocketFD());
  server_manager.RemoveClientFromChannel(client, *channel);
}

void Command::KICK(Client &client, MessageManager &msg_manager,
                   IRCServerManager &server_manager) {
  if (msg_manager.GetParamSize() < 2) {
    client.SetMessageToClient(
        MessageMaker::ERR_NEEDMOREPARAMS_461(client.GetNickname(), "PART"));
    return;
  }
  const std::string &client_nick = client.GetNickname();
  const std::string &channel_name = msg_manager.GetParam(0);
  Channel *channel;
  try {
    channel = &server_manager.GetChannel(channel_name);
  } catch (MyExceptions &e) {
    client.SetMessageToClient(
        MessageMaker::ERR_NOSUCHCHANNEL_403(client_nick, channel_name));
    return;
  }
  if (server_manager.IsClientInChannel(client, channel->GetName()) == false) {
    client.SetMessageToClient(
        MessageMaker::ERR_NOTONCHANNEL_442(client_nick, channel->GetName()));
    return;
  }
  //  irc.local 401 jeyung jiyuuuun :No such nick
  const std::string &target_nick = msg_manager.GetParam(1);
  Client *target_client;
  try {
    target_client = &server_manager.GetClient(msg_manager.GetParam(1));
  } catch (MyExceptions &e) {
    client.SetMessageToClient(
        MessageMaker::ERR_NOSUCHNICK_401(client_nick, target_nick));
    return;
  }
  if (server_manager.IsClientInChannel(*target_client, channel_name) == false) {
    client.SetMessageToClient(MessageMaker::ERR_USERNOTINCHANNEL_441(
        client_nick, target_nick, channel->GetName()));
    return;
  }
  //  :irc.local 482 jeyung #hi :You must be a channel half-operator
  if (!channel->IsOperator(client))
    client.SetMessageToClient(MessageMaker::ERR_CHANOPRIVSNEEDED_482(
        client_nick, channel->GetName()));
  else {
    // 채널에서 target 삭제

    std::string comment = "";
    if (msg_manager.GetParamSize() > 2) comment = msg_manager.GetParam(2);
    channel->BroadcastMessage(MessageMaker::RPL_KICK_SUCCESS(
        client_nick, client.GetUserName(), client.GetSocketIP(), channel_name,
        target_client->GetNickname(), comment));
    if (channel->IsInvitedClient(target_client->GetSocketFD()))
      channel->RemoveInvitedClient(target_client->GetSocketFD());
    if (channel->IsOperator(target_client->GetSocketFD()))
      channel->RemoveOperator(target_client->GetSocketFD());
    server_manager.RemoveClientFromChannel(target_client->GetNickname(),
                                           channel->GetName());
  }
}

void Command::PRIVMSG(Client &client, MessageManager &msg_manager,
                      IRCServerManager &server_manager) {
  if (msg_manager.GetParamSize() < 2) {
    client.SetMessageToClient(MessageMaker::ERR_NEEDMOREPARAMS_461(
        client.GetNickname(), msg_manager.GetCommand()));
  } else if (msg_manager.GetParam(0)[0] == '#') {  // channel
    if (server_manager.IsChannelInServer(msg_manager.GetParam(0)) == false)
      client.SetMessageToClient(MessageMaker::ERR_NOSUCHCHANNEL_403(
          client.GetNickname(), msg_manager.GetParam(0)));
    else if (server_manager.IsClientInChannel(client.GetSocketFD(),
                                              msg_manager.GetParam(0)) ==
             false)  // Getnick => getParam
      client.SetMessageToClient(MessageMaker::ERR_CANNOTSENDTOCHAN_404(
          client.GetNickname(), msg_manager.GetParam(0)));
    else {
      Channel &channel = server_manager.GetChannel(msg_manager.GetParam(0));
      if (msg_manager.GetParam(1).empty())
        client.SetMessageToClient(
            MessageMaker::ERR_NOTEXTTOSEND_412(client.GetNickname()));
      else  // reply to clinets in channel
        channel.BroadcastMessage(
            MessageMaker::RPL_AWAY_301(
                client.GetNickname(), client.GetUserName(),
                client.GetSocketIP(), msg_manager.GetParam(0),
                msg_manager.GetParam(1)),
            client.GetSocketFD());
    }
  } else {  // client
    if (server_manager.IsClientInServer(msg_manager.GetParam(0)) == false)
      client.SetMessageToClient(MessageMaker::ERR_NOSUCHNICK_401(
          client.GetNickname(), msg_manager.GetParam(0)));
    else {
      Client &target_client = server_manager.GetClient(msg_manager.GetParam(0));
      if (msg_manager.GetParam(1).empty())
        client.SetMessageToClient(
            MessageMaker::ERR_NOTEXTTOSEND_412(client.GetNickname()));
      else  // reply to target client
        target_client.SetMessageToClient(MessageMaker::RPL_AWAY_301(
            client.GetNickname(), client.GetUserName(), client.GetSocketIP(),
            msg_manager.GetParam(0), msg_manager.GetParam(1)));
    }
  }
}

// utils
void Command::UnknownCMD(Client &client, MessageManager &msg_manager) {
  client.SetMessageToClient(MessageMaker::ERR_UNKNOWNCOMMAND_421(
      client.GetNickname(), msg_manager.GetCommand()));
}

// connection register utils
void Command::ConnectionRegistration(Client &client) {
  if (CanClientRegister(client) == true) {
    if (client.HasPassword() == true)
      RegisterClient(client);
    else {
      client.SetMessageToClient(MessageMaker::ERR_ClosingLink(
          client.GetUserName(), client.GetSocketIP(),
          "Access denied by wrong password"));
      client.SetTimeouted();
    }
  }
}

bool Command::CanClientRegister(Client &client) {
  if (client.GetNickname() != "*" && client.GetUserName() != "") return true;
  return false;
}

void Command::RegisterClient(Client &client) {
  client.SetRegistered(true);
  // set wellcome messages
  std::string username = client.GetUserName();
  std::string hostname = client.GetSocketIP();
  std::string server_name = client.GetServer().GetServerName();
  std::string server_version = client.GetServer().GetServerVersion();
  std::string user_modes_ = client.GetServer().GetUserModes();
  std::string chan_modes_ = client.GetServer().GetChanModes();

  client.SetMessageToClient(
      MessageMaker::RPL_WELCOME_001(client.GetNickname(), username, hostname));
  client.SetMessageToClient(MessageMaker::RPL_YOURHOST_002(
      client.GetNickname(), server_name, server_version));
  client.SetMessageToClient(
      MessageMaker::RPL_CREATED_003(client.GetNickname()));
  client.SetMessageToClient(MessageMaker::RPL_MYINFO_004(
      client.GetNickname(), server_version, user_modes_, chan_modes_));
  client.SetMessageToClient(
      MessageMaker::RPL_MOTDSTART_375(client.GetNickname()));
  client.SetMessageToClient(MessageMaker::RPL_MOTD_Wellcome_372());
  client.SetMessageToClient(
      MessageMaker::RPL_ENDOFMOTD_376(client.GetNickname()));
}

// INVITE <nickname> <channel>
void Command::INVITE(Client &client, MessageManager &msg_manager,
                     IRCServerManager &server_manager) {
  // Check param under 2
  // invite nick #channel
  if (msg_manager.GetParamSize() < 2) {
    client.SetMessageToClient(
        MessageMaker::RPL_ENDOFINVITELIST_337(client.GetNickname()));
    return;
  }
  const std::string &sender_nick = client.GetNickname();
  const std::string &receiver_nick = msg_manager.GetParam(0);
  const std::string &channel_name = msg_manager.GetParam(1);

  // Check valid channel
  if (server_manager.IsChannelInServer(channel_name) == false) {
    client.SetMessageToClient(
        MessageMaker::ERR_NOSUCHCHANNEL_403(sender_nick, channel_name));
  }
  // Check to exist sender in channel
  else if (server_manager.IsClientInChannel(client.GetSocketFD(),
                                            channel_name) == false) {
    client.SetMessageToClient(
        MessageMaker::ERR_NOTONCHANNEL_442(sender_nick, channel_name));
  }
  // Check name of receiver
  else if (server_manager.IsClientInServer(receiver_nick) == false) {
    client.SetMessageToClient(
        MessageMaker::ERR_NOSUCHNICK_401(sender_nick, receiver_nick));
  }
  // Check user that is operator in channel
  else if (server_manager.GetChannel(channel_name).IsOperator(client) ==
           false) {
    client.SetMessageToClient(
        MessageMaker::ERR_CHANOPRIVSNEEDED_482(sender_nick, channel_name));
  }
  // Check to exist receiver already in channel
  else if (server_manager.IsClientInChannel(
               server_manager.GetClient(receiver_nick).GetSocketFD(),
               channel_name) == true) {
    client.SetMessageToClient(MessageMaker::ERR_USERONCHANNEL_443(
        sender_nick, receiver_nick, channel_name));
  }
  // send invite message to receiver
  else {
    Channel &channel = server_manager.GetChannel(channel_name);
    client.SetMessageToClient(MessageMaker::RPL_INVITING_341(
        sender_nick, receiver_nick, channel_name));

    const std::map<int, Client *> &clients_in_channel =
        channel.GetClientsInChannel();
    std::map<int, Client *>::const_iterator iter = clients_in_channel.begin();
    for (; iter != clients_in_channel.end(); iter++) {
      if (iter->first != server_manager.GetClient(sender_nick).GetSocketFD() &&
          iter->first != server_manager.GetClient(receiver_nick).GetSocketFD())
        iter->second->SetMessageToClient(MessageMaker::INVITE_NOTICE(
            channel_name, sender_nick, receiver_nick));
    }

    server_manager.GetClient(receiver_nick)
        .SetMessageToClient(MessageMaker::RPL_INVITE_SUCCESS(
            sender_nick, receiver_nick, client.GetUserName(),
            client.GetSocketIP(), channel_name));
    // Add invited client fd to client vector in channelf
    channel.AddInvitedClient(
        server_manager.GetClient(receiver_nick).GetSocketFD());
  }
}

// if command == "PING"
void Command::PONG(Client &client, MessageManager &msg_manager) {
  if (msg_manager.GetParamSize() < 1) {
    client.SetMessageToClient(MessageMaker::ERR_NEEDMOREPARAMS_461(
        client.GetNickname(), msg_manager.GetCommand()));
  } else {
    client.SetMessageToClient(
        MessageMaker::RPL_PONG(client.GetNickname(), msg_manager.GetParam(0)));
  }
}
