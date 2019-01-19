#pragma once
#include <iostream>
#include <vector>

//parent node = (nodeIndex/2)-1
//left child = nodeIndex*2
//right child = nodeIndex*2 + 1

namespace Heap {

	template<class T>
	struct Heap 
	{
	private:
		
		//HEAPIFIES

		//used for insertion, propogates upwards
		void heapifyUp(int currentElemNumber = -1) 
		{
			//take the last element of the vector, and decide wether to send it up, then do it again
			if (currentElemNumber == -1) currentElemNumber = nodes->size() - 1;

			for(;;)
			{
				if ((*this->nodes)[currentElemNumber] < (*this->nodes)[(currentElemNumber - 1) / 2]) 
				{
					auto temp = (*this->nodes)[currentElemNumber];

					(*this->nodes)[currentElemNumber] = (*this->nodes)[(currentElemNumber - 1) / 2];

					(*this->nodes)[(currentElemNumber - 1) / 2] = temp;

					currentElemNumber = (currentElemNumber - 1) / 2;
				}
				else 
				{
					break;
				}
			}
		}

		//used in deletions, propogates downwards
		void heapifyDown(unsigned int pos)
		{
			for(;;) 
			{
				//if its in the last row
				if (2 * pos + 1 >= nodes->size()) break;

				//if it only has one kid
				else if (2 * pos + 2 == nodes->size()) goto onlyOneChild;


				else 
				{
					//if its right node is greated than its left node, exchange it with the left one
					if (((*nodes)[2 * pos + 1] < (*nodes)[2 * pos + 2]) && ((*nodes)[2 * pos + 1] < (*nodes)[pos])) 
					{
						T a = (*nodes)[2 * pos + 1];

						(*nodes)[2 * pos + 1] = (*nodes)[pos];

						(*nodes)[pos] = a;

						pos = 2 * pos + 1;
					}
					//if the left one is greater than the right one, switch it with the right
					else if (((*nodes)[2 * pos + 2] < (*nodes)[pos]))
					{
						T a = (*nodes)[2 * pos + 2];

						(*nodes)[2 * pos + 2] = (*nodes)[pos];

						(*nodes)[pos] = a;

						pos = 2 * pos + 2;
					}
					else 
					{
						break;
					}
					goto moreThanOneChild;
				}

			onlyOneChild:
				if ((*nodes)[2 * pos + 1] < (*nodes)[pos]) 
				{
					T a = (*nodes)[2 * pos + 1];

					(*nodes)[2 * pos + 1] = (*nodes)[pos];

					(*nodes)[pos] = a;
				}

				break;

			moreThanOneChild:;
			}
		}
	public:

		std::vector<T> * nodes = DBG_NEW std::vector<T>(0);
		
		//heapsize to try and avoid resizing vector
		Heap(int heapSize = 256)
		{
			this->nodes->reserve(heapSize);
		}

		unsigned int size() { return this->nodes->size(); }

		//insert value into the bottom of heap
		virtual void insert(T val)
		{
			nodes->push_back(val);
			this->heapifyUp();
		}

		virtual void remove(unsigned int pos)
		{
			(*nodes)[pos] = (*nodes)[nodes->size() - 1];

			nodes->pop_back();

			heapifyDown(pos);
		}

		T getMin() { return (*nodes)[0]; };

		int getIndexByVal(T val) 
		{
			for (int i = 0; i < nodes->size(); i++)
			{
				if ((*nodes)[i] == val) return i;
			}
		}

		T getByIndex(int index) { return (*nodes)[index]; }

		T popMin()
		{
			T ret = getMin();

			remove(0);
		
			return ret;
		}

		virtual void reheapify(int pos = -1) 
		{
			if (pos == -1) {//when no index is given, find the first index where the item is too small for its position and go from there
				for (unsigned int i = 1; i < nodes->size(); i++)
				{
					if ((*nodes)[i] < (*nodes)[i / 2])
					{
						heapifyUp(pos);
					}
				}
			}
			else if (nodes[pos / 2] > nodes[pos]) heapifyUp(pos);
			else heapifyDown(pos);
		}

		void display()
		{
			unsigned int twoPow = 0;
			unsigned int leftToCheck = 1;

			for (unsigned int i = 0; i < nodes->size(); i++)
			{

				if (!leftToCheck)
				{
					std::cout << std::endl;
					twoPow++;

					leftToCheck = (1 << twoPow);
				}
				std::cout << (*nodes)[i] << "   ";

				leftToCheck--;
			}
		}

		~Heap() {
			delete nodes;
		}
	};

	template<class T>
	struct MaxHeap : public Heap<T> 
	{
	private:
		//HEAPIFIES

		//used for insertion, propogates upwards
		void heapifyUp(int currentElemNumber = -1) 
		{
			//take the last element of the vector, and decide wether to send it up, then do it again
			if (currentElemNumber == -1) currentElemNumber = nodes->size() - 1;

			for(;;)
			{
				if ((*this->nodes)[currentElemNumber] > (*this->nodes)[(currentElemNumber - 1) / 2])
				{
					auto temp = (*this->nodes)[currentElemNumber];

					(*this->nodes)[currentElemNumber] = (*this->nodes)[(currentElemNumber - 1) / 2];

					(*this->nodes)[(currentElemNumber - 1) / 2] = temp;

					currentElemNumber = (currentElemNumber - 1) / 2;
				}
				else 
				{
					break;
				}
			}
		}

		//used in deletions, propogates downwards
		void heapifyDown(unsigned int pos) 
		{
			for(;;)
			{
				//if its in the last row
				if (2 * pos + 1 >= nodes->size()) break;

				//if it only has one kid
				else if (2 * pos + 2 == nodes->size()) goto onlyOneChild;


				else 
				{
					//if its right node is greated than its left node, exchange it with the left one
					if (((*nodes)[2 * pos + 1] > (*nodes)[2 * pos + 2]) && ((*nodes)[2 * pos + 1] > (*nodes)[pos])) 
					{
						T a = (*nodes)[2 * pos + 1];

						(*nodes)[2 * pos + 1] = (*nodes)[pos];

						(*nodes)[pos] = a;

						pos = 2 * pos + 1;
					}
					//if the left one is greater than the right one, switch it with the right
					else if (((*nodes)[2 * pos + 2] > (*nodes)[pos])) 
					{
						T a = (*nodes)[2 * pos + 2];

						(*nodes)[2 * pos + 2] = (*nodes)[pos];

						(*nodes)[pos] = a;

						pos = 2 * pos + 2;
					}
					else 
					{
						break;
					}
					goto moreThanOneChild;
				}

			onlyOneChild:
				if ((*nodes)[2 * pos + 1] > (*nodes)[pos])
				{
					T a = (*nodes)[2 * pos + 1];

					(*nodes)[2 * pos + 1] = (*nodes)[pos];

					(*nodes)[pos] = a;
				}

				break;

			moreThanOneChild:;
			}
		}
	public:
		MaxHeap(int heapSize = 256) : Heap(heapSize) { }

		void insert(T val) 
		{
			nodes->push_back(val);
			this->heapifyUp();
		}

		virtual void remove(unsigned int pos)
		{
			(*nodes)[pos] = (*nodes)[nodes->size() - 1];

			nodes->pop_back();

			heapifyDown(pos);
		}

		void reheapify(int pos = -1) 
		{
			if (pos == -1) {//if no index is given, find the first one that is too big for its position and go from there
				for (unsigned int i = 1; i < nodes->size(); i++)
				{
					if ((*nodes)[i] > (*nodes)[i / 2])
					{
						heapifyUp(pos);
					}
				}
			}
			else if (nodes[pos / 2] > nodes[pos]) heapifyUp(pos);
			else heapifyDown(pos);
		}
	};
}