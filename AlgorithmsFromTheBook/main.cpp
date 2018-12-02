//identifier poisoning so things cant be used outside of their namespaces
#pragma warning (error : 4995)

//for memory leak detection
#define _CRTDBG_MAP_ALLOC  

//if its debug mode, take more info about memory leak line numbers, if its release then we dont care about it
#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	Graph::ResidualGraph<std::pair<int, int>> y(x);
	//std::cout << dijkstraGcd(468, 24);

	int x = 0;

	for (int i = 0; i < 1; i++) {
		//clock_t startTime = clock();
		runAllTests();
		//std::cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;
	}
	{
		DoublyLinkedList::LinkedList<char> t;

		t.pushBackNode(1);
		t.pushForwardsNode(2);
		t.insertNode(3, 1);
	}
	std::getchar();
}