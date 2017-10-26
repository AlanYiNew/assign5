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



int getDigit(const std::string& str,unsigned int digit){
    if (digit >= str.size()) return -1;
    return str.at(digit) - '0';        
}

int radix_sort(std::vector<std::string*>::iterator s,
               std::vector<std::string*>::iterator e, int digit){
    
    if (digit >=32 || std::distance(s,e) <= 1) return 0;
    std::vector<std::vector<std::string*>> bucket(11,std::vector<std::string*>());
    for (auto c = s; c != e; ++c){
        bucket[getDigit(**c,digit)+1].push_back(*c);
    }
    
    //radix sort next digit
    int accumulate_pos = 0;
    for (size_t i = 0; i < bucket.size(); ++i){
        radix_sort(bucket[i].begin(),bucket[i].end(),digit+1);
        //for (auto iter = bucket[i].begin(); iter != bucket[i].end(); ++ iter){
	//	std::cout << **iter << " ";
	//}
	//std::cout << std::endl;
	std::copy(bucket[i].begin(),bucket[i].end(),s+accumulate_pos);
        accumulate_pos += bucket[i].size();
    }
    return 0;
}




void BucketSort::sort(unsigned int numCores) {

    std::vector<std::string> numbers;
    std::vector<std::vector<std::string*>> numbers_bucket(9,std::vector<std::string*>());
    for ( auto k: numbersToSort){
        numbers.push_back(std::to_string(k)); 
    }
 
    for (auto iter = numbers.begin(); iter != numbers.end(); ++ iter){
	numbers_bucket[getDigit(*iter,0)-1].push_back(&*iter);
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
                         
                        std::transform(numbers_bucket[k-1].begin(),
                                       numbers_bucket[k-1].end(),
                                       this->numbersToSort.begin()+count[k-1],
                                       [](std::string* str){
                                          return std::stoul(*str);
                                       }
                                
                                );                        

                    }
                    
        };
        threads.push_back(std::thread(lambda));
    }
    for (auto & ref: threads){
        ref.join();
    }
}


