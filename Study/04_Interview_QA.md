# Interview Q&A (Fresher / Internship — Based on THIS Project)

## Project overview
**Q: Tell me about this project.**
A: I built a multi-client TCP chat application in C++ using the WinSock API. It has a server that accepts multiple client connections and broadcasts messages between connected clients. Each client enters a username and can chat in real time.

**Q: What are the main features?**
A:
- Multi-threaded server supporting multiple concurrent clients (one thread per client)
- Real-time message broadcasting to all connected clients
- Username identification (client prefixes messages as `username: message`)
- TCP socket communication over IPv4
- Basic client disconnect handling (`recv() <= 0`)
- Port: `12345`

## Technical fundamentals
**Q: What is WinSock?**
A: WinSock (Windows Sockets) is the Windows networking API used for socket programming. We link `ws2_32` and call `WSAStartup()` before using networking functions like `socket()`, `connect()`, `send()`, `recv()`.

**Q: Why did you use TCP instead of UDP?**
A: TCP provides reliable, ordered delivery. For chat, we want messages to arrive and appear in the same order they were sent.

## Server workflow
**Q: Explain the server workflow.**
A:
1. `WSAStartup()` – initialize WinSock
2. `socket(AF_INET, SOCK_STREAM, 0)` – create TCP socket
3. Prepare server address (`sockaddr_in`) with port `12345`
4. `bind()` – bind to local IP/port
5. `listen()` – start listening for incoming connections
6. Loop: `accept()` – accept a client connection
7. Spawn a thread to handle that client (`InteractWithClient`)
8. `recv()` – receive client messages
9. `send()` – broadcast to other clients
10. `closesocket()` – close disconnected client socket
11. `WSACleanup()` – cleanup at the end

**Q: Which IP does the server bind to in your code?**
A: The server binds to `INADDR_ANY` (0.0.0.0), meaning it can accept connections on any network interface, on port `12345`.

## Client workflow
**Q: Explain the client workflow.**
A:
1. `WSAStartup()` – initialize WinSock
2. `socket()` – create TCP socket
3. Create server address using `127.0.0.1:12345`
4. `connect()` – connect to the server
5. Start two threads:
	- `SendMsg()` – reads console input and calls `send()`
	- `ReceiveMsg()` – calls `recv()` and prints messages
6. `closesocket()` + `WSACleanup()` – cleanup on exit

## Multithreading
**Q: How did you handle multiple clients?**
A: I used a thread-per-client model on the server. Every time the server accepts a new client, it creates a `std::thread` running `InteractWithClient()` so multiple clients can chat at the same time.

**Q: What is the purpose of the `InteractWithClient` function?**
A: It handles one connected client: it continuously receives messages from that client and broadcasts them to all other connected clients.

**Q: Why use two threads in the client?**
A: To support sending and receiving at the same time. One thread waits for user input and sends messages, the other thread blocks on `recv()` to display incoming messages.

## Networking concepts
**Q: What is a socket?**
A: A socket is an endpoint used to send and receive data across a network. A TCP socket gives reliable, connection-based communication.

**Q: What is `bind()`?**
A: It associates the server socket with a local IP address and port. In my server, it binds to port `12345` on `INADDR_ANY`.

**Q: What is the difference between `listen()` and `accept()`?**
A: `listen()` puts the server socket into listening mode. `accept()` takes one incoming connection from the queue and returns a new socket used to communicate with that client.

**Q: What does `127.0.0.1` mean?**
A: It’s the localhost/loopback IP. The client uses it to connect to a server running on the same machine.

**Q: What is port `12345`?**
A: It’s the port number where the server listens for incoming connections.

## Code-specific questions
**Q: Why do you check `bytesReceived <= 0`?**
A: `recv()` returns the number of bytes received. If it’s 0, the connection was closed. If it’s negative (error), receiving failed. In both cases, we treat it as a disconnect and clean up.

**Q: What is `htons()`?**
A: “Host to Network Short” converts the port into network byte order (big-endian), which is required by socket APIs.

**Q: Explain `MAKEWORD(2, 2)`.**
A: It specifies the WinSock version requested: 2.2.

## Error handling & testing
**Q: How do you handle errors?**
A: I check return values like `INVALID_SOCKET` / `SOCKET_ERROR` and use `WSAGetLastError()` to print error codes.

**Q: How did you test it?**
A: I ran the server first, then opened multiple client instances and verified that messages from one client were broadcast to the others, and disconnects were handled.

## Improvements (talking points)
**Q: Is your server thread-safe?**
A: Not fully. Multiple threads access the shared `clients` vector without synchronization. A correct fix is to use a `std::mutex` to protect add/remove/broadcast operations.

**Q: What improvements would you make next?**
A:
- Add `std::mutex` for the shared client list
- Add message framing (because TCP is a stream, one `recv()` is not guaranteed to equal one message)
- Avoid calling `WSACleanup()` from multiple threads in the client (do cleanup once)
- Add better command handling (like `/quit`, user join/leave messages)

## Compilation (quick answer)
**Q: How do you compile it?**
A:
```bash
g++ -o server.exe Server/Main.cpp -lws2_32
g++ -o client.exe Client/main.cpp -lws2_32
```

**Q: Why link `ws2_32`?**
A: It provides the WinSock 2 implementation. Without it, the linker can’t resolve functions like `socket()`, `bind()`, `send()`, `recv()`.
