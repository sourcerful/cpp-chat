#include "server.hpp"

#pragma comment (lib, "ws2_32.lib") //including the library to the LINKER settings.

using namespace std;

int main()
{
    Server server(5100);
    thread t(KeyListener, ref(server));
    server.accept_con();

    t.join();

    cout << endl;
    cout << "Connection terminated.\n";


    return 0;
}