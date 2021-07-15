#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include "server.hpp"
#include <thread>

#pragma comment (lib, "ws2_32.lib") //including the library to the LINKER settings.

using namespace std;

int main()
{
    Server server(5555);
    server.accept_con();

    cout << endl;
    cout << "Connection terminated.\n";
    strcpy(server.data, "Connection terminated.\n");
    for(auto s: server.clients)
        send(s, server.data, BUFF_SIZE, 0);

    return 0;
}