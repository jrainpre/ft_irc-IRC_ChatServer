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


void Client::parseCmds()
{
    std::vector<std::string> lines;
    std::size_t pos;
    
    lines = split(cmdBuf, "\n", false); // changed to false, delete backslash n
    for (size_t i = 0; i < lines.size(); ++i)
    {
        std::vector<std::string> split_line = split(lines[i], " ", false);
        if (!split_line.empty())
            this->_cmds.push_back(split_line);
    }
}

void Client::sendReply()
{
	if(this->replyCmd.empty() == false)
	{
    	send(this->_socket_fd, this->replyCmd.c_str(), this->replyCmd.size(), MSG_NOSIGNAL);
		std::cout << "Reply sent: " << this->replyCmd << std::endl;
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

bool Client::cmdIsTerminated()
{
    std::size_t pos;
	while ((pos = this->cmdBuf.find("\r\n")) != std::string::npos)
        this->cmdBuf.replace(pos, 2, "\n");
	if (this->cmdBuf.find("\n") != std::string::npos)
		return true;
	return false;
}

