#include <iostream>
#include <WinSock2.h>
#include "client.h"

#pragma comment (lib, "ws2_32.lib") 

using namespace std;

int main()
{
    Client client("127.0.0.1");
    if (client.connect_socket() == -1)
    {
        cout << "connection failed.";
        exit(1);
    }
    cout << "connection established." << endl;

    while (true)
    {
        if (recv(client.client_socket, client.data, BUFF_SIZE, 0) == 0)
            break; 

        cout << "Server: " << client.data << endl;
        cout << "Client: ";
        cin.getline(client.data, BUFF_SIZE);

        send(client.client_socket, client.data, BUFF_SIZE, 0);
        if (*client.data == '*')
            break;
    }
    cout << "Connection terminated." << endl;
    client.close();

    return 0;
}