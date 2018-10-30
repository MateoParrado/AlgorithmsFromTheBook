#pragma once

template<class T>
//find path between node start and end node end using DFS
SinglyLinkedList::LinkedList<int> * findAugmentingPath(Graph::ResidualGraph<T> * g) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[start] = true;

	SinglyLinkedList::LinkedList<int> & ret = new SinglyLinkedList::LinkedList<int>();

	ret.pushBackNode(g->start);

	while (true) {
		//for easy lookup, nodes will be added at the end
		for (int i = 0; i < (*g).getEdgeNum(ret.getVal(0)); i++) {
			//same algorithm as before except it only will count an edge as an edge if it has leftover flow capacity
			if (g->getResidualCapacity(ret.getVal(0), i)) {
				int temp = (*g).getOtherSideOfEdge(ret.getVal(0), i); //to not have to calculate it so much

				if (!visited[temp]) {
					visited[temp] = true;

					ret.pushForwardsNode(temp);

					goto edgeFound;
				}
			}
		}
		ret.popFrontNode(); //if no edge is found as a child


	edgeFound:

		//prevents segfaults
		if (ret.head) {
			if (ret.getVal(0) == g->end) break;

			if (!ret.size) {
				delete ret;
				return nullptr; 
			}
		}
		else//if head is a nullptr, throw it away
			delete ret;
			return nullptr;
	}

	return ret;
}

template<class T>
//get the value of the minimum possible capacity to increase the flow in a path
unsigned int getBottleneck(Graph::ResidualGraph<T> * g, SinglyLinkedList::LinkedList<unsigned int> * path) {
	SinglyLinkedList::Node<unsigned int> * head = path->head;
	
	unsigned int minVal = -1;

	while (head->next) {
		minVal = std::min(minVal, g->getResidualFlowBetweenNodes(head->obj, head->next->obj));
		head = head->next;
	}

	return minVal;
}

template<class T>
unsigned int fordFulkersonMaxFlow(Graph::WeightedDirectedGraph<T> * graph, unsigned int start, unsigned int end) {
	Graph::ResidualGraph<T> g(graph, start, end);

	int curFlow = 0;

	for (;;) {
		SinglyLinkedList::LinkedList<unsigned int> * augPath = findAugmentingPath(&g);

		//if there are no more paths then end
		if (!augPath)
			return curFlow;

		//find the bottleneck
		unsigned int bottleneck = getBottleneck(&g, augPath);

		delete augPath;
	}
}