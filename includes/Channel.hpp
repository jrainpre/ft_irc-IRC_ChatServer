#pragma once

#include "Client.hpp"
#include <string>
#include <vector>

class Client;

class Channel {
private:
	std::string _name;
	std::string _topic;
	std::vector<Client> _users;
public:
	Channel(std::string name);
	~Channel();
	std::string getName();
	std::string getTopic();
	std::vector<Client> getUsers();
	void setName(std::string name);
	void setTopic(std::string topic);
	void addUser(Client user);
	void removeUser(Client user);
	bool check_channel_name(std::string name);


};
