#include "../includes/irc.hpp"

typedef void (*CommandFunction)(Server& server, Client& client, std::vector<std::string>  &cmd);

std::map<std::string, CommandFunction> fillCmd()
{
    std::map<std::string, CommandFunction> cmd;

	cmd["INVITE"] = invite;
	cmd["JOIN"] = join;
	cmd["KICK"] = kick;
	cmd["MODE"] = mode;
	cmd["NICK"] = nick;
	cmd["NOTICE"] = notice;
	cmd["PART"] = part;
	cmd["PASS"] = pass;
	cmd["PING"] = ping;
	cmd["PRIVMSG"] = privmsg;
	cmd["QUIT"] = quit;
	cmd["TOPIC"] = topic;
	cmd["USER"] = user; 
    return cmd;
}

bool CmdIsValid(std::string cmd_string, std::map<std::string, CommandFunction>& cmd)
{
    return cmd.find(cmd_string) != cmd.end();
}
