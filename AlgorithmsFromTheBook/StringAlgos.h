#pragma once
#include <string>

#include "boost\multi_array.hpp"

#pragma warning (disable: 4018)

//self documenting
std::string greatestCommonSubstring(std::string str1, std::string str2) 
{
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[str1.size()][str2.size()]);

	unsigned int maxInd = 0;
	char * firstLetter = nullptr;

	for (index i = 0; i < str1.size(); i++) 
	{
		for (index j = 0; j < str2.size(); j++)
		{
			//if the two are equal to each otehr
			if (str1[i] == str2[j])
			{
				//if theyre the endpoints make it 1
				if (!i || !j) 
				{
					opt[i][j] = 1;
				}
				//otherwise make it whatever the optimum of one less than them was
				else
				{
					opt[i][j] = opt[i - 1][j - 1] + 1;
				}
				//maxInd is the index of the maximum and firstLetter is the pointer to it, used to trace back through it later
				if (opt[i][j] > maxInd)
				{
					maxInd = opt[i][j];
					firstLetter = &str1[i];
				}
			}
		}
	}

	//i dont understand how or why this works
	std::string y;
	for (unsigned int i = 0; i < maxInd; i++) 
	{
		y += str1[i];
	}
	return y;
}

//finds the best way to pair up two strings i and j
//eg "ab", "aab" -> (0, 0), (1, 2)
//cost func is f aunction that takes two charachters and returns the cost of pairing those two chars
//example is the cost of mathcing a charachter with itself is 0, a vowel with another vowel is three, and anything else is 9
std::vector<std::pair<unsigned int, unsigned int>> * sequenceAlignment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) 
{
	
	if (x.size() == 1 && y.size() == 1)
	{
		std::vector<std::pair<unsigned int, unsigned int>> * retVec = DBG_NEW std::vector<std::pair<unsigned int, unsigned int>>();
		if (costFunc(x[0], y[0]) < delta) 
		{
			retVec->push_back({ 0, 0 });
		}

		return retVec;
	}

	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[x.size() + 1][y.size() + 1]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < x.size() + 1; i++) 
	{
		opt[i][0] = i * delta;
	}
	for (unsigned int i = 0; i < y.size() + 1; i++)
	{
		opt[0][i] = i * delta;
	}

	for (index i = 1; i < x.size() + 1; i++)
	{
		for (index j = 1; j < y.size() + 1; j++)
		{
			//if m is a full matching then for every i, j either i, j is in m
			//or one of them is unmatched (in an matching actually)
			//check the minimum cost of all these possibilities
			opt[i][j] = std::min({ opt[i - 1][j - 1] + costFunc(x[i - 1], y[j - 1]), delta + opt[i - 1][j], delta + opt[i][j - 1] });
		}
	}
	
	std::vector<std::pair<unsigned int, unsigned int>> * retVec = DBG_NEW std::vector<std::pair<unsigned int, unsigned int>>;

	unsigned int i = x.size();
	unsigned int j = y.size();

	//if either i or j is zero its done
	while (i && j) 
	{
		unsigned int min = std::min({ opt[i - 1][j - 1], opt[i][j - 1], opt[i - 1][j] });

		//sometimes the diagonal will be the minimum but its only because it took two deltas to get to the point, so in reality it shouldnt be included
		if (min == opt[i][j] - 2 * delta)
		{
			i--;
			j--;
			continue;
		}
		
		//if the diagonal is the minimum its a pair, otherwise its not adn just keep going
		if (min == opt[i - 1][j - 1]) 
		{
			retVec->push_back(std::make_pair(--i, --j));
		}
		else if (min == opt[i - 1][j]) 
		{
			i--;
		}
		else 
		{
			j--;
		}
	}

	return retVec;
}

//finds the best way to pair up two strings i and j
//eg "ab", "aab" -> (0, 0), (1, 2)
//cost func is f aunction that takes two charachters and returns the cost of pairing those two chars
//example is the cost of mathcing a charachter with itself is 0, a vowel with another vowel is three, and anything else is 9
std::vector<std::pair<unsigned int, unsigned int>> * backwardsSequenceAlignment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char)) 
{
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[x.size() + 1][y.size() + 1]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < x.size() + 1; i++) 
	{
		opt[i][y.size()] = (x.size() - i) * delta;
	}
	for (unsigned int i = 0; i < y.size() + 1; i++) 
	{
		opt[x.size()][i] = (y.size() - i) * delta;
	}

	opt[x.size()][y.size()] = 0;

	for (index i = x.size() - 1; i >= 0; i--)
	{
		for (index j = y.size() - 1; j >= 0; j--) 
		{
			//if m is a full matching then for every i, j either i, j is in m
			//or one of them is unmatched (in an matching actually)
			//check the minimum cost of all these possibilities

			opt[i][j] = std::min({ opt[i + 1][j + 1] + costFunc(x[i], y[j]), delta + opt[i + 1][j], delta + opt[i][j + 1] });
		}
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = DBG_NEW std::vector<std::pair<unsigned int, unsigned int>>;

	unsigned int i = 0;
	unsigned int j = 0;

	//if either i or j is zero its done
	while (i < x.size() && j < y.size()) 
	{
		unsigned int min = std::min({ opt[i + 1][j + 1], opt[i][j + 1], opt[i + 1][j] });

		//sometimes the diagonal will be the minimum but its only because it took two deltas to get to the point, so in reality it shouldnt be included
		if (min == opt[i][j] - 2 * delta) 
		{
			i++;
			j++;
			continue;
		}

		//if the diagonal is the minimum its a pair, otherwise its not adn just keep going
		if (min == opt[i + 1][j + 1])
		{
			retVec->push_back(std::make_pair(i, j));
			i++;
			j++;
		}
		else if (min == opt[i + 1][j])
		{
			i++;
		}
		else
		{
			j++;
		}
	}

	return retVec;
}
//finds the minimum error of the pairings above in linear space, helper function
unsigned int * spaceEfficientSequenceAlignmentVal(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char))
{
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[x.size() + 1][2]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < x.size() + 1; i++) 
	{
		opt[i][0] = i * delta;
	}

	//used to know when were done looping through the second string
	unsigned int notStrDone = 0;

	//used to switch the index being written to between the zeroth and the first for efficiency
	char ind = 1;
	
	//if we havent looped fully through the longer string, keep looping
doItAgain:

	//sets the cost of not matching anything
	opt[0][ind] = delta * (notStrDone + 1);

	//same recurrence as the one for the non space efficient version
	for (index i = 1; i < x.size() + 1; i++) 
	{
		opt[i][ind] = std::min({ opt[i - 1][!ind] + costFunc(x[i - 1], y[notStrDone]), delta + opt[i - 1][ind], delta + opt[i][!ind] });
	}

	//check if done
	if (notStrDone < y.size() - 1) 
	{
		//if done, switch the index being written to
		ind = !ind;
		notStrDone++;
		goto doItAgain;
	}
	
	//return an array slice of the row we need
	unsigned int * ret = DBG_NEW unsigned int[x.size() + 1];
	for (unsigned int i = 0; i < x.size() + 1; i++)
	{
		ret[i] = opt[i][ind];
	}
	return ret;
}

//finds the minimum error of the pairings above in linear space, helper function
std::pair<unsigned int, bool> * backwardsSpaceEfficientSequenceAlignmentVal(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char))
{
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[x.size() + 1][2]);

	//the cost of just not matching it at all
	for (unsigned int i = 0; i < x.size() + 1; i++)
	{
		opt[i][0] = (x.size() - i) * delta;
	}

	//used to know when were done looping through the second, longer string
	unsigned int notStrDone = 0;

	//used to switch the index being written to between the zeroth and the first for efficiency
	char ind = 1;

	//if we havent looped fully through the second string, keep looping
doItAgain:

	//sets the cost of not matching anything
	opt[x.size()][ind] = delta * (notStrDone + 1);

	//same recurrence as the one for the non space efficient version
	for (index i = x.size() - 1; i >= 0; i--) 
	{
		opt[i][ind] = std::min({ opt[i + 1][!ind] + costFunc(x[i], y[y.size() - notStrDone - 1]), delta + opt[i + 1][ind], delta + opt[i][!ind] });
	}

	//check if done
	if (notStrDone < y.size() - 1) 
	{
		//if done, switch the index being written to
		ind = !ind;
		notStrDone++;
		goto doItAgain;
	}

	//return an array slice of the row we need
	std::pair<unsigned int, bool> * ret = DBG_NEW std::pair<unsigned int, bool>[x.size() + 1];
	for (unsigned int i = 0; i < x.size() + 1; i++) 
	{
		ret[i].first = opt[i][ind];

		//if this is false then they souldnt be matched (it is cheaper to leave them free) if it is true they should
		ret[i].second = opt[i][ind] == delta + opt[i][!ind] ? false : true;
	}
	return ret;
}

std::vector<std::pair<unsigned int, unsigned int>> * spaceEfficientSequenceAlignment(std::string x, std::string y, unsigned int delta, unsigned int(*costFunc)(char, char), std::vector<std::pair<unsigned int, unsigned int>> * ptr = nullptr, unsigned int incVal = 0, unsigned int yIncVal = 0) 
{
	if (!ptr)
	{
		ptr = DBG_NEW std::vector<std::pair<unsigned int, unsigned int>>;

		//maximum size of a corner to corner path, therefore maximum space needed
		ptr->reserve(x.size() + y.size());
	}

	//if its two then it isnt more space efficient to do it this way so do it normal, also end of recursion
	if (x.size() < 3 || y.size() < 3) 
	{
		std::vector<std::pair<unsigned int, unsigned int>> * temp = sequenceAlignment(x, y, delta, costFunc);
		for (unsigned int i = 0; i < temp->size(); i++) 
		{
			ptr->push_back(std::make_pair((*temp)[i].first + incVal, (*temp)[i].second + yIncVal));
		}

		delete temp;

		return ptr;
	}
	
	//find the values of the minimum cost of aligning the middle letter of y with any letter of x
	unsigned int * front = spaceEfficientSequenceAlignmentVal(x, y.substr(0, y.size() / 2), delta, costFunc);
	std::pair<unsigned int, bool> * back = backwardsSpaceEfficientSequenceAlignmentVal(x, y.substr(y.size() / 2, y.size() - y.size() / 2), delta, costFunc);

	unsigned int minVal = -1;
	unsigned int minIndex = -1;

	//find which char in x the middle char of y should match with
	for (unsigned int i = 0; i < x.size() + 1; i++) 
	{
		if (front[i] + back[i].first <= minVal) 
		{
			minVal = front[i] + back[i].first;
			minIndex = i;
		}
	}

	//check if it really should be matched or if it should be left blank
	if (back[minIndex].second) 
	{
		ptr->push_back(std::make_pair(minIndex + incVal, y.size() / 2 + yIncVal));
	}

	delete[] front;
	delete[] back;

	//recur on the two subsequences formed
	spaceEfficientSequenceAlignment(x.substr(0, minIndex), y.substr(0, y.size() / 2), delta, costFunc, ptr, incVal, yIncVal);
	if (minIndex <= x.size() - minIndex + 1)
	{
		spaceEfficientSequenceAlignment(x.substr(minIndex, x.size() - minIndex), y.substr(y.size() / 2 + 1, y.size() - y.size() / 2 + 1), delta, costFunc, ptr, incVal + minIndex, yIncVal + y.size() - y.size() / 2);
	}
	return ptr;
}

//get length of longest palindrome in a substring
//note that it will return a value that includes some preprocessed spaces, so you can divide the result by two
//if its odd its centered on a letter, if its even its centered in between two charachters
int getLengthOfLongestPalindrome(std::string * s) 
{
	//holds null chars in between letters
	std::string preprocessedStr;
	preprocessedStr.resize(s->size() * 2 + 1, 0);

	for (int i = 0; i < s->size(); i++)
	{
		preprocessedStr[2 * i + 1] = (*s)[i];
	}

	//holds size of largest plaindrome centered at x
	char * sizeArray = DBG_NEW char[preprocessedStr.size()]{ 0 };

	unsigned int furthestRightPalindrome = 0;
	unsigned int palindromeSize = 0;

	for (int i = 0; i < preprocessedStr.size(); i++) 
	{
		//dist from center of palindrome
		int dist = 1;
		
		//are we within a palindrome?
		if ((i - furthestRightPalindrome) < palindromeSize)
		{
			//are we fully contained?
			//even though i have literally no idea why the negative i works, it comes from furthestPalindrome - (i - furthestPalindrome)
			if (sizeArray[-i] + i - furthestRightPalindrome < furthestRightPalindrome - palindromeSize)
			{
				sizeArray[i] = sizeArray[-i] - 1;

				if (sizeArray[i] + i > furthestRightPalindrome + palindromeSize) 
				{
					furthestRightPalindrome = i;
					palindromeSize = sizeArray[i];
				}
				continue;
			}//are we partially contained
			else 
			{
				//for some reason oi doesn't work here, deal with it later
				sizeArray[i] = sizeArray[furthestRightPalindrome - (i - furthestRightPalindrome)] - 1;

				dist = sizeArray[furthestRightPalindrome - (i - furthestRightPalindrome)];
			}
		}
		for (;;)
		{
			//bottom bounds check
			if (i - dist < 0)
			{
				if (dist > 2 && i + --dist> furthestRightPalindrome + palindromeSize)
				{
					furthestRightPalindrome = i;
					palindromeSize = dist;
				}
				break;
			}
			//top bounds check
			if (i + dist >= preprocessedStr.size()) goto done;


			//does the palindrome continue
			if (preprocessedStr[i - dist] == preprocessedStr[i + dist])
			{
				dist++;
				sizeArray[i]++;
			}
			else {
				//correct palindrome savers
				if (dist > 2 && i + --dist> furthestRightPalindrome + palindromeSize)
				{
					furthestRightPalindrome = i + dist;
					palindromeSize = dist;
				}
				break;
			}
		}
	}
done:

	int retVal = 0;

	//find largest value
	for (int i = 0; i < preprocessedStr.size(); i++) 
	{
		if (sizeArray[i] > retVal)
		{
			retVal = sizeArray[i];
		}
	}

	delete[] sizeArray;

	return retVal;
}

#pragma deprecated(backwardsSpaceEfficientSequenceAlignmentVal, spaceEfficientSequenceAlignmentVal)
#pragma warning (default : 4018)