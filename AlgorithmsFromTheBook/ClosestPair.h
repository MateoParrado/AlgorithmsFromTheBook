#pragma once
#include <assert.h>

#include "Utils.h"

template<class T>
//helper function to the function below
std::pair<std::pair<unsigned int, unsigned int>, double> closestPairHelperFunc(std::pair<T, T> * start, unsigned int size) {
	//end recursion
	if (size == 2) {
		return std::make_pair(std::make_pair(0, 1), dist(start, start + 1));
	}
	if (size == 3) {
		double minDist = dist(start + 1, start + 2);
		std::pair<unsigned int, unsigned int> minPair = { 1, 2 };

		for (unsigned char i = 1; i < 3; i++) {
			double newMin = dist(start, start + i);

			if (newMin < minDist) {
				minDist = newMin;
				minPair = { 0, i };
			}
		}

		return std::make_pair(minPair, minDist);
		
	}

	if (start->first == 11 && start->second == 625) {
		bool b = true;
	}

	//split it into two sections and get the minimums from them
	std::pair<std::pair<unsigned int, unsigned int>, double> min = closestPairHelperFunc(start, size / 2);
	{
		std::pair<std::pair<unsigned int, unsigned int>, double> temp = closestPairHelperFunc(start + size / 2, size - (size / 2));

		if (temp.second < min.second) {
			min = temp;

			min.first.first += (size / 2);
			min.first.second += (size / 2);
		}
	}

	//take all nodes that are withing one minimum length from the middle line
	std::vector<std::pair<std::pair<T, T>, unsigned int>> closeToLine;
	closeToLine.reserve(size / 4);
	for (unsigned int i = 0; i < size; i++) {
		auto auti = *(start + i);
		if (abs((start + size / 2)->first - (start + i)->first) <= min.second) {
			closeToLine.push_back({ *(start + i), i });
		}
	}

	//sort them by y
	std::sort(closeToLine.begin(), closeToLine.end(), [](const std::pair<std::pair<T, T>, unsigned int> a, const std::pair<std::pair<T, T>, unsigned int> b) {
		return a.first.second < b.first.second;
	});

	//check the 3 nodes above every node for a smaller distance
	for (unsigned int i = 0; i < closeToLine.size() - 1; i++) {
		unsigned int maxVal = closeToLine.size() - i;
		if (maxVal > 4) maxVal = 4;
		for (unsigned char j = 1; j < maxVal; j++) {
			double d = dist(&closeToLine[i].first, &closeToLine[i + j].first);
			if (d < min.second) {
				min = { {closeToLine[i].second, closeToLine[i + j].second}, d };
			}
		}
	}

	return min;
}

template<class T>
std::pair<unsigned int, unsigned int> closestPair( std::vector<std::pair<T, T>> * nodes) {
	if (nodes->size() < 2) throw 0;
	else if (nodes->size() == 2) return std::make_pair(0, 1);
	else if (nodes->size() == 3) {
		double minDist = dist(&(*nodes)[1], &(*nodes)[2]);
		std::pair<unsigned int, unsigned int> minPair = { 1, 2 };

		for (unsigned char i = 1; i < 3; i++) {
			double newMin = dist(&(*nodes)[0], &(*nodes)[i]);

			if (newMin < minDist) {
				minDist = newMin;
				minPair = { 0, i };
			}
		}
		return minPair;
	}

	//sort every coordinate by their x value
	std::vector<std::pair<T, T>> * xSort = new std::vector<std::pair<T, T>>;
	xSort->reserve(nodes->size());
	for (unsigned int i = 0; i < nodes->size(); i++) {
		xSort->push_back((*nodes)[i]);
	}
	std::sort(xSort->begin(), xSort->end());
	
	//cut it in haf and find the mins of each half
	std::pair<std::pair<unsigned int, unsigned int>, double> min = closestPairHelperFunc(&(*xSort)[0], xSort->size() / 2);
	{
		std::pair<std::pair<unsigned int, unsigned int>, double> temp = closestPairHelperFunc(&(*xSort)[xSort->size() / 2], xSort->size() - (xSort->size() / 2));

		if (temp.second < min.second) { 
			min = temp; 

			min.first.first += (xSort->size() / 2);
			min.first.second += (xSort->size() / 2);
		}
	}

	//take all points within one min dist of the division
	std::vector<std::pair<std::pair<T, T>, unsigned int>> closeToLine;
	closeToLine.reserve(nodes->size() / 4);
	for (unsigned int i = 0; i < nodes->size(); i++) {
		if (abs((*xSort)[xSort->size() / 2].first - (*xSort)[i].first) <= min.second) {
			closeToLine.push_back({ (*xSort)[i], i });
		}
	}

	//sort them by y
	std::sort(closeToLine.begin(), closeToLine.end(), [](const std::pair<std::pair<T, T>, unsigned int> a, const std::pair<std::pair<T, T>, unsigned int> b) {
		return a.first.second < b.first.second;
	});

	//check the nodes above every point to see if its less than min dist
	for (unsigned int i = 0; i < closeToLine.size(); i++) {
		unsigned int maxVal = closeToLine.size() - i;
		if (maxVal > 4) maxVal = 4;

		for (unsigned char j = 1; j < maxVal; j++) {
			double d = dist(&closeToLine[i].first, &closeToLine[i + j].first);
			if (d < min.second) {
				min = { { closeToLine[i].second, closeToLine[i + j].second }, d };
			}
		}
	}
	
	//now you have to find which indexes the sorted list corresponds to
	std::pair<unsigned int, unsigned int> retPair = { 0, 0 };

	for (unsigned int i = 0; i < nodes->size(); i++) {
		if ((*nodes)[i].first == (*xSort)[min.first.first].first && (*nodes)[i].second == (*xSort)[min.first.first].second) {
			retPair.first = i;
			break;
		}
	}
	for (unsigned int i = 0; i < nodes->size(); i++) {
		if ((*nodes)[i].first == (*xSort)[min.first.second].first && (*nodes)[i].second == (*xSort)[min.first.second].second) {
			retPair.second = i;
			break;
		}
	}

	delete xSort;

	return retPair;
}

template<class T>
//check which pair of points is closest using brute force
//designed only to check the above algoruthms accuracy
std::pair<unsigned int, unsigned int> bruteForceClosestPair(std::vector<std::pair<T, T>> * nodes) {
	double min = 100000000000;
	std::pair<unsigned int, unsigned int> minInd;

	for (unsigned int i = 0; i < nodes->size() - 1; i++) {
		for (unsigned int j = i + 1; j < nodes->size(); j++) {
			if (dist(&(*nodes)[i], &(*nodes)[j]) < min) {
				min = dist(&(*nodes)[i], &(*nodes)[j]);
				minInd = { i, j };
			}
		}
	}
	return minInd;
}

template<class T>
//check which pair of points is closest using brute force
//designed only to check the above algoruthms accuracy
std::pair<unsigned int, unsigned int> bruteForceClosestPair(std::pair<T, T> * start, unsigned int size) {
	double min = 100000000000;
	std::pair<unsigned int, unsigned int> minInd;

	for (unsigned int i = 0; i < size - 1; i++) {
		for (unsigned int j = i + 1; j < size; j++) {
			if (dist(&start[i], &start[j]) < min) {
				min = dist(&start[i], &start[j]);
				minInd = { i, j };
			}
		}
	}
	return minInd;
}

#pragma deprecated (closestPairHelperFunc)