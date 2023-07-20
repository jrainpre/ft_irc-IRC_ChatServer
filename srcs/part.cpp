#include "../includes/irc.hpp"

void    part(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(cmd.size() == 1)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "PART"));
        return;
    }
    std::string channel = cmd[1];
    if(server.channelExists(channel) == false)
        client.addReply(ERR_NOSUCHCHANNEL(client.getNick(), channel));
    else if(server.isUserInChannel(client.getNick(), channel) == false)
        client.addReply(ERR_NOTONCHANNEL(client.getNick(), channel));
    else
    {
        Channel &ch = server.getChannelByName(channel);
        ch.deleteClient(client.getNick());
        server.deleteEmptyChannels();
        client.addReply(PART(client.getNick(), channel));
    }
}
