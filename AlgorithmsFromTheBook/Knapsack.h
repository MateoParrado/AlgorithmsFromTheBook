#pragma once
#include <vector>

#include "boost\multi_array.hpp"

//finds the maximum value able to be put in a knapsack with a given capacity
//input should be (value, weight) pairs
std::vector<int> * knapsackPacker(std::pair<unsigned int, unsigned int> * x, unsigned int size, unsigned int capacity) {
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[size + 1][capacity + 1]);

	//make a two dimensional memoization, one dim is with the different items being included or excluded
	//and the other is how much weight we have left
	for (index i = 0; i < size; i++) {
		for (index w = 1; w < capacity + 1; w++) {
			//would the current item overflow the knapsack?
			if (w < x[i].second) {
				opt[i + 1][w] = opt[i][w];
			}
			else {
				opt[i + 1][w] = std::max(opt[i][w], x[i].first + opt[i][w - x[i].second]);
			}
		}
	}

	std::vector<int> * retVec = new std::vector<int>;

	//backtrack through the memoization
	int checkNum = size;
	int checkCap = capacity;

	while (opt[checkNum][checkCap]) {
		if (x[checkNum - 1].second > checkCap) {
			checkNum--;
			continue;
		}
		unsigned int notIn = opt[checkNum - 1][capacity];
		unsigned int in = x[checkNum - 1].second + opt[checkNum - 1][checkCap - x[checkNum - 1].second];
		if (notIn > in) {
			checkNum--;
		}
		else {
			retVec->push_back(--checkNum);
			checkCap -= x[checkNum].second;
		}
	}
	return retVec;
}