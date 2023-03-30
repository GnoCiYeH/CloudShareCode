#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<thread>
#include<queue>
#include<mutex>
#include<functional>
#include<condition_variable>
#include<future>

template<typename T>
class ThreadQueue
{
public:
    ThreadQueue() {};
    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    void enqueue(T& t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(t);
    }

    bool dequeue(T& t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return false;
        t = std::move(m_queue.front());
        m_queue.pop();

        return true;
    }

    int size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }


private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};

class ThreadPool
{
private:
    class WorkThread
    {
    private:
        ThreadPool* m_pool;
    public:
        WorkThread(ThreadPool* poll) : m_pool(poll) {}

        void operator()()
        {
            std::function<void()> func;
            bool dequeue;

            while (!m_pool->isShutdown)
            {
                {
                    std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);

                    if (m_pool->m_threads.empty())
                    {
                        m_pool->m_conditional_lock.wait(lock);
                    }

                    dequeue = m_pool->wait_queue.dequeue(func);
                }

                if (dequeue)
                {
                    func();
                }
            }
        }
    };

    std::vector<std::thread> m_threads;
    bool isShutdown = false;
    std::mutex m_conditional_mutex; // 线程休眠锁互斥变�?
    std::condition_variable m_conditional_lock;

    ThreadQueue<std::function<void()>> wait_queue;


public:
    ThreadPool() = delete;
    ThreadPool(int maxNum) {
        for (int i = 0; i < maxNum; i++)
        {
            m_threads.push_back(std::thread());
        }
    }
    virtual ~ThreadPool() {};

    void init()
    {
        for (int i = 0; i < m_threads.size(); i++)
        {
            m_threads.at(i) = std::thread(WorkThread(this));
        }
    }

    void shutdown()
    {
        isShutdown = true;
        m_conditional_lock.notify_all();

        for (int i = 0; i < m_threads.size(); i++)
        {
            if (m_threads.at(i).joinable())
            {
                m_threads.at(i).join();
            }
        }
    }

    //学习
    template<typename T, typename... Args>
    auto submit(T&& f, Args&&...args) -> std::future<decltype(f(args...))>
    {
        std::function<decltype(f(args...))()> func = std::bind(std::forward<T>(f), std::forward<Args>(args)...);

        auto func_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        std::function<void()> warpper_func = [func_ptr]() {
            (*func_ptr)();
        };

        wait_queue.enqueue(warpper_func);

        m_conditional_lock.notify_one();

        return func_ptr->get_future();
    }
};

#endif // THREADPOOL_H

