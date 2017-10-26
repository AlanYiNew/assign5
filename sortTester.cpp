#include <iostream>

#include <random>

#include <thread>

#include <chrono>
#include "BucketSort.h"


int main() {

	

	unsigned int totalNumbers =	500000;

	unsigned int printIndex =	259000;

	

	// use totalNumbers required as the seed for the random

	// number generator. 

	std::mt19937 mt(totalNumbers);

	std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());


	// create a sort object

	BucketSort pbs;


	// insert random numbers into the sort object

	for (unsigned int i=0; i < totalNumbers; ++i) {

		pbs.numbersToSort.push_back(dist(mt));

	} 

	

	// call sort giving the number of cores available.

        const unsigned int numCores = std::thread::hardware_concurrency();

        auto start = std::chrono::high_resolution_clock::now();
        pbs.sort(numCores);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "##" << elapsed.count() << "##"<< std::endl;

        std::cout << "number of cores used: " << numCores << std::endl;

	

	// print certain values from the buckets

	std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;

	std::cout << pbs.numbersToSort[0] << " " << pbs.numbersToSort[printIndex - 10000]

		<< " " << pbs.numbersToSort[printIndex] << " " << pbs.numbersToSort[pbs.numbersToSort.size() - 1] 

 	<< std::endl;

	

}
