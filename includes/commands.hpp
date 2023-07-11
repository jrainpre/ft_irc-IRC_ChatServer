#pragma once

#include <map>
#include <string>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

typedef void (*CommandFunction)(Server& server, Client& client);

void cmdInvite(Server &server, Client &client);
void cmdJoin(Server &server, Client &client);
void cmdKick(Server &server, Client &client);
void cmdKill(Server &server, Client &client);
void cmdList(Server &server, Client &client);
void cmdMode(Server &server, Client &client);
void cmdMotd(Server &server, Client &client);
void cmdNames(Server &server, Client &client);
void cmdNick(Server &server, Client &client);
void cmdNotice(Server &server, Client &client);
void cmdOper(Server &server, Client &client);
void cmdPart(Server &server, Client &client);
void cmdPass(Server &server, Client &client);
void cmdPing(Server &server, Client &client);
void cmdPrivmsg(Server &server, Client &client);
void cmdQuit(Server &server, Client &client);
void cmdTopic(Server &server, Client &client);
void cmdUser(Server &server, Client &client);
std::map<std::string, CommandFunction> fillCmd();
bool CmdIsValid(std::string cmd_string, std::map<std::string, CommandFunction>& cmd);
