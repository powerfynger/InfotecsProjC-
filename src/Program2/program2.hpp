#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

class Server {
public:
    Server(int port) : _port(port), _serverSocket(0) {}

    void run();
    void createSocket();
    void bindSocket();
    void listenForClients();
    void handleClient(int clientSocket);
private:
    int _port;
    int _serverSocket;
    struct sockaddr_in _serverAddress;
};