#pragma once
#include <vector>
#include <algorithm>

template<class T>
//algorithm that creates a 'cache' of size k and figures out how to bring things in and kick things out of it to minimize cache misses
//this isnt an actually useful algorithm, just practice
unsigned int cacher(std::vector<T> &calls, unsigned char k) {
	unsigned int cacheMisses = 0;

	std::vector<int> cache;
	cache.reserve(k);

	{///to push i out of scope
		unsigned char i = 0;
		while (cache.size() < k) {
			if (std::find(cache.begin(), cache.end(), calls[i]) == cache.end()){
				cache.push_back(calls[i]);
			}
			i++;
		}
	}

	for (unsigned int i = 0; i < calls.size(); i++) {

		if ((std::find(cache.begin(), cache.end(), calls[i]) == cache.end())) {//if not in cache
			cacheMisses++;

			unsigned int lastPos = 0;
			unsigned char furthestIndex = 0;

			//look through the cache and decide which one is used furthest in the future
			for (unsigned int j = 0; j < cache.size(); j++) {
				unsigned int curPos = std::find(calls.begin() + i, calls.end(), cache[j]) - calls.begin(); //get index of first finding
				if (curPos > lastPos) {
					furthestIndex = j;
					lastPos = curPos;
				}
			}

			*(cache.begin() + furthestIndex) = calls[i];
		}
	}

	return cacheMisses;
}