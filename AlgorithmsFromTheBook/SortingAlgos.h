#pragma once
#include "Heap.h"
#include <vector>

template<class T>
std::vector<T> heapSort(std::vector<T> * vec) {
	Heap::Heap<T> heap(vec->size());
	std::vector<T> retVec;
	retVec.reserve(vec->size());

	for (unsigned int i = 0; i < vec->size(); i++) {
		heap.insert((*vec)[i]);
	}

	while (heap.size()) {
		retVec.push_back(heap.popMin());
	}

	return retVec;
}

template<class T>
std::vector<T> mergeSort(std::vector<T> toSort) {
	//end case for recursion
	if (toSort.size() == 1) return toSort;
	else if (toSort.size() == 2) {
		if (toSort[0] < toSort[1]) return toSort;

		T temp = toSort[0];
		toSort[0] = toSort[1];
		toSort[1] = temp;

		return toSort;
	}

	std::vector<T> firstHalf = mergeSort(std::vector<T>(toSort.begin(), toSort.begin() + toSort.size() / 2));
	std::vector<T> secondHalf = mergeSort(std::vector<T>(toSort.begin() + toSort.size() / 2, toSort.end()));

	std::vector<T> retVec;
	retVec.reserve(toSort.size());
	
	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	while (retVec.size() < toSort.size()) {
		if (firstIndex == firstHalf.size()) {//did the first one reach the edge
			for (unsigned int j = secondIndex; j < secondHalf.size(); j++) {
				retVec.push_back(secondHalf[j]);
			}
			break;
		}
		else if (secondIndex == secondHalf.size()) {//did the second one reach the edge
			for (unsigned int j = firstIndex; j < firstHalf.size(); j++) {
				retVec.push_back(firstHalf[j]);
			}
			break;
		}

		if (firstHalf[firstIndex] < secondHalf[secondIndex]) {//is the first one less than the second
			retVec.push_back(firstHalf[firstIndex]);
			firstIndex++;
			continue;
		}

		retVec.push_back(secondHalf[secondIndex]);//else
		secondIndex++;
	}

	return retVec;
}