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
#include "Geometry.h"
#include "Tests.h"

//TODO
///edmonds algorithm
///optimizaitons in segmented linear regression (266/284), calculate errors in constant time
///point in polygon
///polygon intersection
///creating polygon from given set of points
///fix bugs in: closest pair
//				weighted interval scheduler
//implement tests for every algo (11 left)
///write reverse delete min tree in nlogn
///fix weirdo loop in weighted interval scheduler

using namespace std;

double calc(double r) {
	return 6.67 * pow(10, -11) * 8 * pow(10, 24) / r;
}

int main() {
	//Graph::WeightedGraph<std::pair<int, int>> x(6);
	//
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

	//std::cout << dijkstraGcd(468, 24);

	runAllTests();

	std::getchar();
}