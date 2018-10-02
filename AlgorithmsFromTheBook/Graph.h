#pragma once
#pragma warning(disable: 4250)//some weird inheritance dominance thing, could bite me in the ass later

#include <vector>
#include <iostream>

namespace Graph {
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
	struct Node {
		T obj;
		
		Node() { obj = T(); };

		//create node by pointer
		//only pinters allowed, must point to something already on the heap
		Node(T _obj) : obj(_obj) { };

		~Node() { }
	};

	template<class T>
	struct Graph {
	private:

	public:
		unsigned int size = 0;

		std::vector<Node<T>> nodes;
		std::vector<std::vector<unsigned int> * > edges;

		Graph(unsigned int size = 10) {
			nodes.reserve(size); //leave objs unitnitialized
			edges.reserve(size);
		};


		//number of edges connected to vertex _i
		virtual unsigned int getEdgeNum(int _i) {
			return (*edges[_i]).size();
		}

		//get the node pointed to by the _edge th edge of _node
		virtual unsigned int getOtherSideOfEdge(unsigned int _node, unsigned int _edge) {
			return (*edges[_node])[_edge];
		}

		//get number of edges in graph
		virtual unsigned int getTotalEdgesInGraph() {
			unsigned int ret = 0;
			for (unsigned int i = 0; i < edges.size(); i++) {
				ret += edges[i]->size();
			}

			return ret / 2;
		}

		T getObj(unsigned int index) {
			return nodes[index].obj;
		}

		virtual void display() {
			for (unsigned int i = 0; i < nodes.size(); i++) {
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < edges[i]->size(); j++) {
					std::cout << (*edges[i])[j] << " ";
				}

				std::cout << std::endl;
			}
		}

		/*ADD AND REMOVE*/
		virtual void addNode(T nodeVal) {
			size++;

			nodes.push_back(Node<T>(nodeVal));
			edges.push_back(new std::vector<unsigned int>(0));

			(*(edges.end() - 1))->reserve(10);//default val, may be updated later
		}

		virtual void addEdge(unsigned int index1, unsigned int index2) {
			edges[index1]->push_back(index2);
			edges[index2]->push_back(index1);
		}

		virtual void removeNode(unsigned int num) {
			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++) {
				for (unsigned int j = edges[i]->size() - 1; j < edges[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--) {
					if ((*edges[i])[j] == num) {
						edges[i]->erase(begin(*edges[i]) + j);
					}
					else if((*edges[i])[j] > num){
						(*edges[i])[j]--;
					}
				}
			}


			size--;
		}

		virtual void removeEdge(unsigned int node1, unsigned int node2) {
			for (unsigned int i = 0; i < edges[node2]->size(); i++) {
				if ((*edges[node1])[i] == node2) {
					edges[node1]->erase(edges[node1]->begin() + i);
					break;
				}
			}

			for (unsigned int i = 0; i < edges[node2]->size(); i++) {
				if ((*edges[node2])[i] == node1) {
					edges[node2]->erase(edges[node2]->begin() + i);
					break;
				}
			}
		}

		/*RULE OF THREE*/
		Graph(const Graph& g) {
			for (unsigned int i = 0; i < g.nodes.size(); i++) {
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<Graph&>(g).getEdgeNum(i); j++) {

					(*edges[i]).push_back(const_cast<Graph&>(g).getOtherSideOfEdge(i, j));
				}
			}
		}

		virtual Graph operator=(const Graph&) {
			Graph<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 0; i < this->size; i++) {
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++) {
				for (unsigned int j = 0; j < this->edges[i]->size(); j++) {
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			return ret;
		}

		virtual ~Graph() {
			for (auto i : edges) {
				delete i;
			}
		}
	};

	template<class T>
	struct WeightedGraph : virtual public Graph<T>{
	public:
		WeightedGraph(unsigned int size = 10) : Graph(size) {  }

		unsigned int getTotalEdgesInGraph() {
			return Graph::getTotalEdgesInGraph()/2;
		}

		//number of edges connected to vertex _i
		unsigned int getEdgeNum(int _i) {
			return (*edges[_i]).size() / 2;
		}

		//get the node pointed to by the _edge th edge of _node
		unsigned int getOtherSideOfEdge(unsigned int _node, unsigned int _edge) {
			return (*edges[_node])[_edge * 2];
		}

		//get weight of edge between n and m
		virtual unsigned int getWeightOfEdge(unsigned int n, unsigned int m) {
			if (edges[n]->size() < edges[m]->size()) {
				for (unsigned int i = 0; i < edges[n]->size(); i += 2) {
					if ((*edges[n])[i] == m) return (*edges[n])[i + 1];
				}
			}

			for (unsigned int i = 0; i < edges[m]->size(); i += 2) {
				if ((*edges[m])[i] == n) return (*edges[m])[i + 1];
			}

			return -1;
		}

		//get weight of posth edges of node n
		virtual unsigned int getWeightOfEdgeByPos(unsigned int n, unsigned int pos) {
			return (*edges[n])[pos + 1];
		}

		//add an edge by weight
		void addEdge(unsigned int index1, unsigned int index2, unsigned int weight = 1) {
			edges[index1]->push_back(index2);
			edges[index2]->push_back(index1);

			edges[index1]->push_back(weight);
			edges[index2]->push_back(weight);
		}

		void removeNode(unsigned int num) {
			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++) {
				for (unsigned int j = edges[i]->size() - 2; j < edges[i]->size(); j -= 2) {
					if ((*edges[i])[j] == num) {
						edges[i]->erase(begin(*edges[i]) + j, begin(*edges[i]) + j + 2);//delete edge num and weight
					}
					else if ((*edges[i])[j] > num) {
						(*edges[i])[j]--;
					}
				}
			}

			size--;
		}

		void removeEdge(unsigned int node1, unsigned int node2) {
			for (unsigned int i = 0; i < edges[node1]->size(); i += 2) {
				if ((*edges[node1])[i] == node2) {
					edges[node1]->erase(edges[node1]->begin() + i, edges[node1]->begin() + i + 2);
					break;
				}
			}

			for (unsigned int i = 0; i < edges[node2]->size(); i += 2) {
				if ((*edges[node2])[i] == node1) {
					edges[node2]->erase(edges[node2]->begin() + i, edges[node2]->begin() + i + 2);
					break;
				}
			}
		}

		virtual void display() {
			for (unsigned int i = 0; i < nodes.size(); i++) {
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < edges[i]->size(); j += 2) {
					std::cout << "{ " << (*edges[i])[j] << ", " << (*edges[i])[j+1] << " }, ";
				}

				std::cout << std::endl;
			}
		}

		//copy constructor
		WeightedGraph(const WeightedGraph& g) {
			for (unsigned int i = 0; i < g.nodes.size(); i++) {
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedGraph&>(g).getEdgeNum(i); j++) {

					(*edges[i]).push_back(const_cast<WeightedGraph&>(g).getOtherSideOfEdge(i, j));
					edges[i]->push_back(const_cast<WeightedGraph&>(g).getWeightOfEdge(i, const_cast<WeightedGraph&>(g).getOtherSideOfEdge(i, j)));
				}
			}
		}
	};

	template<class T>
	struct DirectedGraph : virtual public Graph<T>{
	private:

	public:
		std::vector<std::vector<unsigned int> * > parents;//nodes leading to node n


		DirectedGraph(unsigned int size = 10) : Graph(size) {
			parents.reserve(size);
		};


		//number of children pointed to by node _i
		//not deleting because it has a better name in the base class
		virtual unsigned int getChildNum(int _i) {
			return (*edges[_i]).size();
		}

		//number of edges leading to node _i
		virtual unsigned int getParentNum(int _i) {
			return (*parents[_i]).size();
		}

		//sum of node _i's parents and children
		unsigned int getTotalEdgeNum(int _i) {
			return getChildNum(_i) + getParentNum(_i);
		}

		//get the node pointed to by the _edge th edge of _node
		unsigned int getChild(unsigned int _node, unsigned int _edge) {
			return (*edges[_node])[_edge];
		}

		//get the _edgth parent of _node
		unsigned int getParent(unsigned int _node, unsigned int _edge) {
			return (*parents[_node])[_edge];
		}

		//get number of edges in graph
		unsigned int getTotalEdgesInGraph() {
			return 2 * Graph::getTotalEdgesInGraph();
		}

		/*ADD AND REMOVE*/
		void addNode(T nodeVal) {
			Graph::addNode(nodeVal);

			parents.push_back(new std::vector<unsigned int>(0));

			(*(parents.end() - 1))->reserve(10);//default val, may be updated later
		}

		void addEdge(unsigned int parent, unsigned int child) {
			edges[parent]->push_back(child);
			parents[child]->push_back(parent);
		}

		virtual void removeNode(unsigned int num) {
			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			Graph::removeNode(num);
			parents.erase(begin(parents) + num);

			for (unsigned int i = 0; i < parents.size(); i++) {
				for (unsigned int j = parents[i]->size() - 1; j < parents[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--) {
					if ((*parents[i])[j] == num) {
						parents[i]->erase(begin(*parents[i]) + j);
					}
					else if ((*parents[i])[j] > num) {
						(*parents[i])[j]--;
					}
				}
			}
		}

		void removeEdge(unsigned int parent, unsigned int child) {
			for (unsigned int i = 0; i < edges[parent]->size(); i++) {
				if ((*edges[parent])[i] == child) {
					edges[parent]->erase(edges[parent]->begin() + i);
					break;
				}
			}
			for (unsigned int i = 0; i < parents[child]->size(); i++) {
				if ((*parents[child])[i] == parent) {
					parents[child]->erase(parents[child]->begin() + i);
					break;
				}
			}
		}

		virtual void displayParents() {
			for (unsigned int i = 0; i < nodes.size(); i++) {
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < parents[i]->size(); j++) {
					std::cout << (*parents[i])[j] << " ";
				}

				std::cout << std::endl;
			}
		}

		/*RULE OF THREE*/
		DirectedGraph(const DirectedGraph& g) {
			for (unsigned int i = 0; i < g.nodes.size(); i++) {
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<DirectedGraph&>(g).getChildNum(i); j++) {

					(*edges[i]).push_back(const_cast<DirectedGraph&>(g).getChild(i, j));
				}

				for (unsigned int j = 0; j < const_cast<DirectedGraph&>(g).getParentNum(i); j++) {

					(*parents[i]).push_back(const_cast<DirectedGraph&>(g).getParent(i, j));
				}
			}
		}

		//a slicing copy constructor to turn a weighted directed graph into a normal directed one
		DirectedGraph(const WeightedDirectedGraph<T>& g) {
			for (unsigned int i = 0; i < g.nodes.size(); i++) {
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph<T>&>(g).edges[i]->size(); j += 2) {

					(*edges[i]).push_back((*const_cast<WeightedDirectedGraph<T>&>(g).edges[i])[j]);
				}

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph<T>&>(g).parents[i]->size(); j += 2) {

					(*parents[i]).push_back((*const_cast<WeightedDirectedGraph<T>&>(g).parents[i])[j]);
				}
			}
		}

		DirectedGraph operator=(const DirectedGraph&) {
			DirectedGraph<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 0; i < this->size; i++) {
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++) {
				for (unsigned int j = 0; j < this->edges[i]->size(); j++) {
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			for (unsigned int i = 0; i < this->parents.size(); i++) {
				for (unsigned int j = 0; j < this->parents[i]->size(); j++) {
					ret.parents[i]->push_back((*parents[i])[j]);
				}
			}

			return ret;
		}

		~DirectedGraph() {
			for (auto i : edges) {
				delete i;
			}
			for (auto i : parents) {
				delete i;
			}
		}
	};

	template<class T>
	struct WeightedDirectedGraph : public DirectedGraph<T>, public WeightedGraph<T> {
		WeightedDirectedGraph(unsigned int size = 10) : DirectedGraph(size) {  };


		//number of children pointed to by node _i
		virtual unsigned int getChildNum(int _i) {
			return (*edges[_i]).size() / 2;
		}

		//number of edges leading to node _i
		virtual unsigned int getParentNum(int _i) {
			return parents[_i]->size() / 2;
		}

		//get the node pointed to by the _edge th edge of _node
		unsigned int getChild(unsigned int _node, unsigned int _edge) {
			return (*edges[_node])[_edge * 2];
		}

		//get the _edgth parent of _node
		unsigned int getParent(unsigned int _node, unsigned int _edge) {
			return (*parents[_node])[2 * _edge];
		}

		//get weight of edge between n and m
		virtual unsigned int getWeightOfEdge(unsigned int n, unsigned int m) {
			if (edges[n]->size() < edges[m]->size()) {
				for (unsigned int i = 0; i < edges[n]->size(); i += 2) {
					if ((*edges[n])[i] == m) return (*edges[n])[i + 1];
				}

				for (unsigned int i = 0; i < parents[n]->size(); i += 2) {
					if ((*parents[n])[i] == m) return (*parents[n])[i + 1];
				}
			}

			for (unsigned int i = 0; i < edges[m]->size(); i += 2) {
				if ((*edges[m])[i] == n) return (*edges[m])[i + 1];
			}

			for (unsigned int i = 0; i < parents[m]->size(); i += 2) {
				if ((*parents[m])[i] == n) return (*parents[m])[i + 1];
			}

			return -1;
		}

		//get weight of posth edges of node n
		virtual unsigned int getWeightOfEdgeByPos(unsigned int n, unsigned int pos) {
			return (*edges[n])[pos + 1];
		}

		//get number of edges in graph
		unsigned int getTotalEdgesInGraph() {
			return Graph::getTotalEdgesInGraph();
		}

		/*ADD AND REMOVE*/
		void addNode(T val) {
			Graph::addNode(val);

			parents.push_back(new std::vector<unsigned int>(0));

			(*(parents.end() - 1))->reserve(10);//default val, may be updated later
		}

		virtual void addEdge(unsigned int parent, unsigned int child, unsigned int weight) {
			edges[parent]->push_back(child);
			parents[child]->push_back(parent);

			edges[parent]->push_back(weight);
			parents[child]->push_back(weight);
		}

		void removeNode(unsigned int num) {
			WeightedGraph::removeNode(num);

			parents.erase(begin(parents) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < parents.size(); i++) {
				for (unsigned int j = parents[i]->size() - 2; j < parents[i]->size(); j -= 2) {
					if ((*parents[i])[j] == num) {
						parents[i]->erase(begin(*parents[i]) + j, begin(*parents[i]) + j + 2);
					}
					else if ((*parents[i])[j] > num) {
						(*parents[i])[j]--;
					}
				}
			}
		}

		void removeEdge(unsigned int parent, unsigned int child) {
			for (unsigned int i = 0; i < edges[parent]->size(); i++) {
				if ((*edges[parent])[i] == child) {
					edges[parent]->erase(edges[parent]->begin() + i, edges[parent]->begin() + i + 2);
					break;
				}
			}
			for (unsigned int i = 0; i < parents[child]->size(); i++) {
				if ((*parents[child])[i] == parent) {
					parents[child]->erase(parents[child]->begin() + i, edges[child]->begin() + i + 2);
					break;
				}
			}
		}

		virtual void displayParents() {
			for (unsigned int i = 0; i < nodes.size(); i++) {
				std::cout << "NODE " << i << ": ";

				for (unsigned int j = 0; j < parents[i]->size(); j += 2) {
					std::cout << "{ " << (*parents[i])[j] << ", " << (*parents[i])[j + 1] << " }, ";
				}

				std::cout << std::endl;
			}
		}

		/*RULE OF THREE*/
		WeightedDirectedGraph(const WeightedDirectedGraph& g) {
			for (unsigned int i = 0; i < g.nodes.size(); i++) {
				this->addNode(g.nodes[i].obj);

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph&>(g).edges[i]->size(); j++) {

					(*edges[i]).push_back((*const_cast<WeightedDirectedGraph&>(g).edges[i])[j]);
				}

				for (unsigned int j = 0; j < const_cast<WeightedDirectedGraph&>(g).parents[i]->size(); j++) {

					(*parents[i]).push_back((*const_cast<WeightedDirectedGraph&>(g).parents[i])[j]);
				}
			}
		}

		WeightedDirectedGraph operator=(const WeightedDirectedGraph&) {
			WeightedDirectedGraph<T> ret(this->size);

			//copy over the node values
			for (unsigned int i = 0; i < this->size; i++) {
				ret.addNode(nodes[i].obj);
			}

			for (unsigned int i = 0; i < this->edges.size(); i++) {
				for (unsigned int j = 0; j < this->edges[i]->size(); j++) {
					ret.edges[i]->push_back((*edges[i])[j]);
				}
			}

			for (unsigned int i = 0; i < this->parents.size(); i++) {
				for (unsigned int j = 0; j < this->parents[i]->size(); j++) {
					ret.parents[i]->push_back((*parents[i])[j]);
				}
			}

			return ret;
		}

	};

	template<class T>
	struct BinaryTree : public Graph<T> {
	private:

	public:
		BinaryTree(unsigned int size = 10) : Graph(size) {  }

		/*ADD AND REMOVE*/
		void addNode(T nodeVal, unsigned int parentNode) {
			size++;

			nodes.push_back(Node<T>(nodeVal));
			edges.push_back(new std::vector<unsigned int>(0));
			
			(*(edges.end() - 1))->reserve(2);//default val, may be updated later
			
			if (nodes.size() - 1) {
				if (edges[parentNode]->size() >= 3) throw 0;

				//add the edge in 
				edges[parentNode]->push_back(nodes.size() - 1);
				edges[nodes.size() - 1]->push_back(parentNode);
			}
		}

		//add a node onto parent whos nodes obj is parentNodeVal
		void addNodeOnParent(T nodeVal, T parentNodeVal){
			size++;

			nodes.push_back(Node<T>(nodeVal));
			edges.push_back(new std::vector<unsigned int>(0));

			(*(edges.end() - 1))->reserve(2);//default val, may be updated later

			for (unsigned int i = nodes.size() - 2; i < nodes.size(); i--) {
				if (nodes[i].obj == parentNodeVal) {
					if (edges[i]->size() >= 3) throw 0;

					edges[i]->push_back(nodes.size() - 1);
					edges[nodes.size() - 1]->push_back(i);


					break;
				}
			}
		}

		//it wont let me delete
		virtual void addEdge(unsigned int index1, unsigned int index2) {
			throw 0;
		}

		//CHANGE LATER BUT FOR NOW THE SAME AS ABOVE
		virtual void removeNode(unsigned int num) {
			edges.erase(begin(edges) + num);
			nodes.erase(begin(nodes) + num);

			//remove all links to it in its dependencies, as well as reduce the number of each one by one (beacuse it now has one less node)
			for (unsigned int i = 0; i < edges.size(); i++) {
				for (unsigned int j = edges[i]->size() - 1; j < edges[i]->size() /*because its unsigned, when it hits negative one it goes super positive*/; j--) {
					if ((*edges[i])[j] == num) {
						edges[i]->erase(begin(*edges[i]) + j);
					}
					else if ((*edges[i])[j] > num) {
						(*edges[i])[j]--;
					}
				}
			}


			size--;
		}

		//it wont let me delete
		virtual void removeEdge(unsigned int node1, unsigned int node2) {
			throw 0;
		}

		//return index of node with value val
		unsigned int getPosByValue(T val) {
			for (unsigned int i = nodes.size() - 1; i < nodes.size(); i--) {
				if (nodes[i].obj == val) return i;
			}

			return -1;
		}

		//true means that it is the left child, false means it is the right one
		bool isLeftChild(unsigned int i) {
			if (i < 3) return ((*edges[(*edges[i])[0]])[0] == i);

			//check if its parents second value (first child value) is it
			if ((*edges[(*edges[i])[0]])[1] == i) return true;

			return false;
		}
	};
}