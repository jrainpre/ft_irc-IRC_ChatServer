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
public:
	Channel(std::string name);
	Channel(std::string name, std::string key);
	~Channel();
	std::string getName(){return this->_name;}
	std::string getTopic(){return this->_topic;}
	std::vector<Client> getUsers();

	void setName(std::string name){this->_name = name;}
	void setTopic(std::string topic){this->_topic = topic;}
	void addUser(Client &user, std::string &key);
	void removeUser(Client user);
	bool check_channel_name(std::string name);


};
