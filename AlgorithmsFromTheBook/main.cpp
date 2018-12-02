//identifier poisoning so things cant be used outside of their namespaces
#pragma warning (error : 4995)

//for memory leak detection
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC  
#endif

//if its debug mode, take more info about memory leak line numbers, if its release then we dont care about it
#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
	#define DBG_NEW new
#endif

#include <stdlib.h>  
#include <crtdbg.h> 

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
#include "FlightStruct.h"
#include "Knapsack.h"
#include "Tests.h"

//TODO
//figure out whats going on with the template thing in the residual graph from bipartite graph constructor, and get rid of it for speed
///optimizaitons in segmented linear regression (266/284), calculate errors in constant time
//implement tests for A*, kmeans, kruskal cluster, knapsack packer, FFT

using namespace std;

int main() {
#ifdef _DEBUG
	//to track memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//for (int i = 0; i < 100; i++) {
	clock_t startTime = clock();
	runAllTests();
	std::cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;
	//}
	std::getchar();
}