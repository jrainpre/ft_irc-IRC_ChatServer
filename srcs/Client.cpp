#include "../includes/Client.hpp"

Client::Client(int socket_fd): _socket_fd(socket_fd), _pass_match(false), _is_registered(false), _state(0), _mode(0)
{
    std::cout << "Client Created" << std::endl;
}

Client::~Client()
{

}

void Client::addChannel(Channel channel)
{
    this->_channels.push_back(channel);
}

void Client::removeChannel(std::string channel)
{
    //later
}

bool Client::check_nick(std::string nick)
{
    
}

bool Client::check_username(std::string username)
{

}


void Client::parseCmds(std::string &buf)
{
    std::vector<std::string> firstSplit, secondSplit;
    if(buf.find("\r\n") == std::string::npos)
        return;
    firstSplit = split(buf, "\r\n", true);
    for(unsigned long i = 0; i < firstSplit.size(); i++)
    {
        this->_cmds.push_back(split(firstSplit[i], " ", false));
    }
}

// void    Client::execCmds()
// {
//     while(this->_cmds.empty() == false)
//     {
//         if(this->_is_registered == false)
//         {
//             this->unregisteredCmds();
//         }
          


//         this->_cmds.erase(this->_cmds.begin());
//     }
    
// }

// void    Client::unregisteredCmds()
// {
//     std::string msg;

//     if(this->_cmds[0].size() > 1 &&  this->_cmds[0][0] == "PASS")
//     {
//         if(this->_cmds[0][1] == this->_server_password)
//         {
//             this->_pass_match = true;
//             std::cout << "Correct Password" << std::endl;
//         }
//         else
//             std::cout << "Wrong Password" << std::endl;
//     }
//     else if(this->_cmds[0].size() > 1 &&  this->_cmds[0][0] == "NICK") //Check first if Nick Exists
//         this->_nick = this->_cmds[0][1];
//     else if(this->_cmds[0].size() > 1 && this->_cmds[0][0] == "USER") //Check first if User exists
//     {
//         this->_username = this->_cmds[0][1];
//         this->_realname = this->_cmds[0][2];
//     }
//     else if(this->_cmds[0].size() > 0 && this->_cmds[0][0] == "JOIN")
//     {
//         msg = "451 * :You have not registered\r\n";
//         write(this->_socket_fd, msg.c_str(), msg.size());
//     }

//     if(this->_nick.empty() != true && this->_realname.empty() != true && this->_pass_match == true)
//     {
//         this->_is_registered = true;
//         std::cout << "User Registered" << std::endl;
//     }
// }

// void    Client::tryRegister()
// {

// }