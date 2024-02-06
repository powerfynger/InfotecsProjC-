#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

class Server {
public:
    Server(int port) : _port(port), _serverSocket(0) {}

    void run();
private:
    int _port;
    int _serverSocket;
    struct sockaddr_in _serverAddress;
    void _createSocket();
    void _bindSocket();
    void _listenForClients();
    void _handleClient(int clientSocket);
};