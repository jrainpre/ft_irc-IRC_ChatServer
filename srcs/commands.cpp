#include "../includes/commands.hpp"

typedef void (*CommandFunction)(Server& server, Client& client, std::vector<std::string>  &cmd);

void cmdOper(Server& server, Client& client, std::vector<std::string>  &cmd)
{
	(void)server;
	(void)client;
	(void)cmd;
	std::cout << "cmdOper" << std::endl;
}

void cmdPart(Server& server, Client& client, std::vector<std::string>  &cmd)
{
	(void)server;
	(void)client;
	(void)cmd;
	std::cout << "cmdPart" << std::endl;
}

std::map<std::string, CommandFunction> fillCmd()
{
    std::map<std::string, CommandFunction> cmd;

	cmd["INVITE"] = invite;
	cmd["JOIN"] = join;
	cmd["KICK"] = kick;
	cmd["MODE"] = mode;
	cmd["NICK"] = nick;
	cmd["NOTICE"] = cmdNotice;
	cmd["OPER"] = cmdOper;
	cmd["PART"] = part;
	cmd["PASS"] = pass;
	cmd["PING"] = ping;
	cmd["PRIVMSG"] = cmdPrivmsg;
	cmd["QUIT"] = cmdQuit;
	cmd["TOPIC"] = topic;
	cmd["USER"] = user;
    return cmd;
}

bool CmdIsValid(std::string cmd_string, std::map<std::string, CommandFunction>& cmd)
{
    return cmd.find(cmd_string) != cmd.end();
}
