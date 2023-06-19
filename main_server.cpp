#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <vector>

#define MAX_CLIENTS 100
#define BUFF_LEN 1024
#define port 1800


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    int serverSocket;
    int cliLen;
    int len;
    char buffer[BUFF_LEN];
    struct sockaddr_in serv_addr, cli_addr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) // -1 on fail
    {
        std::cout << "Failed creating a socket" << std::endl;
        std::exit(1);
    }

    // Make Socket reusable on restart, otherwise binding fails
    const int enable = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;


    if(bind(serverSocket, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {    
        std::cout << "Binding failed" << std::endl;
        close(serverSocket);
        perror("bind");
        std::exit(1);
    }

    if(listen(serverSocket, MAX_CLIENTS) < 0)
    {
        std::cout << "Couldn't listen" << std::endl;
        close(serverSocket);
        std::exit(1);
    }

    pollfd client;
    client.fd = serverSocket;
    client.events = POLLIN;
    std::vector <struct pollfd> clientSockets;
    clientSockets.push_back(client);

    std::cout << "Server started, waiting for connections" << std::endl;

    while(true)
    {
        if(poll(clientSockets.data(), clientSockets.size(), -42) < 0)
        {
            std::cout << "Poll() failed" << std::endl;
            //Function which closes all FD in vector
            std::exit(1);
        }

        for(int i = 0; i < clientSockets.size(); i++)
        {
            if(clientSockets[i].revents & POLLIN)
            {
                if(clientSockets[i].fd == serverSocket)
                {
                    //add new client
                    cliLen = sizeof(cli_addr);
                    client.fd = accept(serverSocket, (sockaddr *) &cli_addr, (socklen_t *) &cliLen);
                    if(client.fd < 0)
                    {
                        std::cout << "accept() failed" << std::endl;
                        //Function which closes all FD in vector
                        std::exit(1);
                    }
                    client.events = POLLIN;
                    clientSockets.push_back(client);
                }
                else
                {
                    //client wants to write
                    bzero(buffer, BUFF_LEN);
                    len = read(clientSockets[i].fd, buffer,  BUFF_LEN - 1); // read one less to null terminate
                    if(len <= 0)
                    {
                        //Client lost connection
                        clientSockets.erase(clientSockets.begin() + i);
                    }
                    else{
                        std::cout << buffer;
                        write(clientSockets[i].fd, "Hello\n", 6);
                    }
                }
            }
        }
    }
    close(serverSocket);
    return (0);
}