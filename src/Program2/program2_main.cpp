#include "program2.hpp"

unsigned int defaultPort = 9999;

void Server::run()
{
    _createSocket();
    _bindSocket();
    _listenForClients();
}

void Server::_createSocket()
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

void Server::_bindSocket()
{
    if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
    {
        std::cout << "Error binding socket\n";
        exit(EXIT_FAILURE);
    }
}

void Server::_listenForClients()
{
    if (listen(_serverSocket, 15) == -1)
    {
        std::cout << "Error listening for clients\n";
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        int clientSocket = accept(_serverSocket, 0, 0);
        if (clientSocket == -1)
        {
            std::cout << "Error accepting client\n";
        }
        else
        {
            _handleClient(clientSocket);
            close(clientSocket);
        }
    }
}

void Server::_handleClient(int clientSocket)
{
    while (true)
    {
        int number = 0;
        size_t bytesRead = recv(clientSocket, &number, sizeof(number), 0);
        if (bytesRead <= 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }

        if (number >= 100 && number % 32 == 0)
        {
            std::cout << "Recieved number: " << number << std::endl;
        }
        else
        {
            std::cout << "Incorrect data\n";
        }
    }
}

int main()
{
    Server serv(defaultPort);
    serv.run();
}