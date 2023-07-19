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
#include <map>

#define MAX_CLIENTS 100
#define BUFF_LEN 1024

extern bool g_terminate;

class Client;
class Channel;

class Server {
private:
    unsigned int _port;
    std::string _password;
    unsigned int _server_fd;
    std::vector<Client*> _clients;
    std::vector<Channel*> _channels;
    std::vector<struct pollfd> _sockets;

public:
    // Constructors
    Server(unsigned int port, std::string password);
    ~Server();

    // Getters
    std::string getPassword();
    unsigned int getPort();
    unsigned int getSocketFd();
    std::vector<Client*>& getClients() { return this->_clients; }
    std::vector<Channel*>& getChannels() { return this->_channels; }

    // Setters
    void setSocketFd(unsigned int fd);

    // Functions
    bool startServer();
    bool serverLoop();
    bool addClient();
    void handleMessage(int socket_fd);
    void handleWrite(int socket_fd);
    Client& getClientByFd(int socket_fd);
    void cmdLoop(Client& client);
    Client& getClientByNick(std::string nick);
    void execCmd(Client& client);
    // void unregisteredCmds(Client& active_client);
    void sendWelcome(Client& active_client);
    bool isUnregisteredCheck(Client& client, std::string cmd);

    bool channelExists(std::string& name);
    void joinChannel(Client& client, std::string& channel, std::string& key);
    void createChannel(Client& client, std::string& channel, std::string& key);
    bool isNickInUse(std::string& nick);
    Channel& getChannelByName(std::string& name);

    bool isOperator(std::string nick, std::string channel);

    class ExpextionNoMatchingClient {
    public:
        virtual const char* what() const throw() { return "Server Error"; }
    };
    void removeClientAndFd(int fd);
    void unregisteredCmds();
    void registeredCmds(Client& active_client);
    void sendReplyGroup(std::vector<Client*> clients, Client& sender);
    void addReplyGroup(std::string msg, std::vector<Client*> clients, Client& sender);
    bool sendPrivmsgChannel(std::string channel_name, std::string message, Server& server, Client& client);
    bool isUserInChannel(std::string nick, std::string channel);
    bool sendNoticeChannel(std::string channel_name, std::string message, Server& server, Client& client);

};
