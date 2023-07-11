#include "../includes/Channel.hpp"

Channel::Channel(std::string name) : _name(name)
{
}

Channel::Channel(std::string name, std::string key) : _name(name), _key(key)
{
}
Channel::~Channel()
{
    
}

void Channel::addUser(Client &user, std::string &key)
{

}