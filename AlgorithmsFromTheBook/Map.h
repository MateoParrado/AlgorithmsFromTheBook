#pragma once

#include "LinkedList.h"

namespace Map
{
	//each kind of map needs
	//constuctor
	//add key
	//get value
	//get key
	//remove key
	//remove value
	//copy constructor
	//copy assignment operator

	//MAP BASED OFF A LINKED LIST
	//O(N) LOOP UPS
	//O(1) INSERTIONS AND DELETIONS
	//O(N) MEMORY
	template <class K, class V>
	struct SetMap 
	{
		//copy constructor
		//copy assignment operator
	private:
		//default initialized
		SinglyLinkedList::LinkedList<std::pair<K, V>> list;
	public:
		SetMap() 
		{
			list = SinglyLinkedList::LinkedList<std::pair<K, V>>();
		}

		void addKey(K k, V v) 
		{
			list.pushBackNode(std::make_pair(k, v));
		}

		//returns V() if none found
		V getValue(K k) 
		{
			SinglyLinkedList::Node<std::pair<K, V>> * temp = list.head;

			while(temp)
			{
				if (temp->obj.first == k)
				{
					return temp->obj.second;
				}

				temp = temp->next;
			}

			if (!temp)
			{
				return V();
			}
		}

		//returns K() if none found
		K getKey(V v)
		{
			SinglyLinkedList::Node<std::pair<K, V>> * temp = list.head;

			while(temp)
			{
				if (temp->obj.second == v) 
				{
					return temp->obj.first;
				}

				temp = temp->next;
			}

			if (!temp) 
			{
				return K();
			}
		}

		int removeKey(K k)
		{
			SinglyLinkedList::Node<std::pair<K, V>> * prev = nullptr;
			SinglyLinkedList::Node<std::pair<K, V>> * cur = list.head;

			while(cur)
			{
				if (cur->obj.first == k)
				{
					break;
				}

				prev = cur;
				cur = cur->next;
			}

			if (!cur)
			{
				return 0;
			}

			if (prev) //make sure we arent on the head
			{
				prev->next = cur->next;
			}
			else 
			{
				list.head = cur->next;
			}

			delete cur;

			return 1;
		}

		int removeValue(V v)
		{
			SinglyLinkedList::Node<std::pair<K, V>> * prev = nullptr;
			SinglyLinkedList::Node<std::pair<K, V>> * cur = list.head;

			while(cur)
			{
				if (cur->obj.second == v)
				{
					break;
				}

				prev = cur;
				cur = cur->next;
			}

			if (!cur)
			{
				return 0;
			}

			if (prev) //make sure we arent on the head
			{
				prev->next = cur->next;
			}
			else
			{
				list.head = cur->next;
			}

			delete cur;

			return 1;
		}

		/*RULE OF THREE*/

		SetMap(const SetMap& old) 
		{
			list = old.list;
		}

		SetMap& operator=(const SetMap& old)
		{	
			this->list = old.list;

			return *this;
		}
	};

	//implements add and remove keys in O(1)
	//key lokup in O(N), but decreasing based on the number of buckets
	//value lookup is really O(N)
	template<class K, class V>
	struct HashMap
	{
	private:
		std::function<int(K)> hashFunc;
		SinglyLinkedList::LinkedList<std::pair<K, V>> * lists;
		int buckets;

	public:
		//num buckets must be equal to the range of the hash function
		//the more buckets there are, the higher the performance at high capacity, with only 64 bits wasted per bucket
		HashMap(std::function<int(K)> hash, int numBuckets) : hashFunc(hash), buckets(numBuckets)
		{

			lists = DBG_NEW SinglyLinkedList::LinkedList<std::pair<K, V>>[numBuckets];
		}

		void addKey(K k, V v) 
		{
			int hash = hashFunc(k);

			lists[hash].pushBackNode(std::make_pair(k, v));
		}

		V getValue(K k) 
		{
			int hash = hashFunc(k);

			SinglyLinkedList::Node<std::pair<K, V>> * temp = lists[hash].head;

			while (temp)
			{
				if (temp->obj.first == k)
				{
					return temp->obj.second;
				}

				temp = temp->next;
			}

			if (!temp) 
			{
				return V();
			}
		}

		K getKey(V v) 
		{
			for (int i = 0; i < buckets; i++) 
			{
				SinglyLinkedList::Node<std::pair<K, V>> * temp = lists[i].head;

				while (temp)
				{
					if (temp->obj.second == v)
					{
						return temp->obj.first;
					}

					temp = temp->next;
				}
			}

			//if you reach here, return some default value
			return K();
		}

		int removeKey(K k)
		{
			SinglyLinkedList::Node<std::pair<K, V>> * prev = nullptr;
			SinglyLinkedList::Node<std::pair<K, V>> * cur = lists[hashFunc(k)].head;

			while (cur)
			{
				if (cur->obj.first == k)
				{
					break;
				}

				prev = cur;
				cur = cur->next;
			}

			if (!cur)
			{
				return 0;
			}

			if (prev) //make sure we arent on the head
			{
				prev->next = cur->next;
			}
			else
			{
				lists[hashFunc(k)].head = cur->next;
			}

			delete cur;

			return 1;
		}

		int removeValue(V v)
		{
			for (int i = 0; i < buckets; i++)
			{
				SinglyLinkedList::Node<std::pair<K, V>> * prev = nullptr;
				SinglyLinkedList::Node<std::pair<K, V>> * cur = lists[i].head;

				while (cur)
				{
					if (cur->obj.second == v)
					{
						break;
					}

					prev = cur;
					cur = cur->next;
				}

				if (!cur)
				{
					continue;
				}

				if (prev) //make sure we arent on the head
				{
					prev->next = cur->next;
				}
				else
				{
					lists[i].head = cur->next;
				}

				delete cur;

				return 1;
			}

			return 0;
		}

		/*RULE OF FIVE*/
		HashMap(const HashMap& old) 
		{
			this->buckets = old.buckets;

			this->lists = DBG_NEW SinglyLinkedList::LinkedList<std::pair<K, V>>[buckets];

			for (int i = 0; i < buckets; i++) 
			{
				this->lists[i] = old.lists[i];
			}
		}

		HashMap& operator=(const HashMap& old) 
		{
			this->buckets = old.buckets;

			this->lists = DBG_NEW SinglyLinkedList::LinkedList<std::pair<K, V>>[buckets];

			for (int i = 0; i < buckets; i++)
			{
				this->lists[i] = old.lists[i];
			}
		}

		HashMap(HashMap&& old) 
		{
			lists = old.lists;
			buckets = old.buckets;

			old.buckets = 0;
			old.lists = nullptr;
		}

		HashMap& operator=(HashMap&& old)
		{
			lists = old.lists;
			buckets = old.buckets;

			old.buckets = 0;
			old.lists = nullptr;
		}

		~HashMap()
		{
			if(lists) delete[] lists;
		}
	};
}