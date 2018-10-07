#pragma once
#include <string>

#include "boost\multi_array.hpp"

#pragma warning (disable: 4018)

//self documenting
std::string greatestCommonSubstring(std::string str1, std::string str2) {
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[str1.size()][str2.size()]);

	unsigned int maxInd = 0;
	char * firstLetter = nullptr;

	for (index i = 0; i < str1.size(); i++) {
		for (index j = 0; j < str2.size(); j++) {
			//if the two are equal to each otehr
			if (str1[i] == str2[j]) {
				//if theyre the endpoints make it 1
				if (!i || !j) {
					opt[i][j] = 1;
				}
				//otherwise make it whatever the optimum of one less than them was
				else {
					opt[i][j] = opt[i - 1][j - 1] + 1;
				}
				//maxInd is the index of the maximum and firstLetter is the pointer to it, used to trace back through it later
				if (opt[i][j] > maxInd) {
					maxInd = opt[i][j];
					firstLetter = &str1[i];
				}
			}
		}
	}

	//i dont understand how or why this works
	std::string y;
	for (unsigned int i = 0; i < maxInd; i++) {
		y += str1[i];
	}
	return y;
}

//finds the best way to pair up two strings i and j
//eg "ab", "aab" -> (0, 0), (1, 2)
//cost func is f aunction that takes two charachters and returns the cost of pairing those two chars
//example is the cost of mathcing a charachter with itself is 0, a vowel with another vowel is three, and anything else is 9
std::vector<std::pair<unsigned int, unsigned int>> * sequenceAlignment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) {
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[x.size() + 1][y.size() + 1]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < x.size() + 1; i++) {
		opt[i][0] = i * delta;
	}
	for (unsigned int i = 0; i < y.size() + 1; i++) {
		opt[0][i] = i * delta;
	}

	for (index i = 1; i < x.size() + 1; i++) {
		for (index j = 1; j < y.size() + 1; j++) {
			//if m is a full matching then for every i, j either i, j is in m
			//or one of them is unmatched (in an matching actually)
			//check the minimum cost of all these possibilities
			opt[i][j] = std::min({ opt[i - 1][j - 1] + costFunc(x[i - 1], y[j - 1]), delta + opt[i - 1][j], delta + opt[i][j - 1] });
		}
	}
	
	std::vector<std::pair<unsigned int, unsigned int>> * retVec = new std::vector<std::pair<unsigned int, unsigned int>>;

	unsigned int i = x.size();
	unsigned int j = y.size();

	//if either i or j is zero its done
	while (i && j) {
		unsigned int min = std::min({ opt[i - 1][j - 1], opt[i][j - 1], opt[i - 1][j] });

		//sometimes the diagonal will be the minimum but its only because it took two deltas to get to the point, so in reality it shouldnt be included
		if (min == opt[i][j] - 2 * delta) {
			i--;
			j--;
			continue;
		}
		
		//if the diagonal is the minimum its a pair, otherwise its not adn just keep going
		if (min == opt[i - 1][j - 1]) {
			retVec->push_back(std::make_pair(--i, --j));
		}
		else if (min == opt[i - 1][j]) {
			i--;
		}
		else {
			j--;
		}
	}
	for (int i = 0; i < x.size() + 1; i++) {
		for (int j = 0; j < y.size() + 1; j++) {
			std::cout << opt[i][j] << " ";
			if (opt[i][j] < 9) std::cout << " ";
		}
		std::cout << std::endl;
	}
	return retVec;
}

//finds the best way to pair up two strings i and j
//eg "ab", "aab" -> (0, 0), (1, 2)
//cost func is f aunction that takes two charachters and returns the cost of pairing those two chars
//example is the cost of mathcing a charachter with itself is 0, a vowel with another vowel is three, and anything else is 9
std::vector<std::pair<unsigned int, unsigned int>> * backwardsSequenceAlignment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) {
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[x.size() + 1][y.size() + 1]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < x.size() + 1; i++) {
		opt[i][y.size()] = (x.size() - i) * delta;
	}
	for (unsigned int i = 0; i < y.size() + 1; i++) {
		opt[x.size()][i] = (y.size() - i) * delta;
	}

	opt[x.size()][y.size()] = 0;

	for (index i = x.size() - 1; i >= 0; i--) {
		for (index j = y.size() - 1; j >= 0; j--) {
			//if m is a full matching then for every i, j either i, j is in m
			//or one of them is unmatched (in an matching actually)
			//check the minimum cost of all these possibilities

			opt[i][j] = std::min({ opt[i + 1][j + 1] + costFunc(x[i], y[j]), delta + opt[i + 1][j], delta + opt[i][j + 1] });
		}
	}

	for (int i = 0; i < x.size() + 1; i++) {
		for (int j = 0; j < y.size() + 1; j++) {
			std::cout << opt[i][j] << " ";
			if (opt[i][j] < 9) std::cout << " ";
		}
		std::cout << std::endl;
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = new std::vector<std::pair<unsigned int, unsigned int>>;

	unsigned int i = 0;
	unsigned int j = 0;

	//if either i or j is zero its done
	while (i < x.size() && j < y.size()) {
		unsigned int min = std::min({ opt[i + 1][j + 1], opt[i][j + 1], opt[i + 1][j] });

		//sometimes the diagonal will be the minimum but its only because it took two deltas to get to the point, so in reality it shouldnt be included
		if (min == opt[i][j] - 2 * delta) {
			i++;
			j++;
			continue;
		}

		//if the diagonal is the minimum its a pair, otherwise its not adn just keep going
		if (min == opt[i + 1][j + 1]) {
			retVec->push_back(std::make_pair(i, j));
			i++;
			j++;
		}
		else if (min == opt[i + 1][j]) {
			i++;
		}
		else {
			j++;
		}
	}

	return retVec;
}
//finds the minimum error of the pairings above in linear space, helper function
unsigned int * spaceEfficientSequenceAlignmentVal(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) {
	//str is the shorter string, and therefore th ebetter choice for space efficiency
	//std::string * str = x.length() < y.length() ? &x : &y;
	//std::string * notStr = x.length() < y.length() ? &y : &x;

	std::string * str = &x;
	std::string * notStr = &y;

	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[str->size() + 1][2]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < str->size() + 1; i++) {
		opt[i][0] = i * delta;
	}

	//used to know when were done looping through the second, longer string
	unsigned int notStrDone = 0;

	//used to switch the index being written to between the zeroth and the first for efficiency
	char ind = 1;
	
	//if we havent looped fully through the longer string, keep looping
doItAgain:

	//sets the cost of not matching anything
	opt[0][ind] = delta * (notStrDone + 1);

	//same recurrence as the one for the non space efficient version
	for (index i = 1; i < str->size() + 1; i++) {
		opt[i][ind] = std::min({ opt[i - 1][!ind] + costFunc((*str)[i - 1], (*notStr)[notStrDone]), delta + opt[i - 1][ind], delta + opt[i][!ind] });
	}

	//check if done
	if (notStrDone < notStr->size() - 1) {
		//if done, switch the index being written to
		ind = !ind;
		notStrDone++;
		goto doItAgain;
	}
	
	//return an array slice of the row we need
	unsigned int * ret = new unsigned int[str->size() + 1];
	for (unsigned int i = 0; i < str->size() + 1; i++) {
		ret[i] = opt[i][ind];
	}
	return ret;
}

//finds the minimum error of the pairings above in linear space, helper function
unsigned int * backwardsSpaceEfficientSequenceAlignmentVal(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) {
	//str is the shorter string, and therefore th ebetter choice for space efficiency
	//std::string * str = x.length() < y.length() ? &x : &y;
	//std::string * notStr = x.length() < y.length() ? &y : &x;

	std::string * str = &x;
	std::string * notStr = &y;

	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[str->size() + 1][2]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < str->size() + 1; i++) {
		opt[i][0] = (str->size() - i) * delta;
	}

	//used to know when were done looping through the second, longer string
	unsigned int notStrDone = 0;

	//used to switch the index being written to between the zeroth and the first for efficiency
	char ind = 1;

	//if we havent looped fully through the longer string, keep looping
doItAgain:

	for (int i = 0; i < str->size() + 1; i++) {
		std::cout << opt[i][!ind] << " ";
		if (opt[i][!ind] < 10) std::cout << " ";
	}
	std::cout << std::endl;

	//sets the cost of not matching anything
	opt[str->size()][ind] = delta * (notStrDone + 1);

	//same recurrence as the one for the non space efficient version
	for (index i = str->size() - 1; i >= 0; i--) {
		opt[i][ind] = std::min({ opt[i + 1][!ind] + costFunc((*str)[i], (*notStr)[notStr->size() - notStrDone - 1]), delta + opt[i + 1][ind], delta + opt[i][!ind] });
	}

	//check if done
	if (notStrDone < notStr->size() - 1) {
		//if done, switch the index being written to
		ind = !ind;
		notStrDone++;
		goto doItAgain;
	}
	for (int i = 0; i < str->size() + 1; i++) {
		std::cout << opt[i][ind] << " ";
		if (opt[i][!ind] < 10) std::cout << " ";
	}
	std::cout << std::endl;
	//return an array slice of the row we need
	unsigned int * ret = new unsigned int[str->size() + 1];
	for (unsigned int i = 0; i < str->size() + 1; i++) {
		ret[i] = opt[i][ind];
	}
	return ret;
}

std::vector<std::pair<unsigned int, unsigned int>> * spaceEfficientSequenceAlignment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char), std::vector<std::pair<unsigned int, unsigned int>> * ptr = nullptr, unsigned int incVal = 0, unsigned int yIncVal = 0) {
	if (!ptr) {
		ptr = new std::vector<std::pair<unsigned int, unsigned int>>;
		ptr->reserve(x.size() + y.size());
	}

	//if its two then it isnt more space efficient to do it this way so do it normal, also end of recursion
	if (x.size() < 3 || y.size() < 3) {
		std::vector<std::pair<unsigned int, unsigned int>> * temp = sequenceAlignment(x, y, delta, costFunc);
		for (unsigned int i = 0; i < temp->size(); i++) {
			ptr->push_back((*temp)[i]);
		}

		delete temp;

		return ptr;
	}
	
	unsigned int * front = spaceEfficientSequenceAlignmentVal(x, y.substr(0, y.size() / 2), delta, costFunc);
	unsigned int * back = backwardsSpaceEfficientSequenceAlignmentVal(x, y.substr(y.size() / 2, y.size() - y.size() / 2), delta, costFunc);

	unsigned int minVal = -1;
	unsigned int minIndex = -1;
	std::cout << "StART\n";
	for (unsigned int i = 0; i < std::min(x.size() + 1, y.size() / 2 + 1); i++) {
		if (front[i] + back[i] <= minVal) {
			minVal = front[i] + back[i];
			std::cout << front[i] << " " << back[i] << std::endl;
			minIndex = i;
		}
	}
	std::cout << "END\n";
	delete[] front;
	delete[] back;

	auto ygfd = minIndex + incVal;
	auto greds = y.size() / 2 + incVal;

	ptr->push_back(std::make_pair(minIndex + incVal, y.size() / 2 + yIncVal));

	spaceEfficientSequenceAlignment(x.substr(0, minIndex), y.substr(0, y.size() / 2), delta, costFunc, ptr, incVal, yIncVal);
	spaceEfficientSequenceAlignment(x.substr(minIndex, x.size() - minIndex), y.substr(y.size() / 2, y.size() - y.size() / 2), delta, costFunc, ptr, incVal + x.size() - minIndex, yIncVal + y.size() - y.size()/2);

	return ptr;
}

#pragma warning (default : 4018)