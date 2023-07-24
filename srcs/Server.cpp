#include "../includes/Server.hpp"

//Constructers

Server::Server(unsigned int port, std::string password) : _port(port), _password(password), _server_fd(0)
{
    std::cout << "Starting Server..." << std::endl;
}

Server::~Server()
{
    std::cout << "Exiting Server" << std::endl;
	this->freeServer();
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

bool Server::addClient()
{
    struct sockaddr_in cli_addr;
    int cliLen;

    pollfd pollClient;

    // add new client
    cliLen = sizeof(cli_addr);
    pollClient.fd = accept(this->_server_fd, (sockaddr*)&cli_addr, (socklen_t*)&cliLen);
    if (pollClient.fd < 0)
    {
        std::cout << "accept() failed" << std::endl;
        return FAILED;
    }

    pollClient.events = POLLIN | POLLOUT;
    pollClient.revents = 0;
    this->_sockets.push_back(pollClient);
    write(pollClient.fd, WELCOME, strlen(WELCOME));
    Client* newClient = new Client(pollClient.fd, *this);
    this->_clients.push_back(newClient);
    return WORKED;
}



void    Server::handleMessage(int socket_fd)
{
    int     len;
    char    buffer[BUFF_LEN];
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
		active_client.setCmdBuf(active_client.getCmdBuf() + buffer);
		if (!active_client.cmdIsTerminated())
			return;
		active_client.parseCmds();
		active_client.setCmdBuf("");
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
    serverPoll.events = POLLIN | POLLOUT;
    serverPoll.revents = 0;
    this->_sockets.push_back(serverPoll);

    std::cout << "Waiting for connections..." << std::endl;

     while(g_terminate == false)
    {
        if(poll(this->_sockets.data(), this->_sockets.size(), -42) < 0 && !g_terminate)
        {
            std::cout << "Poll() failed" << std::endl;
            return FAILED;
        }
        for(size_t i = 0; i < this->_sockets.size(); i++)
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
					break;
                }
    	    }
			else if(this->_sockets[i].revents & POLLOUT)
			{
				handleWrite(this->_sockets[i].fd);
			}
		}
    }
	return WORKED;
}

void Server::handleWrite(int socket_fd)
{
	Client &active_client = getClientByFd(socket_fd);
	active_client.sendReply();
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
        if(fd == this->_clients[i]->getSocketFd())
        {
            delete this->_clients[i];
            this->_clients.erase(_clients.begin() + i);
        }
    }
}

void    Server::removeClientAndFdByNick(std::string nick)
{
	Client &client = getClientByNick(nick);
    for(unsigned long i = 0; i < this->_sockets.size(); i++)
    {
        if(client.getSocketFd() == this->_sockets[i].fd)
            this->_sockets.erase(this->_sockets.begin() + i);
    }
    for(unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if(client.getNick() == this->_clients[i]->getNick())
        {
            delete this->_clients[i];
            this->_clients.erase(_clients.begin() + i);
        }
    }
}

Client& Server::getClientByFd(int fd)
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i]->getSocketFd() == fd)
            return *this->_clients[i]; 
    }
    throw ExpextionNoMatchingClient();
}

Client &Server::getClientByNick(std::string nick)
{
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		if(this->_clients[i]->getNick() == nick)
			return *this->_clients[i];
	}
	throw ExpextionNoMatchingClient();
}

bool    Server::isNickInUse(std::string &nick)
{
    for(size_t i = 0; i < this->getClients().size(); i++)
    {
        if(this->getClients()[i]->getNick() == nick)
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
		if (client.getIsQuited())
		{
            this->deleteClientCheckChannels(client);
			this->removeClientAndFdByNick(client.getNick());
			return;
		}
    }
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
        client.addReply(RPL_CREATED(this->_startup_time, client.getNick()));
        client.addReply(RPL_MYINFO((std::string) "localhost", (std::string) "1.0", (std::string) "o", (std::string) "itkol", client.getNick()));
        client.addReply(RPL_ISUPPORT((std::string) "MAXCHANNELS=20 NICKLEN=30", client.getNick()));
        sendMsgOfDay(client);
    }
}

void    Server::sendMsgOfDay(Client &client)
{
    std::string irc_art = 
    "IIIIIIIIII  RRRRRRRRRRRRRRRRR            CCCCCCCCCCCCC\n"
    "I::::::::I  R::::::::::::::::R        CCC::::::::::::C\n"
    "I::::::::I  R::::::RRRRRR:::::R     CC:::::::::::::::C\n"
    "II::::::II  RR:::::R     R:::::R   C:::::CCCCCCCC::::C\n"
    "  I::::I      R::::R     R:::::R  C:::::C       CCCCCC\n"
    "  I::::I      R::::R     R:::::RC:::::C\n"
    "  I::::I      R::::RRRRRR:::::R  C:::::C\n"
    "  I::::I      R:::::::::::::RR   C:::::C\n"
    "  I::::I      R::::RRRRRR:::::R  C:::::C\n"
    "  I::::I      R::::R     R:::::RC:::::C\n"
    "  I::::I      R::::R     R:::::RC:::::C\n"
    "  I::::I      R::::R     R:::::R  C:::::C       CCCCCC\n"
    "II::::::II  RR:::::R     R:::::R   C:::::CCCCCCCC::::C\n"
    "I::::::::I  R::::::R     R:::::R    CC:::::::::::::::C\n"
    "I::::::::I  R::::::R     R:::::R      CCC::::::::::::C\n"
    "IIIIIIIIII  RRRRRRRR     RRRRRRR         CCCCCCCCCCCCC";
    std::istringstream iss(irc_art);
    std::string line;
    while(std::getline(iss, line))
        client.addReply(MESSAGE_OF_THE_DAY(client.getNick(), line));
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
{	std::map<std::string, CommandFunction> cmdMap = fillCmd();
    std::string cmd = client.getCmds()[0][0];

	if (isUnregisteredCheck(client, cmd) == false)
		return ;
	if (CmdIsValid(cmd, cmdMap))
		cmdMap[cmd](*this, client, client.getCmds()[0]);
	else
		std::cout << "Command " << cmd << "is not valid" << std::endl;
}

bool Server::channelExists(std::string &name)
{
    for(size_t i = 0; i < this->getChannels().size(); i++)
    {
        if(this->getChannels()[i]->getName() == name)
            return true;
    }
    return false;
}

void    Server::joinChannel(Client &client, std::string &channel, std::string &key)
{
    for(size_t i = 0; i < this->getChannels().size(); i++)
    {
        if(this->getChannels()[i]->getName() == channel)
        {
            if(this->getChannels()[i]->addUser(client, key) == false)
                return;
            this->addReplyGroup(JOIN(client.getNick(), this->getChannels()[i]->getName()), this->getChannels()[i]->getUsers(), client);
            this->addReplyGroup(JOIN(client.getNick(), this->getChannels()[i]->getName()), this->getChannels()[i]->getOperators(), client);
            if(this->getChannels()[i]->getTopic().empty() == false)
                client.addReply(RPL_TOPIC(client.getNick(), channel, this->getChannels()[i]->getTopic()));
            return;
        }
    }
}

void Server::createChannel(Client& client, std::string& channel, std::string& key)
{
    Channel* ch = new Channel(channel, key, client);
    ch->sendWelcome(client);
    this->_channels.push_back(ch);
	client.addChannel(ch);
}

Channel& Server::getChannelByName(std::string& name)
{
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i]->getName() == name)
            return *(this->_channels[i]); 
    }
    throw std::runtime_error("Channel not found");
}

void Server::addReplyGroup(std::string msg, std::vector<Client *> clients, Client &sender)
{
	for(size_t i = 0; i < clients.size(); i++)
	{
		if(clients[i]->getNick() != sender.getNick())
			clients[i]->addReply(msg);
	}
}

void Server::sendReplyGroup(std::vector<Client *> clients, Client &sender)
{
	for(size_t i = 0; i < clients.size(); i++)
	{
		if(clients[i]->getNick() != sender.getNick())
		clients[i]->sendReply();
	}
}

bool Server::isOperator(std::string nick, std::string channel)
{
    if(this->channelExists(channel) == false)
        return false;
    Channel &ch = this->getChannelByName(channel);
    for(size_t i = 0; i < ch.getOperators().size(); i++)
    {
        if(ch.getOperators()[i]->getNick() == nick)
            return true;
    }
    return false;
}

bool Server::isUserInChannel(std::string nick, std::string channel)
{
    if(this->channelExists(channel) == false)
        return false;
    Channel &ch = this->getChannelByName(channel);
    for(size_t i = 0; i < ch.getUsers().size(); i++)
    {
        if(ch.getUsers()[i]->getNick() == nick)
            return true;
    }
    for(size_t i = 0; i < ch.getOperators().size(); i++)
    {
        if(ch.getOperators()[i]->getNick() == nick)
            return true;
    }
    return false;
}

void Server::freeClient()
{
	std::cout << "size: " << this-> _clients.size() << std::endl;
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
			std::cout << "Client: " << this->_clients[i]->getNick() << std::endl;
			std::cout << this->_clients[i]->getSocketFd() << std::endl;

			delete this->_clients[i];
			std::cout << "Client deleted" << std::endl;
	}
}

void Server::freeChannel()
{
	for(size_t i = 0; i < this->_channels.size(); i++)
	{
			delete this->_channels[i];
	}
}

void Server::freeSockets()
{
	for(size_t i = 0; i < this->_sockets.size(); i++)
	{
			close(this->_sockets[i].fd);
	}
}

void Server::freeServer()
{
	this->freeClient();
	this->freeChannel();
	this->freeSockets();
}

void Server::deleteEmptyChannels()
{
	for (size_t i = 0; i < this->getChannels().size(); i++)
	{
		if (this->getChannels()[i]->getUsers().size() == 0 && this->getChannels()[i]->getOperators().size() == 0)
		{
			delete this->getChannels()[i];
			this->getChannels().erase(this->getChannels().begin() + i);
		}
	}
}

void Server::deleteClientCheckChannels(Client &client)
{
	for (size_t i = 0; i < client.getChannels().size(); i++)
	{
		client.getChannels()[i]->deleteClient(client.getNick());
	}
	deleteEmptyChannels();
}

void Server::setStartupTime(struct tm *time)
{
	//date
    this->_startup_time = toString(time->tm_mday) + "."  + toString(time->tm_mon + 1) + "." + toString(time->tm_year + 1900);
	//time
    this->_startup_time += " | " + toString(time->tm_hour) + ":" + toString(time->tm_min) + ":" + toString(time->tm_sec);
}
