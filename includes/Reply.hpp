#ifndef REPLY_HPP
#define REPLY_HPP

/* ERR_REPLY */

#define ERR_NOSUCHNICK(nick, nonExistant)				":ircserv 401 " + nick + " " + nonExistant + " :No such nick/channel\r\n"
#define ERR_NOSUCHSERVER(nick, severName)				":ircserv 402 " + nick + " " + serverName + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel)				":ircserv 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_TOOMANYCHANNELS(nick, channel)				":ircserv 405 " + nick + " " + channel + " :You have joined too many channels\r\n"
#define ERR_NOORIGIN									":ircserv 431 :No origin specified\r\n"
#define ERR_NONICKNAMEGIVEN								":ircserv 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick, badNick)				":ircserv 432 " + nick + " " + badNick + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(currentNick, desiredNick)		":ircserv 433 " + currentNick + " " + desiredNick + " :Nickname is already in use\r\n"
#define ERR_NOTONCHANNEL(nick, channel)					":ircserv 442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(client, nick, channel)		":ircserv 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n"

#define ERR_NEEDMOREPARAMS(nick, command)				":ircserv 461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(nick)						":ircserv 462 " + nick + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(nick)						":ircserv 464 " + nick + " :Password incorrect\r\n"
#define ERR_INVITEONLYCHAN(nick, channel)				":ircserv 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(nick, channel)				":ircserv 475 " + nick + " " + channel + " ::Cannot join channel (+k)\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)				":ircserv 482 " + nick + " " + channel + " :You're not channel operator\r\n"

#define ERR_UMODEUNKNOWNFLAG(nick)						":ircserv 501 " + nick + " :Unknown MODE flag\r\n"
#define ERR_USERSDONTMATCH(nick)						":ircserv 502 " + nick + " :Cannot change mode for other users\r\n"

/* command REPLY */
#define RPL_NICK(oldNick, newNick)						":" + oldNick + " NICK " + newNick + "\r\n"
#define RPL_WELCOME(nick)								":ircserv 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n"
#define RPL_UMODEIS(nick, umode)						":ircserv 221 " + nick + " " + umode + "\r\n"
#define RPL_CHANNELMODEIS(nick, channel, mode)			":ircserv 324 " + nick + " " + channel + " " + mode
#define RPL_NOTOPIC(nick, channel)						":ircserv 331 " + nick + " " + channel + " :No topic is set\r\n"
#define RPL_TOPIC(nick, channel, topic)					":ircserv 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_INVITING(client, nick, channel)				":ircserv 341 " + client + " " + nick + " " + channel + "\r\n"
#define RPL_NAMREPLY(nick,channel,users)				":ircserv 353 " + nick + " = " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(nick,channel)					":ircserv 366 " + nick + " " + channel + " :End of /NAMES list\r\n"
#define RPL_YOUREOPER(nick)								":ircserv 381 " + nick + " :You are now an IRC operator\r\n"

#endif
