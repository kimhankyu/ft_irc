#ifndef REPLY_HPP
#define REPLY_HPP

/* ERR_REPLY */

#define ERR_NOSUCHNICK(nick, nonExistant)				":ircserv 401 " + nick + " " + nonExistant + " :No such nick/channel\n"
#define ERR_NOSUCHSERVER(nick, severName)				":ircserv 402 " + nick + " " + serverName + " :No such server\n"
#define ERR_NOSUCHCHANNEL(nick, channel)				":ircserv 403 " + nick + " " + channel + " :No such channel\n"
#define ERR_TOOMANYCHANNELS(nick, channel)				":ircserv 405 " + nick + " " + channel + " :You have joined too many channels\n"
#define ERR_NOORIGIN									":ircserv 431 :No origin specified\n"
#define ERR_NONICKNAMEGIVEN								":ircserv 431 :No nickname given\n"
#define ERR_ERRONEUSNICKNAME(nick, badNick)				":ircserv 432 " + nick + " " + badNick + " :Erroneus nickname\n"
#define ERR_NICKNAMEINUSE(currentNick, desiredNick)		":ircserv 433 " + currentNick + " " + desiredNick + " :Nickname is already in use\n"
// #define ERR_NICKCOLLISION(nick)	":ircserv 436 " //우리는 다른 서버에서 중복인지 체크를 하지 않는다.
#define ERR_NOTONCHANNEL(nick, channel)					":ircserv 442 " + nick + " " + channel + " :You're not on that channel\n"
#define ERR_USERONCHANNEL(client, nick, channel)		":ircserv 443 " + client + " " + nick + " " + channel + " :is already on channel\n"

#define ERR_NEEDMOREPARAMS(nick, command)				":ircserv 461 " + nick + " " + command + " :Not enough parameters\n"
#define ERR_ALREADYREGISTRED(nick)						":ircserv 462 " + nick + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(nick)						":ircserv 464 " + nick + " :Password incorrect\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)				":ircserv 482 " + nick + " " + channel + " :You're not channel operator\n"

#define ERR_UMODEUNKNOWNFLAG(nick)						":ircserv 501 " + nick + " :Unknown MODE flag\n"
#define ERR_USERSDONTMATCH(nick)						":ircserv 502 " + nick + " :Cannot change mode for other users\n"
/* command REPLY */
#define RPL_NICK(oldNick, newNick)						":" + oldNick + " NICK " + newNick + "\n"
#define RPL_WELCOME(nick)								":ircserv 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\n"
#define RPL_UMODEIS(nick, umode)						":ircserv 221 " + nick + " " + umode + "\n"
#define RPL_CHANNELMODEIS(nick, channel, mode)			":ircserv 324 " + nick + " " + channel + " " + mode
#define RPL_NOTOPIC(nick, channel)						":ircserv 331 " + nick + " " + channel + " :No topic is set\n"
#define RPL_TOPIC(nick, channel, topic)					":ircserv 332 " + nick + " " + channel + " :" + topic + "\n"
#define RPL_INVITING(client, nick, channel)				":ircserv 341 " + client + " " + nick + " " + channel + "\n"
#define RPL_YOUREOPER(nick)								":ircserv 381 " + nick + " :You are now an IRC operator\n"

#endif
