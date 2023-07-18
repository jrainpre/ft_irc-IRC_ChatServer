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
	bool			_has_key;
	bool			_topic_restricted;
	bool			_has_userlimit;
	unsigned int	_clients_limit;

public:
	Channel(std::string name, std::string key);
	Channel(std::string name, std::string key, Client &client);
	~Channel();
	std::string getName(){return this->_name;}
	std::string getTopic(){return this->_topic;}
	std::string getKey(){return this->_key;}
	std::vector<Client> &getUsers();
	std::vector<Client> &getOperators();
	std::vector<Client> &getInvited();
	bool getInviteOnly(){return this->_invite_only;}
	bool getHasKey(){return this->_has_key;}
	bool getTopicRestricted(){return this->_topic_restricted;}
	bool getHasUserlimit(){return this->_has_userlimit;}
	unsigned int getClientsLimit(){return this->_clients_limit;}

	void addOperator(Client &client){_operators.push_back(client);}

	void setName(std::string name){this->_name = name;}
	void setInviteOnly(bool inv){this->_invite_only = inv;}
	void setTopic(std::string topic);
	void setHasKey(bool newKey){this->_has_key = newKey;}
	void setTopicRestriction(bool newRestriction){this->_topic_restricted = newRestriction;}
	void setHasUserlimit(bool limit){this->_has_userlimit = limit;}
	void setKey(std::string key){this->_key = key;}
	void setClientLimit(unsigned long limit){this->_clients_limit = limit;}

	bool addUser(Client &user, std::string &key);
	void removeUser(Client user);
	bool check_channel_name(std::string name);

	bool isClientInvited(std::string nick);
	bool isClientInChannel(std::string nick);
	void sendWelcome(Client &client);
	void clientsInChannel(Client &client);
	void sendJoinMsgs(std::string clientNick);
	void inviteUser(Client &client, Client &target);
	std::string getAllModes();
	void promoteUser(std::string &nick);
	void demoteUser(std::string &nick);
};
