#include "BucketSort.h"

#include <thread>
#include <algorithm>
#include <vector>
#include <cmath>
#include <atomic>
#include <iostream>
bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {



    if (x == y) return false; // if the two numbers are the same then one is not less than the other



    unsigned int a = x;

    unsigned int b = y;



    // work out the digit we are currently comparing on. 

    if (pow == 0) {

        while (a / 10 > 0) {

            a = a / 10; 

        }   

        while (b / 10 > 0) {

            b = b / 10;

        }

    } else {

        while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {

            a = a / 10;

        }

        while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {

            b = b / 10;

        }

    }


    if (a == b)

        return aLessB(x,y,pow + 1);  // recurse if this digit is the same 

    else

        return a < b;

}

int getMSD(unsigned int a){
    while (a >= 10){
        a /= 10;
    }
    return a;
}




void BucketSort::sort(unsigned int numCores) {

        
    std::vector<std::vector<unsigned int>> numbers(9,std::vector<unsigned int>());

    for ( auto k: numbersToSort){
        numbers[getMSD(k)-1].push_back(k);
    }

        
    std::vector<unsigned int> count;
    size_t temp = 0;
    for (auto &k:numbers){
        count.push_back(temp);
        temp += k.size();
    }

    std::vector<std::thread> threads;
    
    std::atomic<unsigned int> cur = ATOMIC_VAR_INIT(1);
    for (unsigned int i = 0; i < numCores ; i++){
        auto lambda = [&numbers,&count,&cur,this](){
                    while (true){
                        auto k = std::atomic_fetch_add(&cur,1U);
                        if (k>= 10) break;        
                         

                        

                        std::sort(numbers[k-1].begin(),numbers[k-1].end(), [](const unsigned int& x, const unsigned int& y){

                            return aLessB(x,y,1);

                        });
                        
                        std::copy(numbers[k-1].begin(),numbers[k-1].end(),this->numbersToSort.begin()+count[k-1]);                        

                    }
                    
        };
        threads.push_back(std::thread(lambda));
    }
    for (auto & ref: threads){
        ref.join();
    }
}
