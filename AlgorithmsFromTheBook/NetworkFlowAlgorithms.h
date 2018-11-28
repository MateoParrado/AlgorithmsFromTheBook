#pragma once

#include <memory>

/*HELPER FUNCTIONS*/

template<class T>
//find path between node start and end node end using DFS
SinglyLinkedList::LinkedList<unsigned int> * findAugmentingPath(Graph::ResidualGraph<T> * g) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[g->start] = true;

	SinglyLinkedList::LinkedList<unsigned int> * ret = new SinglyLinkedList::LinkedList<unsigned int>();

	ret->pushBackNode(g->start);

	for(;;) {
		//for easy lookup, nodes will be added at the end
		for (unsigned int i = 0; i < (*g).getEdgeNum(ret->getVal(0)); i++) {
			//same algorithm as before except it only will count an edge as an edge if it has leftover flow capacity
			if (g->getResidualCapacity(ret->getVal(0), i)) {
				int temp = (*g).getOtherSideOfEdge(ret->getVal(0), i); //to not have to calculate it so much

				if (!visited[temp]) {
					visited[temp] = true;

					ret->pushForwardsNode(temp);

					goto edgeFound;
				}
			}
		}

		//check for backwards edges if no forwards edge is found
		for (unsigned int i = 0; i < (*g).getParentNum(ret->getVal(0)); i++) {
			//same algorithm as before except it only will count an edge as an edge if it has leftover flow capacity
			if (g->getFlowBetweenNodes(g->getParent(ret->getVal(0), i), ret->getVal(0))) {
				int temp = (*g).getParent(ret->getVal(0), i); //to not have to calculate it so much

				if (!visited[temp]) {
					visited[temp] = true;

					ret->pushForwardsNode(temp);

					goto edgeFound;
				}
			}
		}
		ret->popFrontNode(); //if no edge is found as a child


	edgeFound:

		//prevents segfaults
		if (ret->head) {
			if (ret->getVal(0) == g->end) break;

			if (!ret->size) {
				delete ret;
				return nullptr; 
			}
		}
		else {//if head is a nullptr, throw it away
			delete ret;
			return nullptr;
		}
	}

	return ret;
}

template<class T>
//modification of algorithm above, only finds path with a bottleneck capacity above a certain value
SinglyLinkedList::LinkedList<unsigned int> * findAugmentingPath(Graph::ResidualGraph<T> * g, unsigned int scalingFactor) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[g->start] = true;

	SinglyLinkedList::LinkedList<unsigned int> * ret = new SinglyLinkedList::LinkedList<unsigned int>();

	ret->pushBackNode(g->start);

	for (;;) {
		//for easy lookup, nodes will be added at the end
		for (unsigned int i = 0; i < (*g).getEdgeNum(ret->getVal(0)); i++) {
			//same algorithm as before except it only will count an edge as an edge if it has leftover flow capacity
			if (g->getResidualCapacity(ret->getVal(0), i) >= scalingFactor) {
				int temp = (*g).getOtherSideOfEdge(ret->getVal(0), i); //to not have to calculate it so much

				if (!visited[temp]) {
					visited[temp] = true;

					ret->pushForwardsNode(temp);

					goto edgeFound;
				}
			}
		}

		//check for backwards edges if no forwards edge is found
		for (unsigned int i = 0; i < (*g).getParentNum(ret->getVal(0)); i++) {
			//same algorithm as before except it only will count an edge as an edge if it has leftover flow capacity
			if (g->getFlowBetweenNodes(g->getParent(ret->getVal(0), i), ret->getVal(0)) >= scalingFactor) {
				int temp = (*g).getParent(ret->getVal(0), i); //to not have to calculate it so much

				if (!visited[temp]) {
					visited[temp] = true;

					ret->pushForwardsNode(temp);

					goto edgeFound;
				}
			}
		}
		ret->popFrontNode(); //if no edge is found as a child


	edgeFound:

		//prevents segfaults
		if (ret->head) {
			if (ret->getVal(0) == g->end) break;

			if (!ret->size) {
				delete ret;
				return nullptr;
			}
		}
		else {//if head is a nullptr, throw it away
			delete ret;
			return nullptr;
		}
	}

	return ret;
}

template<class T>
//find path between node start and target node end using DFS, ignoring all backwards edges, this is used for the min cut calculation
bool isPath(Graph::ResidualGraph<T> * g, unsigned int target) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[g->start] = true;

	SinglyLinkedList::LinkedList<unsigned int> * ret = new SinglyLinkedList::LinkedList<unsigned int>();

	ret->pushBackNode(g->start);

	for (;;) {
		//for easy lookup, nodes will be added at the end
		for (unsigned int i = 0; i < (*g).getEdgeNum(ret->getVal(0)); i++) {
			//same algorithm as before except it only will count an edge as an edge if it has leftover flow capacity
			if (g->getResidualCapacity(ret->getVal(0), i)) {
				int temp = (*g).getOtherSideOfEdge(ret->getVal(0), i); //to not have to calculate it so much

				if (!visited[temp]) {
					visited[temp] = true;

					ret->pushForwardsNode(temp);

					goto edgeFound;
				}
			}
		}

		ret->popFrontNode(); //if no edge is found as a child


	edgeFound:

		//prevents segfaults
		if (ret->head) {
			if (ret->getVal(0) == target) {
				return true;
			}

			if (!ret->size) {
				delete ret;
				return false;
			}
		}
		else {//if head is a nullptr, throw it away
			delete ret;
			return false;
		}
	}

	return true;
}

template<class T>
//get the value of the minimum possible capacity to increase the flow in a path
int getBottleneck(Graph::ResidualGraph<T> * g, std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> path) {
	SinglyLinkedList::Node<unsigned int> * head = path->head;
	
	unsigned int minVal = INT_MAX;//this is too low, but still lower than all edge weights, and also std::numeric_Limits::infinity for some reason returns zero for unsigned ints

	while (head->next) {
		//check if its a fowrards node or a backwards node
		if (g->hasEdge(head->next->obj, head->obj)) {
			minVal = std::min(minVal, g->getResidualCapacityBetweenNodes(head->next->obj, head->obj));
		}
		else {
			minVal = std::min(minVal, g->getFlowBetweenNodes(head->obj, head->next->obj));
		}
		head = head->next;
	}

	//we need to make this value negative so it can;t be unsigned, and to minimize the number of casts i only cast the last value
	return (int)minVal;
}

/*MAXIMUM FLOW*/

template<class T>
//find the maximum flow that can be put through a weighted directed graph
//start is the node the flow originates at, and end is the sink node tht absorbs all flow
//it works by creating a "residual graph" and then choosing any path from s - t and pushing flow along it, until it cant anymore
unsigned int fordFulkersonMaxFlow(const Graph::WeightedDirectedGraph<T> & graph, unsigned int start, unsigned int end) {
	Graph::ResidualGraph<T> g(graph, start, end);

	int curFlow = 0;

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then end
		if (!augPath)
			return curFlow;

		//find the bottleneck
		int bottleneck = getBottleneck(&g, augPath);

		//if the flow cannot be changed at all then return, could also include a tracker to make sure every path includes at least one forwards node but this is better
		if (!bottleneck) {
			return curFlow;
		}

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for(;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -bottleneck);
			}
			else {
				g.addFlow(head->next->obj, head->obj, bottleneck);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, bottleneck);
				break;
			}

		}

		curFlow += bottleneck;
	}
}

template<class T>
//a modification of the ford fulkerson algorithm above
//tries to find the largest augmenting paths first, to minimize the number of iterations the algorithm is run
unsigned int scalingMaxFlow(const Graph::WeightedDirectedGraph<T> & graph, unsigned int start, unsigned int end) {
	Graph::ResidualGraph<T> g(graph, start, end);

	unsigned int scalingFactor = 0;

	{
		//first find the max flow leaving the source node
		unsigned int c = 0;

		for (unsigned int i = 0; i < g.getChildNum(start); i++) {
			c += g.getWeightOfEdgeByPos(start, i);
		}

		//set the initial scaling factor to be the highest power of two less than the capacity leaving the sink
		scalingFactor = (int)pow(2, (int)log2(c));
	}

	unsigned int curFlow = 0;

	while (scalingFactor) {
		for (;;) {
			std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g, scalingFactor));

			//if there are no more paths then end
			if (!augPath)
				break;

			//find the bottleneck
			int bottleneck = getBottleneck(&g, augPath);

			//if the flow cannot be changed at all then return, could also include a tracker to make sure every path includes at least one forwards node but this is better
			if (!bottleneck) {
				break;
			}

			//follow along the path and add that flow to every node in the graph
			SinglyLinkedList::Node<unsigned int> * head = augPath->head;

			//safe from segfaults because if head is a nullptr it would end at the if !augPath
			for (;;) {
				//check if its a fowrards node or a backwards node
				if (g.hasEdge(head->obj, head->next->obj)) {
					g.addFlow(head->obj, head->next->obj, -bottleneck);
				}
				else {
					g.addFlow(head->next->obj, head->obj, bottleneck);
				}

				head = head->next;
				//make sure that the flow gets added into the starting node
				if (!head->next->next) {
					g.addFlow(g.start, head->obj, bottleneck);
					break;
				}

			}

			curFlow += bottleneck;
		}

		scalingFactor /= 2;
	}

	return curFlow;
}

template<class T>
unsigned int preflowPush(const Graph::WeightedDirectedGraph<T> & graph, unsigned int start, unsigned int end) {

	Graph::Preflow<T> g(graph, start, end);

	//vector holding which nodes have excess flow, {node, excess}
	std::vector<std::pair<unsigned int, unsigned int>> excess;

	//tracks the next edge that needs to be traversed for each node
	std::vector<int> current(graph.size);

	excess.reserve(graph.size - 1);//because the start and end nodes can have no excess

	for (unsigned int i = 0; i < g.getChildNum(start); i++) {
		excess.push_back(std::make_pair(g.getChild(start, i), g.getWeightOfEdgeByPos(start, i)));
	}

	//while there is a node with excess, choose one and try and push flow to all edges
	while (excess.size()) {
		//we choose node excess.size() - 1, for cheap deletions
		unsigned int chosenNode = excess[excess.size() - 1].first;

		//first check all forwards edges, but only if there are still forwards nodes to be ckecked(aka current is positive)
		if (current[chosenNode] >= 0) {
			for (; current[chosenNode] < g.getChildNum(chosenNode); current[chosenNode]++) {
				if (g.getResidualCapacity(chosenNode, current[chosenNode]) && g.labels[g.getChild(chosenNode, current[chosenNode])] <= g.labels[chosenNode]) {
					unsigned int pushedFlow = g.push(chosenNode, current[chosenNode], excess[excess.size() - 1].second);

					//add the excess to the node being pushed to, unless its the end node
					unsigned int childNode = g.getChild(chosenNode, current[chosenNode]);

					if (childNode != end) {
						auto posInExcess = std::find_if(excess.begin(), excess.end(),
							[childNode](std::pair<unsigned int, unsigned int> & p)->bool {return p.first == childNode;});

						if (posInExcess == excess.end()) {
							excess.insert(excess.end() - 1, std::make_pair(childNode, pushedFlow));
						}
						else {
							posInExcess->second += pushedFlow;
						}
					}

					//subtract the excess from node being checked, and delete it from the vector if the excess is too low
					excess[excess.size() - 1].second -= pushedFlow;
					if (excess[excess.size() - 1].second < 1) {
						excess.erase(excess.end() - 1, excess.end());

						//because we were able to remove all excess from the node, we just want to do it over with a different node
						current[chosenNode]++;

						goto checkNextNode;
					}
				}
			}
		}

		//if the forwards counter hasnt found anything, reset current
		if (current[chosenNode] > 0) {
			current[chosenNode] = 0;
		}
		//otherwise, put it into the right space
		else if (current[chosenNode] < 0) {
			current[chosenNode] *= -1;
		}

		//then, all the backwards ones
		for (; current[chosenNode] < g.getParentNum(chosenNode); current[chosenNode]++) {
			unsigned int parentNode = g.getParent(chosenNode, current[chosenNode]);

			//find the index of the chosen node in the parents adjacency list
			unsigned int index = 0;
			for (unsigned int j = 0; j < g.edges[parentNode]->size(); j += 2) {
				if ((*g.edges[parentNode])[j] == chosenNode) {
					index = j / 2;

					break;
				}
			}

			if (g.getFlow(parentNode, index) && g.labels[g.getParent(chosenNode, current[chosenNode])] <= g.labels[chosenNode]) {
				unsigned int pushedFlow = g.push(parentNode, index, excess[excess.size() - 1].second, false);

				//add the excess to the parent
				auto posInExcess = std::find_if(excess.begin(), excess.end(),
					[parentNode](std::pair<unsigned int, unsigned int> & p)->bool {return p.first == parentNode;});

				if (posInExcess == excess.end()) {
					excess.insert(excess.end() - 1, std::make_pair(parentNode, pushedFlow));		
				}
				else {
					posInExcess->second += pushedFlow;
				}

				//remove the excess fromt the chosen node, and remove it from the vector if it is too low
				excess[excess.size() - 1].second -= pushedFlow;
				if (excess[excess.size() - 1].second < 1) {
					excess.erase(excess.end() - 1, excess.end());

					//because we were able to remove all excess from the node, we just want to do it over with a different node
					current[chosenNode] *= -1;
					current[chosenNode]--;

					goto checkNextNode;
				}
			}
		}

		//if flow can't be pushed, raise the height, unless its start, which you just delete from the excess graph
		if (chosenNode != start) {
			current[chosenNode] = 0;
			g.relabel(chosenNode);
		}
		else {
			current[start] = 0;

			excess.erase(excess.end() - 1, excess.end());
		}

	checkNextNode:;
	}

	unsigned int flow = 0;

	//calculate the flow of every edge going into the end node
	for (unsigned int i = 0; i < g.getParentNum(end); i++) {
		flow += g.getFlowBetweenNodes(g.getParent(end, i), end);
	}

	return flow;
}

/*MINIMUM CUT*/

template<class T>
SinglyLinkedList::LinkedList<unsigned int> fordFulkersonMinCut(const Graph::WeightedDirectedGraph<T> & graph, unsigned int start, unsigned int end) {
	Graph::ResidualGraph<T> g(graph, start, end);

	int curFlow = 0;

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then break
		if (!augPath)
			break;

		//find the bottleneck
		int bottleneck = getBottleneck(&g, augPath);

		//if the flow cannot be changed at all then break
		if (!bottleneck) {
			break;
		}

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -bottleneck);
			}
			else {
				g.addFlow(head->next->obj, head->obj, bottleneck);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, bottleneck);
				break;
			}

		}

		curFlow += bottleneck;
	}

	SinglyLinkedList::LinkedList<unsigned int> ret;
	ret.pushBackNode(start);

	for (unsigned int i = 0; i < g.size; i++) {
		if (i == start || i == end) {
			continue;
		}

		if (isPath(&g, i)) {
			ret.pushBackNode(i);
		}
	}

	return ret;
}

/*BIPARTITE MATCHING*/

template<class T>
//returns the number of pairs in the maximum matching of a bipartite graph
unsigned int maximumBipartiteMatchingNum(const Graph::BipartiteGraph<T> & graph) {
	Graph::ResidualGraph<T> g(graph);

	int curFlow = 0;

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then end
		if (!augPath)
			return curFlow;

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -1);
			}
			else {
				g.addFlow(head->next->obj, head->obj, 1);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, 1);
				break;
			}

		}

		curFlow++;
	}
}

template<class T>
//returns the number of pairs in the maximum matching of a bupartite graph
//note: graph MUST BE BIPARTITE
std::unique_ptr<std::vector<std::pair<unsigned int, unsigned int>>> maximumBipartiteMatching(const Graph::BipartiteGraph<T> & graph) {
	Graph::ResidualGraph<T> g(graph);

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then end
		if (!augPath)
			goto done;

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -1);
			}
			else {
				g.addFlow(head->next->obj, head->obj, 1);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, 1);
				break;
			}

		}

	}

done:
	std::unique_ptr<std::vector<std::pair<unsigned int, unsigned int>>> retVec(new std::vector<std::pair<unsigned int, unsigned int>>());
	retVec->reserve(g.size - 2);

	for (unsigned int i = 0; i < graph.firstPartitionSize; i++) {

		for (unsigned int j = 0; j < g.getChildNum(i); j++) {
			if (g.getFlow(i, j)) {
				retVec->push_back(std::make_pair(i, g.getChild(i, j)));
				break;
			}
		}
	}

	return retVec;
}

template<class T>
//returns true if the BIPARTITE graph has a perfect matching
bool bipartiteHasPerfectMatching(const Graph::BipartiteGraph<T> & graph) {
	Graph::ResidualGraph<T> g(graph);

	//for a perfect matching, both sides must be the same size
	if (graph.firstPartitionSize != graph.size / 2) {
		return false;
	}

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then end
		if (!augPath)
			goto done;

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -1);
			}
			else {
				g.addFlow(head->next->obj, head->obj, 1);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, 1);
				break;
			}

		}
	}

done:

	for (unsigned int i = 0; i < graph.firstPartitionSize; i++) {

		//if start does not lead to any of the edges, it is not a perfect matching
		if (!g.getFlow(g.start, i)) {
			return false;
		}
	}

	return true;
}

/*DISJOINT PATHS PROBLEM*/

template<class T>
//find the maximum numebr of edge disjoint paths in a directed graph from a start node to an end node
unsigned int disjointPaths(const Graph::DirectedGraph<T> & graph, unsigned int start, unsigned int end) {
	Graph::ResidualGraph<T> g(graph, start, end);

	int curFlow = 0;

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then end
		if (!augPath)
			return curFlow;

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -1);
			}
			else {
				g.addFlow(head->next->obj, head->obj, 1);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, 1);
				break;
			}

		}

		curFlow++;
	}
}

template<class T>
//find the maximum numebr of edge disjoint paths in an undirected graph from a start node to an end node
unsigned int disjointPaths(const Graph::Graph<T> & graph, unsigned int start, unsigned int end) {
	Graph::ResidualGraph<T> g(graph, start, end);

	int curFlow = 0;

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(&g));

		//if there are no more paths then end
		if (!augPath)
			return curFlow;

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g.hasEdge(head->obj, head->next->obj)) {
				g.addFlow(head->obj, head->next->obj, -1);
			}
			else {
				g.addFlow(head->next->obj, head->obj, 1);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g.addFlow(g.start, head->obj, 1);
				break;
			}

		}

		curFlow++;
	}
}

/*CIRCULATIONS*/

template<class T>
//finds the maximum circulation for a graph
//takes in a graph and an array of "demands"
//positive demands means capacity, and negative demands means supply
//this is in essence a multi-source, multi-sink flow network, where every node with negative demand supplies those with positive demand
//returns nullptr when there is no way to neutralize the supply and demand
Graph::ResidualGraph<T> * maximumCirculation(const Graph::WeightedDirectedGraph<T> & graph, int * demands) {
	//construct the graph, then modify it later
	Graph::ResidualGraph<T> * g = new Graph::ResidualGraph<T>(graph, 0, 1);

	// add a "super" source node to all sources
	g->addNode(graph.nodes[0].obj);

	//add a "super" sink node to take all sources flow
	g->addNode(graph.nodes[0].obj);

	//used to check if there is a feasible circulation later
	unsigned int checkForFeasibility = 0;

	//for each node, it demands[i] is negative make the source link to it, if not link it to the sink
	for (unsigned int i = 0; i < graph.nodes.size(); i++) {
		if (demands[i] < 0) {
			g->addEdge(g->nodes.size() - 2, i, -demands[i]);
			checkForFeasibility -= demands[i];
		}
		else if (demands[i] > 0) {
			g->addEdge(i, g->nodes.size() - 1, demands[i]);
		}
	}

	//then set the start and end nodes to be the supernodes
	g->start = g->nodes.size() - 2;
	g->end = g->nodes.size() - 1;

	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(g));

		//if there are no more paths then end
		if (!augPath)
			goto done;

		//find the bottleneck
		int bottleneck = getBottleneck(g, augPath);

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g->hasEdge(head->obj, head->next->obj)) {
				g->addFlow(head->obj, head->next->obj, -bottleneck);
			}
			else {
				g->addFlow(head->next->obj, head->obj, bottleneck);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g->addFlow(g->start, head->obj, bottleneck);
				break;
			}

		}
	}

done:

	unsigned int endTotal = 0;

	for (unsigned int i = 0; i < g->parents[g->end]->size(); i += 2) {
		endTotal += g->getFlowBetweenNodes((*g->parents[g->end])[i], g->end);
	}

	if (endTotal != checkForFeasibility) {
		delete g;
		return nullptr;
	}

	g->removeNode(g->end);
	g->removeNode(g->start);

	return g;
}

template<class T>
//finds the maximum circulation for a graph, with minimum requirements for each edge
//demands is an array with the minimum requirement for each edge
//returns nullptr when there is no way to neutralize the supply and demand
Graph::ResidualGraph<T> * boundedMaximumCirculation(const Graph::WeightedDirectedGraph<T> & graph, int * demands, unsigned int bound) {
	//construct the graph, then modify it later
	Graph::ResidualGraph<T> * g = new Graph::ResidualGraph<T>(graph, 0, 1);

	// add a "super" source node to all sources
	g->addNode(graph.nodes[0].obj);

	//add a "super" sink node to take all sources flow
	g->addNode(graph.nodes[0].obj);

	//used to check if there is a feasible circulation later
	unsigned int checkForFeasibility = 0;

	for (unsigned int i = 0; i < graph.size; i++) {
		if (demands[i] < 0) {
			checkForFeasibility -= demands[i];
		}
	}

	//go through every edge and subtract the lower bound
	for (unsigned int i = 0; i < g->nodes.size(); i++) {
		for (unsigned int j = 1; j < g->edges[i]->size(); j += 2) {
			(*g->edges[i])[j] -= bound;
			demands[(*g->edges[i])[j - 1]] -= bound;
			demands[i] += bound;
		}
	}

	//for each node, it demands[i] is negative make the source link to it, if not link it to the sink
	for (unsigned int i = 0; i < graph.nodes.size(); i++) {
		if (demands[i] < 0) {
			g->addEdge(g->nodes.size() - 2, i, -demands[i]);
		}
		else if (demands[i] > 0) {
			g->addEdge(i, g->nodes.size() - 1, demands[i]);
		}
	}

	//then set the start and end nodes to be the supernodes
	g->start = g->nodes.size() - 2;
	g->end = g->nodes.size() - 1;


	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(g));

		//if there are no more paths then end
		if (!augPath)
			goto done;

		//find the bottleneck
		int bottleneck = getBottleneck(g, augPath);

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g->hasEdge(head->obj, head->next->obj)) {
				g->addFlow(head->obj, head->next->obj, -bottleneck);
			}
			else {
				g->addFlow(head->next->obj, head->obj, bottleneck);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g->addFlow(g->start, head->obj, bottleneck);
				break;
			}

		}
	}

done:

	//add the weights into the edge, and the flows into the graph
	for (unsigned int i = 0; i < g->nodes.size(); i++) {
		for (unsigned int j = 1; j < g->edges[i]->size(); j += 2) {
			(*g->edges[i])[j] += bound * std::max(1u, g->getParentNum(i) / g->getChildNum(i));
			(*g->flows[i])[j / 2] += bound * std::max(1u, g->getParentNum(i) / g->getChildNum(i));
		}
	}

	unsigned int endTotal = 0;

	for (unsigned int i = 0; i < g->parents[g->end]->size(); i += 2) {
		endTotal += g->getFlowBetweenNodes((*g->parents[g->end])[i], g->end);
	}

	if (endTotal != checkForFeasibility) {
		delete g;
		return nullptr;
	}

g->removeNode(g->end);
g->removeNode(g->start);


return g;
}

template<class T>
//same as above, except the bounds can be different for each edge of the graph
//it will loop through bounds and take the first value of the first node for the first edge, etc
//demands will be modified
bool differentlyBoundedMaximumCirculation(const Graph::WeightedDirectedGraph<T> & graph, int * demands, unsigned int * bounds) {
	//construct the graph, then modify it later
	Graph::ResidualGraph<T> * g = new Graph::ResidualGraph<T>(graph, 0, 1);

	// add a "super" source node to all sources
	g->addNode(graph.nodes[0].obj);

	//add a "super" sink node to take all sources flow
	g->addNode(graph.nodes[0].obj);

	//used to check if there is a feasible circulation later
	unsigned int checkForFeasibility = 0;

	for (unsigned int i = 0; i < graph.size; i++) {
		if (demands[i] < 0) {
			checkForFeasibility -= demands[i];
		}
	}

	//because the nodes have variable length edges, this keeps track of which index of bounds we are in
	unsigned int boundNumOn = 0;

	//go through every edge and subtract the lower bound, and update its demands
	for (unsigned int i = 0; i < g->nodes.size(); i++) {
		for (unsigned int j = 1; j < g->edges[i]->size(); j += 2) {
			(*g->edges[i])[j] -= bounds[boundNumOn];
			demands[(*g->edges[i])[j - 1]] -= bounds[boundNumOn];
			demands[i] += bounds[boundNumOn];

			boundNumOn++;
		}
	}

	for (int i = 0; i < 4; i++) {
		std::cout << demands[i] << " ";
	}

	//for each node, it demands[i] is negative make the source link to it, if not link it to the sink
	for (unsigned int i = 0; i < graph.nodes.size(); i++) {
		if (demands[i] < 0) {
			g->addEdge(g->nodes.size() - 2, i, -demands[i]);
		}
		else if (demands[i] > 0) {
			g->addEdge(i, g->nodes.size() - 1, demands[i]);
		}
	}

	//then set the start and end nodes to be the supernodes
	g->start = g->nodes.size() - 2;
	g->end = g->nodes.size() - 1;


	for (;;) {
		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(g));

		//if there are no more paths then end
		if (!augPath)
			goto done;

		//find the bottleneck
		int bottleneck = getBottleneck(g, augPath);

		//follow along the path and add that flow to every node in the graph
		SinglyLinkedList::Node<unsigned int> * head = augPath->head;

		//safe from segfaults because if head is a nullptr it would end at the if !augPath
		for (;;) {
			//check if its a fowrards node or a backwards node
			if (g->hasEdge(head->obj, head->next->obj)) {
				g->addFlow(head->obj, head->next->obj, -bottleneck);
			}
			else {
				g->addFlow(head->next->obj, head->obj, bottleneck);
			}

			head = head->next;
			//make sure that the flow gets added into the starting node
			if (!head->next->next) {
				g->addFlow(g->start, head->obj, bottleneck);
				break;
			}

		}
	}

done:

	unsigned int flowOutOfStart = 0;

	boundNumOn = 0;

	for (unsigned int i = 0; i < g->getChildNum(g->start); i++){
		flowOutOfStart += g->getFlow(g->start, i);
	}

	delete g;

	return flowOutOfStart == checkForFeasibility;
}

//says wether you can create a survey asking a number of customers about a number of products
//customers can only be asked about a product if they bought it, so customer bought product is a 2d array of wether they bought it
//customers must be asked at least minproductsasked and at most maxproductsasked
//customers must be asked about every product at least mintimesasked and at most maxtimesasked, to gather sufficient information
//bool surveyCanBeDesigned(unsigned int customerNum, unsigned int productNum, bool * customerBoughtProduct, unsigned int minProductsAsked, unsigned int maxProductsAsked, unsigned int minTimesAsked, unsigned int maxTimesAsked) {
//	//construct the graph, then modify it later
//	Graph::WeightedDirectedGraph<unsigned int> graph(productNum + customerNum + 4);
//
//	for (unsigned int i = 0; i < customerNum + productNum; i++) {
//		graph.addNode(i);
//	}
//
//	// add a "super" source node to all sources
//	graph.addNode(graph.nodes[0].obj);
//
//	//add a "super" sink node to take all sources flow
//	graph.addNode(graph.nodes[0].obj);
//
//	//go through every customer, connect them with a minimum
//
//	//for each node, it demands[i] is negative make the source link to it, if not link it to the sink
//	for (unsigned int i = 0; i < graph.nodes.size(); i++) {
//		if (demands[i] < 0) {
//			g->addEdge(g->nodes.size() - 2, i, -demands[i]);
//		}
//		else if (demands[i] > 0) {
//			g->addEdge(i, g->nodes.size() - 1, demands[i]);
//		}
//	}
//
//	Graph::ResidualGraph<T> * g = new Graph::ResidualGraph<T>(graph, 0, 1);
//
//	//then set the start and end nodes to be the supernodes
//	g->start = g->nodes.size() - 2;
//	g->end = g->nodes.size() - 1;
//
//
//	for (;;) {
//		std::shared_ptr<SinglyLinkedList::LinkedList<unsigned int>> augPath(findAugmentingPath(g));
//
//		//if there are no more paths then end
//		if (!augPath)
//			goto done;
//
//		//find the bottleneck
//		int bottleneck = getBottleneck(g, augPath);
//
//		//follow along the path and add that flow to every node in the graph
//		SinglyLinkedList::Node<unsigned int> * head = augPath->head;
//
//		//safe from segfaults because if head is a nullptr it would end at the if !augPath
//		for (;;) {
//			//check if its a fowrards node or a backwards node
//			if (g->hasEdge(head->obj, head->next->obj)) {
//				g->addFlow(head->obj, head->next->obj, -bottleneck);
//			}
//			else {
//				g->addFlow(head->next->obj, head->obj, bottleneck);
//			}
//
//			head = head->next;
//			//make sure that the flow gets added into the starting node
//			if (!head->next->next) {
//				g->addFlow(g->start, head->obj, bottleneck);
//				break;
//			}
//
//		}
//	}
//
//done:
//
//	//add the weights into the edge, and the flows into the graph
//	for (unsigned int i = 0; i < g->nodes.size(); i++) {
//		for (unsigned int j = 1; j < g->edges[i]->size(); j += 2) {
//			(*g->edges[i])[j] += bound * std::max(1u, g->getParentNum(i) / g->getChildNum(i));
//			(*g->flows[i])[j / 2] += bound * std::max(1u, g->getParentNum(i) / g->getChildNum(i));
//		}
//	}
//
//	unsigned int endTotal = 0;
//
//	for (unsigned int i = 0; i < g->parents[g->end]->size(); i += 2) {
//		endTotal += g->getFlowBetweenNodes((*g->parents[g->end])[i], g->end);
//	}
//
//	if (endTotal != checkForFeasibility) {
//		delete g;
//		return nullptr;
//	}
//
//	g->removeNode(g->end);
//	g->removeNode(g->start);
//
//
//	return g;
//}