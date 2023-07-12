	#include "../includes/commands.hpp"
	#include "../includes/numericsMacros.hpp"

typedef void (*CommandFunction)(Server& server, Client& client, std::vector<std::string>  &cmd);

std::string getWholeCmd(std::vector<std::string> &cmd)
{
	std::string wholeCmd;
	for (size_t i = 0; i < cmd.size(); i++)
	{
		wholeCmd += cmd[i];
		if (i != cmd.size() -1)
			wholeCmd += " ";
	}
	return wholeCmd;
}

//This mode is standard. The prefix and mode letter used for it, respectively, are "@" and "+o".
std::string getChannelName(std::string channel)
{
	if (channel[0] == '@')
		channel = channel.substr(1);
}

bool isChannelOperatorMessage(std::string channel)
{
	if (channel[0] == '@')
			return true;
	return false;
}

bool Server::sendPrivmsgChannel(std::string channel_name, std::string message, Server &server, Client &client)
{
	if (isChannelOperatorMessage(channel_name))
	{
		channel_name = getChannelName(channel_name);
		if 	(!server.channelExists(channel_name))
			return false;
		Channel &channel = server.getChannelByName(channel_name);
		server.addReplyGroup(SENDPRIVMSG(client.getNick(), client.getUsername(), channel_name, message), channel.getOperators());
		server.sendReplyGroup(channel.getOperators());
	}
	else
	{
		if (!server.channelExists(channel_name))
			return false;
		Channel &channel = server.getChannelByName(channel_name);
		server.addReplyGroup(SENDPRIVMSG(client.getNick(), client.getUsername(), channel_name, message), channel.getUsers());
		server.sendReplyGroup(channel.getUsers());
	}
	return true;
}




void cmdPrivmsg(Server& server, Client& client, std::vector<std::string>  &cmd)
{
		std::string message = getWholeCmd(cmd);
		std::string recipient_nick = cmd[1];
		message = message.substr(message.find_first_of(':') + 1);
		message.erase(message.find_last_not_of("\r\n"));	

	if (server.sendPrivmsgChannel(recipient_nick, message, server, client))
		return;
	else
	{
		if (!server.isNickInUse(recipient_nick))
			{
				client.addReply(ERR_NOSUCHNICK(client.getNick(), recipient_nick));
				return;
			}
		Client &recipient_client = server.getClientByNick(recipient_nick);
		recipient_client.addReply(SENDPRIVMSG(client.getNick(), client.getUsername(), recipient_client.getNick(), message));
		recipient_client.sendReply();
	}

}


