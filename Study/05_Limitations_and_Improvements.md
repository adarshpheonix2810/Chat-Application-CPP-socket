# Limitations & Improvements (Honest + Interview-Ready)

## Current limitations (from the code)
- **Thread safety issue on server**: shared `vector<SOCKET> clients` is accessed by multiple threads without synchronization.
- **Client cleanup duplication**: both `SendMsg()` and `ReceiveMsg()` call `closesocket()` and `WSACleanup()`. If one thread exits first, it can affect the other.
- **No message framing**: TCP is a stream; a long message could be split across packets or multiple messages could be merged. Current code treats every `recv()` as one full message.
- **No authentication / no encryption**: plain text chat.
- **Server never exits cleanly**: infinite accept loop.

## Suggested improvements
### Safety & correctness
- Add `std::mutex` to protect `clients` vector during add/remove/broadcast
- Make cleanup happen **once** in client (centralized shutdown)
- Add message framing (e.g., newline-delimited or length-prefixed protocol)

### Features
- Private messaging (`/dm user msg`)
- User join/leave notifications
- Chat rooms

### Production-style upgrades
- Use async I/O instead of thread-per-client (scales better)
- Logging (file + log levels)
- TLS (secure chat)
