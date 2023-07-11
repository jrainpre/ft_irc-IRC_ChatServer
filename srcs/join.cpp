#include "../includes/irc.hpp"


// Apart from the requirement of the first character being a valid channel type prefix character; the only restriction on a channel name is that it may not 
// contain any spaces (' ', 0x20), a control G / BELL ('^G', 0x07), or a comma (',', 0x2C) (which is used as a list item separator by the protocol).



void    checkChannel(Server &server, Client &client, std::string channel, std::string key)
{
    if(server.channelExists(channel) == true)
        server.joinChannel(client, channel, key);
    else
        server.createChannel(client, channel, key);
}

void    join(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(client.getIsRegistered() == false)
    {
        client.addReply(ERR_NOTREGISTERED(client.getNick()));
        return;
    }
    
    if(cmd.size() < 2)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "JOIN"));
        return;
    }

    std::string key, channel;
    unsigned int pos;
    while(!cmd[1].empty())
    {
        channel = getSubstr(cmd[1], ",");
        if(cmd.size() > 2)
            key = getSubstr(cmd[2], ",");
        checkChannel(server, client, channel, key);
    }
}
