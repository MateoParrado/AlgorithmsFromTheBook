#pragma once

#include <unordered_set>

//given an array of ints, finds if theres a pair of ints whos sum is sum
bool pairWithSum(int sum, int * arr, int len)
{
	//hash table
	std::unordered_set<int> set;
	
	for (int i = 0; i < len; i++)
	{
		//if sum is even and arr[i] == sum / 2, check if its already in there, because if it is then we have a winner
		if (sum | 1 == sum && *(arr + i) == sum >> 1)
		{
			if (set.find(sum >> 1) != set.end())
			{
				return true;
			}
		}

		set.insert(*(arr + i));
	}

	//constant time for each element
	for (auto i : set)
	{
		//special case, make sure nothing is double counted
		if (i == sum - i)
		{
			continue;
		}

		//is the difference between i and sum in the hash map??
		auto match = set.find( sum - i);

		if (match != set.end())
		{
			return true;
		}
	}

	return false;
}