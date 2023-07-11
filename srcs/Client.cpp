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

bool Client::check_nick(std::string nick)
{
    
}

bool Client::check_username(std::string username)
{

}


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
}

void Client::sendReply()
{
    send(this->_socket_fd, this->replyCmd.c_str(), this->replyCmd.size(), MSG_NOSIGNAL);
    this->replyCmd.clear();
}

// void    Client::execCmds()
// {
//     while(this->_cmds.empty() == false)
//     {
//         if(this->_is_registered == false)
//         {
//             this->unregisteredCmds();
//         }
          


//         this->_cmds.erase(this->_cmds.begin());
//     }
// }

// void    Client::unregisteredCmds()
// {
//     std::string msg;

// void    Client::unregisteredCmds()
// {
//     std::string msg;

//     if(this->_cmds[0].size() > 1 &&  this->_cmds[0][0] == "Nick") //Check first if Nick Exists
//         this->_nick = this->_cmds[0][1];
//     else if(this->_cmds[0].size() > 1 && this->_cmds[0][0] == "USER") //Check first if User exists
//     {
//         this->_username = this->_cmds[0][1];
//         this->_realname = this->_cmds[0][2];
//     }
//     else if(this->_cmds[0].size() > 0 && this->_cmds[0][0] == "JOIN")
//     {
//         msg = "451 * :You have not registered\r\n";
//         write(this->_socket_fd, msg.c_str(), msg.size());
//     }

//     if(this->_nick.empty() != true && this->_realname.empty() != true)
//         this->_is_registered = true;
// }



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

