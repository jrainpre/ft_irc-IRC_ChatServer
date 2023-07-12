#include "../includes/Client.hpp"
#include "../includes/commands.hpp"

Client::Client(int socket_fd, Server &server): _socket_fd(socket_fd), _pass_match(false), _is_registered(false), _state(0), _mode(0), _server(server), _is_welcome_send(false)
{
    std::cout << "Client Created" << std::endl;
}

Client::~Client()
{

}

void Client::addChannel(Channel channel)
{
    this->_channels.push_back(channel);
}

void Client::removeChannel(std::string channel)
{
    //later
}

// bool Client::check_nick(std::string nick)
// {
    
// }

// bool Client::check_username(std::string username)
// {

// }


void Client::parseCmds(std::string &buf)
{
    std::vector<std::string> firstSplit, secondSplit;
    if(buf.find("\r\n") == std::string::npos)
        return;
    firstSplit = split(buf, "\r\n", true);
    for(unsigned long i = 0; i < firstSplit.size(); i++)
    {
        this->_cmds.push_back(split(firstSplit[i], " ", false));
    }

	for(unsigned long i = 0; i < this->_cmds.size(); i++)
	{
		if(this->_cmds[i][this->_cmds[i].size() - 1] != "\r\n")
			this->_cmds.erase(_cmds.begin() + i);
		else
			this->_cmds[i].erase(this->_cmds[i].end());
	}
}

void Client::sendReply()
{
	if(this->replyCmd.empty() == false)
	{
    	send(this->_socket_fd, this->replyCmd.c_str(), this->replyCmd.size(), MSG_NOSIGNAL);
    	this->replyCmd.clear();
	}
}



Client& Client::operator=(const Client& other) {
	_socket_fd = other._socket_fd;
	_nick = other._nick;
	_username = other._username;
	_realname = other._realname;
	_state = other._state;
	_mode = other._mode;
	_pass_match = other._pass_match;
	_is_registered = other._is_registered;
	_channels = other._channels;
	_cmds = other._cmds;
	cmdBuf = other.cmdBuf;
	_server = other._server;
	
	return *this;
}

