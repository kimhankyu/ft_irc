#ifndef REPLY_HPP
#define REPLY_HPP

/* ERR_RYPLY */

#define ERR_NEEDMOREPARAMS(nick, command)			":ircserv 461 " + nick + " " + command + " :Not enough parameters\n"
#define ERR_ALREADYREGISTRED(nick)					":ircserv 462 " + nick + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(nick)					":ircserv 464 " + nick + " :Password incorrect\n"



#endif
