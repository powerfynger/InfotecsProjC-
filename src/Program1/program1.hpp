#ifndef PROGRAM1_H
#define PROGRAM1_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>

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
    Thread2(SharedBuffer &exBuffer) : _buffer(exBuffer) {}
    void run();

private:
    SharedBuffer &_buffer;
};

#endif