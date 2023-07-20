#include "../includes/irc.hpp"

// i = Invite only
// t = restrictions of the TOPIC command to channel operators
// k = Set/remove the channel key (password)
// l = Set/remove the user limit to channel
// o = Give/take channel operator privilege
void    listModes(Server &server, Client &client, std::string channel)
{
    Channel &ch = server.getChannelByName(channel);
    std::string modes, args;
    if(ch.getHasUserlimit() == true)
        args = toString(ch.getClientsLimit());
    modes = ch.getAllModes();
    client.addReply(RPL_CHANNELMODEIS(client.getNick(), channel, modes, args));
}

void    checkMode(Server &server, Client& client, std::vector<std::string> &cmd, Channel &ch, char mode, bool plus)
{
    if(plus)
    {
        if(mode == 'i')
            ch.setInviteOnly(true);
        else if(mode == 't')
            ch.setTopicRestriction(true);
        else if(mode == 'k' && cmd.size() > 3)
        {
            ch.setHasKey(true);
            ch.setKey(cmd[3]);
            cmd.erase(cmd.begin() + 3);
        }
        else if(mode == 'l' && cmd.size() > 3)
        {
            if(!(cmd[3].find_first_not_of("0123456789") == std::string::npos))
            {
                std::cout << "new userlimit not only numbers" << std::endl;
                return;
            }
            ch.setHasUserlimit(true);
            ch.setClientLimit(strtoul(cmd[3].c_str(), NULL, 10));
            cmd.erase(cmd.begin() + 3);
        }
        else if(mode == 'o' && cmd.size() > 3)
        {
            if(ch.isClientInChannel(cmd[3]) && !server.isOperator(cmd[3], ch.getName()))
            {
                ch.promoteUser(cmd[3]);
                server.addReplyGroup(MODES(client.getNick(), ch.getName(), "+o", cmd[3]), ch.getOperators(), client);
                server.addReplyGroup(MODES(client.getNick(), ch.getName(), "+o", cmd[3]), ch.getOperators(), client);
                client.addReply(MODES(client.getNick(), ch.getName(), "+o", cmd[3]));
            }
            cmd.erase(cmd.begin() + 3);
        }
    }
    else
    {
        if(mode == 'i')
            ch.setInviteOnly(false);
        else if(mode == 't')
            ch.setTopicRestriction(false);
        else if(mode == 'k')
        {
            ch.setHasKey(false);
            ch.setKey("");
        }
        else if(mode == 'l')
            ch.setHasUserlimit(false);
        else if(mode == 'o' && cmd.size() > 3)
        {
            if(ch.isClientInChannel(cmd[3]) && server.isOperator(cmd[3], ch.getName()))
            {
                server.addReplyGroup(MODES(client.getNick(), ch.getName(), "-o", cmd[3]), ch.getOperators(), client);
                server.addReplyGroup(MODES(client.getNick(), ch.getName(), "-o", cmd[3]), ch.getOperators(), client);
                client.addReply(MODES(client.getNick(), ch.getName(), "-o", cmd[3]));
                ch.demoteUser(cmd[3]);
            }
            cmd.erase(cmd.begin() + 3);
        }
    }
}

void    changeModes(Server &server, Client &client, std::vector<std::string> &cmd, std::string channel)
{
    std::string modes = cmd[2];
    if(server.isOperator(client.getNick(), channel) == false)
    {
        client.addReply(ERR_CHANOPRIVSNEEDED(client.getNick(), channel));
        return;
    }
    Channel &ch = server.getChannelByName(channel);
    std::string curModes = ch.getAllModes();
    for(size_t i = 0; i < modes.size(); i++)
    {
        if(modes[i] == '+')
        {
            while(i < modes.size() && modes[i] != '-')
            {
                checkMode(server,client, cmd, ch, modes[i], true);
                i++;
            }
        }
        if(modes[i] == '-')
        {
            while(i < modes.size() && modes[i] != '+')
            {
                checkMode(server, client, cmd, ch, modes[i], false);
                i++;
            }
        }
    }
    if(curModes != ch.getAllModes())
    {
        server.addReplyGroup(MODES(client.getNick(), ch.getName(), ch.getAllModes(), ""), ch.getUsers(), client);
        server.addReplyGroup(MODES(client.getNick(), ch.getName(), ch.getAllModes(), ""), ch.getOperators(), client);
        client.addReply(MODES(client.getNick(), ch.getName(), ch.getAllModes(), ""));
    }
}

void    mode(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(cmd.size() == 1)
    {
        client.addReply(ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
        return;
    }
    if(client.getNick() == cmd[1])
        return;
    std::string channel = cmd[1];
    if(server.channelExists(channel) == false)
        client.addReply(ERR_NOSUCHCHANNEL(client.getNick(), channel));
    else if(cmd.size() == 2)
        listModes(server, client, channel);
    else
        changeModes(server, client, cmd, channel);
}