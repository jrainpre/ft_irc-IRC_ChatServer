#pragma once

//Welcome Replys

#define RPL_WELCOME(nick, user) (":localhost 001 " + nick + " :Welcome to the 42 Vienna Network, " + nick + "\r\n")
#define RPL_YOURHOST(servername, version, nick) (":localhost 002 " + nick + " :Your host is " + servername + ", running version " + version + "\r\n")
#define RPL_CREATED(datetime, nick) (":localhost 003 " + nick + " :This server was created " + datetime + "\r\n")
#define RPL_MYINFO(servername, version, usermodes, channelmodes, nick) (":localhost 004 " + nick + " " + servername + " " + version + " " + usermodes + " " + channelmodes + "\r\n")
#define RPL_ISUPPORT(tokens, nick) (":localhost 005 " + nick + " " + tokens + " :are supported by this server\r\n")

//Nick ErrorMsgs

#define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(client ,newNick) (":localhost 432 " + client + " " + newNick + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(client, newNick) (":localhost 433 " + client + " " + newNick + " :Nickname is already in use\r\n")

//Pass ErrorMsg

#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")
#define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister\r\n")

//Join

#define ERR_NOTREGISTERED(client) (":localhost 451 " + client + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters\r\n")
#define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_TOOMANYCHANNELS(client, channel) (":localhost 405 " + client + " " + channel + " :You have joined too many channels\r\n") 

#define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " " + channel + " :" + topic + "\r\n") 
#define RPL_NAMREPLY(client, channel) (":localhost 353 " + client + " = " + channel + " :")
#define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " " + channel + " :End of /NAMES list\r\n")

//Kick
#define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " " + channel + " :No such channel\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define ERR_USERNOTINCHANNEL(client, nick, channel) (":localhost 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n")

//PrivMsg
#define SENDPRIVMSG(nickname, username, recipient, message)(":" + nickname + "!" + username + "@hostname PRIVMSG " + recipient + " " + message + "\r\n")
#define ERR_NOSUCHNICK(client, nickname) (":localhost 401 " + client + " " + nickname + " :No such nick/channel" + "\r\n")
//"<client> <channel> :Cannot send to channel"
#define ERR_CANNOTSENDTOCHAN(client, channel) (":localhost 404 " + client + " " + channel + " :Cannot send to channel" + "\r\n")
//"<client> :No recipient given (<command>)"
#define ERR_NORECIPIENT(client, command) (":localhost 411 " + client + " :No recipient given (" + command + ")" + "\r\n")
//"<client> :No text to send"
#define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client + " :No text to send" + "\r\n")
