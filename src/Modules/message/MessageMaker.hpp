#ifndef MESSAGEMAKER_HPP_
#define MESSAGEMAKER_HPP_

#include <string>

#include "MyUtils.hpp"

#include "Channel.hpp"

class MessageMaker{
	std::string message;

  public:
	static std::string server_name_;
    static std::string server_creation_date_;

// connection register
    static std::string RPL_WELCOME_001(const std::string& nick, const std::string& user, const std::string& host);
    static std::string RPL_YOURHOST_002(const std::string& nick, const std::string& server_name, const std::string& version);
    static std::string RPL_CREATED_003(const std::string& nick);
    static std::string RPL_MYINFO_004(const std::string& nick, const std::string& version, const std::string& user_modes, const std::string& chan_modes);
    static std::string RPL_MOTDSTART_375(const std::string& nick);
    static std::string RPL_ENDOFMOTD_376(const std::string& nick);
    static std::string RPL_MOTD_Wellcome_372();

// PASS
    static std::string ERR_PASSWDMISMATCH_464(const std::string& nick);
    
// NICK
    static std::string ERR_ERRONEUSNICKNAME_432(const std::string& nick, const std::string& weird_nick);
    static std::string ERR_NICKNAMEINUSE_433(const std::string& nick, const std::string& dup_nick);

// JOIN
	static std::string ERR_INVITEONLYCHAN_473(const std::string& nick, const std::string& channel_name);
	static std::string ERR_BADCHANNELKEY_475(const std::string& nick, const std::string& channel_name);
	static std::string ERR_CHANNELISFULL_471(const std::string& nick, const std::string& channel_name);
	static std::string ERR_BADCHANMASK_476(const std::string& nick, const std::string& channel_name);

// KICK
//  :irc.local 441 dog cat #hi :They are not on that channel
	static std::string ERR_USERNOTINCHANNEL_441(const std::string& nick, const std::string& target_nick, const std::string& channel_name);

// TOPIC
    static std::string RPL_NOTOPIC_331(const std::string& nick, const std::string& channel);
    static std::string RPL_TOPIC_332(const std::string& nick, const std::string& channel, const std::string& topic);
    static std::string RPL_TOPICWHOTIME_333(const std::string& nick, const std::string& channel, const std::string& setter, const std::string& time);

// MODE
    static std::string RPL_CHANNELMODEIS_324(const std::string& nick, const std::string& channel, const std::string& mode);
    static std::string RPL_CREATIONTIME_329(const std::string& nick, const std::string& channel, const std::string& time);
    static std::string ERR_UNKNOWNMODE_472(const std::string& nick, const char mode);
// MODE K
	static std::string ERR_KEYSET_467(const std::string& nick, const std::string& channel_name);

// PRIVMSG
    static std::string RPL_AWAY_301(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& target, const std::string& away_message);
    static std::string ERR_NOTEXTTOSEND_412(const std::string& nick);

// utils
    static std::string RPL_PONG(const std::string& server_name, const std::string& message);
    static std::string ERR_ClosingLink(const std::string& user, const std::string& ip, const std::string& error_msg);

// ALL
    static std::string ERR_NOSUCHNICK_401(const std::string& nick, const std::string& target);
	static std::string ERR_NOSUCHCHANNEL_403(const std::string& nick, const std::string& weird_channel_name);
    static std::string ERR_CANNOTSENDTOCHAN_404(const std::string& nick, const std::string& channel_name);
    static std::string ERR_NOTONCHANNEL_442(const std::string& nick, const std::string& channel);
    static std::string ERR_NEEDMOREPARAMS_461(const std::string& nick, const std::string& command);
	static std::string ERR_ALREADYREGISTRED_462(const std::string& nick);
    static std::string ERR_CHANOPRIVSNEEDED_482(const std::string& nick, const std::string& channel);
	static std::string ERR_CHANOPRIVSNEEDED_482(const std::string& nick, const std::string& channel, const char mode);
	static std::string RPL_NAMREPLY_353(const std::string& nick, Channel& channel);
	static std::string RPL_ENDOFNAMES_366(const std::string& nick, const std::string& channel_name);
    static std::string ERR_UNKNOWNCOMMAND_421(const std::string& nick, const std::string& command);

//성공시
	static std::string RPL_CHANGE_NICK_SUCCESS(const std::string& before_nick, const std::string& user_name, const std::string& ip, const std::string& after_nick);
	static std::string RPL_QUIT_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& comment);
	static std::string RPL_PART_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name, const std::string& comment);
	static std::string RPL_JOIN_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name);
	static std::string RPL_KICK_SUCCESS(const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name, const std::string& target_nick, const std::string& comment);
	static std::string RPL_MODE_SUCCESS(const std::string& mode_setter, const std::string& channel_name, const std::string& mode, const std::string& mode_param);
    static std::string RPL_INVITE_SUCCESS(const std::string& sender_nick, const std::string& receiver_nick, const std::string& user_name, const std::string& ip, const std::string& channel_name);
    static std::string RPL_SET_TOPIC_SUCCESS(const std::string& nick, const std::string& channel_name, const std::string& topic);
// INVITE
    static std::string RPL_ENDOFINVITELIST_337(const std::string& nick);
    static std::string RPL_INVITING_341(const std::string& sender_nick, const std::string& receiver_nick, const std::string& channel);
    static std::string ERR_USERONCHANNEL_443(const std::string& sender_nick, const std::string& receiver_nick, const std::string& channel_name);

// NOTICE
	static std::string INVITE_NOTICE(const std::string& channel_name, const std::string& sender_nick, const std::string& receiver_nick);
};

#endif
