#include "../includes/irc.hpp"

void    ping(Server &server, Client &client, std::vector<std::string> &cmd)
{
    (void)server;
    if(cmd.size() > 1)
        client.addReply(PONG(cmd[1]));
}
