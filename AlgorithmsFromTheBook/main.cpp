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
//asynchronous bellman ford

using namespace std;

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

	//Graph::WeightedDirectedGraph<char> g;
	//g.addNode('a');
	//g.addNode('b');
	//g.addNode('c');
	//g.addNode('d');
	//g.addNode('e');
	//g.addNode('f');

	//g.addEdge(0, 1, -4);
	//g.addEdge(0, 5, -3);
	//g.addEdge(1, 3, -1);
	//g.addEdge(1, 4, -2);
	//g.addEdge(2, 1, 8);
	//g.addEdge(2, 5, 3);
	//g.addEdge(3, 0, 6);
	//g.addEdge(3, 5, 4);
	//g.addEdge(4, 2, -3);
	//g.addEdge(4, 5, 2);

	Graph::WeightedDirectedGraph<char> g;
	g.addNode('a');
	g.addNode('b');
	g.addNode('c');
	g.addNode('d');
	g.addNode('e');
	g.addNode('f');

	g.addEdge(0, 1, 2);
	g.addEdge(1, 2, 1);
	g.addEdge(2, 3, -1);
	g.addEdge(3, 4, 1);
	g.addEdge(4, 1, -2);
	g.addEdge(2, 5, 2);

	//auto gtrfd = bellmanFordVectorProtocol(&g, 3, 5);
	//auto gtfd = bellmanFordAsynchronous(&g, 3, 5);
	cout << negativCycleDetector(&g);
	std::getchar();
}