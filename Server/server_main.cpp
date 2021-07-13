#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include "server.h"
#include <thread>

#pragma comment (lib, "ws2_32.lib") //including the library to the LINKER settings.

using namespace std;

int main()
{
    Server server(5555);
    server.accept_con();

    strcpy(server.data, "Connection terminated.\n");
    send(server.clients[0], server.data, BUFF_SIZE, 0);
    server.close();

    return 0;
}