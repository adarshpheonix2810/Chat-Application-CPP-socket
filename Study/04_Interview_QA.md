
# Interview Q&A (Fresher / Internship — Chat Application Project)

---

## 🟢 Project Overview

**Q: Tell me about this project.**  
<span style="color:#555">I built a multi-client TCP chat application in C++ using the WinSock API. The server accepts multiple client connections and broadcasts messages between them. Each client sets a username and chats in real time.</span>

**Q: What are the main features?**
- Multi-threaded server (one thread per client)
- Real-time message broadcasting
- Username identification
- TCP socket communication (IPv4)
- Handles client disconnects
- Listens on port `12345`

---

## 🟢 Technical Fundamentals

**Q: What is WinSock?**  
WinSock (Windows Sockets) is the Windows API for network programming. You link `ws2_32` and call `WSAStartup()` before using socket functions like `socket()`, `connect()`, `send()`, `recv()`.

**Q: Why TCP instead of UDP?**  
TCP is reliable and ordered. For chat, we want messages to arrive and appear in the same order they were sent.

---

## 🟢 Server Workflow

**Q: Explain the server workflow.**
1. `WSAStartup()` – initialize WinSock
2. `socket(AF_INET, SOCK_STREAM, 0)` – create TCP socket
3. Prepare server address (`sockaddr_in`) with port `12345`
4. `bind()` – bind to local IP/port
5. `listen()` – start listening
6. Loop: `accept()` – accept client
7. Spawn thread for client (`InteractWithClient`)
8. `recv()` – receive messages
9. `send()` – broadcast to other clients
10. `closesocket()` – close client socket
11. `WSACleanup()` – cleanup

**Q: Which IP does the server bind to?**  
`INADDR_ANY` (0.0.0.0) — accepts connections on any network interface, port `12345`.

---

## 🟢 Client Workflow

**Q: Explain the client workflow.**
1. `WSAStartup()` – initialize WinSock
2. `socket()` – create TCP socket
3. Server address: `127.0.0.1:12345`
4. `connect()` – connect to server
5. Start two threads:
   - `SendMsg()` – reads input, sends messages
   - `ReceiveMsg()` – receives and prints messages
6. `closesocket()` + `WSACleanup()` – cleanup

---

## 🟢 Multithreading

**Q: How did you handle multiple clients?**  
Thread-per-client model. Each accepted client gets a new `std::thread` running `InteractWithClient()`.

**Q: What is the purpose of `InteractWithClient`?**  
Handles one client: receives messages and broadcasts to all other clients.

**Q: Why use two threads in the client?**  
One for sending, one for receiving — enables full-duplex chat.

---

## 🟢 Networking Concepts

**Q: What is a socket?**  
An endpoint for sending/receiving data across a network. TCP sockets provide reliable, connection-based communication.

**Q: What is `bind()`?**  
Associates the server socket with a local IP and port (`12345` on `INADDR_ANY`).

**Q: Difference between `listen()` and `accept()`?**  
`listen()` puts the socket in listening mode. `accept()` takes a connection from the queue and returns a new socket for that client.

**Q: What does `127.0.0.1` mean?**  
Localhost/loopback IP. Client uses it to connect to a server on the same machine.

**Q: What is port `12345`?**  
The port where the server listens for connections.

---

## 🟢 Code-Specific Questions

**Q: Why check `bytesReceived <= 0`?**  
`recv()` returns the number of bytes received. `0` means closed, negative means error. Both trigger disconnect handling.

**Q: What is `htons()`?**  
Converts port number to network byte order (big-endian).

**Q: Explain `MAKEWORD(2, 2)`.**  
Specifies WinSock version 2.2.

---

## 🟢 Error Handling & Testing

**Q: How do you handle errors?**  
Check return values (`INVALID_SOCKET`, `SOCKET_ERROR`) and use `WSAGetLastError()` for error codes.

**Q: How did you test it?**  
Ran the server, connected multiple clients, verified broadcast and disconnect handling.

---

## 🟢 Improvements & Limitations

**Q: Is your server thread-safe?**  
Not fully. Multiple threads access the shared `clients` vector without a mutex. Use `std::mutex` for safety.

**Q: What improvements would you make next?**
- Add `std::mutex` for client list
- Add message framing (TCP is a stream)
- Centralize client cleanup
- Add better commands (`/quit`, join/leave)

---

## 🟢 Compilation

**Q: How do you compile it?**
```bash
g++ -o server.exe Server/Main.cpp -lws2_32
g++ -o client.exe Client/main.cpp -lws2_32
```

**Q: Why link `ws2_32`?**  
It provides WinSock 2. Without it, the linker can’t resolve networking functions.
