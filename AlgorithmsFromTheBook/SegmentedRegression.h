#pragma once
#include "Utils.h"
#include <vector>

template<class T>
//linear regression for when multiple lines would fit the data better than one single one
//cost is the added cost of making a new line, so that you dont end up with just lines between every pair of points
std::vector<std::pair<unsigned int, unsigned int>> * segmentedLinearRegression(std::pair<T, T> * x, const unsigned int size, unsigned int cost) {
	//the minimum possible cost at every point
	std::vector<double> opt(size);

	//the error[i*size + j] is the minimum error of a line between the two points
	std::vector<double> error(size*size);

	//initialize every error
	for (unsigned int i = 0; i < size; i++) {
		error[size*i + i] = 0;

		if (!i) continue;

		error[size*i + i - 1] = 0;
		error[size*(i - 1) + i] = 0;

		for (unsigned int j = 0; j < i - 1; j++) {
			double temp = squaredError(&x[j], i - j + 1);

			//dont think I need this line but leaving it just in case
			//error[i*size + j] = temp;

			error[j*size + i] = temp;
		}
	}

	//fill the opt array
	for (unsigned int i = 1; i < size; i++) {
		double minVal = 1000000000;
		for (unsigned int j = 1; j < i; j++) {
			double temp = error[i*size + j] + cost + opt[j - 1];
			if (temp < minVal) minVal = temp;
		}

		opt[i] = minVal;
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = new std::vector<std::pair<unsigned int, unsigned int>>;

	//num we are currently finding the minimum for
	unsigned int checkNum = size - 1;

	while (checkNum) {
		unsigned int minI = 0;
		double minIVal = error[checkNum] + cost;

		//find which point to draw a line to to minimize squared error
		for (unsigned int i = 1; i < checkNum; i++) {
			double temp = error[i*size + checkNum] + cost + opt[i - 1];
			if (temp < minIVal) {
				minIVal = temp;
				minI = i;
			}
		}

		retVec->push_back(std::make_pair(minI, checkNum));

		//find the minimum from the point we drew to
		checkNum = minI;
	}

	return retVec;
}