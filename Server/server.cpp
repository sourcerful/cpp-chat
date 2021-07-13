#include <iostream>
#include <WinSock2.h>
#include "server.h"
#include <thread>

using namespace std;

static bool SUCCESS_con = false;

Server::Server(int port)
{
    memset(data, 0, BUFF_SIZE);
    this->port = port;
    
    //init winsock, WSA variables - environment supporting socket programming on windows.
    WORD version = MAKEWORD(2, 2); //version 2.2 or 0x0202
    int validws = WSAStartup(version, &wsData);     
    if (validws != 0) //check if works
    {
        cerr << "Couldn't initialize winsock" << endl;
        exit(1);
    }

    //create socket - SOCKET is a number, a file handler, means that the writing is going to be into that file num
    connection_s = socket(AF_INET, SOCK_STREAM, 0);
    listening_s = socket(AF_INET, SOCK_STREAM, 0); //0 can be also replaced with IPPROTO_TCP
    // Bind an IP address and a port to the socket.  
    addr.sin_family = AF_INET; // version 4
    addr.sin_port = htons(port); //host to network short - network uses big endian, so it converts.
    addr.sin_addr.S_un.S_addr = ip_addr; //any ip - INADDR_ANY, or inet_addr(*address*)

    if (listening_s == INVALID_SOCKET)
    {
        cerr << "couldn't create a socket." << endl;
        exit(1);
    }  
    if (bind(listening_s, (sockaddr*)&addr, sizeof(addr)) < 0)  
    {
        cerr << "Error binding socket." << endl;
        exit(1);
    }
    //tell winsock the socket is for listening
    listen(listening_s, SOMAXCONN); // the socket is for listening, being able to listen to maximum amount.
    cout << "Listening..." << endl;
}
void Server::accept_con()
{
    while (true)
    {
        
        int con_size = sizeof(addr);
        connection_s = accept(listening_s, (sockaddr*)&addr, &con_size); //puts the client info into addr.
        cout << "Connection from "; 
        add_client(connection_s);

        if (connection_s != INVALID_SOCKET)
            break;  
        thread t1(&Server::recieve_messages, this); //calling object methods requires pointer to member
                                                      //and the object itself, the third is the parameters but
                                                      //there are none.
    }
}
void Server::add_client(SOCKET &client)
{
    clients.push_back(client);
}
void Server::recieve_messages(SOCKET client)
{
    if(!SUCCESS_con)
    {
        strcpy(data, "server connected...\n");
        cout << "server is running." << endl;
        SUCCESS_con = true;
    }

    while (true)
    {
        send(clients[0], data, BUFF_SIZE, 0);

        if (recv(clients[0], data, BUFF_SIZE, 0) == 0)
            break;
        if (*data == '*')
            break;

        cin.getline(data, BUFF_SIZE);
    }
}
void Server::close()
{
    closesocket(listening_s);
    closesocket(connection_s);
    WSACleanup();
}