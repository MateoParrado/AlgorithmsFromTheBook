#pragma once

#include <vector>
#include <functional>//for the input lambda

#include "Heap.h"
#include "Graph.h"
#include "Utils.h"

template<class T>
//clusters a vector of data into k groups
//needs a function to tell it how far apart each two are (should return an unsigned int)
//this is a garbage algorithm, but its good practice
Graph::WeightedGraph<T> kruskalClustering(std::vector<T> * itemVec, unsigned int (*costFunc)(T, T), unsigned int k = 2)
{
	//the "nodes" for the union find
	struct unionPtr 
	{
		unsigned int num;
		unionPtr * ptr;

		unionPtr(unsigned int val) : num(val), ptr(nullptr) { }
	};

	//initialize union find
	std::vector < unionPtr > unionFind;
	unionFind.reserve(itemVec->size());

	for (unsigned int i = 0; i < itemVec->size(); i++)
	{
		unionFind.push_back(i);
	}

	unsigned int connectedComponentCount = itemVec->size();//to stop when the time is right

	//priotity queue for the edges
	Heap::Heap < std::pair<unsigned int, std::pair<T, T>>> weightHeap((itemVec->size()*(itemVec->size() - 1)) / 2);//n choose two big

	//initialize heap
	for (unsigned int i = 0; i < itemVec->size() - 1; i++)
	{
		for (unsigned int j = i + 1; j < itemVec->size(); j++)
		{
			weightHeap.insert({ costFunc((*itemVec)[i], (*itemVec)[j]), {i, j} });
		}
	}

	//create a graph with all the nodes being the elements of itemVec
	Graph::WeightedGraph<T> retGraph;
	for (unsigned int i = 0; i < itemVec->size(); i++)
	{
		retGraph.addNode((*itemVec)[i]);
	}

	//run kruskals algorithm until there are exactly k-2 edges
	while (connectedComponentCount != k)
	{
		unionPtr * firstPtr = unionFind[weightHeap.getMin().second.first].ptr;
		unionPtr * secondPtr = unionFind[weightHeap.getMin().second.second].ptr;

		//if either of them are null then just set them to point to themselves as they are their own root
		if (!firstPtr)
		{
			firstPtr = &unionFind[weightHeap.getMin().second.first];
		}
		if (!secondPtr)
		{
			secondPtr = &unionFind[weightHeap.getMin().second.second];
		}

		//find their roots
		while (firstPtr->ptr) 
		{
			firstPtr = firstPtr->ptr;
		}
		while (secondPtr->ptr) 
		{
			secondPtr = secondPtr->ptr;
		}

		//if roots are different then add join the groups
		if (firstPtr != secondPtr)
		{
			retGraph.addEdge(weightHeap.getMin().second.first, weightHeap.getMin().second.second, weightHeap.getMin().first);

			connectedComponentCount--;//two ccs were joined

			firstPtr->ptr = secondPtr;
		}

		weightHeap.popMin();
	}

	return retGraph;
}

std::vector<std::pair<int, int>> calculateCentroid(std::vector<std::vector<std::pair<int, int>>> * bucketsPtr)
{

	std::vector<std::pair<int, int>> centroids(bucketsPtr->size());

	//this is absolute laziness
	int increment = 0;

	//add average of total coords to vector
	for (auto i : *bucketsPtr)
	{
		int xSum = 0, ySum = 0;

		for (unsigned int j = 0; j < i.size(); j++) 
		{
			xSum += i[j].first;
			ySum += i[j].second;
		}
		xSum /= i.size();
		ySum /= i.size();

		centroids[increment++] = std::make_pair(xSum, ySum);
	}

	return centroids;
}

//arranges data into clumps based on their similarity and quantity
std::vector<std::pair<int, int>> kMeans(const std::vector<std::pair<int, int>> * data, int clusters = 2) 
{
	//initialize vector of size clusters
	std::vector<std::pair<int, int>> means(clusters);

	//vector to hold vectors of  data points in a specific cluster
	std::vector<std::vector<std::pair<int, int>>> bucketsPtr(clusters);

	//randomly initialize means
	for (unsigned char i = 0; i < means.size(); i++)
	{
		means[i] = std::make_pair(rand() % 100, rand() % 100);
	}

recalculate:
	for (auto i : *data)
	{
		//infinity
		double d = 100000;
		int mNum = -1;

		//find nearest cluster
		for (unsigned int j = 0; j < means.size(); j++)
		{
			if (dist(&means[j], &i) < d)
			{
				d = dist(&means[j], &i);
				mNum = j;
			}
		}

		//pass reference to i into bucketsPtr in the right bucket
		bucketsPtr[mNum].push_back(i);
	}

	auto updatedPos = calculateCentroid(&bucketsPtr);
	if (!(updatedPos == means)) 
	{
		means = updatedPos;

		//janky recursion
		goto recalculate;
	}
	return means;
}

#pragma deprecated (calculateCentroid)