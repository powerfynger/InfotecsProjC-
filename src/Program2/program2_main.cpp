#include "program2.hpp"

unsigned int defaultPort = 9999;

void Server::run()
{
    createSocket();
    bindSocket();
    listenForClients();
}

void Server::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
    {
        std::cout << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_addr.s_addr = INADDR_ANY;
    _serverAddress.sin_port = htons(_port);
}

void Server::bindSocket()
{
    if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
    {
        std::cout << "Error binding socket\n";
        exit(EXIT_FAILURE);
    }
}

int main()
{
   
}