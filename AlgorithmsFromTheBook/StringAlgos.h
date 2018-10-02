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

	//for (int i = 0; i < x.size() + 1; i++) {
	//	for (int j = 0; j < y.size() + 1; j++) {
	//		std::cout << opt[i][j] << " ";
	//	}
	//	std::cout << std::endl;
	//}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = new std::vector<std::pair<unsigned int, unsigned int>>;

	unsigned int i = 0;
	unsigned int j = 0;

	//if either i or j is zero its done
	while (i < x.size() && j < x.size()) {
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
unsigned int spaceEfficientSequenceAligmment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) {
	//str is the shorter string, and therefore th ebetter choice for space efficiency
	std::string * str = x.length() < y.length() ? &x : &y;
	std::string * notStr = x.length() < y.length() ? &y : &x;

	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[str->size() + 1][2]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < str->size() + 1; i++) {
		opt[i][0] = i * delta;
	}
	opt[0][1] = delta;

	char ind = 1;

	for (index i = 1; i < str->size() + 1; i++) {
		opt[i][ind] = std::min({ opt[i - 1][!ind] + costFunc((*notStr)[i - 1], (*str)[!ind]), delta + opt[i - 1][!ind], delta + opt[i][!ind] });

		ind = !ind;
	}
	for (unsigned int i = 0; i < str->size() + 1; i++) {
		std::cout << opt[i][0] << " ";
	}
	std::cout << std::endl;

	for (unsigned int i = 0; i < str->size() + 1; i++) {
		std::cout << opt[i][1] << " ";
	}

	return opt[str->size()][ind];
}

#pragma warning (default : 4018)