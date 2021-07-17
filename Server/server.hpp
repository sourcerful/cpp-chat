#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <thread>

#define BUFF_SIZE 1024

class Server
{
private:
    int port;
    ULONG ip_addr = INADDR_ANY;
    WSADATA wsData;
    sockaddr_in addr; //socket address input.
    SOCKET listening_s;
    SOCKET connection_s; //connection to client
    std::vector<SOCKET> clients;
    std::vector<std::thread> threads;
public:
    Server(int port = 1234);
    ~Server();
    void accept_con();
    void recieve_messages(SOCKET client);
    void add_client(SOCKET client);
    void broadcast_message(SOCKET &client, char *data);
private:
    void close();
};