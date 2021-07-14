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
    client.start_chat();

    cout << "Connection terminated." << endl;
    client.close();

    return 0;
}