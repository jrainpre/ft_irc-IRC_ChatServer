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

void cmdPrivmsg(Server& server, Client& client, std::vector<std::string>  &cmd)
{
		std::string message = getWholeCmd(cmd);
		std::string recipient_nick = cmd[1];
		message = message.substr(message.find_first_of(':') + 1);
		message.erase(message.find_last_not_of("\r\n"));	
	// if (server.isValidChannel(recipient_nick))
	// 	server.sendMessageChannel(recipient_nick, message);
	if (false)
	;
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


