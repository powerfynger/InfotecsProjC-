#include "program1.hpp"

void Thread1::run()
{
    while (true)
    {
        std::string input;
        std::cout << ">> ";
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

void Thread2::run()
{
    std::string data = _buffer.readFromBuffer();
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
    _cond.wait(lock, !_data.empty());
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