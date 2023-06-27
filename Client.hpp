#pragma once

#include <string>
#include <vector>
#include "Channel.hpp"


// enum class ClientState {
// 	CONNECTED,
// 	REGISTERED,
// 	AWAY,
// 	QUIT
// };

enum class ClientMode {
	OPERATOR,
	REGULAR
};

class Client {
private:
	int _socket_fd;
	std::string _nick;
	std::string _username;
	std::string _realname;
	unsigned int _state;
	unsigned int _mode;
	bool _pass_match;
	bool _is_registered;
	std::vector<Channel> _channels;
public:
	Client(int socket_fd, std::string nick, std::string username, std::string realname);
	~Client();
	std::string getNick();
	std::string getUsername();
	std::string getRealname();
	unsigned int getState();
	unsigned int getMode();
	std::vector<Channel> getChannels();
	void setNick(std::string nick);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setState(unsigned int state);
	void setMode(unsigned int mode);
	void addChannel(std::string channel);
	void removeChannel(std::string channel);
	bool check_nick(std::string nick);
	bool check_username(std::string username);
};