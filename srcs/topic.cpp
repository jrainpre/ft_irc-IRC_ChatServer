#include "../includes/irc.hpp"

bool topicChecks(Server &server, Client &client, std::string channel)
{
    if(server.channelExists(channel) == false)
        client.addReply(ERR_NOSUCHCHANNEL(client.getNick(), channel));
    else if(server.isUserInChannel(client.getNick(), channel) == false)
        client.addReply(ERR_NOTONCHANNEL(client.getNick(), channel));
    else
        return true;
    return false;
}

void    viewTopic(Server &server, Client &client, std::string channel)
{
    Channel &ch = server.getChannelByName(channel);
    if(ch.getTopic().empty() == true)
        client.addReply(RPL_NOTOPIC(client.getNick(), channel));
    else
        client.addReply(RPL_TOPIC(client.getNick(), channel, ch.getTopic()));
}

void    setTopic(Server &server, Client &client, std::string channel, std::vector<std::string> &cmd)
{
    std::string newTopic = addMsgs(2, cmd);
    Channel &ch = server.getChannelByName(channel);
    if(ch.getTopicRestricted() == true && server.isOperator(client.getNick(), channel) == false)
    {
        client.addReply(ERR_CHANOPRIVSNEEDED(client.getNick(), channel));
        return;
    }
    ch.setTopic(newTopic);
    server.addReplyGroup(":" + client.getNick() + "!localhost TOPIC " + channel + " " + newTopic + "\r\n", ch.getUsers(), client);
    server.addReplyGroup(":" + client.getNick() + "!localhost TOPIC " + channel + " " + newTopic + "\r\n", ch.getOperators(), client);
    client.addReply(":" + client.getNick() + "!localhost TOPIC " + channel + " " + newTopic + "\r\n");
    server.sendReplyGroup(ch.getOperators(), client);
    server.sendReplyGroup(ch.getUsers(), client);
}

void    topic(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(cmd.size() == 1)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "TOPIC"));
        return;
    }
    std::string channel = cmd[1];

    if(topicChecks(server, client, channel) == false)
        return;
    else if(cmd.size() == 2)
        viewTopic(server, client, channel);
    else
        setTopic(server, client, channel, cmd);
}
