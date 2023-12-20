# `nstream`
* C++ library for anyone who hates to see low level (TCP/IP)socket again.

Originally started as part of a 1 semester worth of project, for system programming.

## How to use
The library is header only. Just include the file you need and good to go.  
* `basic_socketbuf` class template handles low level socket, behaves almost identical to `basic_streambuf` in standard library.
* `basic_*nstream` class templates provide high level interface for TCP/IP communication. Behaves almost identical to `basic_istream`, `basic_ostream` or `basic_iostream` in standard library.

There are 4 different ways to initialize these objects.
* Via `(in_port_t port)` type signature. (**Server-like behavior**)  
Initializes object with port number, listens from the port provided.
* Via `(in_addr_t ipv4, in_port_t port)` type signature (**Client-like behavior**)  
Initializes object with ip address and port number, attempt to connect to the host provided.  
* Via `(const char* host_name, in_port_t port)` type signature (**Client-like behavior**)  
Initializes object with host name and port number, attempt to connect to the host provided.  
* Via `fromSocket(int&& socket_fd, std::ios_base::openmode m)` member function. (**From existing socket**)  
Initializes object with already existing socket, gains ownership of that socket.

## For terminal use: `NetPipe`
```bash
NetPipe (send | recv) --port <port number> [--name <server name>]
```
Tunnels pipe through internet.  
* send/recv flag: mandatory.  
In send mode, sends texts read from stdin, line by line.  
In recv mode, receives texts and prints to stdout, line by line.  
* --port \<port number\>: mandatory.  
Port number to listen or send request to.  
* --name \<server name\>: optional.  
IPv4 address or host name to send request to.  
If absent, behaves as a server.

## Example: tunneling echo in localhost
* Note: server(one that does not have --name option, sender in this case) should run first.

Sender side:
```bash
echo "Hello, world!" | NetPipe send --port 12345
```
Sender side terminal output:
```
(Nothing is displayed, since the output of echo is sent through network.)
```
Receiver side:
```bash
NetPipe recv --port 12345 --name localhost
```
Receiver side terminal output:
```
Hello, world!
```