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
#define port 6667


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    int serverSocket;
    int cliLen;
    int len;
    char buffer[BUFF_LEN];
    struct sockaddr_in serv_addr, cli_addr;

    std::string buf;
    std::string msg;

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
                    std::cout << "Client connected" << std::endl;
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
                    const char *hi = "NOTICE :Welcome to the IRC server!\r\n";
                    write(client.fd, hi, strlen(hi));
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
                        std::cout << buffer << std::endl;
                        buf = buffer;
                        if(buf.find("\r\n") != std::string::npos && buf.find("JOIN") != std::string::npos)
                        {
                            msg = "451 * :You have not registered\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());
                        }
                        if(buf.find("\r\n") != std::string::npos && buf.find("USER") != std::string::npos)
                        {
                            msg = "001 :Welcome, this is a test\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());

                            msg = "002 :YourHost, this is a test\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());

                            msg = "003 :WasCreated, this is a test\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());

                            msg = "004 :MyInfo, this is a test\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());

                            msg = "005 :IsSupport, this is a test\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());

                            msg = "MOTD :Message of day HELLLO\r\n";
                            write(clientSockets[i].fd, msg.c_str(), msg.size());
                        }

                        // if(buf.find("\r\n") != std::string::npos && buf.find("PING") != std::string::npos)
                        // {

                        // }

                    }
                }
            }
        }
    }
    close(serverSocket);
    return (0);
}

// std::vector<std::string> split(std::string s, std::string delimit)
// {
// }