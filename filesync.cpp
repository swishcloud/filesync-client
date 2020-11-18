//#define _GLIBCXX_USE_CXX11_ABI 0
//#define _WIN32_WINNT 0x501
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <common.h>

#define DEFAULT_PORT "8083"
#define DEFAULT_BUFLEN 512
//#pragma comment(lib, "Ws2_32.lib")
int main()
{
    std::wstring root_path = L"C:\\workspace";
    std::vector<std::wstring> files;
    common::find_files(root_path, files);
    for (size_t i = 0; i < files.size(); i++)
    {
        std::wcout << files[i].c_str() << L"\n";
    }

    return 0;
    WSADATA wsaData;
    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        std::cout << common::string_format(std::string("getaddrinfo failed: %d\n"), iResult);
        WSACleanup();
        return 1;
    }
    SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    auto temp = inet_ntoa(((struct sockaddr_in *)result->ai_addr)->sin_addr);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    std::cout << common::string_format("listening on %s:%s\n", temp, DEFAULT_PORT);
    SOCKET ClientSocket;
    // Accept a client socket
    while (1)
    {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        else
        {
            // Receive data until the server closes the connection
            do
            {
                int recvbuflen = DEFAULT_BUFLEN;

                const char *sendbuf = "this is a test";
                char recvbuf[DEFAULT_BUFLEN];
                iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
                if (iResult > 0)
                {
                    printf("Bytes received: %d\n", iResult);
                    printf("client message:%s\n", recvbuf);
                }
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed: %d\n", WSAGetLastError());
            } while (iResult > 0);
        }
    }
    std::cout << "\n"
              << common::string_format("all is ok");
}