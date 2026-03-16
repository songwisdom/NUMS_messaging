#include <iostream>
#include "Worker.hpp"

void Worker::work(){
    for(int i =1;i<=5;i++){
        std::cout << "Working... step " << i << std::endl;
    }
}
            