//identifier poisoning so things cant be used outside of their namespaces
#pragma warning (error : 4995)

#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

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
//figure out whats going on with the template thing in the residual graph from bipartite graph constructor, and get rid of it for speed
///optimizaitons in segmented linear regression (266/284), calculate errors in constant time
//implement tests for A*, kmeans, kruskal cluster, knapsack packer, FFT

using namespace std;

int main() {

//	Graph::ResidualGraph<std::pair<int, int>> y(x);
	//clock_t startTime = clock();
	//std::cout << dijkstraGcd(468, 24);
	//for (int i = 0; i < 500; i++) {
	runAllTests();
	//}
	//std::cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;
	std::getchar();
}