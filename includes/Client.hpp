#pragma once

#include "irc.hpp"
#include "Channel.hpp"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

// enum class ClientState {
// 	CONNECTED,
// 	REGISTERED,
// 	AWAY,
// 	QUIT
// };

class Channel;

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
	std::vector<std::vector<std::string> > _cmds;
	std::string cmdBuf;

public:
	Client(int socket_fd);
	~Client();
	std::string getNick();
	std::string getUsername();
	std::string getRealname();
	unsigned int getState();
	unsigned int getMode();
	int getSocketFd();
	std::vector<Channel> getChannels();
	int	getSocketFd();

	void setNick(std::string nick);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setState(unsigned int state);
	void setMode(unsigned int mode);
	void addChannel(Channel channel);
	void removeChannel(std::string channel);
	bool check_nick(std::string nick);
	bool check_username(std::string username);
	std::vector<std::vector<std::string> > getCmds();
	void parseCmds(std::string &buf);
	void execCmds();
	void unregisteredMsgs();
};