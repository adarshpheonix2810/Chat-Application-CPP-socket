#define _WIN32_WINNT 0x0600 // Target Windows Vista or later
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
/*
1. intialize winsock library
2. create socket
3. connect to server
4. send and receive data
5. close socket
6. cleanup winsock library
*/

bool Initialize()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        return false;
    }
    return true;
}
void SendMsg(SOCKET s)
{
    cout<<"Enter Your Username: "<<endl;
    string name;
    getline(cin, name);
    string message;
    while (1)
    {
        getline(cin, message);
        string msg=name + ": " + message;
        if (message.empty()) continue;
        int bytesSent = send(s, msg.c_str(), msg.length(), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            cout << "Send failed: " << WSAGetLastError() << endl;
            break;
        }
        if (message == "/quit")
        {
            cout << "Exiting chat." << endl;
            break;
        }
    }
    closesocket(s);
    WSACleanup();
}
void ReceiveMsg(SOCKET s)
{
    char buffer[1024];
    int recvlength;
    string msg="";
    while (1)
    {
        recvlength = recv(s, buffer, sizeof(buffer) , 0);
        if (recvlength <= 0)
        {
            cout << "Connection closed by server." << endl;
            break;
        }else{
            msg=string(buffer, recvlength);
            cout << msg << endl;
        }
        
    }
    closesocket(s);
    WSACleanup();
}
int main()
{
    // 1. Initialize WinSock
    if (!Initialize())
    {
        cout << "WSAStartup failed" << endl;
        return 1;
    }

    // 2. Create TCP socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        cout << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // 3. Setup server address
    int port = 12345;
    string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;   // IPv4
    serveraddr.sin_port = htons(port); // Port number

    // Convert IP string to binary (replace InetPton)
    serveraddr.sin_addr.s_addr = inet_addr(serveraddress.c_str());


    // 4. Connect to server
    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
    {
        cout << "Connection failed: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!" << endl;
    // 5. Send/Receive data here
    thread senderthread(SendMsg, s);
    thread receiverthread(ReceiveMsg, s);
    senderthread.join();
    receiverthread.join();
    
    return 0;
}
