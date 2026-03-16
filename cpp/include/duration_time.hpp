#pragma once

#include <iostream>
#include <chrono>
#include <iostream>
#include <cstdlib>
//#include <thread>
using namespace std;
class duration_time
{
    public:
        duration_time(){
            StartTime = chrono::system_clock::now();
        };
        ~duration_time(){};

        int get_sec_count(){
            EndTime = chrono::system_clock::now();
            sec = chrono::duration_cast<chrono::seconds>(EndTime - StartTime);
            return sec.count();
        }

        int get_mill_count(){
            EndTime = chrono::system_clock::now();
            mill = chrono::duration_cast<chrono::milliseconds>(EndTime - StartTime);
            return mill.count();
        }

        int get_micro_count(){
            EndTime = chrono::system_clock::now();
            micro = chrono::duration_cast<chrono::microseconds>(EndTime - StartTime);
            return micro.count();
        }

		void now_set(){
            StartTime = chrono::system_clock::now();
		}

        void sleep_remain_micro_seconds(){

            EndTime = chrono::system_clock::now();
            micro = chrono::duration_cast<chrono::microseconds>(EndTime - StartTime);

            int remain_micro_seconds = 1;
            if( micro.count() > 0 && micro.count() < 1000000){
                remain_micro_seconds = 1000000 - micro.count();
            }else{
                remain_micro_seconds = 1;
            }

            boost::this_thread::sleep_for(boost::chrono::microseconds(remain_micro_seconds));

            StartTime = chrono::system_clock::now();
        }

#if 0
        sec = chrono::duration_cast<chrono::seconds>(EndTime - StartTime);
        mill = chrono::duration_cast<chrono::milliseconds>(EndTime - StartTime);
        micro = chrono::duration_cast<chrono::microseconds>(EndTime - StartTime);
        min = chrono::duration_cast<chrono::minutes>(EndTime - StartTime);
        hour = chrono::duration_cast<chrono::hours>(EndTime - StartTime);
#endif
    private:
        chrono::system_clock::time_point StartTime;
        chrono::system_clock::time_point EndTime;
        chrono::duration<double> DefaultSec;
        chrono::nanoseconds nano;
        chrono::microseconds micro;
        chrono::milliseconds mill;
        chrono::seconds sec;
        chrono::minutes min;
        chrono::hours hour;
};
