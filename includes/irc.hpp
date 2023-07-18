#pragma once

#include "commands.hpp"
#include "utils.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "numericsMacros.hpp"
#include "Channel.hpp"

#include <string>
#include <vector>
#include <map>

#define FAILED 1
#define WORKED 0

//Join
void    checkChannel(Server &server, Client &client, std::string channel, std::string key);
void    join(Server &server, Client &client, std::vector<std::string> &cmd);

//Nick
bool    isNickInUse(Server &server, std::string &nick);
bool    isValidNick(std::string &nick);
void    nick(Server &server, Client &client, std::vector<std::string> &cmd);

//Pass
void    pass(Server &server, Client &client, std::vector<std::string> &cmd);
void    ping(Server &server, Client &client, std::vector<std::string> &cmd);
void    user(Server &server, Client &client, std::vector<std::string> &cmd);

//Kick
std::string addKickMsgs(std::vector<std::string> &cmd);
void    kickUser(Server &server, Client &client, std::string user, std::string channel, std::string msg);
void    kick(Server &server, Client &client, std::vector<std::string> &cmd);

//Invite
void    invite(Server &server, Client &client, std::vector<std::string> &cmd);

//Topic
void    topic(Server &server, Client &client, std::vector<std::string> &cmd);

//PRIVMSG
bool isChannelOperatorMessage(std::string channel);
std::string getChannelName(std::string channel);
std::string getWholeCmd(std::vector<std::string> &cmd);