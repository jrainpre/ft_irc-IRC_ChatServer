#include "../includes/irc.hpp"

// The NICK command is used to give the client a nickname or change the previous one.

// If the server receives a NICK command from a client where the desired nickname is already in use on the network, it should issue an ERR_NICKNAMEINUSE 
//numeric and ignore the NICK command.

// If the server does not accept the new nickname supplied by the client as valid (for instance, due to containing invalid characters), it should issue an 
//c numeric and ignore the NICK command. Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), 
//backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character. 
//Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:

//     no leading # character or other character advertized in CHANTYPES
//     no leading colon (:)
//     no ASCII space

// If the server does not receive the <nickname> parameter with the NICK command, it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command.

// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. 
//In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

// Numeric Replies:

//     ERR_NONICKNAMEGIVEN (431)
//     ERR_ERRONEUSNICKNAME (432)
//     ERR_NICKNAMEINUSE (433)
//     ERR_NICKCOLLISION (436)
bool    isNickInUse(Client &client, std::vector<std::string> &cmd)
{

}

bool    isValidNick(Client &client, std::string &nick)
{
    if(nick[0] == ':' || nick[0] == '#' || nick[0] == ' ')
        return false;
    if(nick.size() > 30)
        return false;
}

void    nick(Client &client, std::vector<std::string> &cmd)
{
    if(cmd.size() == 1)
    {
        client.addReply(ERR_NONICKNAMEGIVEN(client.getNick()));
        return;
    }
    if(isValidNick(client, cmd[1]) == false)
    {
        client.addReply(ERR_ERRONEUSNICKNAME(client.getNick(), cmd[1]));
        return;
    }
}