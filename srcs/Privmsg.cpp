#include "../includes/commands.hpp"

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
}

void cmdPrivmsg(Server& server, Client& client, std::vector<std::string>  &cmd)
{
	//parse message  form client 
	try
	{
		std::string message = getWholeCmd(cmd);
		std::string recipient = cmd[1];
		

		
	}
	catch(const std::exception& e){}
	

	//PRIVMSG recipient :Hello, how are you?
PRIVMSG
recipient
:Hello,
how
are
you?
	//check if recipient is a channel
	//if yes --> send message to channel
	//if no --> check if recipient is a valid nickname of an online user
	//
}


