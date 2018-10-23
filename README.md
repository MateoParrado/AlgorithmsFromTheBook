# AlgorithmsFromTheBook
This is a project that takes all of the theoretical concepts from Algorithm Design by Jon Kleinberg and Eva Tardos and puts them into practice. The goal was to improve my C++ skills, as well as to be able to write and understand more complex algorithms. It is written as a header only library because many of the functions and data structures are templated, and templates don't play well with .cpp files.

## Getting Started
TODO: download on a different computer and figure out how to get started

### Prerequisites
TODO: figure out if boost needs to be downloaded or if it is packaged in there with it

## How to use?
### Code Examples
Before starting with the algorithms, it is important to create data structures that model your situation correctly. This library provides structures for graphs, heaps, linked lists, and many variants of each. I will show how to handle these with the example of a simple graph.

First, include the appropriate header file.
```
#include "Graph.h"
```
Then instantiate a new object.
```
Graph::Graph<int> g(5);
```
In this case, the 5 in the parameter list of the consturctor signifies the number of nodes the graph will hold, however this number can be changed during runtime, and the parameters will vary by data structure.
The <int> signifies that every node of the graph will contain an integer. This is helpful for many reasons, such as creating graphs of cities, where the node could contain the name of the city.

The next step is to initialize the values of the nodes.
```
for(int i = 0; i < 5; i++){
  g.addNode(i);
}
```
This creates a graph of 5 nodes, where every node contains a number from zero to four inclusive.

However, the nodes are still not connected. 
Say that you want your graph to be a ring, with each node connected to the next node, and the first node connected to the last. 
You would do that as follows:
```
for(int i = 0; i < 6; i++){
  a.addEdge(i, (i + 1) % 6, i);
}
```
You now have a graph that is ready for use.
### Using the Algorithms
The use of the algorithms will be demonstrated using the graph we defined above and depth first search as an example.

As above, the first step is to include the header that contains the algorithm, and if the algorithm returns a data structure that you haven't included yet, include that as well.
Depth first search returns a singly linked list, and is defined int GraphAlgorithms.h so you would write the code as follows:
```
#include "GraphAlgorithms.h"
#include "LinkedList.h"
```

### Running the tests
TODO: show examples of tests and write how they help people
(they show what the expected output is for each algo, and show some examples)

## Built with
Miscoroft Visual Studio 2015 C/C++ Compiler

## License
This project is licensed under the MIT License. See LICENSE.md for more details
