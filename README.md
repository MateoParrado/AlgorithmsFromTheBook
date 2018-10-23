# AlgorithmsFromTheBook
This is a project that takes all of the theoretical concepts from Algorithm Design by Jon Kleinberg and Eva Tardos and puts them into practice. The goal was to improve my C++ skills, as well as to be able to write and understand more complex algorithms. It is written as a header only library because many of the functions and data structures are templated, and templates don't play well with .cpp files.

## Getting Started
Either clone or download the zip of the repository. Take the header files you need and put them into your working directory. 

### Prerequisites
Some of the header files require boost::multiarray for thier algorithms. If you are using an algorithm from those headers but don't need that algorithm, simply delete it. Otherwise, you can download it at www.boost.org.

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
The use of the algorithms will be demonstrated using the graph we defined above. We will find the path between node 0 and node 4 with depth first search.

As above, the first step is to include the header that contains the algorithm, and if the algorithm returns a data structure that you haven't included yet, include that as well.
Depth first search returns a singly linked list, and is defined int GraphAlgorithms.h so you would write the code as follows:
```
#include "GraphAlgorithms.h"
#include "LinkedList.h"
```
Then, you consult the comment above the algorithm's definition for an explanation of the parameters.
The comment above depth first search is:
```
//find path between node start and node end using DFS
```
The function definition on the next line takes a pointer to a graph g, an unsigned int start, and an unsigned int end. 
Using this information combined with what the comment said, you can deduce that the code should look like this:
```
SinglyLinkedList::LinkedList<int> path = depthFirstSearch(&g, 0, 4);
```

An important note is that whenever a function returns a pointer, it must be deleted to prevent a memory leak.
### Running the tests
The tests for this library were mainly used for development purposes. They verify the correctness of the algorithms by checking them against a couple of test cases. However, they can help you to better understand some of the algorithms by seeing example input and output.
For example, take the sequence alignment algorithm. If you don't understand how to use it, or what it does, from the comment above the function definition, simply open the tests header, Tests.h.

Then, look at the example input and output. For sequence alignment, there are two different inputs tested.

The first is "abc" and "def". You see that this returns an empty vector. 
The second is "abc", "acf". This returns a vector with two elements, (0, 0), and (2, 1).
From these two examples, you would be able to deduce that it returns pairs of the two strings that should be matched with each other in order to match them as well as possible. There are no matching charchters in "abc" and "def". However, in "abc" and "acf", the two As match, and the two Cs match.
You could also deduc that backwards sequence alignment and space efficient sequence alignment are simply variations of the same algorithm, as they have the same input and output.
## Built with
Miscoroft Visual Studio 2015 C/C++ Compiler

## License
This project is licensed under the MIT License. See LICENSE.md for more details
