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