#include "MessageMaker.hpp"

std::string MessageMaker::server_name_ = "Was_WebServ";
std::string MessageMaker::server_creation_date_ = MyString::GetPreciseTime();

// connection register
std::string MessageMaker::RPL_WELCOME_001(const std::string& nick, const std::string& user, const std::string& host) {
	return ":" + server_name_ + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n";
}

std::string MessageMaker::RPL_YOURHOST_002(const std::string& nick, const std::string& server_name, const std::string& version) {
	return ":" + server_name_ + " 002 " + nick + " :Your host is " + server_name + ", running version " + version + "\r\n";
}

std::string MessageMaker::RPL_CREATED_003(const std::string& nick) {
	return ":" + server_name_ + " 003 " + nick + " :This server was created " + server_creation_date_ + "\r\n";
}

std::string MessageMaker::RPL_MYINFO_004(const std::string& nick, const std::string& version, const std::string& user_modes, const std::string&chan_modes) {
	return ":" + server_name_ + " 004 " + nick + " " + version + " " + user_modes + " " + chan_modes + "\r\n";
}

// PASS
std::string MessageMaker::ERR_PASSWDMISMATCH_464(const std::string& nick) {
	return ":" + server_name_ + " 464 " + nick + " :the given password was incorrect\r\n";
}

// NICK
std::string MessageMaker::ERR_ERRONEUSNICKNAME_432(const std::string& nick, const std::string& weird_nick) {
	return ":" + server_name_ + " 432 " + nick + " " + weird_nick + " :Erroneous Nickname\r\n";
}

std::string MessageMaker::ERR_NICKNAMEINUSE_433(const std::string& nick, const std::string& dup_nick) {
	return ":" + server_name_ + " 433 " + nick + " " + dup_nick + " :Nickname is already in use.\r\n";
}

//JOIN
//:irc.local 473 jiwo #hi :Cannot join channel (invite only)
std::string MessageMaker::ERR_INVITEONLYCHAN_473(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 473 " + nick + " " + channel_name + " :Cannot join channel (invite only)\r\n";
}

//:irc.local 475 jiwo #hi :Cannot join channel (incorrect channel key)
std::string MessageMaker::ERR_BADCHANNELKEY_475(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 475 " + nick + " " + channel_name + " :Cannot join channel (incorrect channel key)\r\n";
}

//:irc.local 471 jiwo #hi :Cannot join channel (channel is full)
std::string MessageMaker::ERR_CHANNELISFULL_471(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 471 " + nick + " " + channel_name + " :Cannot join channel (channel is full)\r\n";
}

//:irc.local 476 jiyun hi :Invalid channel name
std::string MessageMaker::ERR_BADCHANMASK_476(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 476 " + nick + " " + channel_name + " :Invalid channel name\r\n";
}

// TOPIC
std::string MessageMaker::RPL_NOTOPIC_331(const std::string& nick, const std::string& channel) {
	return ":" + server_name_ + " 331 " + nick + " " + channel + " :No topic is set\r\n";
}
std::string MessageMaker::RPL_TOPIC_332(const std::string& nick, const std::string& channel, const std::string& topic) {
	return ":" + server_name_ + " 332 " + nick + " " + channel + " :" + topic + "\r\n";
}
std::string MessageMaker::RPL_TOPICWHOTIME_333(const std::string& nick, const std::string& channel, const std::string& setter, const std::string& time) {
	return ":" + server_name_ + " 333 " + nick + " " + channel + " " + setter + " " + time + "\r\n";
}

// MODE
std::string MessageMaker::RPL_CHANNELMODEIS_324(const std::string& nick, const std::string& channel, const std::string& mode){
	return ":" + server_name_ + " 324 " + nick + " " + channel + " :" + mode + "\r\n";
}

std::string MessageMaker::RPL_CREATIONTIME_329(const std::string& nick, const std::string& channel, const std::string& time){
	return ":" + server_name_ + " 329 " + nick + " " + channel + " :" + time + "\r\n";
}

std::string MessageMaker::ERR_UNKNOWNMODE_472(const std::string& nick, const char mode){
	return ":" + server_name_ + " 472 " + nick + " " + mode + " :is not a recognised channel mode.\r\n";
}

// :irc.local 467 jiyun #hi :Channel key already set
std::string MessageMaker::ERR_KEYSET_467(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 467 " + nick + " " + channel_name + " :Channel key already set\r\n";
}


// PRIVMSG
std::string MessageMaker::ERR_CANNOTSENDTOCHAN_404(const std::string& nick, const std::string& channel_name) {
	return ":" + server_name_ + " 404 " + nick + " " + channel_name + " :Cannot send to channel\r\n";
}

std::string MessageMaker::ERR_NOTEXTTOSEND_412(const std::string& nick) {
	return ":" + server_name_ + " 412 " + nick + " :No text to send\r\n";
}

std::string MessageMaker::RPL_AWAY_301(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& target, const std::string& away_message) {
	return ":" + nick + "!" + user_name + "@" + ip + " PRIVMSG " + target + " :" + away_message + "\r\n";
}

// Utils
// When Client say PING, Server say PONG
std::string MessageMaker::RPL_PONG(const std::string& server_name, const std::string& message) {
	return ":" + server_name_ + " PONG [" + server_name + "] :" + message + "\r\n";
}

//"ERROR :Closing Link: ["+ it->second->GetSocketIP() + "] (Ping timeout)"
std::string MessageMaker::ERR_ClosingLink(const std::string& user, const std::string& ip, const std::string& error_msg) {
	return ":" + server_name_ + " ERROR :Closing Link: (" + user + "@" + ip + ") [" + error_msg + "]\r\n";
}
//Ping timeout

// ALL
std::string MessageMaker::ERR_NOSUCHNICK_401(const std::string& nick, const std::string& target) {
	return ":" + server_name_ + " 401 " + nick + " " + target + " :No such nick\r\n";
}

/* :irc.local 403 jeyung #hello :No such channel
   :<server nickname> 403 <주도자 nickname> # <잘못 입력한 채널> :No such channel */
std::string MessageMaker::ERR_NOSUCHCHANNEL_403(const std::string& nick, const std::string& channel) {
	return ":" + server_name_ + " 403 " + nick + " " + channel + " :No such channel\r\n";
}

std::string MessageMaker::ERR_USERNOTINCHANNEL_441(const std::string& nick, const std::string& target_nick, const std::string& channel_name) {
  return ":" + server_name_ + " 441 " + nick + " " + target_nick + " " + channel_name + " :They are not on that channel\r\n";
}

//:irc.local 442 jiwo #hi :You're not on that channel
std::string MessageMaker::ERR_NOTONCHANNEL_442(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 442 " + nick + " " + channel_name + " :You're not on that channel\r\n";
}

std::string MessageMaker::ERR_NEEDMOREPARAMS_461(const std::string& nick, const std::string& command) {
  return ":" + server_name_ + " 461 " + nick + " " + command + " :Not enough parameters\r\n";
}

std::string MessageMaker::ERR_ALREADYREGISTRED_462(const std::string& nick) {
  return ":" + server_name_ + " 462 " + nick + " :You may not reregister\r\n";
}

std::string MessageMaker::ERR_CHANOPRIVSNEEDED_482(const std::string& nick, const std::string& channel, const char mode) {
	return ":" + server_name_ + " 482 " + nick + " " + channel + " :You must have channel op access or above to set channel mode "
	 + std::string(1, mode)  + "\r\n";
}

std::string MessageMaker::ERR_CHANOPRIVSNEEDED_482(const std::string& nick, const std::string& channel) {
	return ":" + server_name_ + " 482 " + nick + " " + channel + " :You're not channel operator\r\n";
}

std::string MessageMaker::ERR_UNKNOWNCOMMAND_421(const std::string& nick, const std::string& command) {
	return ":" + server_name_ + " 421 " + nick + " " + command + " :Unknown command\r\n";
}

//성공시****************************************************************************************************************************************

//NICK
// :jiyun!root@127.0.0.1 NICK :jiyu
std::string MessageMaker::RPL_CHANGE_NICK_SUCCESS(const std::string& before_nick, const std::string& user_name, const std::string& ip, const std::string& after_nick) {
  return ":" + before_nick + "!" + user_name + "@" + ip + " NICK :" + after_nick + "\r\n";
}

//QUIT
// :jiwo!root@127.0.0.1 QUIT :Quit: gone to lunch
// :nick!jiwoo@localhost QUIT :Quit: hello
std::string MessageMaker::RPL_QUIT_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& comment) {
  if (!comment.empty())
	return ":" + nick + "!" + user_name + "@" + ip + " QUIT :" + comment + "\r\n";
  else
	return ":" + nick + "!" + user_name + "@" + ip + " QUIT :leaving\r\n";
}

//PART
//:jiyun!root@127.0.0.1 PART #hi :comment
std::string MessageMaker::RPL_PART_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name, const std::string& comment) {
  if (!comment.empty())
	return ":" + nick + "!" + user_name + "@" + ip + " PART :" + channel_name + comment + "\r\n";
  else
	return ":" + nick + "!" + user_name + "@" + ip + " PART :" + channel_name + "\r\n";
}

//JOIN
/*:jiwo!root@127.0.0.1 JOIN :#hi
:irc.local 353 jiwo = #hi :@jiyun jiwo
:irc.local 366 jiwo #hi :End of /NAMES list.

:jiwo!root@127.0.0.1 JOIN :#hi 한번 더 나오는 이유가 있을까? */
std::string MessageMaker::RPL_JOIN_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name) {
  return ":" + nick + "!" + user_name + "@" + ip + " JOIN :" + channel_name + "\r\n";
}

std::string MessageMaker::RPL_NAMREPLY_353(const std::string& nick, Channel& channel) {
  std::string ret(":" + server_name_ + " 353 " + nick + " = " + channel.GetName() + " :");
  std::vector<std::string> client_list;
  std::map<int, Client *>::const_iterator iter = channel.GetClientsInChannel().begin();
  for (; iter != channel.GetClientsInChannel().end(); iter++) {
	if (channel.IsOperator(*iter->second))
	  client_list.push_back("@" + iter->second->GetNickname());
	else
	  client_list.push_back(iter->second->GetNickname());
  }
  std::vector<std::string>::iterator iter2 = client_list.begin();
  for (; iter2 != client_list.end(); iter2++) {
	if (iter2 + 1 == client_list.end())
	  ret += *iter2;
	else
	  ret += *iter2 + " ";
  }
  ret += "\r\n";
  //std::string result = *ret;
  //delete ret;
  return ret;
}

std::string MessageMaker::RPL_ENDOFNAMES_366(const std::string& nick, const std::string& channel_name) {
  return ":" + server_name_ + " 366 " + nick + " " + channel_name + " :End of /NAMES list.\r\n";
}

//MODE
//MODE #hi +l 3
//:dog!root@127.0.0.1 MODE #hi +l :3
//MODE #hi +k 4
//:dog!root@127.0.0.1 MODE #hi +k :4

std::string MessageMaker::RPL_MODE_SUCCESS(const std::string& mode_setter, const std::string& channel_name, const std::string& mode, const std::string& mode_param) {
  if (mode_param.empty())
	return ":" + mode_setter + " MODE " + channel_name + " " + mode + "\r\n";
  else
	return ":" + mode_setter + " MODE " + channel_name + " " + mode + " :" + mode_param + "\r\n";
}

//KICK
//:jiyun!root@127.0.0.1 KICK #hi hakim :so bad boy
// ':' 붙여주기
std::string MessageMaker::RPL_KICK_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name, const std::string& target_nick, const std::string& comment) {
  if (!comment.empty())
	return ":" + nick + "!" + user_name + "@" + ip + " KICK :" + channel_name + " " + target_nick + " :" + comment + "\r\n";
  else
	return ":" + nick + "!" + user_name + "@" + ip + " KICK :" + channel_name + " " + target_nick + "\r\n";
}

// INVITE
std::string MessageMaker::RPL_ENDOFINVITELIST_337(const std::string& nick) {
	return ":" + server_name_ + " 337 " + nick + " :End of invite list\r\n";
}

std::string MessageMaker::RPL_INVITING_341(const std::string& sender_nick, const std::string& receiver_nick, const std::string& channel_name) {
	return ":" + server_name_ + " 341 " + sender_nick + " " + receiver_nick + " :" + channel_name + "\r\n";
}

std::string MessageMaker::ERR_USERONCHANNEL_443(const std::string& sender_nick, const std::string& receiver_nick, const std::string& channel_name) {
	return ":" + server_name_ + " 443 " + sender_nick + " " + receiver_nick + " " + channel_name + " :is already on channel\r\n";
}

// 127.000.000.001.06667-127.000.000.001.49352: :horse!jayoon@localhost INVITE cat :#a
std::string MessageMaker::RPL_INVITE_SUCCESS(const std::string& sender_nick, const std::string& receiver_nick, const std::string& user_name, const std::string& ip, const std::string& channel_name) {
	return ":" + sender_nick + "!" + user_name + "@" + ip + " INVITE " + receiver_nick + " :" + channel_name + "\r\n";
}

//:irc.local NOTICE #hi :*** dog invited snake into the channel
std::string MessageMaker::INVITE_NOTICE(const std::string& channel_name, const std::string& sender_nick, const std::string& receiver_nick) {
  return ":" + server_name_ + " NOTICE " + channel_name + " :*** " + sender_nick + " invited " + receiver_nick + " into the channel\r\n";
}
std::string MessageMaker::RPL_SET_TOPIC_SUCCESS(const std::string& nick, const std::string& channel_name, const std::string& topic) {
	return ":" + nick + " TOPIC " + channel_name + " :" + topic + "\r\n";
}

std::string MessageMaker::RPL_MOTDSTART_375(const std::string& nick) {
	return ":" + server_name_ + " 357 " + nick + " :" + server_name_ + " message of the day\r\n";
}

std::string MessageMaker::RPL_ENDOFMOTD_376(const std::string& nick) {
	return ":" + server_name_ + " 357 " + nick + " :End of message of the day.\r\n"; 
}

std::string MessageMaker::RPL_MOTD_Wellcome_372() {
	return \
":" + server_name_ + " 372 " + "nick :" + " ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\r\n" \
":" + server_name_ + " 372 " + "nick :" + " '·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.'\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |                                                                                  |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |       ▄▄▌ ▐ ▄▌ ▄▄▄· .▄▄ ·         ▄▄▌ ▐ ▄▌▄▄▄ .▄▄▄▄· .▄▄ · ▄▄▄ .▄▄▄   ▌ ▐·       |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |       ██· █▌▐█▐█ ▀█ ▐█ ▀.         ██· █▌▐█▀▄.▀·▐█ ▀█▪▐█ ▀. ▀▄.▀·▀▄ █·▪█·█▌       |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |       ██▪▐█▐▐▌▄█▀▀█ ▄▀▀▀█▄        ██▪▐█▐▐▌▐▀▀▪▄▐█▀▀█▄▄▀▀▀█▄▐▀▀▪▄▐▀▀▄ ▐█▐█•       |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |       ▐█▌██▐█▌▐█ ▪▐▌▐█▄▪▐█        ▐█▌██▐█▌▐█▄▄▌██▄▪▐█▐█▄▪▐█▐█▄▄▌▐█•█▌ ███        |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |        ▀▀▀▀ ▀▪ ▀  ▀  ▀▀▀▀          ▀▀▀▀ ▀▪ ▀▀▀ ·▀▀▀▀  ▀▀▀▀  ▀▀▀ .▀  ▀. ▀         |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |                                                                                  |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |                   WELCOME TO OUR IRC PROJECT \"WAS WEBSERV\"                       |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |                                                                                  |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |     This project was the WEBSERV but there is a very sad story behind it.        |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |     We were working on Webserv project for a month,                              |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |     but we were not able to finish it on time. (especially jchoi)                |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |     So we decided to switch to another project IRC.                              |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |                                                                                  |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |     Team members: jayoon, jchoi, jiyun, jiwolee, seoyoo                          |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " |                                                                                  |\r\n" \
":" + server_name_ + " 372 " + "nick :" + " '·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.·'·.'\r\n" \
":" + server_name_ + " 372 " + "nick :" + "   '   '   '   '   '   '   '   '   '   '   '   '   '   '   '   '   '   '   '   '   ' \r\n";
}
