#include "../includes/irc.hpp"
#include "../includes/numericsMacros.hpp"

typedef void (*CommandFunction)(Server &server, Client &client, std::vector<std::string> &cmd);

std::string getWholeCmd(std::vector<std::string> &cmd)
{
	std::string wholeCmd;
	for (size_t i = 0; i < cmd.size(); i++)
	{
		wholeCmd += cmd[i];
		if (i != cmd.size() - 1)
			wholeCmd += " ";
	}
	return wholeCmd;
}

std::string getChannelName(std::string channel)
{
	if (channel[0] == '@')
		channel = channel.substr(1);
	return channel;
}

bool isChannelOperatorMessage(std::string channel)
{
	if (channel[0] == '@')
		return true;
	return false;
}

bool Server::sendPrivmsgChannel(std::string channel_name, std::string message, Server &server, Client &client)
{

	bool op_message = isChannelOperatorMessage(channel_name);
	if (op_message)
		channel_name = getChannelName(channel_name);
	if (!server.channelExists(channel_name))
		return 0;
	Channel &channel = server.getChannelByName(channel_name);
	if (!channel.isClientInChannel(client.getNick()))
	{
		client.addReply(ERR_CANNOTSENDTOCHAN(client.getNick(), channel.getName()));
		return true;
	}
	server.addReplyGroup(SENDPRIVMSG(client.getNick(), client.getUsername(), channel_name, message), channel.getOperators(), client);
	if (!op_message)
		server.addReplyGroup(SENDPRIVMSG(client.getNick(), client.getUsername(), channel_name, message), channel.getUsers(), client);
	return true;
}

void privmsg(Server &server, Client &client, std::vector<std::string> &cmd)
{
	if (cmd.size() < 2)
		client.addReply(ERR_NORECIPIENT(client.getNick(), "PRIVMSG"));
	else if (cmd.size() < 3)
		client.addReply(ERR_NOTEXTTOSEND(client.getNick()));
	else
	{
		std::string message = getWholeCmd(cmd);
		std::string recipient_nick = cmd[1];
		message = message.substr(message.find_first_of(':') + 1);

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
		}
	}
}
