#include "../includes/irc.hpp"

void    join(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(client.getIsRegistered() == false)
    {
        client.addReply(ERR_NOTREGISTERED(client.getNick()));
        return;
    }
}
