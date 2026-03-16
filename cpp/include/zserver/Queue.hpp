#pragma once

#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>

template <typename T> class Queue {
  public:
    void push(T const &v) {
        {
            std::lock_guard<std::mutex> lk(mutex_);
            queue_.emplace_back(v);
        }
        condition_.notify_one();
    }

    void priority_push(T const &v) {
        {
            std::lock_guard<std::mutex> lk(mutex_);
            queue_.emplace_front(v);
        }
        condition_.notify_one();
    }

    inline bool empty() const {
        std::lock_guard<std::mutex> lk(mutex_);
        return queue_.empty();
    }

    void pop(T &v) {
        std::unique_lock<std::mutex> lk(mutex_);

        while (queue_.empty())
            condition_.wait(lk);

        v = queue_.front();
        queue_.pop_front();

		lk.unlock();
    }

    T pop(void) {
        std::unique_lock<std::mutex> lk(mutex_);

        while (queue_.empty())
            condition_.wait(lk);

        T v = queue_.front();
        queue_.pop_front();

		lk.unlock();

		return v;
    }


    bool try_pop(T &v) {
        std::lock_guard<std::mutex> lk(mutex_);
        if (queue_.empty())
            return false;

        v = queue_.front();
        queue_.pop_front();
        return true;
    }


  private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::deque<T> queue_;
};
