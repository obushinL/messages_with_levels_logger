#ifndef APP_THREAD_QUEUE_H
#define APP_THREAD_QUEUE_H

#include <queue>
#include <mutex>
// примитив из stl, позволяющий потокам заснуть до пробуждения другим потоком
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(std::move(value));
        }
        m_cond.notify_one();
    }

    bool waitAndPop(T& out) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this] { return !m_queue.empty() || m_stopped; });
        if (m_queue.empty())
            return false;
        out = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopped = true;
        }
        m_cond.notify_all();
    }

private:
    std::queue<T>           m_queue;
    std::mutex              m_mutex;
    std::condition_variable m_cond;
    bool                    m_stopped = false;
};

#endif