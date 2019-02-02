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
#include "Map.h"
#include "Tests.h"

//assembly externals, ignore the green squigglies
extern "C" void asm_even_odd();
extern "C" int asm_multiply(int, int);
extern "C" int asm_get_barcode_checksum(char *);
extern "C" int asm_check_ISBN(char *);
extern "C" void asm_rot_encrypt(char *);
extern "C" void asm_rot_decrypt(char *);
extern "C" int asm_xorshift(int);
extern "C" int asm_letters_needed(char *, char *);
extern "C" char asm_longest_palindromic_substring(char *);

//TODO
//write three network flow algorithms, they just got too tedius and boring (start on page 414/396)
//figure out whats going on with the template thing in the residual graph from bipartite graph constructor, and get rid of it for speed
///optimizaitons in segmented linear regression (266/284), calculate errors in constant time

using namespace std;

int main()
{
#ifdef _DEBUG
	//to track memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	clock_t startTime = clock();
	runAllTests();
	std::cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;

	std::function<int(int)> lambda = [](int x) { return x % 10;};
	Map::HashMap<int, int> map(lambda, 10);

	map.addKey(1, 1);
	map.addKey(0, 2);

	Map::HashMap<int, int> ma = std::move(map);

	std::cout << ma.getKey(1) << ma.getKey(2) << ma.getKey(12);

	std::getchar();
}
