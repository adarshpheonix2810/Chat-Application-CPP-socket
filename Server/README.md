# Chat Application (C++)

A multi-client TCP chat application with server and client components using WinSock.

## Features
- Multi-threaded server supporting multiple clients
- Message broadcasting between connected clients
- Username identification
- IPv4 TCP socket communication

## Prerequisites
- MinGW (GCC compiler)
- Windows OS

## Compilation

### Server:
```bash
g++ -o server.exe Server/Main.cpp -lws2_32
```

### Client:
```bash
g++ -o client.exe Client/main.cpp -lws2_32
```

## Running

### Start Server:
```bash
.\server.exe
```

### Start Client:
```bash
.\client.exe
```

Server listens on `127.0.0.1:12345`

## License
MIT License

## 👨‍💻 Author

**Adarsh Singh**
- GitHub: [@adarshpheonix2810](https://github.com/adarshpheonix2810)
- LinkedIn: [Connect with me](https://www.linkedin.com/in/adarsh-singh-412298273/)

---

## 📊 Project Stats

![GitHub Stars](https://img.shields.io/github/stars/adarshpheonix2810/Chat-Application-Using-Cpp?style=social)
![GitHub Forks](https://img.shields.io/github/forks/adarshpheonix2810/Chat-Application-Using-Cpp?style=social)
![GitHub Issues](https://img.shields.io/github/issues/adarshpheonix2810/Chat-Application-Using-Cpp)

---

<div align="center">

### ⭐ Star this repo if you find it helpful!

**Built with ❤️ and C++**

</div>
