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
		this->execCmds(active_client);
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

void    Server::execCmds(Client &active_client)
{
    while(active_client.getCmds().empty() == false)
    {
        // if(active_client.getIsRegistered() == false)
        // {
        //     this->unregisteredCmds(active_client);
        // }
        


        active_client.getCmds().erase(active_client.getCmds().begin());
    }
}

// void    Server::unregisteredCmds(Client &active_client)
// {
//     std::string msg;

//     if(active_client.getCmds()[0].size() > 1 &&  active_client.getCmds()[0][0] == "PASS")
//     {
//         if(active_client.getCmds()[0][1] == this->_password)
//         {
//             active_client.setPassMatch(true);
//             std::cout << "Correct Password" << std::endl;
//         }
//         else
//             std::cout << "Wrong Password" << std::endl;
//     }
//     else if(active_client.getCmds()[0].size() > 1 &&  active_client.getCmds()[0][0] == "NICK") //Check first if Nick Exists
//         active_client.setNick(active_client.getCmds()[0][1]);
//     else if(active_client.getCmds()[0].size() > 1 && active_client.getCmds()[0][0] == "USER") //Check first if User exists
//     {
//         active_client.setUsername(active_client.getCmds()[0][1]);
//         active_client.setRealname(active_client.getCmds()[0][2]);
//     }
//     else if(active_client.getCmds()[0].size() > 0 && active_client.getCmds()[0][0] == "JOIN")
//     {
//         msg = "451 * :You have not registered\r\n";
//         write(active_client.getSocketFd(), msg.c_str(), msg.size());
//     }

//     if(active_client.getNick().empty() != true && active_client.getUsername().empty() != true && active_client.getPassMatch() == true)
//     {
//         active_client.setIsRegistered(true);
//         std::cout << "User Registered" << std::endl;
//         this->sendWelcome(active_client);
//     }
// }

void    Server::sendWelcome(Client &active_client)
{
    //Send welcome MSGs, create MACROS first!
    active_client.addReply(RPL_WELCOME(active_client.getNick(), active_client.getUsername()));
    active_client.addReply(RPL_YOURHOST((std::string)"localhost",(std::string) "1.0", active_client.getNick()));
    active_client.addReply(RPL_CREATED((std::string) "2023.09.10", active_client.getNick()));
    active_client.addReply(RPL_MYINFO((std::string) "localhost", (std::string) "1.0", (std::string) "o", (std::string) "itkol", active_client.getNick()));
    active_client.addReply(RPL_ISUPPORT((std::string) "MAXCHANNELS=20 CHANNELLEN=32 NICKLEN=30", active_client.getNick()));
    active_client.sendReply();
}

void Server::registeredCmds(Client &client)
{
	std::map<std::string, CommandFunction> cmd = fillCmd();
	if (CmdIsValid(client.getCmds()[0][0], cmd))
		cmd[client.getCmds()[0][0]](*this, client, client.getCmds()[0]);
	else
		std::cout << "Command " << client.getCmds()[0][0] << "is not valid" << std::endl;
}

// //client wants to write
//                     bzero(buffer, BUFF_LEN);
//                     len = read(clientSockets[i].fd, buffer,  BUFF_LEN - 1); // read one less to null terminate
//                     if(len <= 0)
//                     {
//                         //Client lost connection
//                         clientSockets.erase(clientSockets.begin() + i);
//                     }
//                     else{
//                         std::cout << buffer << std::endl;
//                         buf = buffer;
//                         if(buf.find("\r\n") != std::string::npos && buf.find("JOIN") != std::string::npos)
//                         {
//                             msg = "451 * :You have not registered\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());
//                         }
//                         if(buf.find("\r\n") != std::string::npos && buf.find("USER") != std::string::npos)
//                         {
//                             msg = "001 :Welcome, this is a test\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());

//                             msg = "002 :YourHost, this is a test\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());

//                             msg = "003 :WasCreated, this is a test\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());

//                             msg = "004 :MyInfo, this is a test\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());

//                             msg = "005 :IsSupport, this is a test\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());

//                             msg = "MOTD :Message of day HELLLO\r\n";
//                             write(clientSockets[i].fd, msg.c_str(), msg.size());
//                         }