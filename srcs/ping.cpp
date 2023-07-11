#include "../includes/irc.hpp"

void    ping(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(cmd.size() != 2)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "PING"));
        return;
    }
    client.addReply("PONG localhost " + cmd[1] + "\r\n");
}
