# Project Overview — Chat Application (C++ WinSock)

## What I built
A multi-client TCP chat app with two programs:
- **Server**: accepts multiple clients, receives messages, broadcasts to all other connected clients.
- **Client**: connects to the server, runs two threads (send + receive) for real-time chat.

## Key features (as implemented)
- TCP-based chat over IPv4
- Multiple clients supported using `std::thread` (one thread per client on server)
- Broadcast: messages from one client go to all other clients
- Username prefixing: client sends messages as `username: message`
- Basic disconnect handling on server (`recv() <= 0`)

## How to run (current code)
- **Server listens** on port `12345` using `INADDR_ANY` (0.0.0.0)
- **Client connects** to `127.0.0.1:12345`

### Build
```bash
g++ -o server.exe Server/Main.cpp -lws2_32
g++ -o client.exe Client/main.cpp -lws2_32
```

### Run
```bash
.\server.exe
.\client.exe
```

## Core files
- Server: [../Server/Main.cpp](../Server/Main.cpp)
- Client: [../Client/main.cpp](../Client/main.cpp)

## Important functions (your code)
### Server
- `bool Initialize()`
  - Calls `WSAStartup(MAKEWORD(2,2), ...)`.
- `void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients)`
  - `recv()` loop for a single client
  - Broadcast with `send()` to all other sockets in `clients`
  - Removes disconnected socket from `clients`

### Client
- `bool Initialize()`
  - Calls `WSAStartup(MAKEWORD(2,2), ...)`.
- `void SendMsg(SOCKET s)`
  - Reads username, sends messages using `send()`
  - `/quit` exits
- `void ReceiveMsg(SOCKET s)`
  - Receives server broadcasts using `recv()` and prints them

## One-line “elevator pitch”
A WinSock-based, multi-client TCP chat app in C++ where the server handles multiple clients via threads and broadcasts messages in real time.
