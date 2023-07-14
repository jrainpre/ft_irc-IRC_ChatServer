#include "../includes/irc.hpp"

void    pass(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(client.getIsRegistered() == true)
        client.addReply(ERR_ALREADYREGISTERED(client.getNick()));
    if(cmd.size() == 1)
    {
        client.addReply(ERR_NEEDMOREPARAMS((std::string)"*", "PASS"));
        return;
    }
    std::string password = cmd[1];
    if(server.getPassword() == password)
        client.setPassMatch(true);
    else
        client.addReply(ERR_PASSWDMISMATCH((std::string)"*"));
}