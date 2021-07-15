#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <WinSock2.h>

#define BUFF_SIZE 1024

class Server
{
public:
    SOCKET listening_s;
    SOCKET connection_s; //connection to client
    char data[BUFF_SIZE];
    std::vector<SOCKET> clients;
private:
    int port;
    ULONG ip_addr = INADDR_ANY;
    WSADATA wsData;
    sockaddr_in addr; //socket address input.
public:
    Server(int port = 1234);
    ~Server();
    void accept_con();
    void recieve_messages(SOCKET &client);
    void add_client(SOCKET &client);
    void broadcast_message(SOCKET &client);
private:
    void close();
};