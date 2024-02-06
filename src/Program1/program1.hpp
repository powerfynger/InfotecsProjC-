#ifndef PROGRAM1_H
#define PROGRAM1_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>


unsigned int defaultPort = 9999;

class SharedBuffer
{
public:
    void writeToBuffer(const std::string &data);
    std::string readFromBuffer();

private:
    std::string _data;
    std::mutex _mutex;
    std::condition_variable _cond;
};

class Client 
{
public:
    Client(int port) : _port(port) {}
    void connectToServer();
    void reconnectToServer();
    void sendNumberToServer(int number);

private:
    int _port;
    int _clientSocket;
    bool _connected;
    struct sockaddr_in _serverAddress;

    void _createSocket();
    void _establishConnection();
};

class Thread1
{

public:
    Thread1(SharedBuffer &exBuffer) : _buffer(exBuffer) {}
    void run();

private:
    bool _validateInput(std::string& userInput);
    void _processInput(std::string& userInput);
    SharedBuffer &_buffer;
};

class Thread2
{
public:
    Thread2(SharedBuffer &exBuffer, Client client);
    void run();

private:
    Client _webClient;
    void _processData(std::string& data);
    unsigned int _currSum;
    int _socket;
    SharedBuffer &_buffer;
};

#endif