#pragma once
#include "Graph.h"
#include "LinkedList.h"
#include "Heap.h"
#include "Utils.h"

#include <iostream>
#include <algorithm>
#include <string>

/*UTILITY ALGORITHMS*/

//useful for constuctive algorithms
template<class T>
T * copyWithoutEdges(T * g) {
	T * ret = new T(g->size);
	for (unsigned int i = 0; i < g->size; i++) {
		ret->addNode(g->nodes[i].obj);
	}

	return ret;
}


/*TRAVERSALS*/

template<class T>
//find minimum length path from node start to node end
std::string breadthFirstSearch(Graph::Graph<T> * g, int start, int end) {
	std::vector<bool> visited(g->size);//to not double count nodes
	visited[start] = true;

	SinglyLinkedList::LinkedList<std::string> ret;

	ret.pushBackNode(std::to_string(start));

	while (ret.size) {
		int nextCheck = ret.getVal(0)[ret.getVal(0).size() - 1] - '0';//0 is 48 in ascii for some reason

		for (int i = 0; i < g->getEdgeNum(nextCheck); i++) {//loop through nextCheck's children

			if (g->getOtherSideOfEdge(nextCheck, i) == end) {

				ret.getVal(0) += std::to_string(g->getOtherSideOfEdge(nextCheck, i));

				goto endLoop;
			}
			else if (!visited[g->getOtherSideOfEdge(nextCheck, i)]) {

				visited[g->getOtherSideOfEdge(nextCheck, i)] = true;

				std::string str;

				str.reserve(ret.getVal(0).size() + 1);

				for (int i = 0; i < ret.getVal(0).size(); i++) {
					str += ret.getVal(0)[i];
				} 
					
				str += std::to_string(g->getOtherSideOfEdge(nextCheck, i));

				ret.pushBackNode(str);
			}
		} ret.popFrontNode();
	}

	//this should only be called if the algorithm can't find a path between the points, otherwise the only way to exit the loop is a goto to endLoop
	return "";

endLoop:

	return ret.getVal(0) + std::to_string(end);
}

template<class T>
//find path between node start and node end using DFS
SinglyLinkedList::LinkedList<int> depthFirstSearch(Graph::Graph<T> * g, int start, int end) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[start] = true;

	SinglyLinkedList::LinkedList<int> ret;

	ret.pushBackNode(start);

	while (true) {
		//for easy lookup, nodes will be added at the end
		for (int i = 0; i < (*g).getEdgeNum(ret.getVal(0)); i++) {
			int temp = (*g).getOtherSideOfEdge(ret.getVal(0), i); //to not have to calculate it so much

			if (!visited[temp]) {
				visited[temp] = true;

				ret.pushForwardsNode(temp);

				goto edgeFound;
			}
		}
		ret.popFrontNode(); //if no edge is found as a child


	edgeFound:

		if (ret.head) {
			if (ret.getVal(0) == end) break;

			if (!ret.size) { return ret; }
		}
		else
			return ret;
	}

	return ret;
}

//Djikstras for finding distances from starting node N to every other node in the graph
template<class T>
std::vector<unsigned int> djikstraMinDist(Graph::WeightedGraph<T> * g, unsigned int startNode) {
	std::vector<unsigned int> minDists(g->size, -1);//minimum length of path to node n
	minDists[startNode] = 0;

	Heap::Heap<std::pair<unsigned int *, unsigned int>> heap(g->size / 2);//priority queue where smallest length path is at the front
	heap.insert({ &minDists[startNode], startNode });

	while (heap.size()) {
		//check the clsoest node
		int index = heap.popMin().second;

		//if the path from current node to its children is smaller than their current path, change their path length
		for (unsigned int i = 0; i < g->getEdgeNum(index); i++) {
			int otherSideOfEdge = g->getOtherSideOfEdge(index, i);

			int len = g->getWeightOfEdge(index, otherSideOfEdge) + minDists[index];

			if (minDists[otherSideOfEdge] == -1) {
				minDists[otherSideOfEdge] = len;
				heap.insert({ &minDists[otherSideOfEdge], otherSideOfEdge });
			}
			else if (len < minDists[otherSideOfEdge]) {
				minDists[otherSideOfEdge] = len;
				heap.reheapify();
			}
		}
	}

	return minDists;
}

template<class T>
//Djikstras pathfinding algorithm
SinglyLinkedList::LinkedList<unsigned int> djikstra(Graph::WeightedGraph<T> * g, unsigned int startNode, unsigned int endNode) {
	//edge case
	if (startNode == endNode) {
		SinglyLinkedList::LinkedList<unsigned int> l;
		l.pushBackNode((unsigned int)0);
		return l;
	}

	//the minimum cost of getting to any node, which is initialized at infinity
	std::vector<unsigned int> minDists(g->size, -1);
	minDists[startNode] = 0;

	std::vector<unsigned int> parents(g->size, -1);//the node leading to n in the shortest path to n

	//heap to make choosing the cheapest unvisited node cheaper
	Heap::Heap<std::pair<unsigned int *, unsigned int>> heap(g->size / 2);
	heap.insert({ &minDists[startNode], startNode });

	//used to make sure that nodes dont get checked after theyve been popped off the heap
	bool * noMoreChecking = new bool[g->size];
	for (unsigned int i = 0; i < g->size; i++) {
		noMoreChecking[i] = false;
	}

	//as long as there are unchecked nodes
	while (heap.size()) {
		//take the cheapest node
		int index = heap.popMin().second;

		noMoreChecking[index] = true;

		if (index == endNode) {
			break;//done
		}

		//check all edges coming off of the node we picked to see if they create a shorter path to that node
		for (unsigned int i = 0; i < g->getEdgeNum(index); i++) {
			int otherSideOfEdge = g->getOtherSideOfEdge(index, i);

			//make sure it hasnt been popped yet
			if (noMoreChecking[otherSideOfEdge]) continue;

			int len = g->getWeightOfEdge(index, otherSideOfEdge) + minDists[index];

			//if the node hasnt been visited yet do
			if (minDists[otherSideOfEdge] == -1) {
				minDists[otherSideOfEdge] = len;
				parents[otherSideOfEdge] = index;

				//add it into the heap
				heap.insert({ &minDists[otherSideOfEdge], otherSideOfEdge });
			}//if its already been check then just adjust its value
			else if (len < minDists[otherSideOfEdge]) {
				minDists[otherSideOfEdge] = len;
				parents[otherSideOfEdge] = index;
				heap.reheapify();
			}
		}
	}

	SinglyLinkedList::LinkedList<unsigned int> ret;

	ret.pushBackNode(endNode);

	//trace back through the array
	while (parents[ret.getVal(0)] != -1) {
		ret.pushForwardsNode(parents[ret.getVal(0)]);
	}

	return ret;
}

template<class T>
//A* informed search algorithm
//only works for pairs of integer types that represet coordinates, might change that later
SinglyLinkedList::LinkedList<unsigned int> aStar(Graph::Graph<std::pair<T, T>> * g, unsigned int start, unsigned int end) {
	struct heuristicScore {
		double f = 0;
		double g = 0;
		
		bool operator<(heuristicScore x) {
			return f + g < x.f + x.g;
		}
	};

	std::vector<bool> visited(g->size);
	visited[start] = true;

	std::vector<unsigned int> parents(g->size, -1);

	std::vector<heuristicScore> minDists(g->size);
	minDists[start].g = 0;
	minDists[start].f = dist(g->getObj(start), g->getObj(start));

	Heap::Heap<std::pair<heuristicScore *, unsigned int>> minHeap(g->size / 2);
	minHeap.insert({ &minDists[start], start });

	while (minHeap.size()) {
		std::pair<heuristicScore *, unsigned int > tempPair = minHeap.popMin();

		if (tempPair.second == end) break;

		for (unsigned int i = 0; i < g->getEdgeNum(tempPair.second); i++) {
			unsigned int otherSideOfEdge = g->getOtherSideOfEdge(tempPair.second, i);

			if (visited[otherSideOfEdge]) {//if its visited then check if it is easier to reach this way
				double possibleMin = dist(g->getObj(tempPair.second), g->getObj(otherSideOfEdge));

				if (minDists[otherSideOfEdge].g > possibleMin) {

					minDists[otherSideOfEdge].g = possibleMin;
					parents[otherSideOfEdge] = tempPair.second;

					minHeap.reheapify();
				}
			}
			else {//add to heap and update values
				minDists[otherSideOfEdge].g = minDists[i].g + dist(g->getObj(tempPair.second), g->getObj(otherSideOfEdge));
				minDists[otherSideOfEdge].f = dist(g->getObj(otherSideOfEdge), g->getObj(end));

				minHeap.insert({ &minDists[otherSideOfEdge], otherSideOfEdge });

				parents[otherSideOfEdge] = tempPair.second;

				visited[otherSideOfEdge] = true;
			}
		}
	}

	SinglyLinkedList::LinkedList<unsigned int> retList;
	retList.pushBackNode(end);

	while (retList[0] != start) {
		retList.pushForwardsNode(parents[retList[0]]);
	}

	return retList;
}

/*CONNECTIVITY TESTS*/

template<class T>
bool breadthFirstConnectivityTest(Graph::Graph<T> * g) {
	std::vector<bool> visited(g->size);

	SinglyLinkedList::LinkedList<int> nextToCheck;//zero init

	nextToCheck.pushBackNode(0);

	visited[0] = true;

	while (nextToCheck.size) {
		for (unsigned int i = 0; i < g->getEdgeNum(nextToCheck[0]); i++) {
			if (!visited[g->getOtherSideOfEdge(nextToCheck[0], i)]) {
				visited[g->getOtherSideOfEdge(nextToCheck[0], i)] = true;

				nextToCheck.pushBackNode(g->getOtherSideOfEdge(nextToCheck[0], i));
			}
		} nextToCheck.popFrontNode();
	}

	for (unsigned int i = 0; i < visited.size(); i++) {
		if (!visited[i]) return false;
	}

	return true;
}

template<class T>
//test if a graph is connected using DFS
bool depthFirstConnecivityTest(Graph::Graph<T> * g) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[0] = true;

	SinglyLinkedList::LinkedList<int> path;

	path.pushBackNode(0);

	while (path.size) {
		bool newNodeFound = false;

		//for easy lookup, nodes will be added at the end
		for (int i = 0; i < (*g).getEdgeNum(path.getVal(0)); i++) {
			int temp = (*g).getOtherSideOfEdge(path.getVal(0), i); //to not have to calculate it so much

			if (!visited[temp]) {
				visited[temp] = true;
				newNodeFound = true;

				path.pushForwardsNode(temp);
				break;
			}
		}
		if(!newNodeFound) path.popFrontNode(); //if no edge is found as a child

	}
	if (std::all_of(visited.begin(), visited.end(), [](bool i) { return i;})) {
		return true;
	} 
	return false;
}

/*CYCLE CHECKING*/

template<class T>
//test if a graph contains a cylce using DFS
bool depthFirstCycleTest(Graph::Graph<T> * g) {
	std::vector<bool> visited((*g).size);//to not double count nodes

	std::vector<unsigned int> nextCheck(g->size);//so that when a parents child gets popped it restarts its search where it should

	SinglyLinkedList::LinkedList<int> path;

	path.pushBackNode(0);

tryTheRest://to check every connected component for loops

	visited[path.getVal(0)] = true;

	while (path.size) {
		bool newNodeFound = false;

		//for easy lookup, nodes will be added at the end
		for (unsigned int i = nextCheck[path.getVal(0)]; i < (*g).getEdgeNum(path.getVal(0)); i++) {
			nextCheck[path.getVal(0)]++;

			int temp = (*g).getOtherSideOfEdge(path.getVal(0), i); //to not have to calculate it so much

			if (path.size > 1 && visited[temp] && temp != path.getVal(1)) {
				return true;
			}
			if (!visited[temp]) {
				visited[temp] = true;
				newNodeFound = true;

				path.pushForwardsNode(temp);
				break;
			}
		}
		if (!newNodeFound) path.popFrontNode(); //if no edge is found as a child

	}

	auto temp = std::find(visited.begin(), visited.end(), false);
	if (temp != visited.end()) {
		path.pushBackNode(temp - visited.begin());
		goto tryTheRest;
	}

	return false;
}

template<class T>
//returns any randkm cycle from a graph
std::vector<unsigned int> depthFirstCycleGetter(Graph::Graph<T> * g) {
	std::vector<bool> visited((*g).size);//to not double count nodes

	std::vector<unsigned int> nextCheck(g->size);//so that when a parents child gets popped it restarts its search where it should

	SinglyLinkedList::LinkedList<int> path;

	path.pushBackNode(0);

tryTheRest://to check every connected component for loops

	visited[path.getVal(0)] = true;

	while (path.size) {
		bool newNodeFound = false;

		//for easy lookup, nodes will be added at the end
		for (unsigned int i = nextCheck[path.getVal(0)]; i < (*g).getEdgeNum(path.getVal(0)); i++) {
			nextCheck[path.getVal(0)]++;

			int temp = (*g).getOtherSideOfEdge(path.getVal(0), i); //to not have to calculate it so much

			if (path.size > 1 && visited[temp] && temp != path.getVal(1)) {
				std::vector<unsigned int> ret;
				ret.reserve(path.size);

				SinglyLinkedList::Node<T> * curPtr = path.head;

				while(curPtr) {
					ret.push_back(curPtr->obj);

					curPtr = curPtr->next;
				}

				return ret;
			}
			if (!visited[temp]) {
				visited[temp] = true;
				newNodeFound = true;

				path.pushForwardsNode(temp);
				break;
			}
		}
		if (!newNodeFound) path.popFrontNode(); //if no edge is found as a child

	}

	auto temp = std::find(visited.begin(), visited.end(), false);
	if (temp != visited.end()) {
		path.pushBackNode(temp - visited.begin());
		goto tryTheRest;
	}

	//the no cycle case
	std::vector<unsigned int> retVec(0);

	return retVec;
}

template<class T>
//test if a graph is bipartite by finding any odd cycles
bool isBipartite(Graph::Graph<T> * g) {
	std::vector<bool> visited((*g).size);//to not double count nodes

	std::vector<unsigned int> nextCheck(g->size);//so that when a parents child gets popped it restarts its search where it should

	SinglyLinkedList::LinkedList<int> path;

	path.pushBackNode(0);

tryTheRest://to check every connected component for loops

	visited[path.getVal(0)] = true;

	while (path.size) {
		bool newNodeFound = false;

		//for easy lookup, nodes will be added at the end
		for (unsigned int i = nextCheck[path.getVal(0)]; i < (*g).getEdgeNum(path.getVal(0)); i++) {
			nextCheck[path.getVal(0)]++;

			int temp = (*g).getOtherSideOfEdge(path.getVal(0), i); //to not have to calculate it so much

			if (path.size > 1 && visited[temp] && temp != path.getVal(1)) {
				if (path.size % 2) {
					return false;
				}
			}
			else if (!visited[temp]) {
				visited[temp] = true;
				newNodeFound = true;

				path.pushForwardsNode(temp);
			}
		}
		if (!newNodeFound) path.popFrontNode(); //if no edge is found as a child

	}

	auto temp = std::find(visited.begin(), visited.end(), false);
	if (temp != visited.end()) {
		path.pushBackNode(temp - visited.begin());
		goto tryTheRest;
	}

	return true;
}

template<class T>
//test if a directed graph is acyclic
//if you want to pass by copy, then you have to remove every node from the graph before you return a value
bool isDAG(Graph::DirectedGraph<T> g) {//object pointed to is destroyed by deletions
	while (g.size) {
		bool batman = false;//has no parents

		for (unsigned int i = g.size - 1; i < g.size; i--) {
			if (!g.getParentNum(i)) {
				g.removeNode(i);
				batman = true; 
			}
		}
		if (!batman) {
			return false;
		}
	}

	return true;
}

template<class T>
//returns a sorted vector where every element of the vector has no parents in the graph that come before it
std::vector<unsigned int> * topologicalSort(Graph::DirectedGraph<T> g) {
	std::vector<unsigned int> * retVec = new std::vector<unsigned int>;
	retVec->reserve(g.size);

	while(g.size) {
		bool batman = false;

		for (unsigned int i = g.size - 1; i < g.size; i--) {
			if (!g.getParentNum(i)) {
				retVec->push_back(g.nodes[i].obj);

				g.removeNode(i);
				batman = true;
			}
		}

		if (!batman) {
			delete retVec;

			return nullptr;
		}
	}

	return retVec;
}

/*MINIMUM SPANNING TREE*/

////build a minimum spanning tree by adding in the cheapest nodes that dont create a cycle
//slow, better version is written below, but i kept it because i like it
template<class T>
Graph::WeightedGraph<T> * kruskalMinTreePriorityQueue(Graph::WeightedGraph<T> * g) {
	Heap::Heap<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>> weightHeap(g->getTotalEdgesInGraph());//the orderings of the weights
																												 
	//initialize heap
	for (int i = 0; i < g->edges.size(); i++) {
		for (int j = 0; j < g->edges[i]->size(); j += 2) {
			auto test = (*g->edges[i])[j];
			if ((*g->edges[i])[j] > i) {
				weightHeap.insert({ (*g->edges[i])[j + 1] ,{ i, (*g->edges[i])[j] } });
			}
		}
	}

	Graph::WeightedGraph<T> * ret = copyWithoutEdges(g);

	while (weightHeap.size()) {
		ret->addEdge(weightHeap.getMin().second.first, weightHeap.getMin().second.second, weightHeap.getMin().first);
		if (depthFirstCycleTest(ret)) {
			ret->removeEdge(weightHeap.getMin().second.first, weightHeap.getMin().second.second);
		}

		weightHeap.popMin();
	}

	return ret;
}

template<class T>
//same as function above but uses a union find data structure instead of a priority queue
Graph::WeightedGraph<T> * kruskalMinTree(Graph::WeightedGraph<T> * g) {
	//the "nodes" for the union find
	struct unionPtr {
		unsigned int num;
		unionPtr * ptr;

		unionPtr(unsigned int val) : num(val), ptr(nullptr) { }
	};

	//initialize union find
	std::vector < unionPtr > unionFind;
	unionFind.reserve(g->size);
	for (int i = 0; i < g->size; i++) {
		unionFind.push_back(i);
	}

	//want to create the same priotity queue as above
	Heap::Heap<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>> weightHeap(g->getTotalEdgesInGraph());//the orderings of the weights

	//initialize heap
	for (int i = 0; i < g->edges.size(); i++) {
		for (int j = 0; j < g->edges[i]->size(); j += 2) {
			auto test = (*g->edges[i])[j];
			if ((*g->edges[i])[j] > i) {
				weightHeap.insert({ (*g->edges[i])[j + 1] ,{ i, (*g->edges[i])[j] } });
			}
		}
	}

	Graph::WeightedGraph<T> * ret = copyWithoutEdges(g);

	while (weightHeap.size()) { 
		unionPtr * firstPtr = unionFind[weightHeap.getMin().second.first].ptr;
		unionPtr * secondPtr = unionFind[weightHeap.getMin().second.second].ptr;

		//if either of them are null then just set them to point to themselves as they are their own root
		if (!firstPtr) {
			firstPtr = &unionFind[weightHeap.getMin().second.first];
		}
		if (!secondPtr) {
			secondPtr = &unionFind[weightHeap.getMin().second.second];
		}

		//find their roots
		while (firstPtr->ptr) {
			firstPtr = firstPtr->ptr;
		}
		while (secondPtr->ptr) {
			secondPtr = secondPtr->ptr;
		}

		//if roots are different then add join the groups
		if (firstPtr != secondPtr) {
			ret->addEdge(weightHeap.getMin().second.first, weightHeap.getMin().second.second, weightHeap.getMin().first);

			firstPtr->ptr = secondPtr;
		}

		weightHeap.popMin();
	}

	return ret;
}

////build a minimum spanning tree by adding in th echeapest nodes that dont create a cycle
template<class T>
Graph::WeightedGraph<T> reverseDeleteMinTree(Graph::WeightedGraph<T> g) {
	Heap::MaxHeap<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>> weightHeap(g.getTotalEdgesInGraph());//the orderings of the weights

	//initialize heap
	for (int i = 0; i < g.edges.size(); i++) {
		for (int j = 0; j < g.edges[i]->size(); j += 2) {
			auto test = (*g.edges[i])[j];
			if ((*g.edges[i])[j] > i) {
				weightHeap.insert({ (*g.edges[i])[j + 1] ,{ i, (*g.edges[i])[j] } });
			}
		}
	}

	while (weightHeap.size()) {
		g.removeEdge(weightHeap.getMin().second.first, weightHeap.getMin().second.second);

		if (!depthFirstConnecivityTest(&g)){
			g.addEdge(weightHeap.getMin().second.first, weightHeap.getMin().second.second, weightHeap.getMin().first);
		}

		weightHeap.popMin();
	}

	return g;
}

template<class T>
//creates min tree by adding the edge with least cost onto its tree repeatedly
Graph::WeightedGraph<T> * primMinTree(Graph::WeightedGraph<T> * g) {
	std::vector<bool> visited(g->size);

	Heap::Heap<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>> distHeap(g->size);//estimate size

	visited[0] = true;

	for (int i = 0; i < g->getEdgeNum(0); i++) {
		unsigned int index = g->getOtherSideOfEdge(0, i);
		unsigned int weight = g->getWeightOfEdge(0, index);

		distHeap.insert({ weight, {0, index} });
	}

	Graph::WeightedGraph<T> * ret = copyWithoutEdges(g);

	while (distHeap.size()) {
		ret->addEdge(distHeap.getMin().second.first, distHeap.getMin().second.second, distHeap.getMin().first);

		if (depthFirstCycleTest(ret)) {
			ret->removeEdge(distHeap.getMin().second.first, distHeap.getMin().second.second);
			distHeap.popMin();
			goto cycleFound;
		}

		unsigned int newNode = (visited[distHeap.getMin().second.first]) ? distHeap.getMin().second.second : distHeap.getMin().second.first;

		distHeap.popMin();

		visited[newNode] = true;

		for (unsigned int i = 0; i < g->getEdgeNum(newNode); i++) {
			unsigned int otherSideOfEdge = g->getOtherSideOfEdge(newNode, i);

			if(visited[otherSideOfEdge]) continue;

			distHeap.insert({ g->getWeightOfEdge(newNode, otherSideOfEdge), {otherSideOfEdge, newNode} });
		}

	cycleFound:;

	}

	return ret;
}

//
//template<class T>
////finds the minimum cost arborescence for a weighted directed graph
////DOES NOT WORK
//Graph::WeightedDirectedGraph<T> * edmondsAlgorithm(Graph::WeightedDirectedGraph<T> * graph, unsigned int rootNode = 0) {
//	Graph::WeightedDirectedGraph<T> * retGraph = copyWithoutEdges(graph);
//
//	//add the minimum cost edge into every vertex v except the root vertex
//	for (unsigned int i = 0; i < retGraph->size; i++) {
//		if (i == rootNode) continue;
//
//		unsigned int min = -1;
//		unsigned int minParent = -1;
//
//		//add the cheapest entrance edge into v into retGraph
//		for (unsigned int j = 0; j < graph->parents[i]->size(); j += 2) {
//			if ((*graph->parents[i])[j + 1] < min) {
//				min = (*graph->parents[i])[j + 1];
//				minParent = (*graph->parents[i])[j];
//			}
//		}
//
//		if (min != -1) {
//			retGraph->addEdge(minParent, i, min);
//		}
//	}
//
//	Graph::WeightedDirectedGraph<T> tempGraph = *retGraph;//because isDAG destroys the copy provided to it
//
//	retGraph->display();
//
//	if (isDAG(&tempGraph)) {
//		return retGraph;
//	}
//	else {//contract the cycle into one node
//		Graph::WeightedDirectedGraph<T> * nextLevelGraph = new Graph::WeightedDirectedGraph<T>(*graph);
//
//		std::vector<unsigned int> cycle = depthFirstCycleGetter(retGraph);
//
//		unsigned int contractedNode = cycle[0];
//
//		nextLevelGraph->display();
//
//		//make any edges coming into the graph point to node 0 and update their weights
//		for (unsigned int i = 1; i < cycle.size(); i++) {
//			for (unsigned int j = 0; j < graph->getParentNum(cycle[i]); j++) {
//				nextLevelGraph->addEdge(graph->getParent(cycle[i], j), cycle[0], graph->getWeightOfEdge(graph->getParent(cycle[i], j), cycle[i]) - graph->getWeightOfEdgeByPos(cycle[i], 0));
//			}
//		}
//
//		nextLevelGraph->display();
//
//		//make any edges leaving the cycle be rooted at node 0
//		for (unsigned int i = 1; i < cycle.size(); i++) {
//			for (unsigned int j = 0; j < graph->getChildNum(cycle[i]); j += 2) {
//				if (!graph->getChild(cycle[i], j) == cycle[0]) {
//					nextLevelGraph->addEdge(cycle[0], graph->getChild(cycle[i], j), graph->getWeightOfEdge(cycle[i], j));
//				}
//			}
//		}
//
//		nextLevelGraph->display();
//
//		//delete all the non contacted nodes
//		for (unsigned int i = 1; i < cycle.size(); i++) {
//			nextLevelGraph->removeNode(cycle[i]);
//		}
//
//		Graph::WeightedDirectedGraph<T> * returnedGraph = edmondsAlgorithm(nextLevelGraph, rootNode);
//
//		//unroll the cycle
//		for (unsigned int i = 1; i < cycle.size(); i++) {
//			returnedGraph->addNode(cycle[i]);
//			returnedGraph->addEdge(cycle[i], cycle[i - 1], graph->getWeightOfEdge(cycle[i], cycle[i-1]));
//		}
//		returnedGraph->addEdge(cycle[0], cycle[cycle.size() - 1], graph->getWeightOfEdge(cycle[0], cycle[cycle.size() - 1]));
//
//		std::cout << "NEW ITERATION \n\n\n";
//		returnedGraph->display();
//		std::cout << "\n\n";
//		returnedGraph->displayParents();
//
//		return returnedGraph;
//	}
//
//	return retGraph;
//}

template<class T>
//optimal pathfinding with negative edge weights
//graph cannot contain negative cycles
std::vector<unsigned int> * bellmanFord(Graph::WeightedDirectedGraph<T> * g, unsigned int start, unsigned int end) {
	//array of the minimum cost of getting to every node
	int * opt = new int[g->size];
	//array of the node leading to every node(in the path 1->2->3 prev[3] is 2 and prev[2] is one)
	unsigned int * prev = new unsigned int[g->size];

	//initialize infinite initial costs for each node
	for (unsigned int i = 0; i < g->size; i++) {
		opt[i] = INT_MAX / 2;//to prevent overfows
		prev[i] = -1;
	}

	//path from start to start is zero
	opt[start] = 0;

	//will find the solution in at most n - 1 iterations
	for (unsigned int k = 0; k < g->size - 1; k++) {
		//check every edge in g
		for (unsigned int i = 0; i < g->size; i++) {
			//for each edge pointing at vertex i
			for (unsigned int j = 0; j < g->getParentNum(i); j++) {
				unsigned int parent = g->getParent(i, j);

				//cast to an int because weights are stored as unsigned ints, returns the sign
				int tempCost = opt[parent] + (int)g->getWeightOfEdge(parent, i);

				//if this is a cheaper path, update the values of opt and prev
				if (tempCost < opt[i]) {
					opt[i] = tempCost;
					prev[i] = parent;
				}
			}
		}
	}

	std::vector<unsigned int> * retPath = new std::vector<unsigned int>;
	retPath->reserve(g->size);

	//trace back through the optimal array
	while (end != start) {
		retPath->push_back(end);
		end = prev[end];
	}
	retPath->push_back(start);

	delete[] opt;
	delete[] prev;

	return retPath;
}

template<class T>
//pathfinding in graphs with potentially negative edge weights
//must be a graph without any negative cycles
//improved version of bellman ford
std::vector<unsigned int> * bellmanFordVectorProtocol(Graph::WeightedDirectedGraph<T> * g, unsigned int start, unsigned int end) {
	int * opt = new int[g->size];
	unsigned int * prev = new unsigned int[g->size];

	//tracks wether its value has been changed, and therefore wether it needs to be recalculated
	bool * changed = new bool[g->size];

	for (unsigned int i = 0; i < g->size; i++) {
		opt[i] = INT_MAX / 2;
		prev[i] = -1;
		changed[i] = 1;
	}

	opt[start] = 0;

	//used to check if iteration terminated early
	bool somethingChanged = false;

	//must be rechecked at least n - 1 times 
	for (unsigned int k = 0; k < g->size - 1; k++) {
		//used to check every edge in g

		for (unsigned int i = 0; i < g->size; i++) {
			//for each edge pointing at vertex i
			bool changedThisIter = false;

			for (unsigned int j = 0; j < g->getParentNum(i); j++) {
				unsigned int parent = g->getParent(i, j);

				//cast to an int because weights are stored as unsigned ints, returns the sign
				int tempCost = opt[parent] + (int)g->getWeightOfEdge(parent, i);

				if (tempCost < opt[i]) {
					opt[i] = tempCost;
					prev[i] = parent;
					somethingChanged = true;
					changedThisIter = true;
				}
			}

			if (!changedThisIter) changed[i] = false;
		}
		if (!somethingChanged) break;
	}

	std::vector<unsigned int> * retPath = new std::vector<unsigned int>;
	retPath->reserve(g->size);

	while (end != start) {
		retPath->push_back(end);
		end = prev[end];
	}
	retPath->push_back(start);

	delete[] opt;
	delete[] prev;
	delete[] changed;

	return retPath;
}

template<class T>
//pathfinding in graphs with potentially negative edge weights
//must be a graph without any negative cycles
//improved version of bellman ford
std::vector<unsigned int> * bellmanFordAsynchronous(Graph::WeightedDirectedGraph<T> * g, unsigned int start, unsigned int end) {
	int * opt = new int[g->size];
	unsigned int * prev = new unsigned int[g->size];

	//tracks wether its value has been changed, and therefore wether it needs to be recalculated
	bool * active = new bool[g->size];

	for (unsigned int i = 0; i < g->size; i++) {
		opt[i] = INT_MAX / 2;
		prev[i] = -1;
		active[i] = false;
	}

	opt[end] = 0;
	active[end] = true;

	//keep going until all inactive
	for(;;) {
		
		bool oneActive = false;

		//used to check every edge in g
		for (unsigned int i = 0; i < g->size; i++) {
			//for each edge pointing at vertex i
			if (active[i]) {
				for (unsigned int j = 0; j < g->getParentNum(i); j++) {
					unsigned int parent = g->getParent(i, j);

					//cast to an int because weights are stored as unsigned ints, returns the sign
					int tempCost = opt[i] + (int)g->getWeightOfEdge(parent, i);

					if (tempCost < opt[parent]) {
						opt[parent] = tempCost;
						prev[parent] = i;
						active[parent] = true;
						oneActive = true;
					}
				}

				active[i] = false;
			}
		}
		if (!oneActive) break;
	}

	std::vector<unsigned int> * retPath = new std::vector<unsigned int>;
	retPath->reserve(g->size);

	while (end != start) {
		retPath->push_back(start);
		start = prev[start];
	}
	retPath->push_back(end);

	delete[] opt;
	delete[] prev;
	delete[] active;

	return retPath;
}

template<class T>
//returns wether there is a negative cycle in the graph or not
bool negativeCycleDetector(Graph::WeightedDirectedGraph<T> * g) {
	int * opt = new int[g->size];

	for (unsigned int i = 0; i < g->size; i++) {
		opt[i] = INT_MAX / 2;
	}

	opt[0] = 0;

	//must be rechecked at least n - 1 times 
	for (unsigned int k = 0; k < g->size; k++) {
		//used to check every edge in g

		for (unsigned int i = 0; i < g->size; i++) {
			//for each edge pointing at vertex i
			for (unsigned int j = 0; j < g->getParentNum(i); j++) {
				unsigned int parent = g->getParent(i, j);

				//cast to an int because weights are stored as unsigned ints, returns the sign
				int tempCost = opt[parent] + (int)g->getWeightOfEdge(parent, i);

				if (tempCost < opt[i]) {
					//if it gets updated after n - 1 iterations then there must be a negative cycle because any graph without one would already have converged
					if (k == g->size - 1) {
						delete[] opt;
						return true;
					}

					opt[i] = tempCost;
				}
			}
		}
	}

	delete[] opt;
	return false;
}

template<class T>
//returns a negative cycle of g
std::vector<unsigned int> * negativeCycleGetter(Graph::WeightedDirectedGraph<T> * g) {
	int * opt = new int[g->size];
	unsigned int * prev = new unsigned int[g->size];

	for (unsigned int i = 0; i < g->size; i++) {
		opt[i] = INT_MAX / 2;
		prev[i] = -1;
	}

	opt[0] = 0;

	//the starting index of the negative loop
	unsigned int negLoop = -1;

	//must be rechecked at least n - 1 times 
	for (unsigned int k = 0; k < g->size; k++) {
		//used to check every edge in g

		for (unsigned int i = 0; i < g->size; i++) {
			//for each edge pointing at vertex i
			for (unsigned int j = 0; j < g->getParentNum(i); j++) {
				unsigned int parent = g->getParent(i, j);

				//cast to an int because weights are stored as unsigned ints, returns the sign
				int tempCost = opt[parent] + (int)g->getWeightOfEdge(parent, i);

				if (tempCost < opt[i]) {
					//if it gets updated after n - 1 iterations then there must be a negative cycle because any graph without one would already have converged
					if (k == g->size - 1) {
						prev[i] = parent;
						negLoop = i;

						goto cycleFound;
					}

					prev[i] = parent;
					opt[i] = tempCost;
				}
			}
		}
	}

	delete[] opt;
	delete[] prev;
	
	return nullptr;

cycleFound:

	std::vector<unsigned int> * retVec = new std::vector<unsigned int>;
	retVec->reserve(g->size / 2);

	unsigned int loopVal = prev[negLoop];

	retVec->push_back(negLoop);

	while (loopVal != negLoop) {
		retVec->push_back(loopVal);
		loopVal = prev[loopVal];
	}

	delete[] prev;
	delete[] opt;

	return retVec;
}