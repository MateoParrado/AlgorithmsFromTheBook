#pragma once
#pragma warning(disable: 4250)//some weird inheritance dominance thing



#include <vector>
#include <iostream>

namespace Graph
{
	//forward declarations for building slicing copy constructors
	//as of right only implemented slicing consturctors are WeightedDirectedGraph to directed graph
	template<class T>
	struct Node;
	
	template<class T>
	struct Graph;

	template<class T>
	struct DirectedGraph;

	template<class T>
	struct WeightedGraph;

	template<class T>
	struct WeightedDirectedGraph;

	template<class T>
	struct Node
	{
		T obj;
		
		Node() { obj = T(); };

		//create node by pointer
		//only pinters allowed, must point to something already on the heap
		Node(T _obj) : obj(_obj) { };

		~Node() { }
	};

	template<class T>
	struct Graph 
	{
	private:

	public:
		unsigned int size = 0;

		std::vector<Node<T>> nodes;
		std::vector<std::vector<unsigned int> * > edges;

		Graph(unsigned int size = 10)
		{
			nodes.reserve(size); //leave objs unitnitialized
			edges.reserve(size);
		};


		//number of edges connected to vertex _i
		virtual unsigned int getEdgeNum(int _i)
		{
			return (*edges[_i]).size();
		}

		//get the node pointed to by the _edge th edge of _node
		virtual unsigned int getOtherSideOfEdge(unsigned int _node, unsigned int _edge)
		{
			return (*edges[_node])[_edge];
		}

		//get is the node n1 is connected to n by position, not object value
		virtual bool hasChild(unsigned int n, unsigned int n1) 
		{
			for (unsigned int i = 0; i < edges[n]->size(); i++)
			{
				if (getOtherSideOfEdge(n, i) == n1) return true;
			}

			return false;
		}

		//get number of edges in graph
		virtual unsigned int getTotalEdgesInGraph() 
		{
			unsigned int ret = 0;
			for (unsigned int i = 0; i < edges.size(); i++) 
			{
				ret += edges[i]->size();
			}

			return ret / 2;
		}

		T getObj(unsigned int index) 
		{
			return nodes[index].obj;
		}

		virtual void display()
		{
			for (unsigned int i = 0; i < nodes.size(); i++) 
			{
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < edges[i]->size(); j++)
				{
					std::cout << (*edges[i])[j] << " ";
				}

				std::cout << std::endl;
			}
		}

		/*ADD AND REMOVE*/
		virtual void addNode(T nodeVal) 
		{
			size++;

			nodes.push_back(Node<T>(nodeVal));
			edges.push_back(new std::vector<unsigned int>(0));

			(*(edges.end() - 1))->reserve(10);//default val, may be updated later
		}

		virtual void addEdge(unsigned int index1, unsigned int index2) 
		{
			edges[index1]->push_back(index2);
			edges[index2]->push_back(index1);
		}

		virtual void removeNode(unsigned int num)
		{
			delete edges[num];

			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++)
			{
				for (unsigned int j = edges[i]->size() - 1; j < edges[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--)
				{
					if ((*edges[i])[j] == num)
					{
						edges[i]->erase(begin(*edges[i]) + j);
					}
					else if((*edges[i])[j] > num)
					{
						(*edges[i])[j]--;
					}
				}
			}


			size--;
		}

		virtual void removeEdge(unsigned int node1, unsigned int node2)
		{
			for (unsigned int i = 0; i < edges[node1]->size(); i++)
			{
				if ((*edges[node1])[i] == node2)
				{
					edges[node1]->erase(edges[node1]->begin() + i);
					break;
				}
			}

			for (unsigned int i = 0; i < edges[node2]->size(); i++)
			{
				if ((*edges[node2])[i] == node1)
				{
					edges[node2]->erase(edges[node2]->begin() + i);
					break;
				}
			}
		}

		//return true if the graph contains that edge
		virtual bool hasEdge(unsigned int node1, unsigned int node2)
		{
			if (edges[node1]->size() < edges[node2]->size())
			{
				for (unsigned int i = 0; i < edges[node1]->size(); i++)
				{
					if ((*edges[node1])[i] == node2)
					{
						return true;
					}
				}

				return false;
			}

			for (unsigned int i = 0; i < edges[node2]->size(); i++)
			{
				if ((*edges[node2])[i] == node1)
				{
					return true;
				}
			}

			return false;
		}

		/*RULE OF THREE*/
		Graph(const Graph& g) {
			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<Graph&>(g).getEdgeNum(i); j++)
				{

					(*edges[i]).push_back(const_cast<Graph&>(g).getOtherSideOfEdge(i, j));
				}
			}
		}

		virtual Graph operator=(const Graph&) 
		{
			Graph<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 0; i < this->size; i++)
			{
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++) 
			{
				for (unsigned int j = 0; j < this->edges[i]->size(); j++) 
				{
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			return ret;
		}

		virtual ~Graph() {
			for (auto i : edges)
			{
				delete i;
			}
		}
	};

	template<class T>
	struct BipartiteGraph : Graph<T> 
	{
		unsigned int firstPartitionSize;

		BipartiteGraph(unsigned int nodesInFirstPartition, unsigned int size = 10) : Graph(size), firstPartitionSize(nodesInFirstPartition) {   }

		//you can only add edges between two edges that are in different partitions
		virtual void addEdge(unsigned int index1, unsigned int index2)
		{
			if ((index1 < firstPartitionSize && index2 >= firstPartitionSize) || (index1 >= firstPartitionSize && index2 < firstPartitionSize)) {
				edges[index1]->push_back(index2);
				edges[index2]->push_back(index1);
			}
			else 
			{
				throw;
			}
		}
	};

	//IMPORTANT NODE: THIS GRAPH IS NOT NECESSARY FOR TREES
	//THEY CAN EASILY BE REPRESENTD BY A TRADITIONAL GRAPH
	//HOWEVER SOME ALGORITHMS REQUIRE THE KNOWLEDGE OF THE LEVEL OF THE TOP, WHICH IS THE ONLY REASON TO USE THIS CLASS
	//IT ADDS O(N) SPACE
	//ONE RECOMMENDATION IS TO MAKE SURE THAT THE PARENT NODES GET ADDED TO FIRST, THAT IS FOR ALL I < J, I IS NOT J'S PARENT
	//THIS ISN'T IMPORTANT TO THE IMPLEMENTATION OF THE TREE, BUT FOR SOME ALGORITHMS (WEIGHTED INDEPENDENT SET), IT IS A REQUIREMENT
	template<class T>
	struct Tree : Graph<T>
	{
		std::vector<unsigned int> levels;
		unsigned int root;

		//becaus of the way the levels system, the root node needs a default value
		//it can still be changed later
		//this roots it at zero, only way to avoid this is to construct it from a graph
		Tree(T rootVal, unsigned int size = 10) : Graph(size) , root(0)
		{
			levels.reserve(size);

			addNode(rootVal);

			levels[0] = 0;
		}

		Tree(unsigned int rootNum) : root(rootNum)
		{
			//this is just helping cover some weird inheritance cases down the line
		}

		Tree(Graph<T> * g, unsigned int rootNode) : root(rootNode)
		{
			bool * visited = new bool[g->size]{ false };
			std::vector<unsigned int> pending;
			pending.reserve(g->size / 2);

			levels.resize(g->size, -1);

			for (unsigned int i = 0; i < g->size; i++)
			{
				Graph::addNode(g->nodes[i].obj);
			}

			levels[rootNode] = 0;
			visited[rootNode] = true;

			//add the roots kids
			for (unsigned int i = 0; i < g->getEdgeNum(rootNode); i++) 
			{
				addEdge(rootNode, g->getOtherSideOfEdge(rootNode, i));
				visited[g->getOtherSideOfEdge(rootNode, i)] = true;
				pending.push_back(g->getOtherSideOfEdge(rootNode, i));
			}

			while (pending.size()) 
			{
				unsigned int end = pending[pending.size() - 1];
				pending.pop_back();

				for (unsigned int i = 0; i < g->getEdgeNum(end); i++) 
				{
					if (!visited[g->getOtherSideOfEdge(end, i)])
					{
						addEdge(end, g->getOtherSideOfEdge(end, i));
						visited[g->getOtherSideOfEdge(end, i)] = true;
						pending.push_back(g->getOtherSideOfEdge(end, i));
					}
				}
			}

			delete[] visited;
		}

		virtual void addNode(T obj) 
		{
			Graph::addNode(obj);

			levels.push_back(-1);
		}

		//keep in mind that this does not hold up for combining two trees
		//say your levels is [0, -1, 0]
		//the second zero is like that because it is negative one plus one
		//then after adding an edge between zero and one it becomes
		// [0, 1, 0] which is wrong
		//avoid this if possible, but if its unavoidable, use a forest
		virtual void addEdge(unsigned int i, unsigned int j) 
		{
			if (levels[i] == -1)
			{
				levels[i] = levels[j] + 1;
				Graph::addEdge(i, j);

				return;
			}
			if (levels[j] == -1)
			{
				levels[j] = levels[i] + 1;
				Graph::addEdge(i, j);

				return;
			}

			//if neither of them are negative one then adding the edge will add a cycle
			throw 5;
		}

		//deletes not only n, but also all of its children
		virtual void removeNode(unsigned int n)
		{
			//if you delete the root, just erase everything
			if (n == root) 
			{
				size = 0;

				nodes.erase(nodes.begin(), nodes.end());
				edges.erase(edges.begin(), edges.end());

				levels.erase(levels.begin(), levels.end());

				return;
			}

			//the original parent
			unsigned int nPar = (*edges[n])[0];

			while (n != nPar)
			{
				//if it has children, go to them
				if (edges[n]->size() > 1) 
				{
					n = (*edges[n])[1];
				}
				else 
				{
					unsigned int oldN = n;

					//make n its parent
					n = (*edges[n])[0];

					levels.erase(levels.begin() + oldN, levels.begin() + oldN + 1);

					Graph::removeNode(oldN);
				}
			}
		}

		//because it is a tree, removing an edge disconnects it, and makes you need to throw out a lot of nodes
		virtual void removeEdge(unsigned int node, unsigned int edge)
		{
			//if were deleting the node to its parent, delete it
			if (!edge && node != root)
			{
				removeNode(node);
			}
			//else remove the child it leads to
			else 
			{
				removeNode((*edges[node])[edge]);
			}
		}

		virtual bool hasChild(unsigned int n, unsigned int m)
		{
			return getParent(m) == n;
		}

		virtual bool hasEdge(unsigned int n, unsigned int m)
		{
			if (getParent(n) == m)
			{
				return true;
			}

			return getParent(m) == n;
		}

		unsigned int getParent(unsigned int i) 
		{
			if (i != root)
			{
				return (*edges[i])[0];
			}
			return -1;
		}

		virtual unsigned int getChild(unsigned int i, unsigned int child)
		{
			return (*edges[i])[child + 1];
		}

		virtual unsigned int getChildNum(unsigned int i)
		{
			return edges[i]->size() - 1;
		}

		/*RULE OF THREE*/
		Tree(const Tree& g) 
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++) 
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<Tree&>(g).edges[i]->size(); j++)
				{

					edges[i]->push_back((*const_cast<Tree&>(g).edges[i])[j]);
				}
			}

			levels = g.levels;
			root = g.root;
		}

		virtual Tree operator=(const Tree&)
		{
			Tree<T> ret(nodes[0].obj, this->size);

			//copy over the node values
			for (unsigned int i = 1; i < this->size; i++) 
			{
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++)
			{
				for (unsigned int j = 0; j < this->edges[i]->size(); j++)
				{
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			ret.levels = levels;
			ret.root = root;

			return ret;
		}
	};

	template<class T>
	struct WeightedTree : Tree<T> {
		//becaus of the way the levels system, the root node needs a default value
		//it can still be changed later
		//this roots it at zero, only way to avoid this is to construct it from a graph
		WeightedTree(T rootVal = T(), unsigned int size = 10) : Tree(rootVal, size)
		{
		}

		WeightedTree(WeightedGraph<T> * g, unsigned int rootNode) : Tree(rootNode) 
		{
			bool * visited = new bool[g->size]{ false };
			std::vector<unsigned int> pending;
			pending.reserve(g->size / 2);

			levels.resize(g->size, -1);

			for (unsigned int i = 0; i < g->size; i++)
			{
				Graph::addNode(g->nodes[i].obj);
			}

			levels[rootNode] = 0;
			visited[rootNode] = true;

			//add the roots kids
			for (unsigned int i = 0; i < g->getEdgeNum(rootNode); i++)
			{
				addEdge(rootNode, g->getOtherSideOfEdge(rootNode, i), g->getWeightOfEdgeByPos(rootNode, i << 1));
				visited[g->getOtherSideOfEdge(rootNode, i)] = true;
				pending.push_back(g->getOtherSideOfEdge(rootNode, i));
			}
			this;
			while (pending.size())
			{
				unsigned int end = pending[pending.size() - 1];
				pending.pop_back();

				for (unsigned int i = 0; i < g->getEdgeNum(end); i++)
				{
					if (!visited[g->getOtherSideOfEdge(end, i)])
					{
						addEdge(end, g->getOtherSideOfEdge(end, i), g->getWeightOfEdgeByPos(end, i << 1));
						visited[g->getOtherSideOfEdge(end, i)] = true;
						pending.push_back(g->getOtherSideOfEdge(end, i));
					}
				}
			}

			delete[] visited;
		}

		//deletes not only n, but also all of its children
		virtual void removeNode(unsigned int n) 
		{
			//if you delete the root, just erase everything
			if (n == root)
			{
				size = 0;

				nodes.erase(nodes.begin(), nodes.end());
				edges.erase(edges.begin(), edges.end());

				levels.erase(levels.begin(), levels.end());

				return;
			}

			//the original parent
			unsigned int nPar = (*edges[n])[0];

			while (n != nPar) 
			{
				//if it has children, go to them
				if (edges[n]->size() > 2)
				{
					n = (*edges[n])[2];
				}
				else 
				{
					unsigned int oldN = n;

					//make n its parent
					n = (*edges[n])[0];

					levels.erase(levels.begin() + oldN, levels.begin() + oldN + 1);

					//WeightedGraph::removeNode 
					delete edges[oldN];

					edges.erase(begin(edges) + oldN);
					nodes.erase(begin(nodes) + oldN);

					//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
					for (unsigned int i = 0; i < edges.size(); i++) 
					{
						for (unsigned int j = edges[i]->size() - 2; j < edges[i]->size(); j -= 2)
						{
							if ((*edges[i])[j] == oldN)
							{
								edges[i]->erase(begin(*edges[i]) + j, begin(*edges[i]) + j + 2);
							}
							else if ((*edges[i])[j] > oldN)
							{
								(*edges[i])[j]--;
							}
						}
					}


					size--;
				}
			}
		}

		//because it is a tree, removing an edge disconnects it, and makes you need to throw out a lot of nodes
		virtual void removeEdge(unsigned int node, unsigned int edge)
		{
			//if were deleting the node to its parent, delete it
			if (!edge && node != root)
			{
				removeNode(node);
			}
			//else remove the child it leads to
			else
			{
				removeNode((*edges[node])[edge << 1]);
			}
		}

		virtual void addEdge(unsigned int i, unsigned int j, unsigned int w)
		{
			if (levels[i] == -1)
			{
				levels[i] = levels[j] + 1;

				edges[i]->push_back(j);
				edges[j]->push_back(i);

				edges[i]->push_back(w);
				edges[j]->push_back(w);

				return;
			}
			if (levels[j] == -1)
			{
				levels[j] = levels[i] + 1;
				
				edges[i]->push_back(j);
				edges[j]->push_back(i);

				edges[i]->push_back(w);
				edges[j]->push_back(w);

				return;
			}

			//if neither of them are negative one then adding the edge will add a cycle
			throw 5;
		}

		virtual unsigned int getChild(unsigned int n, unsigned int i)
		{
			if (n == root)
			{
				return (*edges[n])[(i << 1)];
			}
			return (*edges[n])[(i << 1) + 2];
		}

		virtual unsigned int getChildNum(unsigned int n)
		{
			if (n == root) 
			{
				return (edges[n]->size() >> 1);
			}
			return (edges[n]->size() >> 1 ) - 1;
		}

		virtual bool hasEdge(unsigned int n, unsigned int m)
		{
			if (getParent(n) == m)
			{
				return true;
			}

			return getParent(m) == n;
		}

		virtual unsigned int getEdgeNum(unsigned int n) 
		{
			return edges[n]->size() >> 1;
		}

		virtual unsigned int getOtherSideOfEdge(unsigned int n, unsigned int i)
		{
			return (*edges[n])[i << 1];
		}

		virtual unsigned int getTotalEdgesInGraph()
		{
			unsigned int tot = 0;

			for (unsigned int i = 0; i < size; i++) 
			{
				tot += edges[i]->size();
			}

			return tot >> 1;
		}

		unsigned int getWeightOfEdge(unsigned int n, unsigned int i)
		{
			return (*edges[n])[(i << 1) + 1];
		}

		unsigned int getWeightOfChild(unsigned int n, unsigned int i)
		{
			return (*edges[n])[(i << 1) + 3];
		}

		unsigned int getWeightOfParent(unsigned int n)
		{
			if (n == root)
			{
				throw 3;
			}
			return (*edges[n])[1];
		}

		/*RULE OF THREE*/
		WeightedTree(const WeightedTree& g) : Tree(g) 
		{

		}
	};

	//doesnt inherit from tree because it doesnt save any time or space, as every function would have to be overwritten
	template<class T>
	struct Forest : Graph<T>
	{
	private:
		//reset the root of the tree containing r to be r
		void resetRoot(unsigned int r) {
			unsigned int depth = 1;

			XORLinkedList::LinkedList<unsigned int> list;

			list.pushBackNode(r);

			unsigned char * numOn = new unsigned char[size] {0};

			//if it has no kids, dont bother doing all this
			if (getEdgeNum(list[list.size - 1]))
			{
				do {
					unsigned int endNode = list[list.size - 1];

					if (numOn[endNode] >= getEdgeNum(endNode))
					{
						levels[endNode] = depth;

						//make sure the right parent is the new parent
						if (getParent(endNode) != list[list.size - 2]) 
						{
							unsigned int secondToEnd = list[list.size - 2];
							for (unsigned int j = 1; j < edges[endNode]->size(); j++)
							{

								if ((*edges[endNode])[j] == secondToEnd)
								{
									unsigned int temp = getParent(endNode);

									(*edges[endNode])[0] = secondToEnd;
									(*edges[endNode])[j] = temp;

									break;
								}
							}
						}

						list.popBackNode();
						depth--;
					}
					else
					{
						unsigned int nodeToCheck = getOtherSideOfEdge(endNode, numOn[endNode]++);

						if (!numOn[nodeToCheck])
						{
							list.pushBackNode(nodeToCheck);
							depth++;
						}
					}
				} while (list.size > 1);

				delete[] numOn;
			}
		}

	public:
		std::vector<unsigned int> levels;

		Forest(unsigned int size = 10) : Graph(size)
		{

		}

		//construct it from a tree
		//note that the tree cant really be a forest, because even though it is allowed technically (for easy construction), it isnt a valid tree
		Forest(const Tree<T>& g) 
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++) {
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<Tree<T>&>(g).edges[i]->size(); j++) {

					edges[i]->push_back((*const_cast<Tree<T>&>(g).edges[i])[j]);
				}
			}

			levels = g.levels;
		}

		void addNode(T val) 
		{
			Graph::addNode(val);

			//it is its own parent
			levels.push_back(0);
		}

		void addEdge(unsigned int n, unsigned int m)
		{
			unsigned int r = getRoot(n);
			unsigned int k = getRoot(m);

			if (r == k)
			{
				//they have the same parent, so you just created a cycle
				throw 4;
			}

			levels[m] = levels[n] + 1;

			resetRoot(m);

			Graph::addEdge(n, m);

			//switch the first and last elements of the edge, so the parents are right
			unsigned int temp = getParent(m);
			(*edges[m])[0] = n;
			(*edges[m])[edges[m]->size() - 1] = temp;
		}

		void removeEdge(unsigned int n, unsigned int m)
		{
			unsigned int other = getOtherSideOfEdge(n, m);

			Graph::removeEdge(n, other);

			if (m)
			{
				setRoot(other);
			}
			else
			{
				setRoot(n);
			}

			//the child is now orphaned, so make a new tree
			if (getParent(n) == other)
			{
				Graph::removeEdge(n, other);

				setRoot(n);
			}
			else
			{
				Graph::removeEdge(n, other);

				setRoot(other);
			}
		}

		void removeNode(unsigned int num) 
		{

			while (edges[num]->size())
			{
				//delete from end to not have to scoot them
				removeEdge(num, edges[num]->size() - 1);
			}

			delete edges[num];

			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);
			levels.erase(begin(levels) + num);
			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++)
			{
				for (unsigned int j = edges[i]->size() - 1; j < edges[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--) 
				{
					if ((*edges[i])[j] == num) 
					{
						edges[i]->erase(begin(*edges[i]) + j);
					}
					else if ((*edges[i])[j] > num)
					{
						(*edges[i])[j]--;
					}
				}
			}

			size--;
		}
 
		//gets the root of the particular branch of the forest
		unsigned int getRoot(unsigned int n) 
		{
			for (;;)
			{
				if (!levels[n]) 
				{
					return n;
				}
				else 
				{
					n = (*edges[n])[0];
				}
			}
		}

		//same as above, excdpt it makes it a true root, instead of preparing it to be joined by an add edge
		//set the root of the tree containing r to be r
		void setRoot(unsigned int r) 
		{
			levels[r] = 0;

			unsigned int depth = 0;

			XORLinkedList::LinkedList<unsigned int> list;

			list.pushBackNode(r);

			unsigned char * numOn = new unsigned char[size] {0};

			//if it has no kids, dont bother doing all this
			if (getEdgeNum(list[list.size - 1])) 
			{
				do 
				{
					unsigned int endNode = list[list.size - 1];

					if (numOn[endNode] >= getEdgeNum(endNode)) 
					{
						levels[endNode] = depth;

						//make sure the right parent is the new parent
						if (getParent(endNode) != list[list.size - 2])
						{
							unsigned int secondToEnd = list[list.size - 2];
							for (unsigned int j = 1; j < edges[endNode]->size(); j++)
							{

								if ((*edges[endNode])[j] == secondToEnd)
								{
									unsigned int temp = getParent(endNode);

									(*edges[endNode])[0] = secondToEnd;
									(*edges[endNode])[j] = temp;

									break;
								}
							}
						}

						list.popBackNode();
						depth--;
					}
					else 
					{
						unsigned int nodeToCheck = getOtherSideOfEdge(endNode, numOn[endNode]++);

						if (!numOn[nodeToCheck])
						{
							list.pushBackNode(nodeToCheck);
							depth++;
						}
					}
				} while (list.size > 1);

				delete[] numOn;
			}
		}

		unsigned int getParent(unsigned int n)
		{
			if (levels[n])
			{
				return (*edges[n])[0];
			}
			return -1;
		}

		unsigned int getChild(unsigned int n, unsigned int child) 
		{
			return (*edges[n])[child + 1];
		}

		unsigned int getChildNum(unsigned int n)
		{
			if (n == root)
			{
				return edges[n]->size();
			}
			return edges[n]->size() - 1;
		}

		bool hasChild(unsigned int n, unsigned int child) 
		{
			return getParent(child) == n;
		}

		bool hasEdge(unsigned int n, unsigned int m)
		{
			if (getParent(n) == m) 
			{
				return true;
			}

			return getParent(m) == n;
		}

		/*RULE OF THREE*/
		Forest(const Forest& g)
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<Forest&>(g).edges[i]->size(); j++) 
				{

					edges[i]->push_back((*const_cast<Forest&>(g).edges[i])[j]);
				}
			}

			levels = g.levels;
		}

		virtual Forest operator=(const Forest&) 
		{
			Forest<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 1; i < this->size; i++)
			{
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++) 
			{
				for (unsigned int j = 0; j < this->edges[i]->size(); j++)
				{
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			ret.levels = levels;

			return ret;
		}
	};

	template<class T>
	struct WeightedGraph : virtual public Graph<T>
	{
	public:
		WeightedGraph(unsigned int size = 10) : Graph(size) {  }

		unsigned int getTotalEdgesInGraph()
		{
			return Graph::getTotalEdgesInGraph() / 2;
		}

		//number of edges connected to vertex _i
		unsigned int getEdgeNum(int _i)
		{
			return (*edges[_i]).size() / 2;
		}

		//get the node pointed to by the _edge th edge of _node
		unsigned int getOtherSideOfEdge(unsigned int _node, unsigned int _edge)
		{
			return (*edges[_node])[_edge * 2];
		}

		//get weight of edge between n and m
		virtual unsigned int getWeightOfEdge(unsigned int n, unsigned int m)
		{
			if (edges[n]->size() < edges[m]->size()) {
				for (unsigned int i = 0; i < edges[n]->size(); i += 2)
				{
					if ((*edges[n])[i] == m) return (*edges[n])[i + 1];
				}
			}

			for (unsigned int i = 0; i < edges[m]->size(); i += 2)
			{
				if ((*edges[m])[i] == n) return (*edges[m])[i + 1];
			}

			return -1;
		}

		//get weight of posth edges of node n
		virtual unsigned int getWeightOfEdgeByPos(unsigned int n, unsigned int pos)
		{
			return (*edges[n])[pos + 1];
		}

		//add an edge by weight
		void addEdge(unsigned int index1, unsigned int index2, unsigned int weight = 1)
		{
			edges[index1]->push_back(index2);
			edges[index2]->push_back(index1);

			edges[index1]->push_back(weight);
			edges[index2]->push_back(weight);
		}

		void removeNode(unsigned int num)
		{
			delete edges[num];

			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++)
			{
				for (unsigned int j = edges[i]->size() - 2; j < edges[i]->size(); j -= 2)
				{
					if ((*edges[i])[j] == num)
					{
						edges[i]->erase(begin(*edges[i]) + j, begin(*edges[i]) + j + 2);//delete edge num and weight
					}
					else if ((*edges[i])[j] > num)
					{
						(*edges[i])[j]--;
					}
				}
			}

			size--;
		}

		void removeEdge(unsigned int node1, unsigned int node2)
		{
			for (unsigned int i = 0; i < edges[node1]->size(); i += 2)
			{
				if ((*edges[node1])[i] == node2)
				{
					edges[node1]->erase(edges[node1]->begin() + i, edges[node1]->begin() + i + 2);
					break;
				}
			}

			for (unsigned int i = 0; i < edges[node2]->size(); i += 2)
			{
				if ((*edges[node2])[i] == node1)
				{
					edges[node2]->erase(edges[node2]->begin() + i, edges[node2]->begin() + i + 2);
					break;
				}
			}
		}

		//return true if the graph contains that edge
		virtual bool hasEdge(unsigned int node1, unsigned int node2)
		{
			for (unsigned int i = 0; i < edges[node1]->size(); i += 2)
			{
				if ((*edges[node1])[i] == node2) 
				{
					return true;
				}
			}

			return false;
		}

		virtual void display()
		{
			for (unsigned int i = 0; i < nodes.size(); i++)
			{
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < edges[i]->size(); j += 2)
				{
					std::cout << "{ " << (*edges[i])[j] << ", " << (*edges[i])[j + 1] << " }, ";
				}

				std::cout << std::endl;
			}
		}

		//copy constructor
		WeightedGraph(const WeightedGraph& g)
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++) 
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedGraph&>(g).getEdgeNum(i); j++)
				{

					(*edges[i]).push_back(const_cast<WeightedGraph&>(g).getOtherSideOfEdge(i, j));
					edges[i]->push_back(const_cast<WeightedGraph&>(g).getWeightOfEdge(i, const_cast<WeightedGraph&>(g).getOtherSideOfEdge(i, j)));
				}
			}
		}
	};

	template<class T>
	struct DirectedGraph : virtual public Graph<T>
	{
	private:

	public:
		std::vector<std::vector<unsigned int> * > parents;//nodes leading to node n


		DirectedGraph(unsigned int size = 10) : Graph(size) 
		{
			parents.reserve(size);
		};


		//number of children pointed to by node _i
		//not deleting because it has a better name in the base class
		virtual unsigned int getChildNum(int _i)
		{
			return (*edges[_i]).size();
		}

		//number of edges leading to node _i
		virtual unsigned int getParentNum(int _i) 
		{
			return (*parents[_i]).size();
		}

		//sum of node _i's parents and children
		unsigned int getTotalEdgeNum(int _i)
		{
			return getChildNum(_i) + getParentNum(_i);
		}

		//get the node pointed to by the _edge th edge of _node
		unsigned int getChild(unsigned int _node, unsigned int _edge)
		{
			return (*edges[_node])[_edge];
		}

		//get the _edgth parent of _node
		unsigned int getParent(unsigned int _node, unsigned int _edge)
		{
			return (*parents[_node])[_edge];
		}

		//get number of edges in graph
		unsigned int getTotalEdgesInGraph() 
		{
			return 2 * Graph::getTotalEdgesInGraph();
		}

		/*ADD AND REMOVE*/
		void addNode(T nodeVal) 
		{
			Graph::addNode(nodeVal);

			parents.push_back(new std::vector<unsigned int>(0));

			(*(parents.end() - 1))->reserve(10);//default val, may be updated later
		}

		void addEdge(unsigned int parent, unsigned int child)
		{
			edges[parent]->push_back(child);
			parents[child]->push_back(parent);
		}

		virtual void removeNode(unsigned int num)
		{
			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			Graph::removeNode(num);

			delete parents[num];

			parents.erase(begin(parents) + num);

			for (unsigned int i = 0; i < parents.size(); i++)
			{
				for (unsigned int j = parents[i]->size() - 1; j < parents[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--)
				{
					if ((*parents[i])[j] == num)
					{
						parents[i]->erase(begin(*parents[i]) + j);
					}
					else if ((*parents[i])[j] > num)
					{
						(*parents[i])[j]--;
					}
				}
			}
		}

		void removeEdge(unsigned int parent, unsigned int child)
		{
			for (unsigned int i = 0; i < edges[parent]->size(); i++) 
			{
				if ((*edges[parent])[i] == child) 
				{
					edges[parent]->erase(edges[parent]->begin() + i);
					break;
				}
			}
			for (unsigned int i = 0; i < parents[child]->size(); i++)
			{
				if ((*parents[child])[i] == parent) 
				{
					parents[child]->erase(parents[child]->begin() + i);
					break;
				}
			}
		}

		virtual void displayParents() {
			for (unsigned int i = 0; i < nodes.size(); i++)
			{
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < parents[i]->size(); j++)
				{
					std::cout << (*parents[i])[j] << " ";
				}

				std::cout << std::endl;
			}
		}

		/*RULE OF THREE*/
		DirectedGraph(const DirectedGraph& g)
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++) 
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<DirectedGraph&>(g).getChildNum(i); j++) 
				{

					(*edges[i]).push_back(const_cast<DirectedGraph&>(g).getChild(i, j));
				}

				for (unsigned int j = 0; j < const_cast<DirectedGraph&>(g).getParentNum(i); j++) 
				{

					(*parents[i]).push_back(const_cast<DirectedGraph&>(g).getParent(i, j));
				}
			}
		}

		//a slicing copy constructor to turn a weighted directed graph into a normal directed one
		DirectedGraph(const WeightedDirectedGraph<T>& g)
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++) 
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph<T>&>(g).edges[i]->size(); j += 2)
				{

					(*edges[i]).push_back((*const_cast<WeightedDirectedGraph<T>&>(g).edges[i])[j]);
				}

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph<T>&>(g).parents[i]->size(); j += 2) 
				{

					(*parents[i]).push_back((*const_cast<WeightedDirectedGraph<T>&>(g).parents[i])[j]);
				}
			}
		}

		DirectedGraph operator=(const DirectedGraph&)
		{
			DirectedGraph<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 0; i < this->size; i++)
			{
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++)
			{
				for (unsigned int j = 0; j < this->edges[i]->size(); j++) 
				{
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			for (unsigned int i = 0; i < this->parents.size(); i++)
			{
				for (unsigned int j = 0; j < this->parents[i]->size(); j++) 
				{
					ret.parents[i]->push_back((*parents[i])[j]);
				}
			}

			return ret;
		}

		~DirectedGraph()
		{
			for (auto i : parents) 
			{
				delete i;
			}
		}
	};

	template<class T>
	struct WeightedDirectedGraph : public DirectedGraph<T>, public WeightedGraph<T> 
	{
		WeightedDirectedGraph(unsigned int size = 10) : DirectedGraph(size) {  };


		//number of children pointed to by node _i
		virtual unsigned int getChildNum(int _i)
		{
			return (*edges[_i]).size() / 2;
		}

		//number of edges leading to node _i
		virtual unsigned int getParentNum(int _i)
		{
			return parents[_i]->size() / 2;
		}

		//get the node pointed to by the _edge th edge of _node
		unsigned int getChild(unsigned int _node, unsigned int _edge)
		{
			return (*edges[_node])[_edge * 2];
		}

		//get the _edgth parent of _node
		unsigned int getParent(unsigned int _node, unsigned int _edge)
		{
			return (*parents[_node])[2 * _edge];
		}

		//get weight of edge between n and m
		virtual unsigned int getWeightOfEdge(unsigned int n, unsigned int m) 
		{
			if (edges[n]->size() < edges[m]->size()) {

				for (unsigned int i = 0; i < edges[n]->size(); i += 2) 
				{
					if ((*edges[n])[i] == m) return (*edges[n])[i + 1];
				}

				for (unsigned int i = 0; i < parents[n]->size(); i += 2)
				{
					if ((*parents[n])[i] == m) return (*parents[n])[i + 1];
				}
			}

			for (unsigned int i = 0; i < edges[m]->size(); i += 2)
			{
				if ((*edges[m])[i] == n) return (*edges[m])[i + 1];
			}

			for (unsigned int i = 0; i < parents[m]->size(); i += 2)
			{
				if ((*parents[m])[i] == n) return (*parents[m])[i + 1];
			}

			return -1;
		}

		//get weight of posth edges of node n
		virtual unsigned int getWeightOfEdgeByPos(unsigned int n, unsigned int pos) 
		{
			return (*edges[n])[2*pos + 1];
		}

		//get weight of edge by pos from the parent vector, performance improvement
		virtual unsigned int getWeightOfEdgeFromParentByPos(unsigned int child, unsigned int parentNum) 
		{
			return (*parents[child])[2 * parentNum + 1];
		}

		//get the endpoints of the ith edge of the graph in the order (parent, child)
		std::pair<unsigned int, unsigned int> getEndpointsOfEdge(unsigned int i) 
		{
			unsigned int j = 0;
			for (;;)
			{
				if(i * 2 >= edges[j]->size()){
					i -= edges[j]->size() / 2;
					j++;
				}
				else
				{
					return std::make_pair( j, (*edges[j])[2 * i] );
				}
			}
		}

		//get number of edges in graph
		unsigned int getTotalEdgesInGraph()
		{
			return Graph::getTotalEdgesInGraph();
		}

		/*ADD AND REMOVE*/
		void addNode(T val)
		{
			Graph::addNode(val);

			parents.push_back(new std::vector<unsigned int>(0));

			(*(parents.end() - 1))->reserve(10);//default val, may be updated later
		}

		virtual void addEdge(unsigned int parent, unsigned int child, unsigned int weight)
		{
			edges[parent]->push_back(child);
			parents[child]->push_back(parent);

			edges[parent]->push_back(weight);
			parents[child]->push_back(weight);
		}

		void removeNode(unsigned int num) 
		{
			WeightedGraph::removeNode(num);

			delete parents[num];

			parents.erase(begin(parents) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < parents.size(); i++)
			{
				for (unsigned int j = parents[i]->size() - 2; j < parents[i]->size(); j -= 2) {
					if ((*parents[i])[j] == num) 
					{
						parents[i]->erase(begin(*parents[i]) + j, begin(*parents[i]) + j + 2);
					}
					else if ((*parents[i])[j] > num)
					{
						(*parents[i])[j]--;
					}
				}
			}
		}

		void removeEdge(unsigned int parent, unsigned int child)
		{
			for (unsigned int i = 0; i < edges[parent]->size(); i += 2)
			{
				if ((*edges[parent])[i] == child) {
					edges[parent]->erase(edges[parent]->begin() + i, edges[parent]->begin() + i + 2);
					break;
				}
			}
			for (unsigned int i = 0; i < parents[child]->size(); i += 2) 
			{
				if ((*parents[child])[i] == parent) 
				{
					parents[child]->erase(parents[child]->begin() + i, parents[child]->begin() + i + 2);
					break;
				}
			}
		}

		virtual bool hasEdge(unsigned int node1, unsigned int node2)
		{
			return WeightedGraph::hasEdge(node1, node2);
		}

		virtual void displayParents() {
			for (unsigned int i = 0; i < nodes.size(); i++)
			{
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < parents[i]->size(); j += 2) 
				{
					std::cout << "{ " << (*parents[i])[j] << ", " << (*parents[i])[j + 1] << " }, ";
				}

				std::cout << std::endl;
			}
		}

		/*RULE OF THREE*/
		WeightedDirectedGraph(const WeightedDirectedGraph& g)
		{
			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph&>(g).edges[i]->size(); j++) 
				{

					(*edges[i]).push_back((*const_cast<WeightedDirectedGraph&>(g).edges[i])[j]);
				}

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph&>(g).parents[i]->size(); j++) 
				{

					(*parents[i]).push_back((*const_cast<WeightedDirectedGraph&>(g).parents[i])[j]);
				}
			}
		}

		WeightedDirectedGraph operator=(const WeightedDirectedGraph&) 
		{
			WeightedDirectedGraph<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 0; i < this->size; i++)
			{
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++)
			{
				for (unsigned int j = 0; j < this->edges[i]->size(); j++) 
				{
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			for (unsigned int i = 0; i < this->parents.size(); i++)
			{
				for (unsigned int j = 0; j < this->parents[i]->size(); j++) 
				{
					ret.parents[i]->push_back((*parents[i])[j]);
				}
			}

			return ret;
		}

	};

	template<class T>
	struct ResidualGraph : WeightedDirectedGraph<T> 
	{
	public:
		unsigned int start, end;

		std::vector<std::vector<unsigned int> * > flows;

		void addEdge(unsigned int parent, unsigned int child, unsigned int weight)
		{
			WeightedDirectedGraph::addEdge(parent, child, weight);

			flows[parent]->push_back(0);
		}

		void addNode(T val) 
		{
			WeightedDirectedGraph::addNode(val);

			flows.push_back(new std::vector<unsigned int>(0));

			(*(flows.end() - 1))->reserve(10);
		}

		virtual void removeNode(unsigned int num)
		{
			delete edges[num];

			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++)
			{
				for (unsigned int j = edges[i]->size() - 2; j < edges[i]->size(); j -= 2) 
				{
					if ((*edges[i])[j] == num) 
					{
						edges[i]->erase(begin(*edges[i]) + j, begin(*edges[i]) + j + 2);//delete edge num and weight
						flows[i]->erase(begin(*flows[i]) + j / 2, begin(*flows[i]) + j / 2 + 1);
					}
					else if ((*edges[i])[j] > num) 
					{
						(*edges[i])[j]--;
					}
				}
			}

			size--;

			delete parents[num];

			parents.erase(begin(parents) + num);

			delete flows[num];

			flows.erase(begin(flows) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < parents.size(); i++) 
			{
				for (unsigned int j = parents[i]->size() - 2; j < parents[i]->size(); j -= 2) 
				{
					if ((*parents[i])[j] == num) {
						parents[i]->erase(begin(*parents[i]) + j, begin(*parents[i]) + j + 2);
					}
					else if ((*parents[i])[j] > num) 
					{
						(*parents[i])[j]--;
					}
				}
			}

		}

		//construct it from a weighted directed graph
		ResidualGraph(const WeightedDirectedGraph& g, unsigned int start, unsigned int end) : start(start), end(end)
		{
			nodes.reserve(g.size);
			edges.reserve(g.size);
			flows.reserve(g.size);

			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph&>(g).edges[i]->size(); j++)
				{
					//make sure we only add a flow for each node, not each weight
					if (!(j % 2))
					{
						(*flows[i]).push_back(0);
					}
	
					(*edges[i]).push_back((*const_cast<WeightedDirectedGraph&>(g).edges[i])[j]);
				}

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph&>(g).parents[i]->size(); j++)
				{

					(*parents[i]).push_back((*const_cast<WeightedDirectedGraph&>(g).parents[i])[j]);
				}
			}
		}

		//construct it from a directed graph
		ResidualGraph(const DirectedGraph& g, unsigned int start, unsigned int end) : start(start), end(end)
		{
			nodes.reserve(g.size);
			edges.reserve(g.size);
			flows.reserve(g.size);

			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<DirectedGraph&>(g).edges[i]->size(); j++)
				{
					(*flows[i]).push_back(0);

					(*edges[i]).push_back((*const_cast<DirectedGraph&>(g).edges[i])[j]);

					//all edges should have capacity one
					(*edges[i]).push_back(1);
				}

				for (unsigned int j = 0; j < const_cast<DirectedGraph&>(g).parents[i]->size(); j++) 
				{

					(*parents[i]).push_back((*const_cast<DirectedGraph&>(g).parents[i])[j]);
				}
			}
		}


		//construct a residual graph from a directed graph where every edge is represented by two edges, one pointing each way
		ResidualGraph(const Graph& g, unsigned int start, unsigned int end) : start(start), end(end) 
		{
			nodes.reserve(g.size);
			edges.reserve(g.size);
			flows.reserve(g.size);
			
			//add the nodes before the second step to prevent access violations
			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);
			}
			
			for (unsigned int i = 0; i < g.nodes.size(); i++) 
			{

				for (unsigned int j = 0; j < const_cast<Graph&>(g).edges[i]->size(); j++) 
				{
					(*flows[i]).push_back(0);

					(*edges[i]).push_back((*const_cast<Graph&>(g).edges[i])[j]);

					//all edges should have capacity one
					(*edges[i]).push_back(1);

					//start has no parents
					if (!(*const_cast<Graph&>(g).edges[i])[j] == start)
					{
						//add the node as a parent of the other
						(*parents[(*const_cast<Graph&>(g).edges[i])[j]]).push_back(i);
					}
				}
			}
		}

		//construct a network flow from a bipartite graph, such that each edge in the graph g points from partition A to B (that is, make them all directed the same way)
		//and add an s node that connects to the nodes in the first partition, and a t node that is led to by all nodes in the second partition
		ResidualGraph(const BipartiteGraph<T>& g) : start(g.size), end(g.size + 1)
		{
			nodes.reserve(g.size);
			edges.reserve(g.size);
			flows.reserve(g.size);

			//only do this for the ones in the first partition
			for (unsigned int i = 0; i < g.firstPartitionSize; i++) 
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<BipartiteGraph<T>&>(g).edges[i]->size(); j++) 
				{
					(*flows[i]).push_back(0);

					(*edges[i]).push_back((*const_cast<BipartiteGraph<T>&>(g).edges[i])[j]);

					//add a weight of one to an edge
					(*edges[i]).push_back(1);
				}
			}

			//for the others, just add them without edges
			for (unsigned int i = g.firstPartitionSize; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);
			}

			//start node, value doesnt matter
			this->addNode(nodes[0].obj);

			for (unsigned int i = 0; i < g.firstPartitionSize; i++)
			{
				//connect it to all nodes in first partition
				(*(edges.end() - 1))->push_back(i);
				(*(edges.end() - 1))->push_back(1);

				(*(flows.end() - 1))->push_back(0);
			}

			//sink node
			this->addNode(nodes[0].obj);

			for (unsigned int i = g.firstPartitionSize; i < g.nodes.size(); i++)
			{
				//connect all nodes to it
				edges[i]->push_back(nodes.size() - 1);
				edges[i]->push_back(1);

				flows[i]->push_back(0);
			}
		}

		//make sure it doesnt use its parent constructor
		ResidualGraph(unsigned int size = 10) = delete;

		unsigned int getFlow(unsigned int node, unsigned int edge)
		{
			return (*flows[node])[edge];
		}

		//get flow between the two nodes
		unsigned int getFlowBetweenNodes(unsigned int n, unsigned int m)
		{
			for (unsigned int i = 0; i < edges[n]->size(); i += 2) {
				if ((*edges[n])[i] == m) return getFlow(n, i / 2);
			}

			return -1;
		}

		//weight of edge with better name
		unsigned int getFlowCapacity(unsigned int node, unsigned int edge)
		{
			return WeightedDirectedGraph::getWeightOfEdgeByPos(node, edge);
		}

		//get how much more flow you can put through an edge
		unsigned int getResidualCapacity(unsigned int node, unsigned int edge)
		{
			return getFlowCapacity(node, edge) - getFlow(node, edge);
		}

		//get residual capacity between the two nodes
		unsigned int getResidualCapacityBetweenNodes(unsigned int n, unsigned int m)
		{
			for (unsigned int i = 0; i < edges[n]->size(); i += 2)
			{
				if ((*edges[n])[i] == m) return (*edges[n])[i + 1] - getFlow(n, i / 2);
			}

			return -1;
		}

		//add flow to an edge from n to m
		virtual void addFlow(unsigned int n, unsigned int m, int flow)
		{
			for (unsigned int i = 0; i < edges[n]->size(); i += 2) 
			{
				if ((*edges[n])[i] == m) 
				{
					(*flows[n])[i/2] += flow;
					return;
				}
			}
		}

		//copy constructor
		ResidualGraph(const ResidualGraph& g) 
		{
			nodes.reserve(g.size);
			edges.reserve(g.size);
			flows.reserve(g.size);

			for (unsigned int i = 0; i < g.nodes.size(); i++)
			{
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<ResidualGraph&>(g).edges[i]->size(); j++) 
				{
					//only want to push back a flow for every node name, not the weights
					if (!(j % 2)) 
					{
						(*flows[i]).push_back(0);
					}
					
					(*edges[i]).push_back((*const_cast<ResidualGraph&>(g).edges[i])[j]);
				}

				for (unsigned int j = 0; j < const_cast<ResidualGraph&>(g).parents[i]->size(); j++) 
				{

					(*parents[i]).push_back((*const_cast<ResidualGraph&>(g).parents[i])[j]);
				}
			}
		}

		~ResidualGraph() 
		{
			for (auto i : flows)
			{
				delete i;
			}
		}
	};

	template<class T>
	struct Preflow : ResidualGraph<T>
	{

		std::vector<unsigned int> labels;

		//construct if from a weighted directed graph
		Preflow(const WeightedDirectedGraph& g, unsigned int start, unsigned int end) : ResidualGraph(g, start, end)
		{
			labels.resize(g.size);
			labels[start] = g.size;

			//initialize all edges leaving the start node to maximum flow
			for (unsigned int i = 0; i < const_cast<WeightedDirectedGraph&>(g).getChildNum(start); i++)
			{
				(*flows[start])[i] += const_cast<WeightedDirectedGraph&>(g).getWeightOfEdgeByPos(start, i);
			}
		}

		//push flow along an edge e
		unsigned int push(unsigned int node, unsigned int edge, unsigned int excess, bool isForwards = true) 
		{
			if (isForwards)
			{
				//take away either the excess of the edge of the capacity of the edge
				unsigned int delta = std::min(excess, ResidualGraph::getResidualCapacity(node, edge));
				(*flows[node])[edge] += delta;
				
				return delta;
			}
			
			unsigned int delta = std::min(excess, ResidualGraph::getFlow(node, edge));
			(*flows[node])[edge] -= delta;

			return delta;
		}

		//raise the height of n by one
		void relabel(unsigned int n)
		{
			labels[n]++;
		}

		//make sure you cant construct it from scratch
		Preflow(unsigned int i = 10) = delete;

		//make sure you can;t accidentally add flow using the residual graph method
		virtual void addFlow(unsigned int m, unsigned int n, unsigned int flow) = delete;
	};

	template<class T>
	struct BinaryTree : public Graph<T>
	{
	private:

	public:
		BinaryTree(unsigned int size = 10) : Graph(size) {  }

		/*ADD AND REMOVE*/
		void addNode(T nodeVal, unsigned int parentNode)
		{
			size++;

			nodes.push_back(Node<T>(nodeVal));
			edges.push_back(new std::vector<unsigned int>(0));
			
			(*(edges.end() - 1))->reserve(2);//default val, may be updated later
			
			if (nodes.size() - 1) 
			{
				if (edges[parentNode]->size() >= 3) throw 0;

				//add the edge in 
				edges[parentNode]->push_back(nodes.size() - 1);
				edges[nodes.size() - 1]->push_back(parentNode);
			}
		}

		//add a node onto parent whos nodes obj is parentNodeVal
		void addNodeOnParent(T nodeVal, T parentNodeVal)
		{
			size++;

			nodes.push_back(Node<T>(nodeVal));
			edges.push_back(new std::vector<unsigned int>(0));

			(*(edges.end() - 1))->reserve(2);//default val, may be updated later

			for (unsigned int i = nodes.size() - 2; i < nodes.size(); i--)
			{
				if (nodes[i].obj == parentNodeVal)
				{
					if (edges[i]->size() >= 3) throw 0;

					edges[i]->push_back(nodes.size() - 1);
					edges[nodes.size() - 1]->push_back(i);


					break;
				}
			}
		}

		//it wont let me delete
		virtual void addEdge(unsigned int index1, unsigned int index2)
		{
			throw 0;
		}

		//CHANGE LATER BUT FOR NOW THE SAME AS ABOVE
		virtual void removeNode(unsigned int num) 
		{
			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++) 
			{
				for (unsigned int j = edges[i]->size() - 1; j < edges[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--) {
					if ((*edges[i])[j] == num)
					{
						edges[i]->erase(begin(*edges[i]) + j);
					}
					else if ((*edges[i])[j] > num)
					{
						(*edges[i])[j]--;
					}
				}
			}


			size--;
		}

		//it wont let me delete
		virtual void removeEdge(unsigned int node1, unsigned int node2) 
		{
			throw 0;
		}

		//return index of node with value val
		unsigned int getPosByValue(T val) 
		{
			for (unsigned int i = nodes.size() - 1; i < nodes.size(); i--)
			{
				if (nodes[i].obj == val) return i;
			}

			return -1;
		}

		//true means that it is the left child, false means it is the right one
		bool isLeftChild(unsigned int i) 
		{
			if (i < 3) return ((*edges[(*edges[i])[0]])[0] == i);

			//check if its parents second value (first child value) is it
			if ((*edges[(*edges[i])[0]])[1] == i) return true;

			return false;
		}
	};
}