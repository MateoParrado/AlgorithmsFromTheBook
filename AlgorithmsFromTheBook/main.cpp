//identifier poisoning so things cant be used outside of their namespaces
#pragma warning (error : 4995)

#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>

#include "LinkedList.h"
#include "StableMatching.h"
#include "Heap.h"
#include "Graph.h"
#include "GraphAlgorithms.h"
#include "IntervalScheduling.h"
#include "Caching.h"
#include "Clustering.h"
#include "DataCompression.h"
#include "SortingAlgos.h"
#include "InversionCounter.h"
#include "ClosestPair.h"
#include "Utils.h"
#include "FastFourierTransform.h"
#include "RNA.h"
#include "StringAlgos.h"
#include "SegmentedRegression.h"

//TODO
//edmond algo
//optimizaitons in segmented linear regression (266/284), calculate errors in constant time
//divide and conquor linear space sequence aligmnent
//when done with sequence aligmment remove the printing comments
//when done witg sequence alirnment, get rid of str and notstr in the sequence alignment vals they arent used anymore

using namespace std;

unsigned int costFunc(char i, char j) {
	if (i == j) return 0;
	return 100;
}

int main() {
	//Graph::WeightedGraph<std::pair<int, int>> x(6);

	//x.addNode({ 0, 5 });
	//x.addNode({ -1, 0 });
	//x.addNode({ 0, 1 });
	//x.addNode({ 1, 0 });
	//x.addNode({ 0, -1 });
	//x.addNode({ 0, -5 });

	//x.addEdge(0, 1, 10);
	//x.addEdge(0, 3, 10);
	//x.addEdge(1, 2, 1);
	//x.addEdge(2, 3, 4);
	//x.addEdge(3, 4, 2);
	//x.addEdge(4, 2, 2);
	//x.addEdge(3, 5, 4);
	//x.addEdge(5, 1, 8);

	string x = "aaabbbccc";
	string y = "aabbb";

	//auto ygfd = sequenceAlignment(x, y, 2, &costFunc);
	std::cout << "\n\n\n";
	auto p = backwardsSequenceAlignment(x, y, 2, &costFunc);
	std::cout << "\n\n";
	
	//unsigned int * front = spaceEfficientSequenceAlignmentVal(x, y.substr(0, y.size() / 2), 2, &costFunc);
	unsigned int * back = backwardsSpaceEfficientSequenceAlignmentVal(x, y, 2, &costFunc);
	for (unsigned int i = 0; i < 10; i++) {
		//std::cout << front[i] << " ";
	}
	std::cout << std::endl;
	for (unsigned int i = 0; i < 10; i++) {
	//	std::cout << back[i] << " ";
	}

	//auto yl = spaceEfficientSequenceAlignment(x, y, 2, &costFunc);

	std::getchar();
}