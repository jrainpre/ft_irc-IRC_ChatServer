#include "../includes/irc.hpp"

void quit(Server &server, Client &client, std::vector<std::string> &cmd)
{
	std::string message = getWholeCmd(cmd);

	if (cmd.size() < 2)
		message = " Quit without message";
	else
		message = message.substr(message.find_first_of(':') + 1);
	for (size_t i = 0; i < client.getChannels().size(); i++)
	{
		server.addReplyGroup(SENDQUIT(client.getNick(), client.getUsername(), message), client.getChannels()[i]->getUsers(), client);
		server.addReplyGroup(SENDQUIT(client.getNick(), client.getUsername(), message), client.getChannels()[i]->getOperators(), client);
	}
	client.setIsQuited(true);
}
