# AlgorithmsFromTheBook
This is a library of algorithms taken from the book Algorithm Design by Jon Kleinberg and Eva Tardos. The goal of the project was to help me learn C++, as well as to be able to understand more complex data structures and algorithms. It contains a wide variety of data structures, from hash maps to graphs to bit arrays, and an even wider variety of algorithms that build off these data structures, covering topics such as dynamic programming and recursion. Please feel free to submit any bug reports, or any feature requests, in the contribution guidelines!

### Organization
This is  a headers only library, because all of the data structures are templated. Every data structure has its own header file, that it shares with its derived classes. These classes are all under the same namespace as well. For example, there are many structs under the Graph header, including Graph, DirectedGraph, WeightedGraph, and many more.

One apparent exception to this rule is found in the linked list header. SinglyLinkedList, DoublyLinkedList and XORLinkedList are all in different namespaces. Singly and doubly linked lists rely on different node objects. This means that doubly linked list cant inherit from singly linked list as it would have useless pointers. Singly linked list and xor linked list rely on the same internal node object, but every one of singly linked list's functions would have needed to be overrided by xor linked list, which would have just slowed the computer down through the use of virtual functions. Because of this, each kind of linked list has its own namespace, yet they all share the same header.

All namespaces and data structures use Pascal case.

Many algorithms are in headers based on the data structure they deal with. For example, there is a header for algorithms that have to do with graphs, and another for algorithms that have to do with network flows. These headers are subdivided into categories based on the purpose of the algorithm, like minimum spanning tree algorithms, or maximum flow algorithms. Algorithms that are less connected to specific data structures (though they still may use them) are in headers based off of their purpose, such as data compression or clustering. 

All algorithms use camel case.

Another thing to note is the FlightStruct header. This header defines a small struct used in an algorithm defined in NetworkFlowAlgorithms.h. It is included in its own header so it doesn't clutter the already very large header, even though other small data structures are typically defined in the header with their appropriate algorithms.

Each assembly language function is included in its own .asm file. Assemply procedures are named using snake case, and preceeded by an asm_.

## Getting Started
Either clone or download the zip of the repository. Then simply take the header files you need and place them in your working directory, deleting all algorithms you don't need.

### Prerequisites
The knapsack packer algorithm, the RNA structure finder algorithm, and the sequence alignment algorithm all use boost::multiarray. If you need to use any of those algorithms, simply download the header at www.boost.org. If you don't want to use those specific algorithms, but still want to use something in their header, simply delete the algorithms and remove the include from the top, and your program will run as intended.

All assembly language functions are written for the x86 architecture, and some will only be able to run in 32 bit mode.

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
The <int> signifies that every node of the graph will contain an integer. This is helpful for many reasons, such as creating graphs of cities, where the node could contain a string of the name of the city.

The next step is to initialize the values of the nodes.
```
for(int i = 0; i < 5; i++){
  g.addNode(i);
}
```
The graph does not create the nodes upon instantiation, so you need to add them in manually. 

However, the nodes are still not connected. 
Say that you want your graph to be a ring, with each node connected to the next node, and the first node connected to the last. 
You would do that as follows:
```
for(int i = 0; i < 5; i++){
  a.addEdge(i, (i + 1) % 5);
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
Using this information, you can deduce that the code should look like this:
```
SinglyLinkedList::LinkedList<int> path = depthFirstSearch(&g, 0, 4);
```

An important note is that whenever a function returns a pointer, it must be deleted to prevent a memory leak. Some later algorithms implement smart pointers to avoid this issue, or return through move semantics. 

### Running Assembly Procedures
Assembly procedures have comments around their names describing their function, and stating their parameters and return values. For example, the asm_xorshift procedure definition looks like this.

```
;--------------------------------------------------------
asm_xorshift PROC
;
; Function that generates a pseudorandom number
; using a the xorshift method, a type of linear
; feedback shift register. Note that the numbers
; do not appear fully random, and are not usable 
; at full size. They should be modded down to a 
; smaller number. At least the first 16 bits 
; should be removed.
; 
; Receives:
;	a seed dword on the stack
; 
; Returns: 
;	EAX: a pseudorandom number
;--------------------------------------------------------
```

However, before we can use this function, we need to define it in our C code.

```
extern "C" int asm_xorshift(int);
```

We can now use this as we would any other C or C++ function.

```
std::cout << asm_xorshift(12) << std::endl;
```
### Running the tests
The tests for this library were mainly used for development purposes. They verify the correctness of the algorithms by checking them against a couple of test cases. However, they can help you to better understand some of the algorithms by seeing example input and output.

For example, take the sequence alignment algorithm. If you don't understand how to use it, or what it does, from the comment above the function definition, simply open the tests header, Tests.h.

Then, look at the example input and output. For sequence alignment, there are two different inputs tested.

The first is "abc" and "def". You see that this returns an empty vector.

The second is "abc", "acf". This returns a vector with two elements, (0, 0), and (2, 1).

From these two examples, you would be able to deduce that it returns pairs of the two strings that should be matched with each other in order to match them as well as possible. There are no matching charachters in "abc" and "def". However, in "abc" and "acf", the two As match, and the two Cs match.

You could also deduce that backwards sequence alignment and space efficient sequence alignment are simply variations of the same algorithm, as they have the same input and output.

Another possible use for the testing functions is to check if modifications to the current algorithms are correct. Say you make an optimization in the sequence alignment algorithm. You can simply type:

```
sequenceAlignmentTester();
```
to verify the correctness of your algorithm. You can also add more tests into it if you suspect you have created a bug somewhere else. 

## Contributing
All suggestions on this project are welcome. Please feel free to report any bugs, and offer any feature requests using the issue templates provided.

## Built with
Miscoroft Visual Studio 2015 C/C++ Compiler

## License
This project is licensed under the MIT License. See LICENSE.md for more details
