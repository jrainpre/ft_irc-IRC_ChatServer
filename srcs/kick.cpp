#include "../includes/irc.hpp"

void    kickUser(Server &server, Client &client, std::string user, std::string channel, std::string msg)
{
    if(server.channelExists(channel) == false)
        client.addReply(ERR_NOSUCHCHANNEL(client.getNick(), channel));
    if(server.isOperator(client.getNick(), channel) == false)
        client.addReply(ERR_CHANOPRIVSNEEDED(client.getNick(), channel));
    else if(!server.isUserInChannel(user, channel))
        client.addReply(ERR_USERNOTINCHANNEL(client.getNick(), user, channel));
    else
    {
        Channel &ch = server.getChannelByName(channel);
        client.addReply(KICK(client.getNick(), user, channel, msg));
        server.addReplyGroup(KICK(client.getNick(), user, channel, msg), ch.getUsers(), client);
        server.addReplyGroup(KICK(client.getNick(), user, channel, msg), ch.getOperators(), client);
        ch.deleteClient(user);
        server.deleteEmptyChannels();
    }
}

void    kick(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(cmd.size() < 3)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "KICK"));
    }
    else if(cmd.size() >= 3)
    {
        std::string user, channel, msg;
        channel = cmd[1];
        while(!cmd[2].empty())
        {
            user = getSubstr(cmd[2], ",");
            if(cmd.size() > 3)
                msg = addMsgs(3, cmd);
            kickUser(server, client, user, channel, msg);
        }
    }
}
