/*
 * Tutorial:
 * http://www.linuxhowtos.org/C_C++/socket.htm
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <iostream>

void error(const char *message)
{
    perror(message);
    exit(1);
}

int main(void)
{
    socklen_t  sock_fd, newsock_fd,  client_addr_len;
    int  msg_received, msg_send;
    struct sockaddr_in server, client;
    char client_message[512];

    uint16_t port_number = 1337;

    //Creating socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1)
    {
        error("ERROR: Can't open socket!");
    }

    //Setting up server details
    memset(&server, 0, sizeof(server)); //cleaning up struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port_number); //host-to-network short

    //Binding socket we just created to an address!
    if( bind(sock_fd, (const sockaddr *)&server, sizeof(server)) < 0 )
    {
        error("ERROR: Can't bind sock! Are you sure if this port is not already used?");
    }

    //Start socket listening for connections
    listen(sock_fd, 5); // If sockfd is valid, this can't fail!
    //5 the number of connections that can be waiting while the process is handling a particular connection

    puts("Waiting for incoming connections...");
    //Accepting client connection. It actually block process till the client is getting connected
    client_addr_len = sizeof(client);
    newsock_fd = accept(sock_fd, (struct sockaddr *)&client, &client_addr_len);
    if(newsock_fd < 0)
    {
        error("ERROR: Accept failed!");
    }
    write(newsock_fd, "Welcome!\n", 9);

    //We want to exchange information, so we use a loop
    while(1)
    {
        //Time to read message from the other side
        memset(&client_message, 0, sizeof(client_message));
        msg_received = (int) read(newsock_fd, client_message, sizeof(client_message));
        if (msg_received < 0)
        {
            //Current connection failed. We wait for another
            perror("ERROR");

            puts("Waiting for another connection...");
            //Accepting client connection. It actually block process till the client is getting connected
            client_addr_len = sizeof(client);
            newsock_fd = accept(sock_fd, (struct sockaddr *)&client, &client_addr_len);
            if(newsock_fd < 0)
            {
                perror("ERROR: Accept failed!");
            }
            write(newsock_fd, "Welcome!\n", 9);
        }
        else
        {
            std::string tmp(client_message);
            tmp.erase( std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end() );
            tmp.push_back('\0');

            if( tmp.find("stop") != std::string::npos)
            {
                puts("Received STOP command!");
                break;
            }

            if( !tmp.empty() )
            {
                puts("\nMessage received: \n===========");
                puts(tmp.c_str());
                puts("===========");
            }
        }

        //Time to send back a kind of ACK, we let client known that we have his message!
        char ackMsg[] = "Roger that!\n";
        msg_send = (int) write(newsock_fd, ackMsg, strlen(ackMsg));
        if (msg_send < 0)
        {
            error("Can't send back confirmation for message received to client!");
        }
    }

    close(sock_fd);
    close(newsock_fd);

    return 0;
}
