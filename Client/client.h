#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>

#define BUFF_SIZE 1024

class Client
{
public:
    SOCKET client_socket;
    char data[BUFF_SIZE];
    char name[BUFF_SIZE];
    char* ip; //loopback addr = "127.0.0.1"
private:
    int port;
    WSADATA wsData;
    sockaddr_in addr; //socket address input.
public:
    Client(const char* ip_addr);
    int connect_socket();
    void start_chat();
    void incoming_messages();
    void send_messages();
    void close();
};