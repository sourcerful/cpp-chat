#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>

#define BUFF_SIZE 1024

class Client
{
private:
    int port;
    WSADATA wsData;
    sockaddr_in addr; //socket address input.
    SOCKET client_socket;
    char data[BUFF_SIZE];
    char name[BUFF_SIZE];
    char* ip; //loopback addr = "127.0.0.1"
    bool sExit = false;
public:
    Client(const char* ip_addr);
    ~Client();
    int connect_socket();
    void start_chat();
    void incoming_messages();
    void send_messages();
    void close();
};