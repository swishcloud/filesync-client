#include <iostream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#pragma comment(lib, "Ws2_32.lib")
#include "tcp_client.h"
int main(int argc, char **argv)
{
    // Walk through list of strings until a NULL is encountered.
    for (int i = 0; i != argc; ++i)
    {
        std::cout << argv[i] << "[arg]\n";
    }
    std::cout << "client started\n";
    const filesync::tcp_client &client{argv[1]};
    if (client.connect() != 0)
    {
        std::cout
            << "Failed to connect to server.\n";
    }
    else
    {
        std::cout
            << "Connected to server successfully.\n";
    }
    system("pause");
}