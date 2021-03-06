#pragma once
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
#include "FlightStruct.h"
#include "Map.h"
#include "BitPacking.h"
#include "Interpreters.h"
#include "ArrayAlgorithms.h"
#include "Hash.h"

#include <vector>
#include <algorithm>
#include <assert.h>
#include <ctime>

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
extern "C" int asm_arithmetic_expression_calculator(char *);
extern "C" bool asm_havel_hakimi(int *, int);
extern "C" bool asm_sort_in_place(int *, int);

#pragma region ARRAY ALGORITHMS

void pairWithSumTester()
{
	std::vector<int> p{ 1, 2, 3, 4, 5, 6, 7, 8 };

	assert(pairWithSum(14, &(p[0]), p.size()));

	p[7] = 0;

	assert(!pairWithSum(14, &(p[0]), p.size()));

	p[7] = 7;

	assert(pairWithSum(14, &(p[0]), p.size()));
}

void zeroSumSubarrayTester()
{
	std::vector<int> v{ 4, 8, -3, -1, 4 };

	assert(zeroSumSubarray(&(v[0]), v.size()) == std::make_pair(2, 4));

	v[4] = 0;

	assert(zeroSumSubarray(&(v[0]), v.size()) == std::make_pair(4, 4));

	v[4] = 5;

	assert(zeroSumSubarray(&(v[0]), v.size()) == std::make_pair(-1, -1));
}

#pragma endregion

#pragma region CACHE TESTERS

void cacheTester()
{
	std::vector<char> vec = { 'a', 'b', 'c', 'd', 'a', 'd', 'e', 'a', 'd', 'b', 'c' };
	assert(cacher(vec, 3) == 4);
	std::vector<char> vec1 = { 'a', 'b', 'c', 'b', 'c', 'a', 'b' };
	assert(cacher(vec1, 2) == 2);
	assert(cacher(vec1, 3) == 0);
}

#pragma endregion

/*HASH*/

void hashTester() {
	uint32_t* ptr = SHA256("", 0);

	assert(ptr[0] == 0xe3b0c442);
	assert(ptr[1] == 0x98fc1c14);
	assert(ptr[2] == 0x9afbf4c8);
	assert(ptr[3] == 0x996fb924);
	assert(ptr[4] == 0x27ae41e4);
	assert(ptr[5] == 0x649b934c);
	assert(ptr[6] == 0xa495991b);
	assert(ptr[7] == 0x7852b855);

	delete[] ptr;
}

/*CLOSEST PAIR*/

void closestPairTester() 
{
	//make a huge random list and test if it gets the right answer with a brute force check
	std::vector<std::pair<int, int>> testVec(1000);
	for (int i = 0; i < 1000; i++)
	{
		testVec[i] = std::make_pair(rand() % 1000, rand() % 1000);
	}

	std::pair<unsigned int, unsigned int> a = closestPair(&testVec);
	std::pair<unsigned int, unsigned int> b = bruteForceClosestPair(&testVec);

	assert(dist(&testVec[a.first], &testVec[a.second]) == dist(&testVec[b.first], &testVec[b.second]));
}

/*CLUSTERING*/
/*DATA COMPRESSION*/

void huffmanTester() 
{
	assert(huffmanDecoder(huffmanEncoder("float like a butterfly sting like a bee")) == "float like a butterfly sting like a bee");
	assert(huffmanDecoder(huffmanEncoder("why be a king, when you could be a god")) == "why be a king, when you could be a god");
}

/*FAST FOURIER TRANSFORM*/
/*GEOMETRY*/

void rectIntersectsEllipseTester()
{
	//testing normal rect collision
	assert(rectIntersectsEllipse({ -1, -1 }, 2, 2, { -3, 0 }, 2, 3));
	assert(!rectIntersectsEllipse({ -1, -1 }, 2, 2, { -3, 0 }, 1.9, 3));

	//testing crossing of major axis collision
	assert(rectIntersectsEllipse({ -1, -1 }, 2, 2, { -2, -1 }, 2, 3));

	//testing corner detection
	assert(rectIntersectsEllipse({ -2, -2 }, 1, 1, { 0, 0 }, 2, 2));
}

void lineSegmentIntersectionTester() 
{
	assert(!lineSegmentIntersection({ 1, 1 }, { 10, 1 }, { 1, 2 }, { 10, 2 }));
	assert(!lineSegmentIntersection({ -5, -5 }, { 0, 0 }, { 1, 1 }, { 10, 10 }));
	assert(lineSegmentIntersection({ 10, 0 }, { 0, 10 }, { 0, 0 }, { 10, 10 }));
}

void pointInPolygonTester()
{
	std::pair<double, double> poly[3] = { {0, 0}, {1, 0}, {.5, 1} };

	assert(pointInPolygon({ .5, .5 }, &poly[0], 3));
	assert(!pointInPolygon({ 0, 1 }, &poly[0], 3));
	assert(pointInPolygon({ .5, 1 }, &poly[0], 3));
	assert(!pointInPolygon({ .5, 1.5 }, &poly[0], 3));
	assert(!pointInPolygon({ 1.5, .5 }, &poly[0], 3));
	assert(!pointInPolygon({ -.5, .5 }, &poly[0], 3));
}

void convexHullTester()
{
	std::pair<double, double> * pts = DBG_NEW std::pair<double, double>[5]{ std::make_pair(0.0, 0.0), std::make_pair(1.0, 0.0), std::make_pair(0.0, 1.0), std::make_pair(1.0, 1.0), std::make_pair(0.5, 0.5) };

	auto p = smallestConvexHull(pts, 5);

	assert(p[0] == std::make_pair(0.0, 0.0));
	assert(p[1] == std::make_pair(1.0, 0.0));
	assert(p[2] == std::make_pair(1.0, 1.0));
	assert(p[3] == std::make_pair(0.0, 1.0));

	delete[] pts;
}

/*GRAPH ALGORITHMSH*/

void connectivityTestTester()
{
	Graph::WeightedGraph<int> g;
	for (int i = 0; i < 6; i++) {
		g.addNode(i);
	}

	g.addEdge(0, 1, 1);
	g.addEdge(0, 2, 1);
	g.addEdge(0, 3, 5);
	g.addEdge(2, 1, 2);
	g.addEdge(2, 3, 2);
	g.addEdge(0, 4, 1);
	g.addEdge(0, 5, 3);
	g.addEdge(4, 5, 1);

	assert(breadthFirstConnectivityTest(&g));
	assert(depthFirstConnecivityTest(&g));

	g.removeEdge(0, 4);
	g.removeEdge(0, 5);

	assert(!breadthFirstConnectivityTest(&g));
	assert(!depthFirstConnecivityTest(&g));
}

void cycleTestTester()
{

	Graph::WeightedGraph<int> g;
	for (int i = 0; i < 6; i++)
	{
		g.addNode(i);
	}

	g.addEdge(0, 1, 1);
	g.addEdge(0, 2, 1);
	g.addEdge(0, 3, 5);
	g.addEdge(2, 1, 2);
	g.addEdge(2, 3, 2);
	g.addEdge(0, 4, 1);
	g.addEdge(0, 5, 3);
	g.addEdge(4, 5, 1);

	assert(depthFirstCycleTest(&g));

	//there are multiple cycles possible for this graph, and so I just hand checked that it was really a cycle, and it was
	std::vector<unsigned int> vec = depthFirstCycleGetter(&g);

	g.removeEdge(0, 4);
	g.removeEdge(0, 1);
	g.removeEdge(0, 3);

	assert(!depthFirstCycleTest(&g));
	assert(!depthFirstCycleGetter(&g).size());

}

void bipartiteTester()
{
	Graph::Graph<int> g;
	for (int i = 0; i < 7; i++) 
	{
		g.addNode(i);
	}
	g.addEdge(0, 4);
	g.addEdge(0, 5);
	g.addEdge(1, 5);
	g.addEdge(2, 5);
	g.addEdge(2, 6);
	g.addEdge(3, 5);

	assert(isBipartite(&g));

	g.addEdge(0, 1);

	assert(!isBipartite(&g));
}

void firstSearchesTester() 
{
	Graph::Graph<int> g;
	for (int i = 0; i < 6; i++)
	{
		g.addNode(i);
	}

	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(0, 3);
	g.addEdge(2, 1);
	g.addEdge(2, 3);
	g.addEdge(0, 4);
	g.addEdge(0, 5);
	g.addEdge(4, 5);

	//this path can really be anything, so it needs to be checked by hand
	XORLinkedList::LinkedList<int> list = depthFirstSearch(&g, 4, 3);

	std::string str = breadthFirstSearch(&g, 4, 3);
	assert(str == "403");//can only be one thing because it always finds optimal path

	g.removeEdge(4, 0);
	g.removeEdge(5, 4);

	XORLinkedList::LinkedList<int> x = depthFirstSearch(&g, 4, 3);
	assert(!x.size);

	str = breadthFirstSearch(&g, 4, 3);
	assert(str == "");
}

void dijkstraMinDistTester() 
{
	Graph::WeightedGraph<int> g;
	for (int i = 0; i < 6; i++) {
		g.addNode(i);
	}

	g.addEdge(0, 1, 1);
	g.addEdge(0, 2, 1);
	g.addEdge(0, 3, 5);
	g.addEdge(2, 1, 2);
	g.addEdge(2, 3, 2);
	g.addEdge(0, 4, 1);
	g.addEdge(0, 5, 3);
	g.addEdge(4, 5, 1);

	std::vector<unsigned int> dists = djikstraMinDist(&g, 0);
	assert(dists[0] == 0);
	assert(dists[1] == 1);
	assert(dists[2] == 1);
	assert(dists[3] == 3);
	assert(dists[4] == 1);
	assert(dists[5] == 2);
}

void dijkstraTester() 
{
	Graph::WeightedGraph<int> g;
	for (int i = 0; i < 6; i++) 
	{
		g.addNode(i);
	}

	g.addEdge(0, 1, 1);
	g.addEdge(0, 2, 1);
	g.addEdge(0, 3, 5);
	g.addEdge(2, 1, 2);
	g.addEdge(2, 3, 2);
	g.addEdge(0, 4, 1);
	g.addEdge(0, 5, 3);
	g.addEdge(4, 5, 1);

	XORLinkedList::LinkedList<unsigned int> path = djikstra(&g, 0, 0);

	assert(path.size == 1);
	assert(path.popFrontNode() == 0);

	XORLinkedList::LinkedList<unsigned int> path1 = djikstra(&g, 0, 3);

	assert(path1.size == 3);
	assert(path1.popFrontNode() == 0);
	assert(path1.popFrontNode() == 2);
	assert(path1.popFrontNode() == 3);
}

void bellmanFordTester() 
{
	Graph::WeightedDirectedGraph<char> g;
	g.addNode('a');
	g.addNode('b');
	g.addNode('c');
	g.addNode('d');
	g.addNode('e');
	g.addNode('f');
	//return;
	g.addEdge(0, 1, -4);
	g.addEdge(0, 5, -3);
	g.addEdge(1, 3, -1);
	g.addEdge(1, 4, -2);
	g.addEdge(2, 1, 8);
	g.addEdge(2, 5, 3);
	g.addEdge(3, 0, 6);
	g.addEdge(3, 5, 4);
	g.addEdge(4, 2, -3);
	g.addEdge(4, 5, 2);

	std::vector<unsigned int> * vec = bellmanFord(&g, 3, 5);

	assert((*vec)[0] == 5);
	assert((*vec)[1] == 2);
	assert((*vec)[2] == 4);
	assert((*vec)[3] == 1);
	assert((*vec)[4] == 0);
	assert((*vec)[5] == 3);

	delete vec;

	vec = bellmanFordVectorProtocol(&g, 3, 5);

	assert((*vec)[0] == 5);
	assert((*vec)[1] == 2);
	assert((*vec)[2] == 4);
	assert((*vec)[3] == 1);
	assert((*vec)[4] == 0);
	assert((*vec)[5] == 3);

	delete vec;

	vec = bellmanFordAsynchronous(&g, 3, 5);

	assert((*vec)[5] == 5);
	assert((*vec)[4] == 2);
	assert((*vec)[3] == 4);
	assert((*vec)[2] == 1);
	assert((*vec)[1] == 0);
	assert((*vec)[0] == 3);

	delete vec;
}

void negativeCycleTester()
{
	Graph::WeightedDirectedGraph<int> g;

	for (int i = 0; i < 6; i++)
	{
		g.addNode(i);
	}

	g.addEdge(0, 1, 2);
	g.addEdge(1, 2, 1);
	g.addEdge(2, 3, -1);
	g.addEdge(3, 4, 1);
	g.addEdge(4, 1, -2);
	g.addEdge(2, 5, 1);

	assert(negativeCycleDetector(&g));

	std::vector<unsigned int> * vec = negativeCycleGetter(&g);

	assert((*vec)[0] == 1);
	assert((*vec)[1] == 4);
	assert((*vec)[2] == 3);
	assert((*vec)[3] == 2);

	delete vec;

	g.removeEdge(4, 1);
	g.addEdge(4, 1, 2);

	assert(!negativeCycleDetector(&g));

	vec = negativeCycleGetter(&g);

	assert(!vec);
}

void minimumSpanningTreeTester() 
{
	Graph::WeightedGraph<int> g(9);

	for (int i = 0; i < 9; i++) g.addNode(i);

	g.addEdge(0, 1, 4);
	g.addEdge(0, 7, 8);
	g.addEdge(1, 2, 8);
	g.addEdge(1, 7, 11);
	g.addEdge(2, 3, 7);
	g.addEdge(2, 5, 4);
	g.addEdge(2, 8, 2);
	g.addEdge(3, 4, 9);
	g.addEdge(3, 5, 14);
	g.addEdge(4, 5, 10);
	g.addEdge(6, 5, 2);
	g.addEdge(6, 7, 1);
	g.addEdge(6, 8, 6);
	g.addEdge(7, 8, 7);

	Graph::WeightedGraph<int> * g1 = primMinTree(&g);

	assert(g1->size == 9);
	assert(g1->getTotalEdgesInGraph() == 8);
	assert(g1->hasChild(0, 1));
	assert(g1->hasChild(0, 7));
	assert(g1->hasChild(2, 8));
	assert(g1->hasChild(2, 3));
	assert(g1->hasChild(2, 5));
	assert(g1->hasChild(3, 4));
	assert(g1->hasChild(6, 5));
	assert(g1->hasChild(6, 7));

	delete g1;

	g1 = kruskalMinTree(&g);

	assert(g1->size == 9);
	assert(g1->getTotalEdgesInGraph() == 8);
	assert(g1->hasChild(0, 1));
	assert(g1->hasChild(0, 7));
	assert(g1->hasChild(2, 8));
	assert(g1->hasChild(2, 3));
	assert(g1->hasChild(2, 5));
	assert(g1->hasChild(3, 4));
	assert(g1->hasChild(6, 5));
	assert(g1->hasChild(6, 7));

	delete g1;

	g1 = kruskalMinTreePriorityQueue(&g);

	assert(g1->size == 9);
	assert(g1->getTotalEdgesInGraph() == 8);
	assert(g1->hasChild(0, 1));
	assert(g1->hasChild(0, 7));
	assert(g1->hasChild(2, 8));
	assert(g1->hasChild(2, 3));
	assert(g1->hasChild(2, 5));
	assert(g1->hasChild(3, 4));
	assert(g1->hasChild(6, 5));
	assert(g1->hasChild(6, 7));

	delete g1;

	Graph::WeightedGraph<int> g2 = reverseDeleteMinTree(g);

	assert(g2.size == 9);
	assert(g2.getTotalEdgesInGraph() == 8);
	assert(g2.hasChild(0, 1));
	assert(g2.hasChild(0, 7));
	assert(g2.hasChild(2, 8));
	assert(g2.hasChild(2, 3));
	assert(g2.hasChild(2, 5));
	assert(g2.hasChild(3, 4));
	assert(g2.hasChild(6, 5));
	assert(g2.hasChild(6, 7));
}

void edmondsTester() 
{
	Graph::WeightedDirectedGraph<int> g(7);

	for (int i = 0; i < 7; i++)
	{
		g.addNode(i);
	}

	g.addEdge(0, 3, 1);
	g.addEdge(0, 1, 2);
	g.addEdge(1, 3, 3);
	g.addEdge(1, 4, 10);
	g.addEdge(2, 0, 4);
	g.addEdge(2, 5, 5);
	g.addEdge(3, 2, 2);
	g.addEdge(3, 4, 2);
	g.addEdge(3, 6, 4);
	g.addEdge(3, 5, 8);
	g.addEdge(4, 6, 6);
	g.addEdge(6, 5, 1);

	Graph::WeightedDirectedGraph<int> * edmonds = edmondsAlgorithm(&g, 3);

	unsigned int totalWeight = 0;

	for (unsigned int i = 0; i < edmonds->size; i++)
	{
		for (unsigned int j = 0; j < edmonds->getEdgeNum(i); j++)
		{
			totalWeight += edmonds->getWeightOfEdgeByPos(i, j);
		}
	}

	assert(totalWeight == 15);

	delete edmonds;

	Graph::WeightedDirectedGraph<int> g2(6);

	for (int i = 0; i < 6; i++)
	{
		g2.addNode(i);
	}

	g2.addEdge(0, 1, 2);
	g2.addEdge(0, 2, 10);
	g2.addEdge(0, 3, 10);
	g2.addEdge(1, 3, 4);
	g2.addEdge(2, 1, 1);
	g2.addEdge(2, 5, 8);
	g2.addEdge(3, 4, 2);
	g2.addEdge(3, 5, 4);
	g2.addEdge(4, 2, 2);

	edmonds = edmondsAlgorithm(&g2, 0);

	totalWeight = 0;

	for (unsigned int i = 0; i < edmonds->size; i++)
	{
		for (unsigned int j = 0; j < edmonds->getEdgeNum(i); j++) 
		{
			totalWeight += edmonds->getWeightOfEdgeByPos(i, j);
		}
	}

	assert(totalWeight == 14);

	delete edmonds;
}

void isDAGTester()
{
	Graph::DirectedGraph<int> g;

	for (int i = 0; i < 6; i++)
	{
		g.addNode(i);
	}

	g.addEdge(5, 0);
	g.addEdge(5, 2);
	g.addEdge(4, 0);
	g.addEdge(4, 1);
	g.addEdge(2, 3);
	g.addEdge(3, 1);

	assert(isDAG(g));

	g.addEdge(0, 5);
	g.addEdge(1, 4);

	assert(!isDAG(g));
}

void topologicalSortTester()
{
	Graph::DirectedGraph<int> g;

	for (int i = 0; i < 6; i++)
	{
		g.addNode(i);
	}

	g.addEdge(5, 0);
	g.addEdge(5, 2);
	g.addEdge(4, 0);
	g.addEdge(4, 1);
	g.addEdge(2, 3);
	g.addEdge(3, 1);

	std::vector<unsigned int> * vec = topologicalSort(g);

	assert((*vec)[0] == 5 || (*vec)[0] == 4);
	assert((*vec)[1] == 5 || (*vec)[1] == 4);
	assert((*vec)[2] == 0 || (*vec)[2] == 2);
	assert((*vec)[3] == 0 || (*vec)[3] == 2);
	assert((*vec)[4] == 3);
	assert((*vec)[5] == 1);

	delete vec;

	g.addEdge(0, 5);
	g.addEdge(1, 4);

	vec = topologicalSort(g);

	assert(!vec);

	assert(!isDAG(g));
}

void vertexCoverTester() {
	for (unsigned int i = 0; i < 5; i++)
	{
		Graph::Graph<int> g(7);

		for (int i = 0; i < 7; i++)
		{
			g.addNode(i);
		}

		g.addEdge(0, 1);
		g.addEdge(0, 2);
		g.addEdge(1, 2);
		g.addEdge(1, 3);
		g.addEdge(1, 4);
		g.addEdge(2, 5);
		g.addEdge(2, 6);
		g.addEdge(3, 6);
		g.addEdge(4, 6);
		g.addEdge(5, 6);

		assert(hasKVertexCover(&g, i) == (i > 2));
	}
}

void independentSetTester()
{
	Graph::Graph<int> g(10);

	for (int i = 0; i < 5; g.addNode(i++));

	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(2, 3);
	g.addEdge(2, 4);
	
	assert(independentSetOnTree(g) == 3);

	g.addNode(5);

	assert(independentSetOnTree(g) == 4);

	g.addEdge(1, 5);

	assert(independentSetOnTree(g) == 4);

	Graph::WeightedGraph<int> k(10);

	for (int i = 0; i < 5; k.addNode(i++));

	k.addEdge(0, 1, 1);
	k.addEdge(0, 2, 3);
	k.addEdge(2, 3, 2);
	k.addEdge(2, 4, 3);

	Graph::WeightedTree<int> p(&k, 0);

	assert(weightedIndependentSetOnTree(&p, 0) == 6);
	assert(weightedIndependentSetOnTree(&p, 2) == 7);
}

/*INTERPRETERS*/
void arithmeticInterpreterTester()
{
	assert(arithmeticExpressionCalculator("1+2+3") == 6);
	assert(arithmeticExpressionCalculator("45") == 45);
	assert(arithmeticExpressionCalculator("2*12*3") == 72);
	assert(arithmeticExpressionCalculator("1+2*3") == 7);
	assert(arithmeticExpressionCalculator("4/2") == 2);
	assert(arithmeticExpressionCalculator(" 4 - 2 / 2") == 3);

	assert(asm_arithmetic_expression_calculator("1+2+3") == 6);
	assert(asm_arithmetic_expression_calculator("45") == 45);
	assert(asm_arithmetic_expression_calculator("2*12*3") == 72);
	assert(asm_arithmetic_expression_calculator("1+2*3") == 7);
	assert(asm_arithmetic_expression_calculator("4/2") == 2);
	assert(asm_arithmetic_expression_calculator(" 4 - 2 / 2") == 3);
}

/*INTERVAL SCHEDULING*/

void schedulingTester() 
{
	std::vector<std::pair<int, int>> tasks = { { 0, 80 },{ 0, 10 },{ 0, 20 },{ 15, 30 },{ 28, 32 },{ 40, 50 },{ 45, 60 },{ 80, 90 },{ 85, 95 } };
	assert(intervalScheduler(tasks).size() == 4);//the only thing that matters is the number of tasks scheduled, as there are many optimums
	assert(parallelScheduler(tasks).size() == 3);
	
	std::vector<std::pair<int, int>> tasks1 = { { 10, 20 },{ 10, 10 },{ 5, 2 },{ 15, 30 },{ 28, 32 },{ 40, 50 },{ 45, 60 },{ 80, 90 },{ 85, 95 } };

	std::vector<std::pair<int, int>> vec = minimalLatenessScheduler(tasks1);
	assert(std::is_sorted(vec.begin(), vec.end()));

	std::vector<weightedTask> x;
	x.reserve(6);

	x.push_back(weightedTask(0, 3, 2));
	x.push_back(weightedTask(1, 4, 4));
	x.push_back(weightedTask(3, 5, 4));
	x.push_back(weightedTask(2, 8, 7));
	x.push_back(weightedTask(7, 9, 2));
	x.push_back(weightedTask(7, 9, 1));

	std::string sched = weightedIntervalScheduler(&x);
	
	assert(sched == "420");
}

/*INVERSION COUNTER*/

void inversionCounterTester()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	assert(inversionCounter(vec) == 0);
	std::vector<int> vec1 = { 2, 4, 1, 3, 5 };
	assert(inversionCounter(vec1) == 3);
	std::vector<int> vec2 = { 5, 4, 3, 2, 1 };
	assert(inversionCounter(vec2) == 11);
}

/*KNAPSACK*/

/*NETWORK FLOW ALGORITHMS*/

void maxFlowTester() 
{
	Graph::WeightedDirectedGraph<int> g(4);
	g.addNode(0);
	g.addNode(1);
	g.addNode(2);
	g.addNode(3);

	g.addEdge(0, 1, 20);
	g.addEdge(0, 2, 10);
	g.addEdge(1, 2, 30);
	g.addEdge(1, 3, 10);
	g.addEdge(2, 3, 20);

	assert(fordFulkersonMaxFlow(g, 0, 3) == 30);
	assert(scalingMaxFlow(g, 0, 3) == 30);
	//assert(preflowPush(g, 0, 3) == 30);

	Graph::WeightedDirectedGraph<int> k(6);
	for (int i = 0; i < 6; i++) 
	{
		k.addNode(i);
	}
	k.addEdge(0, 1, 10);
	k.addEdge(0, 2, 10);
	k.addEdge(1, 2, 2);
	k.addEdge(1, 3, 4);
	k.addEdge(1, 4, 8);
	k.addEdge(2, 4, 9);
	k.addEdge(3, 5, 10);
	k.addEdge(4, 3, 6);
	k.addEdge(4, 5, 10);

	assert(fordFulkersonMaxFlow(k, 0, 5) == 19);
	assert(scalingMaxFlow(k, 0, 5) == 19);
	assert(preflowPush(k, 0, 5) == 19);
}

void minCutTester()
{
	Graph::WeightedDirectedGraph<int> g(4);
	g.addNode(0);
	g.addNode(1);
	g.addNode(2);
	g.addNode(3);

	g.addEdge(0, 1, 20);
	g.addEdge(0, 2, 10);
	g.addEdge(1, 2, 30);
	g.addEdge(1, 3, 10);
	g.addEdge(2, 3, 20);

	assert(fordFulkersonMinCut(g, 0, 3).size == 1);
	assert(fordFulkersonMinCut(g, 0, 3).head->obj == 0);


	Graph::WeightedDirectedGraph<int> k(6);
	for (int i = 0; i < 6; i++) 
	{
		k.addNode(i);
	}
	k.addEdge(0, 1, 10);
	k.addEdge(0, 2, 10);
	k.addEdge(1, 2, 2);
	k.addEdge(1, 3, 4);
	k.addEdge(1, 4, 8);
	k.addEdge(2, 4, 9);
	k.addEdge(3, 5, 10);
	k.addEdge(4, 3, 6);
	k.addEdge(4, 5, 10);

	SinglyLinkedList::LinkedList<unsigned int> e = fordFulkersonMinCut(k, 0, 5);

	assert(e.size == 2);
	assert(e.head->obj == 0);
	assert(e.tail->obj == 2);
}

void bipartiteMatchingTester() 
{
	Graph::BipartiteGraph<char> g(5);

	for (int i = 0; i < 10; i++)
	{
		g.addNode(i);
	}

	g.addEdge(0, 5);
	g.addEdge(0, 7);
	g.addEdge(1, 5);
	g.addEdge(1, 6);
	g.addEdge(1, 7);
	g.addEdge(1, 8);
	g.addEdge(2, 8);
	g.addEdge(3, 8);
	g.addEdge(4, 8);
	g.addEdge(4, 9);

	assert(maximumBipartiteMatchingNum(g) == 4);

	auto p = maximumBipartiteMatching(g);

	assert(p->size() == 4);
	
	//this is one of many valid matchings
	//just happens to be the one the program picks out
	assert((*p)[0].first == 0 && (*p)[0].second == 5);
	assert((*p)[1].first == 1 && (*p)[1].second == 6);
	assert((*p)[2].first == 2 && (*p)[2].second == 8);
	assert((*p)[3].first == 4 && (*p)[3].second == 9);

	assert(!bipartiteHasPerfectMatching(g));

	g.addEdge(3, 7);

	assert(bipartiteHasPerfectMatching(g));
}

void disjointPathsTester() 
{
	Graph::DirectedGraph<char> g(7);

	for (int i = 0; i < 7; i++)
	{
		g.addNode(i);
	}

	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(0, 3);
	g.addEdge(1, 4);
	g.addEdge(1, 5);
	g.addEdge(2, 5);
	g.addEdge(3, 2);
	g.addEdge(3, 6);
	g.addEdge(4, 6);
	g.addEdge(5, 6);
	g.addEdge(5, 3);

	assert(disjointPaths(g, 0, 6) == 3);

	Graph::Graph<char> k(7);

	for (int i = 0; i < 7; i++)
	{
		k.addNode(i);
	}

	k.addEdge(0, 1);
	k.addEdge(0, 2);
	k.addEdge(0, 3);
	k.addEdge(1, 4);
	k.addEdge(1, 5);
	k.addEdge(2, 5);
	k.addEdge(3, 2);
	k.addEdge(3, 6);
	k.addEdge(4, 6);
	k.addEdge(5, 6);
	k.addEdge(5, 3);

	assert(disjointPaths(k, 0, 6) == 3);
}

void circulationsTester()
{
	Graph::WeightedDirectedGraph<char> g(4);
	g.addNode(0);
	g.addNode(1);
	g.addNode(2);
	g.addNode(3);

	int x[] = { -3, -3, 2, 4 };

	g.addEdge(0, 1, 3);
	g.addEdge(0, 2, 3);
	g.addEdge(1, 2, 2);

	auto p = maximumCirculation(g, &x[0]);

	assert(!p);

	g.addEdge(1, 3, 2);
	g.addEdge(2, 3, 2);

	p = maximumCirculation(g, &x[0]);
	
	assert((*p->flows[0])[0] == 1);
	assert((*p->flows[0])[1] == 2);
	assert((*p->flows[1])[0] == 2);
	assert((*p->flows[1])[1] == 2);
	assert((*p->flows[2])[0] == 2);

	delete p;

	p = boundedMaximumCirculation(g, &x[0], 1);

	assert((*p->flows[0])[0] == 1);
	assert((*p->flows[0])[1] == 2);
	assert((*p->flows[1])[0] == 2);
	assert((*p->flows[1])[1] == 2);
	assert((*p->flows[2])[0] == 2);
	
	delete p;

	 int q[] = { -3, -3, 2, 4 };

	p = boundedMaximumCirculation(g, &q[0], 2);

	assert(!p);

	 int r[] = { -3, -3, 2, 4 };

	unsigned int y[] = { 2, 0, 0, 0, 0 };

	assert(!differentlyBoundedMaximumCirculation(g, &r[0], &y[0]));

	int s[] = { -3, -3, 2, 4 };

	unsigned int z[] = { 1, 0, 0, 0, 0 };

	assert(differentlyBoundedMaximumCirculation(g, &s[0], &z[0]));
}

void surveyTester()
{
	bool x[3][3] = { {1, 1, 0}, {1, 0, 1}, {0, 1, 1} };

	assert(surveyCanBeDesigned(3, 3, &x[0][0], 1, 3, 1, 3));
	assert(!surveyCanBeDesigned(3, 3, &x[0][0], 4, 3, 2, 3));
}

void airlinesTester()
{
	FlightStruct x[6] = { FlightStruct(0, 1, 6), FlightStruct(2, 3, 7),
						 FlightStruct(1, 4, 8), FlightStruct(2, 5, 11),
						 FlightStruct(5, 6, 14.25), FlightStruct(7, 6, 17) };

	float costs[8][8] = { {0, 1, 10000, 10000, 10000, 10000, 10000, 10000 },
						 { 1, 0, 10000, 10000, 3, 10000, 10000, 10000 },
						 { 10000, 10000, 0, 1, 10000, 3, 10000, 10000 },
						 { 10000, 10000, 1, 0, 10000, 10000, 10000, 10000 },
						 { 10000, 3, 10000, 10000, 0, 1, 10000, 1 },
						 { 10000, 10000, 3, 10000, 10000, 0, 1, 1 },
						 { 10000, 10000, 10000, 10000, 10000, 1, 0, 1 },
						 { 10000, 10000, 10000, 10000, 1, 1, 1, 0 } };

	assert(planesCanBeScheduled(&x[0], 6, &costs[0][0], 8, 3, 1));
	assert(planesCanBeScheduled(&x[0], 6, &costs[0][0], 8, 2, 1));
	assert(!planesCanBeScheduled(&x[0], 6, &costs[0][0], 8, 1, 1));
	assert(!planesCanBeScheduled(&x[0], 6, &costs[0][0], 8, 2, 3));
}

/*RNA*/

void rnaStructureTester() 
{
	//all that matters is the optimal size, it can pick whichever pairs it wants
	RNA * rna = DBG_NEW RNA("UGUACCGGUAGUACA");

	std::vector<std::pair<unsigned int, unsigned int>> * vec = rnaStructure(rna);
	assert(vec->size() == 5);

	delete rna;
	delete vec;

	rna = DBG_NEW RNA("CAGAUCGGCGAUACGAGCAUAGCAAUGCUAAGCGAGCUUAGCUGCA");
	vec = rnaStructure(rna);

	assert(vec->size() == 14);

	delete rna;
	delete vec;
}

#pragma region REGRESSION

//this is a sufficient test for squared error because it needs squared error to work
void segmentedLinearRegressionTester() 
{
	std::pair<int, int> x[9] = { { 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 2 },{ 4, 4 },{ 5, 2 },{ 6, 0 },{ 7, 0 },{ 8, 0 } };

	std::vector<std::pair<unsigned int, unsigned int>> * vec = segmentedLinearRegression(&x[0], 9, 3);

	assert(vec->size() == 4);

	delete vec;
}

#pragma endregion

/*SORTING ALGOS*/

void sortingtester()
{
	std::vector<int> toSort = { 3, 2, 4, 5, 6, 8, 7, 8, 7, 8, 9, 0, 9, 8, 9, 0, 9, 7 };

	std::vector<int> heap = heapSort(&toSort);

	assert(std::is_sorted(heap.begin(), heap.end()));

	asm_sort_in_place(&toSort[0], toSort.size());
	assert(std::is_sorted(toSort.begin(), toSort.end()));

	toSort = { 3, 2, 4, 5, 6, 8, 7, 8, 7, 8, 9, 0, 9, 8, 9, 0, 9, 7 };

	toSort = mergeSort(toSort);
	assert(std::is_sorted(toSort.begin(), toSort.end()));
}

/*STABLE MATCHING*/

void stableMatchingTester()
{
	unsigned int men[4][4] = { { 0, 1, 2, 3 },{ 2, 1, 3, 0 },{ 0, 2, 3, 1 },{ 3, 2, 0, 1 } };
	unsigned int women[4][4] = { { 3, 2, 0, 1 },{ 0, 2, 3, 1 },{ 0, 1, 2, 3 },{ 2, 1, 3, 0 } };

	XORLinkedList::LinkedList<unsigned int> menList;
	menList.pushBackNode((unsigned int)0);
	menList.pushBackNode(1);
	menList.pushBackNode(2);
	menList.pushBackNode(3);

	std::vector<std::pair<unsigned int, unsigned int>> match = stableMatch(menList, &men[0][0], &women[0][0]);

	assert(match.size() == 4);

	for (unsigned int i = 0; i < match.size(); i++)
	{
		//make sure that every element in the matching is one of the correct elements of the answer, but not necessarily in any order
		assert((match[i].first == 0 && match[i].second == 1) ||
			(match[i].first == 1 && match[i].second == 2) ||
			(match[i].first == 2 && match[i].second == 0) ||
			(match[i].first == 3 && match[i].second == 3));
	}
}

/*STRING ALGOS*/

void greatestCommonSubstringTester()
{
	assert(greatestCommonSubstring("abc", "arm") == "a");
	assert(greatestCommonSubstring("arm", "pine") == "");
	assert(greatestCommonSubstring("crab", "cra") == "cra");
}

unsigned int testingOnlyCostFunc(char a, char b) 
{
	if (a == b) return 0;
	return 3;
}

void sequenceAlignmentTester() 
{
	std::vector<std::pair<unsigned int, unsigned int>> * vec = sequenceAlignment("abc", "def", 1, testingOnlyCostFunc);

	assert(!vec->size());

	delete vec;

	vec = sequenceAlignment("abc", "acf", 2, testingOnlyCostFunc);

	assert(vec->size() == 2);
	assert((*vec)[0].first == 2 && (*vec)[0].second == 1);
	assert((*vec)[1].first == 0 && (*vec)[1].second == 0);

	delete vec;

	vec = backwardsSequenceAlignment("abc", "def", 1, testingOnlyCostFunc);

	assert(!vec->size());

	delete vec;

	vec = backwardsSequenceAlignment("abc", "acf", 2, testingOnlyCostFunc);

	assert(vec->size() == 2);
	assert((*vec)[0].first == 0 && (*vec)[0].second == 0);
	assert((*vec)[1].first == 2 && (*vec)[1].second == 1);

	delete vec;

	vec = spaceEfficientSequenceAlignment("abc", "def", 1, testingOnlyCostFunc);

	assert(!vec->size());

	delete vec;

	vec = spaceEfficientSequenceAlignment("abc", "acf", 2, testingOnlyCostFunc);

	assert(vec->size() == 2);
	assert((*vec)[0].first == 2 && (*vec)[0].second == 1);
	assert((*vec)[1].first == 0 && (*vec)[1].second == 0);

	delete vec;

}

/*UTILS*/
void checksumTester()
{
	char * p = DBG_NEW char[11]{ 0,4,2,1,0,0,0,0,5,2,6 };
	assert(asm_get_barcode_checksum(p) == 4);
	p[1]++;
	assert(asm_get_barcode_checksum(p) == 3);
	p[1] += 3;
	assert(asm_get_barcode_checksum(p) == 0);
	delete[] p;

	char * c = DBG_NEW char[10]{ 0,3,8,5,4,8,6,8,0,4 };
	assert(asm_check_ISBN(c));
	c[1]++;
	assert(!asm_check_ISBN(c));
	delete[] c;
}

void multiplicationTester() 
{
	assert(asm_multiply(4, 5) == 20);
	assert(asm_multiply(3, 5) == 15);
	assert(asm_multiply(4, 4) == 16);
	assert(asm_multiply(4, 6) == 24);
	assert(asm_multiply(6, 6) == 36);
}

void encryptionTester()
{
	char * test = DBG_NEW char[26]{"abcdefghijklmnopgrstuv"};
	char * encrypt = DBG_NEW char[26]{ "abcdefghijklmnopgrstuv" };
	asm_rot_encrypt(encrypt);
	asm_rot_decrypt(encrypt);
	for (unsigned int i = 0; i < 26; i++) 
	{
		assert(test[i] == encrypt[i]);
	}

	delete[] test;
	delete[] encrypt;
}

void wordFunnelTester() 
{
	//WORD FUNNEL IS CORRUPTING THE PTR 
	char * test = DBG_NEW char[6]{ 'l', 'e', 'a', 'v', 'e', 0 };
	char * test2 = DBG_NEW char[5]{ 'e', 'a', 'v', 'e', 0 };

	assert(asm_letters_needed(test, test2) != 1);

	delete[] test;
	delete[] test2;

	test = DBG_NEW char[6]{ 's', 'l', 'e', 'e', 't', 0 };
	test2 = DBG_NEW char[5]{ 'l', 'e', 't', 's', 0 };

	assert(asm_letters_needed(test, test2) == -1);

	delete[] test;
	delete[] test2;
}

void getPrimesTester() 
{
	assert(getPrimes(0) == 2);
	assert(getPrimes(3) == 7);
	assert(getPrimes(5) == 13);
}

void rngTester()
{
	for (int i = 0; i < 1000; i++)
	{

		int x = i;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;

		assert(x == asm_xorshift(i));
	}
}

void longestPalindromeTester() {
	char x[] = { 'a', 'b', 'a', 'b', 'a', 'b', 'a', 0 };

	std::string y = "abababa";
	std::string z = "acncacn";
	std::string raceCar = "racecar";
	std::string p = "abba";

	assert(asm_longest_palindromic_substring(x) == 7);
	assert(getLengthOfLongestPalindrome(&y) == 7);
	assert(getLengthOfLongestPalindrome(&z) == 5);
	assert(getLengthOfLongestPalindrome(&raceCar) == 7);
	assert(getLengthOfLongestPalindrome(&p) == 4);
}

void isEvenTester ()
{
	//this works, I ran the whole loop, but it makes the testAll function run forever, so I'm commenting it out
	for (int i = INT_MAX - 1; i != INT_MAX; i--)
	{
		assert(isEven(i) == !(i % 2));
	}

	//asm_even_odd is its own tester, and is an otherwise completely useless method
	asm_even_odd();
}

void factorialTester() 
{
	assert(factorial(4) == 24);
	assert(factorial(0) == 1);
	assert(factorial(1) == 1);
}

void distTester()
{

	std::pair<int, int> p = { 0, 0 };
	std::pair<int, int> p1 = { 5, 0 };
	std::pair < int, int > p2 = { -8, -3 };

	assert(dist(&p, &p) == 0);
	assert(dist(&p, &p1) == dist(&p1, &p) && dist(&p, &p1) == 5);
	assert(dist(&p1, &p2) == dist(&p2, &p1) && dist(&p2, &p1) == sqrt(178));
}

void exponentTester()
{
	assert(exponent(2, 10) == 1024);
	assert(exponent(18, 1) == 18);
	assert(exponent(3, 0) == 1);
	assert(exponent(-1, 8) == 1);
}

//c++ does not support local functions, so this needs to be defined above the tests
double testDeriv(double x) 
{ 
	return x * x; 
}

void derivativeTester() 
{
	for (int i = -8; i < 8; i++)
	{
		assert(derivative(testDeriv, i) == 2 * i);
	}
}

//c++ does not support local functions, so this needs to be defined above the tests
double testInt(double x)
{
	return 3 * x * x;
}

void integralTester()
{
	for (int i = -8; i < 8; i++)
	{
		//make sure its a very good estimate
		assert(abs(integral(testInt, 0, i, 20000) - (i*i*i)) < 0.1);
	}
}

//tests both gcd and djykstra gcd
void gcdTester()
{
	assert(gcd(10, 15) == 5);
	assert(gcd(35, 10) == 5);
	assert(gcd(31, 2) == 1);

	assert(dijkstraGcd(10, 15) == 5);
	assert(dijkstraGcd(35, 10) == 5);
	assert(dijkstraGcd(31, 2) == 1);
}

void linearRegressionTester() 
{
	std::pair<int, int> * x = DBG_NEW std::pair<int, int>[5];
	x[0] = { 0, 1 };
	x[1] = { 1, 3 }; 
	x[2] = { 2, 5 }; 
	x[3] = { 3, 7 };
	x[4] = { 4, 9 };

	for (int i = 2; i < 5; i++) 
	{
		assert(linearRegression(x, i) == std::make_pair(2.0, 1.0));
	}

	delete[] x;
}



/*TESTS*/
void runAllTests()
{
	//this is an out of order disorganized mess
	checksumTester();
	factorialTester();
	distTester();
	exponentTester();
	gcdTester();
	integralTester();
	linearRegressionTester();
	cacheTester();
	closestPairTester();
	inversionCounterTester();
	greatestCommonSubstringTester();
	stableMatchingTester();
	rnaStructureTester();
	dijkstraMinDistTester();
	dijkstraTester();
	connectivityTestTester();
	cycleTestTester();
	bellmanFordTester();
	sortingtester();
	bipartiteTester();
	huffmanTester();
	segmentedLinearRegressionTester();
	lineSegmentIntersectionTester();
	minimumSpanningTreeTester();
	isDAGTester();
	arithmeticInterpreterTester();
	topologicalSortTester();
	sequenceAlignmentTester();
	negativeCycleTester();
	firstSearchesTester();
	rectIntersectsEllipseTester();
	schedulingTester();
	pointInPolygonTester();
	//isEvenTester();
	edmondsTester();
	getPrimesTester();
	maxFlowTester();
	minCutTester();
	bipartiteMatchingTester();
	disjointPathsTester();
	circulationsTester();
	surveyTester();
	airlinesTester();
	vertexCoverTester();
	wordFunnelTester();
	rngTester();
	independentSetTester();
	multiplicationTester();
	encryptionTester();
	longestPalindromeTester();
	pairWithSumTester();
	zeroSumSubarrayTester();
	hashTester();
	convexHullTester();
}

//should honestly deprecate every function in here except run all tests but whatever
#pragma deprecated (testDeriv, testInt, testingOnlyCostFunc)