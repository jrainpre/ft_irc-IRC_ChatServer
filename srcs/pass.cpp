#include "../includes/irc.hpp"

void    pass(Server &server, Client &client, std::vector<std::string> &cmd)
{
    if(client.getIsRegistered() == true)
    {
        client.addReply(ERR_ALREADYREGISTERED(client.getNick()));
        return;
    }
    if(cmd.size() == 1)
    {
        client.addReply(ERR_PASSWDMISMATCH(client.getNick()));
        return;
    }
    std::string password = cmd[1];
    if(server.getPassword() == password)
    {
        client.setPassMatch(true);
        std::cout << "Correct Password" << std::endl;
    }
    else
    {
        client.addReply(ERR_PASSWDMISMATCH(client.getNick()));
        std::cout << "Wrong Password" << std::endl;
    }
}