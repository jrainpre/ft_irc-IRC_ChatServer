#pragma once

//Welcome Replys

#define RPL_WELCOME(nick, user) (":localhost 001 " + nick + " :Welcome to the 42 Vienna Network, " + nick + "\r\n")
#define RPL_YOURHOST(servername, version, nick) (":localhost 002 " + nick + " :Your host is " + servername + ", running version " + version + "\r\n")
#define RPL_CREATED(datetime, nick) (":localhost 003 " + nick + " :This server was created " + datetime + "\r\n")
#define RPL_MYINFO(servername, version, usermodes, channelmodes, nick) (":localhost 004 " + nick + " " + servername + " " + version + " " + usermodes + " " + channelmodes + "\r\n")
#define RPL_ISUPPORT(tokens, nick) (":localhost 005 " + nick + " " + tokens + " :are supported by this server\r\n")

//Nick ErrorMsgs

#define ERR_NONICKNAMEGIVEN(nick) (":localhost 431 " + nick + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick ,newNick) (":localhost 432 " + nick + " " + newNick + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(nick ,newNick) (":localhost 433 " + nick + " " + newNick + " :Nickname is already in use\r\n")
