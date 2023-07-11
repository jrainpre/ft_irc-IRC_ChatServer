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
#include <signal.h>

#include "../includes/irc.hpp"



void testfunc(Server &server, Client &client)
{
    std::cout << "testfunc" << std::endl;
}

std::map<std::string, void (*)(Server &server, Client &client)> cmdHandler()
{
    std::map<std::string, void (*)(Server &server, Client &client)> cmd;
    cmd["Test"] = testfunc;
    return cmd;
}

bool g_terminate = false;

void signalhandler(int)
{
    g_terminate = true;
    std::cout << "Terminating server..." << std::endl;
}


int main(int argc, char **argv)
{

    if(argc != 3 || validPort(argv[1]) == 0)
        return errorMsg("Usage: ./ircserv <port> <password>");
    Server server(std::atoi(argv[1]), argv[2]);
    signal(SIGINT, signalhandler);
    if(server.startServer() == FAILED)
        return 1;
    if(server.serverLoop() == FAILED)
        return 1;
    return (0);
}

// std::vector<std::string> split(std::string s, std::string delimit)
// {
// }