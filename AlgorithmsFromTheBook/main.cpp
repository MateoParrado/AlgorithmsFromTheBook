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
#include "NetworkFlowAlgorithms.h"
#include "Tests.h"

//TODO
//write move constructor for linked list
//edmonds karp algorithm
///optimizaitons in segmented linear regression (266/284), calculate errors in constant time
//implement tests for A*, kmeans, kruskal cluster, knapsack packer, FFT

using namespace std;

int main() {

//	Graph::ResidualGraph<std::pair<int, int>> y(x);

	//std::cout << dijkstraGcd(468, 24);
	//for (int i = 0; i < 100000; i++) {
	runAllTests();
	//}

	std::getchar();
}