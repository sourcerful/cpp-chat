#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "server.hpp"
#include <mutex>

using namespace std;

mutex m;

Server::Server(int port)
{
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
    cout << "Listening to incoming connections." << endl;
}
void Server::accept_con()
{
    sockaddr_in client_addr;
    int clientSize;

    while (true)
    {
        clientSize = sizeof(client_addr);
        connection_s = accept(listening_s, (sockaddr*)&client_addr, &clientSize); //puts the client info into addr.
        cout << "Connection from " << "(" << inet_ntoa(client_addr.sin_addr) << " , " << ntohs(client_addr.sin_port) << ")" << endl; 
        
        add_client(connection_s);

        if (connection_s == INVALID_SOCKET)
            break;
        
        threads.emplace_back(thread(&Server::recieve_messages, this, connection_s)); //and the object itself, the third is the parameters but
                                                                              //there are none.
        threads.back().detach();
    }
}
void Server::add_client(SOCKET client)
{   
    clients.emplace_back(client); //emplace back
}
void Server::recieve_messages(SOCKET client)
{
    char local_data[BUFF_SIZE];
    ZeroMemory(local_data, BUFF_SIZE);

    while (true)
    {
        if (recv(client, local_data, BUFF_SIZE, 0) <= 0)
            break;
        cout << "message recieved: " << local_data << endl;
        if (strstr(local_data, "/exit"))
            break;
        broadcast_message(client, local_data);
        ZeroMemory(local_data, BUFF_SIZE);
    }
}
Server::~Server()
{
    close();
}
void Server::broadcast_message(SOCKET& client, char* data)
{
    std::lock_guard<mutex> lock_guard(m);
    for (auto c : clients)
    {
        if (c != client)
            send(c, data, BUFF_SIZE, 0); //instead of clients[i], use clients.at()
    }
}
void KeyListener(Server &s)
{
    const int ESC = 27;
    while (true)
    {
        if (_kbhit())
        {
            if (_getch() == ESC)
            {
                s.~Server();
                break;
            }
        }
            
    }
}
void Server::close()
{
    closesocket(listening_s);
    closesocket(connection_s);
    WSACleanup();
}