#include "../includes/Client.hpp"

Client::Client(int socket_fd): _socket_fd(socket_fd), _pass_match(false), _is_registered(false), _state(0), _mode(0)
{
    std::cout << "Client Created" << std::endl;
}

Client::~Client()
{

}

std::string Client::getNick()
{
    return this->_nick;
}

std::string Client::getUsername()
{
    return this->_username;
}

std::string Client::getRealname()
{
    return this->_realname;
}

unsigned int Client::getState()
{
    return this->_state;
}

unsigned int Client::getMode()
{
    return this->_mode;
}

std::vector<Channel> Client::getChannels()
{
    //later
}
void Client::setNick(std::string nick)
{
    this->_nick = nick;
}

void Client::setUsername(std::string username)
{
    this->_username = username;
}

void Client::setRealname(std::string realname)
{
    this->_realname = realname;
}

void Client::setState(unsigned int state)
{
    this->_state = state;
}

void Client::setMode(unsigned int mode)
{
    this->_mode = mode;
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

int Client::getSocketFd()
{
    return this->_socket_fd;
}

std::vector<std::vector<std::string> > Client::getCmds()
{
    return this->_cmds;
}

void Client::parseCmds(std::string &buf)
{
    if(buf.find("\r\n") == std::string::npos)
        return;
    while(!buf.empty())
        this->_cmds.push_back(split(buf, "\r\n"));
    for(int i = 0; i < this->_cmds.size(); i++)
    {
        buf = this->_cmds[i][0];
        this->_cmds.erase(this->_cmds.begin() + i);
        this->_cmds.insert(this->_cmds.begin() + i, split(buf, " "));
    }    
}

void    Client::execCmds()
{
    while(this->_cmds.empty() == false)
    {
        if(this->_is_registered == false)
        {
            this->unregisteredCmds();
        }
          


        this->_cmds.erase(this->_cmds.begin());
    }
    
}

void    Client::unregisteredCmds()
{
    std::string msg;

    if(this->_cmds[0].size() > 1 &&  this->_cmds[0][0] == "Nick") //Check first if Nick Exists
        this->_nick = this->_cmds[0][1];
    else if(this->_cmds[0].size() > 1 && this->_cmds[0][0] == "USER") //Check first if User exists
    {
        this->_username = this->_cmds[0][1];
        this->_realname = this->_cmds[0][2];
    }
    else if(this->_cmds[0].size() > 0 && this->_cmds[0][0] == "JOIN")
    {
        msg = "451 * :You have not registered\r\n";
        write(this->_socket_fd, msg.c_str(), msg.size());
    }

    if(this->_nick.empty() != true && this->_realname.empty() != true)
        this->_is_registered = true;
}

// void    Client::tryRegister()
// {

// }