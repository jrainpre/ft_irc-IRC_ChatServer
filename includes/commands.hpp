#pragma once

#include <map>
#include <string>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

typedef void (*CommandFunction)(Server& server, Client& client, std::vector<std::string>  &cmd);

void cmdInvite(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdJoin(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdKick(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdKill(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdList(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdMode(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdMotd(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdNames(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdNick(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdNotice(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdOper(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdPart(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdPass(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdPing(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdPrivmsg(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdQuit(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdTopic(Server& server, Client& client, std::vector<std::string>  &cmd);
void cmdUser(Server& server, Client& client, std::vector<std::string>  &cmd);
std::map<std::string, CommandFunction> fillCmd();
bool CmdIsValid(std::string cmd_string, std::map<std::string, CommandFunction>& cmd);
