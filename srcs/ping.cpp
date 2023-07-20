#include "../includes/irc.hpp"

void    ping(Server &server, Client &client, std::vector<std::string> &cmd)
{
    (void)server;
    // if(cmd.size() != 2)
    // {
    //     client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "PING"));
    //     return;
    // }
    if(cmd.size() > 1)
        client.addReply(PONG(cmd[2]));
}
