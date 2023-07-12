#include "../includes/irc.hpp"

std::string addKickMsgs(std::vector<std::string> &cmd)
{
    std::string ret;
    for(int i = 3; i < cmd.size(); i++)
    {
        ret += cmd[i] + " ";
    }
    if(ret[0] == ':')
        ret.erase(ret.begin());
    return ret;
}

void    kickUser(Server &server, Client &client, std::string user, std::string channel, std::string msg)
{
    if(server.channelExists(channel) == false)
        client.addReply(ERR_NOSUCHCHANNEL(client.getNick(), channel));
    Channel &ch = server.getChannelByName(channel);
    if(server.isOperator(client.getNick(), channel) == false)
        client.addReply(ERR_CHANOPRIVSNEEDED(client.getNick(), channel));
    else if(server.isUserInChannel(user, channel))
        client.addReply(ERR_USERNOTINCHANNEL(client.getNick(), user, channel));
    else
        client.addReply(":" + client.getNick() + "!localhost  KICK " + channel + " " + user + " " + msg + "\r\n");
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
                msg = addKickMsgs(cmd);
            kickUser(server, client, user, channel, msg);
        }
    }
}
