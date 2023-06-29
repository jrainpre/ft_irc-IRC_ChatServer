#pragma once

#include "../includes/irc.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <vector>

#define MAX_CLIENTS 100
#define BUFF_LEN 1024

class Client;

class Server {
private:
	unsigned int _port;
	std::string _password;
    unsigned int _server_fd;
    std::vector<Client> _clients;
    std::vector <struct pollfd> _sockets;
public:
    //Constructors
	Server(unsigned int port, std::string password);
	~Server();
	
    //Getters
    std::string getPassword();
	unsigned int getPort();
    unsigned int getSocketFd();
    
    //Setters
    void    setSocketFd(unsigned int fd);

    //Functions
    bool    startServer();
    bool    serverLoop();
    bool    addClient();
    void    handleMessage(int socket_fd);
    void    removeClientAndFd(int fd);
};
