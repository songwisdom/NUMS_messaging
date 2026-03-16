#include <thread>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

void Foo(std::stop_token st){
    //std::atomic<bool> : 공유 데이터에 사용하는 것
    int stopcnt = 5;
    while(!st.stop_requested()){//(!st.stop_requested()) && 
        --stopcnt;
        std::cout << "Foo thread\n";
        std::this_thread::sleep_for(5s);
    }
    std::cout << "Foo END\n";
}

int main(){
    std::jthread t(Foo);
    std::this_thread::sleep_for(10s);
    t.request_stop();
}

/*
    std::thread my_thread1(Foo);   
    my_thread1.join(); //main이 my_thread1 끝날 때까지 기다림(블로킹)
    my_thread1.detach(); //my_thread1의 lifetime은 OS가 관리
      // 이는 즉 join 불가, 종료 시점 제어 불가, 예외 발생해도 잡을 방법 없음
    */
    //main에서 my_thread1 만들었다고 자식,부모 관계가 아님 - fork()