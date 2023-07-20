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
#include <ctime>

#include "../includes/irc.hpp"


bool g_terminate = false;


void signalhandler(int)
{
	(void)signal;
    g_terminate = true;
    std::cout << "Terminating server..." << std::endl;
}

struct tm *getTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	return timeinfo;
}

int main(int argc, char **argv)
	{
    if(argc != 3 || validPort(argv[1]) == 0)
        return errorMsg("Usage: ./ircserv <port> <password>");
    Server server(std::atoi(argv[1]), argv[2]);
    server.setStartupTime(getTime());
    signal(SIGINT, signalhandler);
    if(server.startServer() == FAILED)
        return 1;
    if(server.serverLoop() == FAILED)
        return 1;
    return (0);
}
