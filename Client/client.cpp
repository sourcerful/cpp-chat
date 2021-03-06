#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "client.hpp"
#include <thread>

using namespace std;

bool sending = false;

Client::Client(const char* ip_addr)
{
    //init winsock, WSA variables - environment supporting socket programming on windows.
    WORD version = MAKEWORD(2, 2); //version 2.2
    int validws = WSAStartup(version, &wsData);

    if (validws != 0) //check if works
    {
        cerr << "Couldn't initialize winsock" << endl;
        exit(1);
    }

    ip = new char[sizeof(ip_addr)];
    strcpy(this->ip, ip_addr);
    ZeroMemory(data, BUFF_SIZE);
    port = 5100;
    cout << "Enter username: ";
    cin.getline(name, BUFF_SIZE);
    strcat(name, ": "); 

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        cerr << "Error creating socket." << endl;
        exit(1);
    }
    cout << "Client socket created." << endl;
}
Client::~Client()
{
    close();
}
void Client::start_chat()
{

    thread t1(&Client::send_messages, this);
    thread t2(&Client::incoming_messages, this);
    
    t1.join();
    t2.join();
}
void Client::incoming_messages()
{
    while (true)
    {
        if (sExit || recv(client_socket, data, BUFF_SIZE, 0) < 0)
        {
            sExit = true;
            break;
        }
        
        cout << "\r";
        for (int i = 0; i < 80; i++)
            cout << " ";
        cout << "\r" << data << endl;
        cout << name;
    }
}
void Client::send_messages()
{
    bool running = true;
    char temp[BUFF_SIZE];
    strcpy(temp, name);
    
    while (running)
    {
        cout << name;
        if (!sExit)
            cin.getline(data, BUFF_SIZE);
        if (strcmp(data, "/exit") == 0)
        {
            strcat(temp, "*Closing the socket*");
            running = false;
            sExit = true;
        }
        else
            strcat(temp, data);
        if (sExit && running)
            break;
        send(client_socket, temp, BUFF_SIZE, 0);
        strcpy(temp, name);
    }
    close();
}
void Client::close()
{
    closesocket(client_socket);
    WSACleanup();
}
int Client::connect_socket()
{
    DWORD ip_addr = inet_addr(ip);
    // Bind an IP address and a port to the socket.  
    addr.sin_family = AF_INET; // version 4
    addr.sin_port = htons(port); //host to network short - network uses big endian, so it converts.
    addr.sin_addr.S_un.S_addr = ip_addr;

    if (connect(client_socket, (sockaddr*)&addr, sizeof(addr)) == 0)
    {
        cout << "connecting to server." << endl;
        return 0;
    }
    return -1;
}