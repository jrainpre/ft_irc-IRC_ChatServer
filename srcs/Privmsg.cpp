#include "../includes/commands.hpp"

typedef void (*CommandFunction)(Server& server, Client& client, std::vector<std::string>  &cmd);

void cmdPrivmsg(Server& server, Client& client, std::vector<std::string>  &cmd)
{
	//parse message  form client 
	//PRIVMSG recipient :Hello, how are you?
	//check if recipient is a channel
	//if yes --> send message to channel
	//if no --> check if recipient is a valid nickname of an online user
	//
}


