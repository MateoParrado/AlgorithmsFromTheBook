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
//linear space reverse sequence alignment just val
//divide and conquor linear space sequence aligmnent
///square root finder
///sine and cosine and tangernt finder
//when done with sequence aligmment remove the printing comments

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

	string x = "aaabbb";
	string y = "aabbb";

	auto p = sequenceAlignment(x, y, 2, &costFunc);

	std::cout << "\n\n\n\n\n\n" << spaceEfficientSequenceAligmment(x, y, 2, &costFunc);

	std::getchar();
}