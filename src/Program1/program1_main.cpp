#include "program1.hpp"

void Thread1::run()
{
    while (true)
    {
        std::string input;
        std::cin >> input;
        if (!_validateInput(input))
            continue;
        _processInput(input);
        _buffer.writeToBuffer(input);
    }
}

bool Thread1::_validateInput(std::string &userInput)
{
    return userInput.find_first_not_of("0123456789") == std::string::npos && userInput.length() <= 64;
}

void Thread1::_processInput(std::string &userInput)
{
    std::vector<int> digits;
    for (auto digit : userInput)
    {
        digits.push_back(digit - '0');
    }

    std::sort(digits.begin(), digits.end());

    for (int &digit : digits)
    {
        if (digit % 2 == 0)
        {
            digit = -1;
        }
    }

    userInput = "";
    for (int digit : digits)
    {
        if (digit == -1)
        {
            userInput += "KB";
        }
        else
        {
            userInput += std::to_string(digit);
        }
    }
}

Thread2::Thread2(SharedBuffer &exBuffer) : _buffer(exBuffer)
{
    _connected = false;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
    {
        std::cout << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Thread2::run()
{
    _establishConnection();
    while (true)
    {
        std::string data = _buffer.readFromBuffer();
        std::cout << data << std::endl;
        _processData(data);
        _sendCursumToSecondProg();
    }
}

void Thread2::_establishConnection()
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(defaultPort);

    if (connect(_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cout << "Error connecting to server" << std::endl;
    }
    _connected = true;
}

void Thread2::_processData(std::string &data)
{
    _currSum = 0;
    for (int i = 0; i < data.length(); i++)
    {
        std::isdigit(data[i]) ? _currSum += data[i] - '0' : i++;
    }
}

void Thread2::_sendCursumToSecondProg()
{
    if (!_connected)
    {
        _establishConnection();
        if (!_connected) return;
    }
    if(send(_socket, &_currSum, sizeof(_currSum), MSG_NOSIGNAL) == -1)
    {
        _connected = false;


    }
}

void SharedBuffer::writeToBuffer(const std::string &data)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _data = data;
    _cond.notify_all();
}

std::string SharedBuffer::readFromBuffer()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock,  [this] { return !_data.empty(); });
    std::string tmp = _data;
    _data.clear();
    return tmp;
}

int main()
{
    SharedBuffer buffer;
    Thread1 thread1(buffer);
    Thread2 thread2(buffer);

    std::thread t1(&Thread1::run, &thread1);
    std::thread t2(&Thread2::run, &thread2);

    t1.join();
    t2.join();
    return 0;
}