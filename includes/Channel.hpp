#pragma once

#include "Client.hpp"
#include <string>
#include <vector>

class Client;

class Channel {
private:
	std::string _name;
	std::string _key;
	std::string _topic;
	std::vector<Client> _users;
	std::vector<Client> _operators;
	std::vector<Client> _invited;
	bool			_invite_only;
	unsigned int	_active_clients;
	unsigned int	_clients_limit;

public:
	Channel(std::string name);
	Channel(std::string name, std::string key);
	Channel(std::string name, std::string key, Client &client);
	~Channel();
	std::string getName(){return this->_name;}
	std::string getTopic(){return this->_topic;}
	std::string getKey(){return this->_key;}
	std::vector<Client> &getUsers();
	std::vector<Client> &getOperators();
	std::vector<Client> &getInvited();

	void addOperator(Client &client){_operators.push_back(client);}

	void setName(std::string name){this->_name = name;}
	void setTopic(std::string topic){this->_topic = topic;}
	void addUser(Client &user, std::string &key);
	void removeUser(Client user);
	bool check_channel_name(std::string name);

	bool isClientInvited(Client &client);
	void sendWelcome(Client &client);
	void clientsInChannel(Client &client);


};
