#include "../includes/Server.hpp"

extern bool g_terminate;
//Constructers

Server::Server(unsigned int port, std::string password) : _port(port), _password(password), _server_fd(0)
{
    std::cout << "Starting Server..." << std::endl;
}

Server::~Server()
{
    std::cout << "Exiting Server" << std::endl;
    if(this->_server_fd > 0)
        close(this->_server_fd);
}
//Getters

std::string Server::getPassword()
{
    return this->_password;
}

unsigned int Server::getPort()
{
    return this->_port;
}

unsigned int Server::getSocketFd()
{
    return this->_server_fd;
}

//Setters

void    Server::setSocketFd(unsigned int fd)
{
    this->_server_fd = fd;
}

//Functions


//Return 1 on Error, 0 on noraml Shutdown
bool    Server::startServer()
{
    struct sockaddr_in serv_addr;

    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->_server_fd < 0) // -1 on fail
    {
        std::cout << "Failed creating a socket" << std::endl;
        return FAILED;
    }

    // Make Socket reusable on restart, otherwise binding fails
    const int enable = 1;
    setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->_port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;


    if(bind(this->_server_fd, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {    
        std::cout << "Binding failed" << std::endl;
        close(this->_server_fd);
        perror("bind");
        return FAILED;
    }

    if(listen(this->_server_fd, MAX_CLIENTS) < 0)
    {
        std::cout << "Couldn't listen" << std::endl;
        close(this->_server_fd);
        return FAILED;
    }
    return WORKED;
}

bool    Server::addClient()
{
    struct sockaddr_in cli_addr;
    int cliLen;

    pollfd pollClient;

    //add new client
    cliLen = sizeof(cli_addr);
    pollClient.fd = accept(this->_server_fd, (sockaddr *) &cli_addr, (socklen_t *) &cliLen);
    if(pollClient.fd < 0)
    {
        std::cout << "accept() failed" << std::endl;
        //Function which closes all FD in vector
        return FAILED;
    }

    pollClient.events = POLLIN;
    this->_sockets.push_back(pollClient);
    const char *hi = "NOTICE :Welcome to the IRC server!\r\n";
    write(pollClient.fd, hi, strlen(hi));
    Client newClient(pollClient.fd, *this);
    this->_clients.push_back(newClient);
}

void    Server::handleMessage(int socket_fd)
{
    int     len;
    char    buffer[BUFF_LEN];
    std::string buf;
	try
	{
		Client &active_client = getClientByFd(socket_fd);

		len = recv(socket_fd, buffer,  BUFF_LEN - 1, 0); // read one less to null terminate

		if(len < 0)
		{
			std::cout << "Error with recv()" << std::endl;
			return;
		}
		else if(len == 0)
		{
			this->removeClientAndFd(socket_fd);
            return;
		}
		buffer[len] = 0;
		std::cout << buffer << std::endl;
		buf = buffer;

		//Below Parses commands into std::vector<std::vector<std::string> >

		active_client.parseCmds(buf);
		this->cmdLoop(active_client);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

bool    Server::serverLoop()
{
    pollfd serverPoll;
    serverPoll.fd = this->_server_fd;
    serverPoll.events = POLLIN;
    this->_sockets.push_back(serverPoll);

    std::cout << "Waiting for connections..." << std::endl;

    while(!g_terminate)
    {
        if(poll(this->_sockets.data(), this->_sockets.size(), -42) < 0)
        {
            std::cout << "Poll() failed" << std::endl;
            //Function which closes all FD in vector
            return FAILED;
        }

        for(int i = 0; i < this->_sockets.size(); i++)
        {
            if(this->_sockets[i].revents & POLLIN)
            {
                if(this->_sockets[i].fd == this->_server_fd)
                {
                    addClient();
                }
                else
                {
                    handleMessage(this->_sockets[i].fd);
                }
            }
        }
    }
}

void    Server::removeClientAndFd(int fd)
{
    for(unsigned long i = 0; i < this->_sockets.size(); i++)
    {
        if(fd == this->_sockets[i].fd)
            this->_sockets.erase(this->_sockets.begin() + i);
    }

    for(unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if(fd == this->_clients[i].getSocketFd());
            this->_clients.erase(_clients.begin() + i);
    }
}

Client &Server::getClientByFd(int fd)
{
    for(int i = 0; i < this->_clients.size(); i++)
    {
        if(this->_clients[i].getSocketFd() == fd)
            return this->_clients[i];
    }
    throw ExpextionNoMatchingClient();
}

Client &Server::getClientByNick(std::string nick)
{
	for(int i = 0; i < this->_clients.size(); i++)
	{
		if(this->_clients[i].getNick() == nick)
			return this->_clients[i];
	}
	throw ExpextionNoMatchingClient();
}

bool    Server::isNickInUse(std::string &nick)
{
    for(int i = 0; i < this->getClients().size(); i++)
    {
        if(this->getClients()[i].getNick() == nick)
            return FAILED;
    }
    return WORKED;
}

void    Server::cmdLoop(Client &client)
{
    while(client.getCmds().empty() == false)
    {
        execCmd(client);
        client.getCmds().erase(client.getCmds().begin());
    }
    client.sendReply();
    this->sendWelcome(client);
}

void    Server::sendWelcome(Client &client)
{
    //Send welcome MSGs, create MACROS first!
    if(client.getIsRegistered() == false && client.getIsWelcomeSend() == false && 
        client.getNick().empty() != true && client.getUsername().empty() != true && client.getPassMatch() == true)
    {
        client.setIsRegistered(true);
        client.addReply(RPL_WELCOME(client.getNick(), client.getUsername()));
        client.addReply(RPL_YOURHOST((std::string)"localhost",(std::string) "1.0", client.getNick()));
        client.addReply(RPL_CREATED((std::string) "2023.09.10", client.getNick()));
        client.addReply(RPL_MYINFO((std::string) "localhost", (std::string) "1.0", (std::string) "o", (std::string) "itkol", client.getNick()));
        client.addReply(RPL_ISUPPORT((std::string) "MAXCHANNELS=20 CHANNELLEN=32 NICKLEN=30", client.getNick()));
        client.sendReply();
    }
}

bool    Server::isUnregisteredCheck(Client &client, std::string cmd)
{
    if(client.getIsRegistered() == false)
    {
        if(cmd != "JOIN" && cmd != "USER" && cmd != "NICK" && cmd != "PASS" && cmd != "CAP")
        {
            std::cout << "Command " << cmd << "is not valid, Client not Registered yet" << std::endl;
            return false;
        }
    }
    return true;
}

void Server::execCmd(Client &client)
{
	std::map<std::string, CommandFunction> cmdMap = fillCmd();
    std::string cmd = client.getCmds()[0][0];

    if(isUnregisteredCheck(client, cmd) == false)
        return;
	if (CmdIsValid(cmd, cmdMap))
		cmdMap[cmd](*this, client, client.getCmds()[0]);
	else
		std::cout << "Command " << cmd << "is not valid" << std::endl;
}

bool Server::channelExists(std::string &name)
{
    for(int i = 0; i < this->getChannels().size(); i++)
    {
        if(this->getChannels()[i].getName() == name)
            return true;
    }
    return false;
}

void    Server::joinChannel(Client &client, std::string &channel, std::string &key)
{
    for(int i = 0; i < this->getChannels().size(); i++)
    {
        if(this->getChannels()[i].getName() == channel)
            this->getChannels()[i].addUser(client, key);
    }
}

void    Server::createChannel(Client &client, std::string &channel, std::string &key)
{
    Channel ch(channel, key, client);
    ch.sendWelcome(client);
    this->_channels.push_back(ch);
}

Channel&	Server::getChannelByName(std::string &name)
{
	for(int i = 0; i < this->getChannels().size(); i++)
	{
		if(this->getChannels()[i].getName() == name)
			return this->getChannels()[i];
	}
}

void Server::addReplyGroup(std::string msg, std::vector<Client> &clients)
{
	for(int i = 0; i < clients.size(); i++)
	{
		clients[i].addReply(msg);
	}
}

void Server::sendReplyGroup(std::vector<Client> &clients)
{
	for(int i = 0; i < clients.size(); i++)
	{
		clients[i].sendReply();
	}
}

bool Server::isOperator(std::string nick, std::string channel)
{
    if(this->channelExists(channel) == false)
        return false;
    Channel &ch = this->getChannelByName(channel);
    for(int i = 0; i < ch.getOperators().size(); i++)
    {
        if(ch.getOperators()[i].getNick() == nick)
            return true;
    }
    return false;
}

bool Server::isUserInChannel(std::string nick, std::string channel)
{
    if(this->channelExists(channel) == false)
        return false;
    Channel &ch = this->getChannelByName(channel);
    for(int i = 0; i < ch.getUsers().size(); i++)
    {
        if(ch.getUsers()[i].getNick() == nick)
            return true;
    }
    return false;
}