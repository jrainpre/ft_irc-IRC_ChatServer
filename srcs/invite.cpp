#include "../includes/irc.hpp"

void    invite(Server &server, Client &client, std::vector<std::string> &cmd)
{
    std::string nick, channel;
    if(cmd.size() < 3)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "INVITE"));
        return;
    }
    nick = cmd[1];
    channel = cmd[2];
    if(server.channelExists(channel) == false)
    {
        client.addReply(ERR_NOSUCHCHANNEL(client.getNick(), channel));
        return;
    }
        
    Channel &ch = server.getChannelByName(channel);
    if(server.isNickInUse(nick) == false)
        client.addReply(ERR_NOSUCHNICK(client.getNick(), nick));
    else if(ch.isClientInChannel(nick) == true)
        client.addReply(ERR_USERONCHANNEL(client.getNick(), nick, channel));
    else if(ch.isClientInChannel(client.getNick()) == false)
        client.addReply(ERR_NOTONCHANNEL(client.getNick(), channel));
    else if(ch.getInviteOnly() == true && server.isOperator(client.getNick(), channel) == false)
        client.addReply(ERR_CHANOPRIVSNEEDED(client.getNick(), channel));
     else
     {
        Client &target = server.getClientByNick(nick);
        ch.inviteUser(client, target);
        target.addReply(INVITE(client.getNick(), nick, channel));
        target.sendReply();
     }
}