#define _WIN32_WINNT 0x0600 // Target Windows Vista or later
#include <iostream>
// Declares WinSock functions
#include <winsock2.h>

#include <ws2tcpip.h> // For InetPton
#include <tchar.h>
#include <thread>
#include <algorithm>
#include <vector>
using namespace std;
// Links functions to your program
#pragma comment(lib, "ws2_32.lib")

/*
1. intialize winsock library
2. create socket
3. get ip and port
4. bind socket to ip and port
5. listen for connections
6. accept connection
7. send and receive data
8. close socket
9. cleanup winsock library

*/
/*
Every WinSock program follows this order:

1.WSAStartup() – initialize WinSock
2.socket() – create socket
3.bind() – (server only) bind to IP/port
4.listen() – (server only) listen for connections
5.accept() – (server only) accept client
6.connect() – (client only) connect to server
7.send() / recv() – exchange data
8.closesocket() – close socket
9.WSACleanup() – cleanup
*/

// 1. intialize winsock library
bool Initialize()
{
    WSADATA wsaData;
    // Request WinSock version 2.2
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        // Initialization failed
        return false;
    }
    return true;
}

void InteractWithClient(SOCKET clientSocket ,vector<SOCKET>& clients)
{
    cout<< "Client connected. " <<  endl;
    char buffer[1024];
    while (1){
        // 7. send and receive data
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            cout << "Client disconnected." << endl;
            break;
        }
        string message(buffer, bytesReceived);
        cout << "Received message: " << message << endl;

        for( auto client : clients){
            if (client != clientSocket){
                send(client, message.c_str(), message.length(), 0);
            }
        }
    }
    auto it = find(clients.begin(), clients.end(), clientSocket);
    if (it != clients.end()){
        clients.erase(it);
    }
    // 8. close socket
    closesocket(clientSocket);
}
int main()
{
    if (!Initialize())
    {
        return 1;
    }

    // 2. create socket
    /*
        AF_INET     -> IPv4
        SOCK_STREAM -> TCP
        0           -> Protocol: let the system choose (TCP for SOCK_STREAM)
    */
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed. Error: "
             << WSAGetLastError() << endl;
        return 1;
    }

    // 3. get ip and port(create address structure)
    int port = 12345;
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;              // IPv4
    serveraddr.sin_port = htons(port);            // htons() → Host TO Network Short
    serveraddr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANY → 0.0.0.0

    // 4. bind socket to ip and port
    if (bind(listenSocket, reinterpret_cast<sockaddr *>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
    {
        cout << "Bind failed. Error: "
             << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 5. listen for connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "Listen failed. Error: "
             << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    cout << "Server is listening on port " << port << "..." << endl;
    vector<SOCKET> clients;
    while (1)
    {
        // 6. accept connection
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            cout << "Accept failed. Error: "
                 << WSAGetLastError() << endl;
        }
        clients.push_back(clientSocket);

        thread t1(InteractWithClient, clientSocket, ref(clients));
        t1.detach();
    }

    // 8. close socket
    closesocket(listenSocket);

    // 9. cleanup winsock library
    WSACleanup();
    return 0;
}
