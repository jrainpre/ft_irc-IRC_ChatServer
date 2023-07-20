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

	struct tm *act_time = getTime();
	std::cout << "Server started at " << act_time->tm_hour << ":" << act_time->tm_min << ":" << act_time->tm_sec << std::endl;
	std::cout << "Server started at " << act_time->tm_mday << "/" << act_time->tm_mon + 1 << "/" << act_time->tm_year + 1900 << std::endl;
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
