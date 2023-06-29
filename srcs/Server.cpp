#include "../includes/Server.hpp"

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
    Client newClient(pollClient.fd);
    this->_clients.push_back(newClient);
}

void    Server::handleMessage(int socket_fd)
{
    int     len;
    char    buffer[BUFF_LEN];
    std::string buf;

    len = recv(socket_fd, buffer,  BUFF_LEN - 1, 0); // read one less to null terminate
    
    if(len < 0)
    {
        std::cout << "Error with recv()" << std::endl;
        return;
    }
    else if(len == 0)
    {
        this->removeClientAndFd(socket_fd);
    }
    buffer[len] = 0;
    std::cout << buffer << std::endl;
    buf = buffer;

    //Below Parses commands into std::vector<std::vector<std::string> >

    //execCmds();

}

bool    Server::serverLoop()
{
    pollfd serverPoll;
    serverPoll.fd = this->_server_fd;
    serverPoll.events = POLLIN;
    this->_sockets.push_back(serverPoll);

    std::cout << "Waiting for connections..." << std::endl;

    while(true)
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