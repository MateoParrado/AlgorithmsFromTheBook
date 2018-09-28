#pragma once

#include <string>
#include <vector>

#include "Graph.h"

//generate an encryption scheme for a given set of frequencies
//frequencies must add up to 1, do not put in a value for the second parameter
//must be in the form (freq, chaachter) for heap sort reasons
//even though frequencies is a pointer it will still get modified
Graph::BinaryTree<char> * generateHoffmanCodes(std::vector<std::pair<float, char>> * frequencies, Heap::Heap<std::pair<float, char>> * minWeights = nullptr, Graph::BinaryTree<char> * retHeap = nullptr) {

	if (!minWeights) {//if this is the first time being run
		minWeights = new Heap::Heap<std::pair<float, char>>(frequencies->size());
		
		//set up a heap with lowest frequencies at the bottom
		for (unsigned int i = 0; i < frequencies->size(); i++) {
			minWeights->insert((*frequencies)[i]);
		}
	}

	if (minWeights->size() == 2) {
		//initialize retHeap
		retHeap = new Graph::BinaryTree<char>(frequencies->size());

		retHeap->addNode(0, -1);//insert some random holder value because the bottom node doesn't matter

		//encode one letter with zero and one with one
		retHeap->addNode(minWeights->popMin().second, 0);
		retHeap->addNode(minWeights->popMin().second, 0);

		return retHeap;
	}
	else {//add the two  least used nodes together and combine them

		std::pair<float, char> charOne = minWeights->popMin();
		std::pair<float, char> charTwo = minWeights->popMin();

		//add their frequencies
		float newFreq = charOne.first;
		newFreq += charTwo.first;

		minWeights->insert({ newFreq, charOne.second });

		retHeap = generateHoffmanCodes(frequencies, minWeights);

		retHeap->addNodeOnParent(charTwo.second, charOne.second);
		retHeap->addNodeOnParent(charOne.second, charOne.second);

		return retHeap;
	}

}

//adds the huffman encoding of char str to the vector of booleans vec 
void retraceThroughTree(char str, std::vector<bool> * vec, Graph::BinaryTree<char> * codes) {
	SinglyLinkedList::LinkedList<bool> addToVec;

	unsigned int i = codes->getPosByValue(str);

	while (i) {
		addToVec.pushForwardsNode(codes->isLeftChild(i));

		i = (*codes->edges[i])[0];//make i its parents value
	}

	while(addToVec.size) {
		vec->push_back(addToVec[0]);
		addToVec.popFrontNode();
	}
}

//return a bitstream (in the form of a bool vector) of a huffman encoding of a string
std::pair<std::vector<bool>, Graph::BinaryTree<char> *> huffmanEncoder(std::string str) {
	//get the frequency list for all the chars in the string
	std::vector < std::pair< float, char >> freq;
	freq.reserve(str.size());//this is more than neccessary but will stop it from resizing in the worst case scenario, may change later

	for (unsigned int i = 0; i < str.size(); i++) {
		auto tempIter = std::find_if(freq.begin(), freq.end(), [&str, &i](std::pair<float, char> pair) { return pair.second == str[i];});

		if (tempIter == freq.end()) {
			freq.push_back({ 1.0f / str.size(), str[i] });
		}
		else {
			freq[tempIter - freq.begin()].first += 1.0f / str.size();
		}
	}

	Graph::BinaryTree<char> * codes = generateHoffmanCodes(&freq);

	std::vector<bool> ret;
	ret.reserve(str.size() * 4); //this is just a random number that i feel like is a good approximation

	for (unsigned int i = 0; i < str.size(); i++) {
		retraceThroughTree(str[i], &ret, codes);
	}

	return{ ret, codes };
}

//returns the string that a huffman code was used to encode
std::string huffmanDecoder(std::pair<std::vector<bool>, Graph::BinaryTree<char> *> bitStream) {
	std::string ret;
	
	unsigned int nodeNum = 0;//the number of the current node whos children are being checked

	for (unsigned int i = 0; i < bitStream.first.size(); i++) {
		if (bitStream.second->edges[nodeNum]->size() == 1) {
			ret += bitStream.second->nodes[nodeNum].obj;

			nodeNum = 0;
		}
		
		unsigned  char offset = 0;//because the root node has one less edge than all the others

		if (!nodeNum) offset = 1;

		if (bitStream.first[i]) {
			nodeNum = (*bitStream.second->edges[nodeNum])[1 - offset];
		}
		else {
			nodeNum = (*bitStream.second->edges[nodeNum])[2 - offset];
		}
	}

	return ret;
}

#pragma deprecated (generateHoffmanCodes)
#pragma deprecated (retraceThroughTree)