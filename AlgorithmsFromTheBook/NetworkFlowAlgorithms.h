#pragma once

#include <memory>

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
			minVal = std::min(minVal, g->getFlowBetweenNodes(head->next->obj, head->obj));
		}
		head = head->next;
	}

	//we need to make this value negative so it can;t be unsigned, and to minimize the number of casts i only cast the last value
	return (int)minVal;
}

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