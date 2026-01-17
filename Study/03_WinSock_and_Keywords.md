# WinSock + Keywords Used (Cheat Sheet)

## WinSock functions used
- `WSAStartup(MAKEWORD(2,2), &wsaData)`
  - Initializes WinSock; must be called before `socket()`.
- `WSACleanup()`
  - Releases WinSock resources.
- `socket(AF_INET, SOCK_STREAM, 0)`
  - Creates a TCP (stream) socket using IPv4.
- `bind(sock, sockaddr*, sizeof(sockaddr_in))` (server)
  - Attaches a local IP/port to the server socket.
- `listen(sock, SOMAXCONN)` (server)
  - Marks socket as passive and ready to accept.
- `accept(listenSock, NULL, NULL)` (server)
  - Returns a new socket for a connected client.
- `connect(sock, sockaddr*, sizeof(sockaddr_in))` (client)
  - Connects to server.
- `send(sock, buffer, len, flags)`
  - Sends bytes over TCP.
- `recv(sock, buffer, size, flags)`
  - Receives bytes; `<= 0` means closed/error.
- `closesocket(sock)`
  - Closes a socket.
- `WSAGetLastError()`
  - Retrieves last error code for debugging.

## Networking keywords
- `AF_INET`: IPv4 address family
- `SOCK_STREAM`: TCP socket
- `INADDR_ANY`: 0.0.0.0 (bind to all interfaces)
- `htons(port)`: host-to-network byte order conversion
- `127.0.0.1`: localhost (client connects to own machine)

## C++ / STL keywords used
- `std::thread`: multithreading
- `detach()` (server): lets client thread run independently
- `join()` (client): waits for send/receive threads
- `std::vector<SOCKET>`: stores connected client sockets
- `std::find` + `erase`: remove disconnected client from vector
- `std::string(buffer, bytes)`: construct message from received bytes

## Important code-level details
- Buffer size is `1024` bytes.
- Client formats outgoing messages as: `username + ": " + message`.
- `/quit` triggers client shutdown.
