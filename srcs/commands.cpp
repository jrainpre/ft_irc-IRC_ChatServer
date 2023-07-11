#include "../includes/commands.hpp"

typedef void (*CommandFunction)(Server& server, Client& client);

void cmdInvite(Server &server, Client &client)
{
	std::cout << "cmdInvite" << std::endl;
}

void cmdJoin(Server &server, Client &client)
{
	std::cout << "cmdJoin" << std::endl;
	std::string msg = "451 * :you have not registered\r\n";
	write(client.getSocketFd(),msg.c_str(), msg.size());
}

void cmdKick(Server &server, Client &client)
{
	std::cout << "cmdKick" << std::endl;
}

void cmdKill(Server &server, Client &client)
{
	std::cout << "cmdKill" << std::endl;
}

void cmdList(Server &server, Client &client)
{
	std::cout << "cmdList" << std::endl;
}

void cmdMode(Server &server, Client &client)
{
	std::cout << "cmdMode" << std::endl;
}

void cmdMotd(Server &server, Client &client)
{
	std::cout << "cmdMotd" << std::endl;
}

void cmdNames(Server &server, Client &client)
{
	std::cout << "cmdNames" << std::endl;
}

void cmdNick(Server &server, Client &client)
{
	std::cout << "cmdNick" << std::endl;
	std::vector<std::vector<std::string>> cmds = client.getCmds();
	if (cmds[0].size() > 1)
		client.setNick(cmds[0][1]);
}

void cmdNotice(Server &server, Client &client)
{
	std::cout << "cmdNotice" << std::endl;
}

void cmdOper(Server &server, Client &client)
{
	std::cout << "cmdOper" << std::endl;
}

void cmdPart(Server &server, Client &client)
{
	std::cout << "cmdPart" << std::endl;
}

void cmdPass(Server &server, Client &client)
{
	std::cout << "cmdPass" << std::endl;
}

void cmdPing(Server &server, Client &client)
{
	std::cout << "cmdPing" << std::endl;
}

void cmdPrivmsg(Server &server, Client &client)
{
	std::cout << "cmdPrivmsg" << std::endl;
}

void cmdQuit(Server &server, Client &client)
{
	std::cout << "cmdQuit" << std::endl;
}

void cmdTopic(Server &server, Client &client)
{
	std::cout << "cmdTopic" << std::endl;
}

void cmdUser(Server &server, Client &client)
{
	std::cout << "cmdUser" << std::endl;
	std::vector<std::vector<std::string>> cmds = client.getCmds();
	if (cmds[0].size() > 1)
	{
		client.setUsername(cmds[0][1]);
		client.setRealname(cmds[0][2]);
	}
	std::cout << client.getUsername() << std::endl;
	std::cout << client.getRealname() << std::endl;
}


std::map<std::string, CommandFunction> fillCmd()
{
    std::map<std::string, CommandFunction> cmd;

	cmd["INVITE"] = cmdInvite;
	cmd["JOIN"] = cmdJoin;
	cmd["KICK"] = cmdKick;
	cmd["KILL"] = cmdKill;
	cmd["LIST"] = cmdList;
	cmd["MODE"] = cmdMode;
	cmd["MOTD"] = cmdMotd;
	cmd["NAMES"] = cmdNames;
	cmd["NICK"] = cmdNick;
	cmd["NOTICE"] = cmdNotice;
	cmd["OPER"] = cmdOper;
	cmd["PART"] = cmdPart;
	cmd["PASS"] = cmdPass;
	cmd["PING"] = cmdPing;
	cmd["PRIVMSG"] = cmdPrivmsg;
	cmd["QUIT"] = cmdQuit;
	cmd["TOPIC"] = cmdTopic;
	cmd["USER"] = cmdUser;
    return cmd;
}

bool CmdIsValid(std::string cmd_string, std::map<std::string, CommandFunction>& cmd)
{
    return cmd.find(cmd_string) != cmd.end();
}


