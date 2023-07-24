#include "../includes/irc.hpp"
#include "../includes/numericsMacros.hpp"

void notice(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
        client.addReply(ERR_NORECIPIENT(client.getNick(), "NOTICE"));
    else if (cmd.size() < 3)
        client.addReply(ERR_NOTEXTTOSEND(client.getNick()));
    else
    {
        std::string message = getWholeCmd(cmd);
        std::string recipient_nick = cmd[1];
        message = message.substr(message.find_first_of(':') + 1);

        if (server.sendNoticeChannel(recipient_nick, message, server, client))
            return;
        else
        {
            if (!server.isNickInUse(recipient_nick))
            {
                client.addReply(ERR_NOSUCHNICK(client.getNick(), recipient_nick));
                return;
            }
            Client &recipient_client = server.getClientByNick(recipient_nick);
            recipient_client.addReply(SENDNOTICE(client.getNick(), client.getUsername(), recipient_client.getNick(), message));
        }
    }
}


bool Server::sendNoticeChannel(std::string channel_name, std::string message, Server &server, Client &client)
{
    bool op_message = isChannelOperatorMessage(channel_name);
    if (op_message)
        channel_name = getChannelName(channel_name);
    if (!server.channelExists(channel_name))
        return 0;
    Channel &channel = server.getChannelByName(channel_name);
    if (!channel.isClientInChannel(client.getNick()))
    {
        client.addReply(ERR_CANNOTSENDTOCHAN(client.getNick(), channel.getName()));
        return true;
    }
    server.addReplyGroup(SENDNOTICE(client.getNick(), client.getUsername(), channel_name, message), channel.getOperators(), client);
    if (!op_message)
    {
        server.addReplyGroup(SENDNOTICE(client.getNick(), client.getUsername(), channel_name, message), channel.getUsers(), client);
    }
    return true;
}