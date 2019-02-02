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
	protected:
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

			if (!temp) {
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

			return 1;
		}

		/*RULE OF THREE*/

		SetMap(const SetMap& old) {
			std::cout << &(old.list) << std::endl;
			list = old.list;
		}

		SetMap& operator=(const SetMap& old) {
			

			this->list = old.list;

			return *this;
		}
	};
}