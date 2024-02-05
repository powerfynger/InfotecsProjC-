#include "program1.hpp"

void Thread1::run()
{

}

void Thread2::run()
{

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