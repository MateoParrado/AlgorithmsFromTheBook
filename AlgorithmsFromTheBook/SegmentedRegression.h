#pragma once
#include "Utils.h"
#include <vector>

template<class T>
//an algorithm that performs linear regresion where more than one line would fit the data better than a single one
//cost is the cost of creating a DBG_NEW line (to prevent having it just connect every consecutive pair of points)
std::vector<std::pair<unsigned int, unsigned int>> * segmentedLinearRegression(std::pair<T, T> * x, const unsigned int size, unsigned int cost) 
{
	//the cost of the optimal solution at every point
	std::vector<double> opt(size);

	//the error[i*size + j] is the minimum error of a line between the two points i and j
	std::vector<double> error(size*size);

	//calculate the errors array
	for (unsigned int i = 1; i < size; i++)
	{

		for (unsigned int j = 0; j < i - 1; j++) 
		{
			double sqError = squaredError(&x[j], i - j + 1);

			error[j*size + i] = sqError;
		}
	}

	//fill the opt array
	for (unsigned int i = 2; i < size; i++) 
	{//starts at 2 because opt[0] and opt[1] are both zero
		double minVal = std::numeric_limits<double>::infinity();

		for (unsigned int j = 1; j < i; j++)
		{//starts at one for same reason as above
			double e = error[i*size + j] + cost + opt[j - 1];
			if (e < minVal) minVal = e;
		}

		//opt[i] is the minimum possible value for the cost of matching i with anything before it
		opt[i] = minVal;
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = DBG_NEW std::vector<std::pair<unsigned int, unsigned int>>;

	//number we are currently finding the minimum for
	unsigned int checkNum = size - 1;

	while (checkNum) 
	{
		unsigned int minI = 0;
		double minIVal = error[checkNum] + cost;

		//find which point to draw a line to to minimize squared error
		for (unsigned int i = 1; i < checkNum; i++)
		{
			double temp = error[i*size + checkNum] + cost + opt[i - 1];
			if (temp < minIVal) 
			{
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