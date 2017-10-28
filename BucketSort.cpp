#include "BucketSort.h"

#include <thread>
#include <algorithm>
#include <vector>
#include <cmath>
#include <atomic>
#include <iostream>


struct{
    std::vector<unsigned int>::iterator s;
    std::vector<unsigned int>::iteartor e;
    unsigned int digit;

}

int getDigit(const std::string& str,unsigned int digit){
    if (digit >= str.size()) return -1;
    return str.at(digit) - '0';        
}

int getDigitInt(unsigned int a, unsigned int digit){
    if (a <  (unsigned int) std::round(std::pow(10,digit))) return -1;
    while (a / 10 >= (unsigned int) std::round(std::pow(10,digit))) {
        a = a / 10;
    }
    return a%10;
}

int radix_sort(std::vector<unsigned int>::iterator s,
               std::vector<unsigned int>::iterator e, int digit){
    
    if (digit >=11 || std::distance(s,e) <= 1) return 0;
    std::vector<std::vector<unsigned int>> bucket(11,std::vector<unsigned int>());
    for (auto c = s; c != e; ++c){
        bucket[getDigitInt(*c,digit)+1].push_back(*c);
    }
    
    //radix sort next digit
    int accumulate_pos = 0;
    for (size_t i = 0; i < bucket.size(); ++i){
        radix_sort(bucket[i].begin(),bucket[i].end(),digit+1);
    	std::copy(bucket[i].begin(),bucket[i].end(),s+accumulate_pos);
        accumulate_pos += bucket[i].size();
    }
    return 0;
}


void BucketSort::sort(unsigned int numCores) {

    std::vector<std::vector<unsigned int>> numbers_bucket(9,std::vector<unsigned int>());
    
 
    for (auto iter = numbersToSort.begin(); iter != numbersToSort.end(); ++ iter){
	    numbers_bucket[getDigitInt(*iter,0)-1].push_back(*iter);
    }
        
    std::vector<unsigned int> count;
    size_t temp = 0;
    for (auto &k:numbers_bucket){
        count.push_back(temp);
	    temp += k.size();
    }

    std::vector<std::thread> threads;
    
    std::atomic<unsigned int> cur = ATOMIC_VAR_INIT(1);
    for (unsigned int i = 0; i < numCores ; i++){
        auto lambda = [&,this](){
                    while (true){
                        auto k = std::atomic_fetch_add(&cur,1U);
                        if (k>= 10) break;        
                                                
                        radix_sort(numbers_bucket[k-1].begin(),numbers_bucket[k-1].end(),1); 
                         
                        std::copy(numbers_bucket[k-1].begin(),
                                       numbers_bucket[k-1].end(),
                                       this->numbersToSort.begin()+count[k-1]
                                );                        
                    }
                    
        };
        threads.push_back(std::thread(lambda));
    }
    for (auto & ref: threads){
        ref.join();
    }
}


