#include "../includes/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _key(""), _invite_only(false), _active_clients(0)
{
}

Channel::Channel(std::string name, std::string key) : _name(name), _key(key), _invite_only(false), _active_clients(0), _clients_limit(30)
{
}

Channel::Channel(std::string name, std::string key, Client &client) : _name(name), _key(key), _invite_only(false), _active_clients(0), _clients_limit(30)
{
    this->_operators.push_back(client);
}

Channel::~Channel()
{
    
}

void Channel::addUser(Client &client, std::string &key)
{
    if(this->_key.empty() == false && this->_key != key)
    {
        client.addReply(ERR_BADCHANNELKEY(client.getNick(), this->_name));
        return;
    }
    if(this->_clients_limit <= this->_active_clients)
    {
        client.addReply(ERR_CHANNELISFULL(client.getNick(), this->_name));
        return;
    }
    if(this->_invite_only == true && this->isClientInvited(client) == false)
    {
        client.addReply(ERR_CHANNELISFULL(client.getNick(), this->_name));
        return;
    }
    this->_users.push_back(client);
    this->_active_clients++;
    this->sendWelcome(client);
}

void Channel::clientsInChannel(Client &client)
{
    client.addReply(RPL_NAMREPLY(client.getNick(), this->_name));

    for(int i = 0; i < this->_users.size(); i++)
    {
        client.addReply(this->_users[i].getNick() + " ");
    }

    for(int i = 0; i < this->_operators.size(); i++)
    {
        client.addReply("@" + this->_operators[i].getNick() + " ");
    }

    client.addReply("\r\n");
}

void Channel::sendWelcome(Client &client)
{
    client.addReply(":" + client.getNick() + "!localhost JOIN " + this->_name + "\r\n");
    if(this->_topic.empty() == false)
        client.addReply(RPL_TOPIC(client.getNick(), this->_name, this->_topic));
    this->clientsInChannel(client);
    client.addReply(RPL_ENDOFNAMES(client.getNick(), this->_name));
}

bool Channel::isClientInvited(Client &client)
{
    for(int i = 0; i < this->_invited.size(); i++)
    {
        if(this->_invited[i].getNick() == client.getNick())
            return true;
    }
    return false;
}

std::vector<Client> &Channel::getUsers()
{
	return this->_users;
}

std::vector<Client> &Channel::getOperators()
{
	return this->_operators;
}

std::vector<Client> &Channel::getInvited()
{
	return this->_invited;
}