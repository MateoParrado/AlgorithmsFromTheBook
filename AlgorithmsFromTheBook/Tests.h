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

#include <vector>
#include <algorithm>
#include <utility>

void factorialTester() {
	assert(factorial(4) == 24);
	assert(factorial(0) == 1);
	assert(factorial(1) == 1);
}

void distTester() {

	std::pair<int, int> p = { 0, 0 };
	std::pair<int, int> p1 = { 5, 0 };
	std::pair < int, int > p2 = { -8, -3 };

	assert(dist(&p, &p) == 0);
	assert(dist(&p, &p1) == dist(&p1, &p) && dist(&p, &p1) == 5);
	assert(dist(&p1, &p2) == dist(&p2, &p1) && dist(&p2, &p1) == sqrt(178));
}

void exponentTester() {
	assert(exponent(2, 10) == 1024);
	assert(exponent(18, 1) == 18);
	assert(exponent(3, 0) == 1);
	assert(exponent(-1, 8) == 1);
}

//c++ does not support local functions, so this needs to be defined above the tests
double testDeriv(double x) { 
	return x * x; 
}

void derivativeTester() {
	for (int i = -8; i < 8; i++) {
		assert(derivative(testDeriv, i) == 2 * i);
	}
}

//c++ does not support local functions, so this needs to be defined above the tests
double testInt(double x) {
	return 3 * x * x;
}

void integralTester() {
	for (int i = -8; i < 8; i++) {
		//make sure its a very good estimate
		assert(abs(integral(testInt, 0, i, 20000) - (i*i*i)) < 0.1 );
	}
}

//tests both gcd and djykstra gcd
void gcdTester() {
	assert(gcd(10, 15) == 5);
	assert(gcd(35, 10) == 5);
	assert(gcd(31, 2) == 1);

	assert(dijkstraGcd(10, 15) == 5);
	assert(dijkstraGcd(35, 10) == 5);
	assert(dijkstraGcd(31, 2) == 1);
}

void linearRegressionTester() {
	std::pair<int, int> * x = new std::pair<int, int>[5];
	x[0] = { 0, 1 };
	x[1] = { 1, 3 }; 
	x[2] = { 2, 5 }; 
	x[3] = { 3, 7 };
	x[4] = { 4, 9 };

	for (int i = 2; i < 5; i++) {
		assert(linearRegression(x, i) == std::make_pair(2.0, 1.0));
	}

	delete[] x;
}

void cacheTester() {
	std::vector<char> vec = { 'a', 'b', 'c', 'd', 'a', 'd', 'e', 'a', 'd', 'b', 'c' };
	assert(cacher(vec, 3) == 4);
	std::vector<char> vec1 = { 'a', 'b', 'c', 'b', 'c', 'a', 'b' };
	assert(cacher(vec1, 2) == 2);
	assert(cacher(vec1, 3) == 0);
}

void closestPairTester() {
	//make a huge random list and test if it gets the right answer with a brute force check
	std::vector<std::pair<int, int>> testVec(1000);
	for (int i = 0; i < 1000; i++) {
		testVec[i] = std::make_pair(rand() % 1000, rand() % 1000);
	}

	std::pair<unsigned int, unsigned int> a = closestPair(&testVec);
	std::pair<unsigned int, unsigned int> b = bruteForceClosestPair(&testVec);
	std::cout << a.first << " " << a.second << " " << b.first << " " << b.second;
	assert(dist(&testVec[a.first], &testVec[a.second]) == dist(&testVec[b.first], &testVec[b.second]));
}

void inversionCounterTester() {
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	assert(inversionCounter(vec) == 0);
	std::vector<int> vec1 = { 2, 4, 1, 3, 5 };
	assert(inversionCounter(vec1) == 3);
	std::vector<int> vec2 = { 5, 4, 3, 2, 1 };
	assert(inversionCounter(vec2) == 11);
}

void greatestCommonSubstringTester() {
	assert(greatestCommonSubstring("abc", "arm") == "a");
	assert(greatestCommonSubstring("arm", "pine") == "");
	assert(greatestCommonSubstring("crab", "cra") == "cra");
}

void stableMatchingTester() {
	unsigned int men[4][4] = { { 0, 1, 2, 3 },{ 2, 1, 3, 0 },{ 0, 2, 3, 1 },{ 3, 2, 0, 1 } };
	unsigned int women[4][4] = { { 3, 2, 0, 1 },{ 0, 2, 3, 1 },{ 0, 1, 2, 3 },{ 2, 1, 3, 0 } };

	SinglyLinkedList::LinkedList<unsigned int> menList;
	menList.pushBackNode((unsigned int)0);
	menList.pushBackNode(1);
	menList.pushBackNode(2);
	menList.pushBackNode(3);

	std::vector<std::pair<unsigned int, unsigned int>> match = stableMatch(menList, &men[0][0], &women[0][0]);

	assert(match.size() == menList.size);

	for (int i = 0; i < match.size(); i++) {
		//make sure that every element in the matching is one of the correct elements of the answer, but not necessarily in any order
		assert((match[i].first == 0 && match[i].second == 1) || 
			   (match[i].first == 1 && match[i].second == 2) || 
			   (match[i].first == 2 && match[i].second == 0) || 
			   (match[i].first == 3 && match[i].second == 3));
	}
}

void rnaStructureTester() {
	//all that matters is the optimal size, it can pick whichever pairs it wants
	RNA * rna = new RNA("UGUACCGGUAGUACA");

	std::vector<std::pair<unsigned int, unsigned int>> * vec = rnaStructure(rna);
	assert(vec->size() == 5);

	delete rna;
	delete vec;

	rna = new RNA("CAGAUCGGCGAUACGAGCAUAGCAAUGCUAAGCGAGCUUAGCUGCA");
	vec = rnaStructure(rna);

	assert(vec->size() == 14);

	delete rna;
	delete vec;
}

void dijkstraMinDistTester() {
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

void dijkstraTester() {
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

	SinglyLinkedList::LinkedList<unsigned int> path = djikstra(&g, 0, 0);

	assert(path.size == 1);
	assert(path.popFrontNode() == 0);

	SinglyLinkedList::LinkedList<unsigned int> path1 = djikstra(&g, 0, 3);

	assert(path1.size == 3);
	assert(path1.popFrontNode() == 0);
	assert(path1.popFrontNode() == 2);
	assert(path1.popFrontNode() == 3);
}

void connectivityTestTester() {
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

void cycleTestTester() {

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

	assert(depthFirstCycleTest(&g));

	//there are multiple cycles possible for this graph, and so I just hand checked that it was really a cycle, and it was
	std::vector<unsigned int> vec = depthFirstCycleGetter(&g);

	g.removeEdge(0, 4);
	g.removeEdge(0, 1);
	g.removeEdge(0, 3);

	assert(!depthFirstCycleTest(&g));
	assert(!depthFirstCycleGetter(&g).size());
	
}

void bellmanFordTester() {
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

void sortingtester() {
	std::vector<int> toSort = { 3, 2, 4, 5, 6, 8, 7, 8, 7, 8, 9, 0, 9, 8, 9, 0, 9, 7 };

	std::vector<int> heap = heapSort(&toSort);

	assert(std::is_sorted(heap.begin(), heap.end()));

	toSort = mergeSort(toSort);
	assert(std::is_sorted(toSort.begin(), toSort.end()));
}

void bipartiteTester() {
	Graph::Graph<int> g;
	for (int i = 0; i < 7; i++) {
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

void huffmanTester() {
	assert(huffmanDecoder(huffmanEncoder("float like a butterfly sting like a bee")) == "float like a butterfly sting like a bee");
	assert(huffmanDecoder(huffmanEncoder("why be a king, when you could be a god")) == "why be a king, when you could be a god");
}

//this is a sufficient test for squared error because it needs squared error to work
void segmentedLinearRegressionTester() {
	std::pair<int, int> x[9] = { {0, 0}, {1, 0}, {2, 0}, {3, 2}, {4, 4}, {5, 2}, {6, 0}, {7, 0}, {8, 0} };

	std::vector<std::pair<unsigned int, unsigned int>> * vec = segmentedLinearRegression(&x[0], 9, 3);

	assert(vec->size() == 4);

	delete vec;
}

void lineSegmentIntersectionTester() {
	assert(!lineSegmentIntersection({ 1, 1 }, { 10, 1 }, { 1, 2 }, { 10, 2 }));
	assert(!lineSegmentIntersection({ -5, -5 }, { 0, 0 }, { 1, 1 }, { 10, 10 }));
	assert(lineSegmentIntersection({ 10, 0 }, { 0, 10 }, { 0, 0 }, { 10, 10 }));
}

void minimumSpanningTreeTester() {
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
#pragma deprecated (testDeriv, testInt)