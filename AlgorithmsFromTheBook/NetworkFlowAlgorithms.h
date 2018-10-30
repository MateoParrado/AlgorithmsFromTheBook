#pragma once

template<class T>
//find path between node start and node end using DFS
SinglyLinkedList::LinkedList<int> depthFirstSearch(Graph::ResidualGraph<T> * g, int start, int end) {
	std::vector<bool> visited((*g).size);//to not double count nodes
	visited[start] = true;

	SinglyLinkedList::LinkedList<int> ret;

	ret.pushBackNode(start);

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

		if (ret.head) {
			if (ret.getVal(0) == end) break;

			if (!ret.size) { return ret; }
		}
		else
			return ret;
	}

	return ret;
}