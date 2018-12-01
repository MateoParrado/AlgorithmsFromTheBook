#pragma once
#include <vector>

template<class T>
//helper func to handle the recursion in the lower func
std::vector<T> inversionCounter(std::vector<T> vec, unsigned int * counter) {
	//end recursion
	if (vec.size() == 1) {
		return vec;
	}
	if (vec.size() == 2) {
		if (vec[0] < vec[1]) {
			return vec;
		}

		T temp = vec[0];
		vec[0] = vec[1];
		vec[1] = temp;

		(*counter)++;

		return vec;
	}

	//recur on the first and second half of the lists
	std::vector<T> firstHalf = inversionCounter(std::vector<T>(vec.begin(), vec.begin() + vec.size() / 2), counter);
	std::vector<T> secondHalf = inversionCounter(std::vector<T>(vec.begin() + vec.size() / 2, vec.end()), counter);

	std::vector<T> retVec;
	retVec.reserve(vec.size());

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	while (retVec.size() < vec.size()) {
		if (firstIndex == firstHalf.size()) {
			for (unsigned int j = secondIndex; j < secondHalf.size(); j++) {
				retVec.push_back(secondHalf[j]);
			}
			break;
		}
		else if (secondIndex == secondHalf.size()) {
			for (unsigned int j = firstIndex; j < firstHalf.size(); j++) {
				retVec.push_back(firstHalf[j]);
			}

			(*counter) += firstHalf.size() - firstIndex;
			break;
		}

		if (firstHalf[firstIndex] <= secondHalf[secondIndex]) {
			retVec.push_back(firstHalf[firstIndex]);
			firstIndex++;
			continue;
		}

		(*counter)++;

		retVec.push_back(secondHalf[secondIndex]);
		secondIndex++;
	}

	return retVec;
}

template<class T>
//counts the number of times a list of objects is out of order
//for example, 2, 4, 1, 3, 5 has 3
//(4, 1), (4, 3), (2, 1)
unsigned int inversionCounter(std::vector<T> vec) {
	if (vec.size() <= 1) {
		return 0;
	}
	if (vec.size() == 2) {
		if (vec[0] < vec[1]) {
			return 0;
		}
		return 1;
	}

	unsigned int * counter = DBG_NEW unsigned int(0);

	std::vector<T> firstHalf = inversionCounter(std::vector<T>(vec.begin(), vec.begin() + vec.size() / 2), counter);
	std::vector<T> secondHalf = inversionCounter(std::vector<T>(vec.begin() + vec.size() / 2, vec.end()), counter);

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	while (true) {
		if (firstIndex == firstHalf.size()) {
			break;
		}
		else if (secondIndex == secondHalf.size()) {
			(*counter) += firstHalf.size() - firstIndex + 1;
			break;
		}

		if (firstHalf[firstIndex] <= secondHalf[secondIndex]) {
			firstIndex++;
			continue;
		}

		(*counter) += firstIndex + 1;
		secondIndex++;
	}

	unsigned int ret = *counter;

	delete counter;

	return ret;
}