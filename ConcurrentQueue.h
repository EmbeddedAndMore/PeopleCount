//
// Created by mohamad on 10.01.21.
//

#ifndef BLOBDETECTION_CONCURRENTQUEUE_H
#define BLOBDETECTION_CONCURRENTQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class ConcurrentQueue{


private:
    std::queue<T> _queue;
    std::mutex    _mutex;
    std::condition_variable _cond;
    unsigned int BufferSize;

public:
    T pop()
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        while(_queue.empty())
            _cond.wait(mlock);

        auto val = _queue.front();
        _queue.pop();
        mlock.unlock();
        _cond.notify_one();
        return val;
    }

    void pop (T& item)
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        while(_queue.empty())
            _cond.wait(mlock);

        item = _queue.front();
        _queue.pop();
        mlock.unlock();
        _cond.notify_one();
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        while (_queue.size() >= BufferSize) {
            _cond.wait(mlock);
        }
        _queue.push(item);
        mlock.unlock();
        _cond.notify_one();
    }

    ConcurrentQueue():BufferSize(10){};

    ConcurrentQueue(unsigned int init_size){
        BufferSize = init_size;
    }
    ConcurrentQueue(const ConcurrentQueue&) = delete;            // disable copying
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete; // disable assignment
};

#endif //BLOBDETECTION_CONCURRENTQUEUE_H
