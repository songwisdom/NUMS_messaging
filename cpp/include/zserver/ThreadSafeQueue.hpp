#pragma once
#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

template<class T> // class VS typename
class ThreadSafeQueue {
    public:
        void push_noti(T value) {
            std::lock_guard<std::mutex> lock(mtx_);
            q_.push(std::move(value));
            cv_.notify_one();
        }

        std::optional<T> pop_wait(std::stop_token st) { 
            // blocking - while 안에서 돌려도 spin X
            std::unique_lock lock(mtx_);
            cv_.wait(lock, st, [&]{ return !q_.empty(); }); //stop_token 지원
            if(q_.empty()) return std::nullopt;

            T v = std::move(q_.front());
            q_.pop();
            return v;
        }

        template<typename Rep, typename Period>
        std::optional<T> pop_wait_for(
            std::stop_token st,
            const std::chrono::duration<Rep, Period>& dur
        ) {
            // dur 만큼만 기다림
            // 큐가 비어도 주기적으로 깨어나서 reconnect 요청 확인 가능
            std::unique_lock lock(mtx_);
            cv_.wait_for(lock,st, dur, [&] { return !q_.empty(); });

            if (q_.empty()) return std::nullopt;

            T v = std::move(q_.front());
            q_.pop();
            return v;
        }

        void notify_all() { cv_.notify_all(); }

    private:
        std::queue<T> q_;
        std::mutex mtx_;
        std::condition_variable_any cv_;
};


/*
nonblocking pop(while 안에 넣으면 busy waiting)
std::optional<T> pop() {
    std::lock_guard<std::mutex> lock(mtx_);
    //unique_lock
    if (q_.empty()) return std::nullopt; //비어있으면 계속 null을 return 함 (busy waiting)
    T v = std::move(q_.front());
    q_.pop();
    return v;
}
*/