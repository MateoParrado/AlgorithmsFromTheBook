#pragma once

#include <unordered_set>
#include <unordered_map>

//given an array of ints, finds if theres a pair of ints whos sum is sum
bool pairWithSum(int sum, int * arr, int len)
{
	//hash table
	std::unordered_set<int> set;
	
	for (int i = 0; i < len; i++)
	{
		//if sum is even and arr[i] == sum / 2, check if its already in there, because if it is then we have a winner
		if (((sum | 1) != sum) && *(arr + i) == sum >> 1)
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

//takes an array and finds the first continuous subarray with zero sum
//returns the index it starts and the index it ends
//returns (-1, -1) when no match found
std::pair<int, int> zeroSumSubarray(int * arr, int len)
{
	//if the first element is zero, return the first element
	if (*arr == 0)
	{
		return std::make_pair(0, 0);
	}
	
	std::unordered_map<int, int> map;

	int sum = 0;

	//if we have reached this sum before, then the elements between the last sum and this one have zero sum
	for (int i = 0; i < len; i++)
	{
		sum += arr[i];

		auto iter = map.find(sum);
		
		if (iter != map.end())
		{
			return std::make_pair(iter->second + 1, i);
		}
		
		map.insert(std::make_pair(sum, i));
	}

	return std::make_pair(-1, -1);
}