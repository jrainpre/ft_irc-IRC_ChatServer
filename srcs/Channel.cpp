#include "../includes/Channel.hpp"

// Channel::Channel(std::string name) : _name(name), _key(""), _invite_only(false), _active_clients(0)
// {
// }

// Channel::Channel(std::string name, std::string key) : _name(name), _key(key), _invite_only(false), _active_clients(0), _clients_limit(30)
// {
// }

Channel::Channel(std::string name, std::string key, Client &client) : _name(name), _key(key), _invite_only(false), _has_userlimit(false), _topic_restricted(false), _clients_limit(-1)
{
    this->_operators.push_back(client);
    if(!key.empty())
        this->_has_key = true;
    else
        this->_has_key = false;
}

Channel::~Channel()
{
    
}

bool Channel::addUser(Client &client, std::string &key)
{
    if(this->_has_key == true && this->_key.empty() == false && this->_key != key)
        client.addReply(ERR_BADCHANNELKEY(client.getNick(), this->_name));
    else if(this->_has_userlimit == true && this->_clients_limit <= this->getUsers().size() + this->getOperators().size())
        client.addReply(ERR_CHANNELISFULL(client.getNick(), this->_name));
    else if(this->_invite_only == true && this->isClientInvited(client.getNick()) == false)
        client.addReply(ERR_CHANNELISFULL(client.getNick(), this->_name));
    else
    {
        this->_users.push_back(client);
        this->sendWelcome(client);
        return true;
    }
    return false;
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

bool Channel::isClientInvited(std::string nick)
{
    for(int i = 0; i < this->_invited.size(); i++)
    {
        if(this->_invited[i].getNick() == nick)
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

bool Channel::isClientInChannel(std::string nick)
{
	for(int i = 0; i < this->_users.size(); i++)
	{
		if(this->_users[i].getNick() == nick)
			return true;
	}
	for(int i = 0; i < this->_operators.size(); i++)
	{
		if(this->_operators[i].getNick() == nick)
		return true;
	}
	return false;
}

void Channel::sendJoinMsgs(std::string clientNick)
{
    for(size_t i = 0; i < this->getOperators().size(); i++)
    {
        if(this->getOperators()[i].getNick() != clientNick)
        {
            this->getOperators()[i].addReply(":" + clientNick + "!localhost JOIN " + this->getName() + "\r\n");
            this->getOperators()[i].sendReply();
        }
    }

    for(size_t i = 0; i < this->getUsers().size(); i++)
    {
        if(this->getUsers()[i].getNick() != clientNick)
        {
            this->getUsers()[i].addReply(":" + clientNick + "!localhost JOIN " + this->getName() + "\r\n");
            this->getUsers()[i].sendReply();
        }
    }
}

void Channel::inviteUser(Client &client, Client &target)
{
    if(this->isClientInvited(target.getNick()) == false)
        this->getInvited().push_back(target);
    client.addReply(RPL_INVITING(client.getNick(), target.getNick(), this->getName()));
}

void Channel::setTopic(std::string topic)
{
    this->_topic = topic;
}

std::string Channel::getAllModes()
{
    std::string modes = "+";
    if(this->_invite_only)
        modes += "i";
    if(this->_has_key)
        modes += "k";
    if(this->_topic_restricted)
        modes += "t";
    if(this->_has_userlimit)
        modes += "l " + this->_clients_limit;
    return modes;
}

void Channel::promoteUser(std::string &nick)
{
    for(size_t i = 0; i < _users.size(); i++)
    {
        if(_users[i].getNick() == nick)
        {
            this->addOperator(_users[i]);
            _users.erase(_users.begin() + i);
            return;
        }
    }
}

void Channel::demoteUser(std::string &nick)
{
    for(size_t i = 0; i < _operators.size(); i++)
    {
        if(_operators[i].getNick() == nick)
        {
            _users.push_back(_operators[i]);
            _operators.erase(_operators.begin() + i);
            return;
        }
    }
}