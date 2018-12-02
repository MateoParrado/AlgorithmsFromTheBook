#pragma once
#include <iostream>

namespace SinglyLinkedList {
	template <class T>
	struct Node {
		T obj;
		Node<T> * next;

		Node(T _obj) : obj(_obj), next(nullptr) {};
		Node(Node<T> * _node) : obj(_node->obj), next(nullptr) {};
	};

	template<class T>
	struct LinkedList {
	public:
		Node<T> * head = nullptr;
		Node<T> * tail = nullptr;

		unsigned int size = 0;

		LinkedList() {  }

		//add node at end of list by value
		virtual void pushBackNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->next = temp; //covers the case when head == tail too
				tail = temp;
			}

			size++;
		}

		//add to the front by value
		virtual void pushForwardsNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			temp->next = head;
			head = temp;

			size++;
		}

		//insert a node by value
		virtual void insertNode(T val, unsigned int pos) {			
			Node<T> * temp = DBG_NEW Node<T>(val);
			if (!size) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else if (!pos) {//first?
				temp->next = head;
				head = temp;
			}
			else if (pos < size) {
				Node<T> * cur = head;
				Node<T> * prev = nullptr;

				for (unsigned int i = 0; i < pos - 1; i++) {
					prev = cur;
					cur = cur->next;
				}

				temp->next = cur->next;
				cur->next = temp;
			}
			else {//if its past the end, just do push back node, copied for memory leak reasons
				if (head == nullptr) {
					head = temp;
					tail = temp;
					temp = nullptr;
				}
				else {
					tail->next = temp; //covers the case when head == tail too
					tail = temp;
				}
			}

			size++;
		}

		//add node at end of list by pointer
		virtual void pushBackNode(Node<T> * temp) {
			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->next = temp; //covers the case when head == tail too
				tail = temp;
			}

			size++;
		}

		//add to the front by pointer
		virtual void pushForwardsNode(Node<T> * temp) {
			temp->next = head;
			head = temp;

			size++;
		}

		//insert a node by pointer
		virtual void insertNode(Node<T> * temp, unsigned int pos) {
			if (!pos) {//first?
				temp->next = head;
				head = temp;
			}
			else if (pos < size) {
				Node<T> * cur = head;
				Node<T> * prev = nullptr;

				for (unsigned int i = 0; i < pos - 1; i++) {
					prev = cur;
					cur = cur->next;
				}

				temp->next = cur->next;
				cur->next = temp;
			}
			else {//if its past the end, just do push back node, copied for memory leak reasons
				if (head == nullptr) {
					head = temp;
					tail = temp;
					temp = nullptr;
				}
				else {
					tail->next = temp; //covers the case when head == tail too
					tail = temp;
				}
			}

			size++;
		}

		//delete front node
		T popFrontNode() {
			if (size == 0) throw true;
			
			Node<T> * temp = head;

			head = head->next;

			T obj = temp->obj;

			delete temp;

			size--;

			return obj;
		}

		//deleting a node at any position, no specific one for popping from back because it has no performance change
		virtual T popNode(unsigned int pos) {
			if (pos >= size) throw false;

			Node<T> * prev = nullptr;
			Node<T> * cur = head;

			for (unsigned int i = 0; i < pos; i++) {
				prev = cur;
				cur = cur->next;
			}

			prev->next = cur->next;

			T obj = cur->obj;

			delete cur;

			size--;

			return obj;
		}

		//remove front node without deleting
		void unlinkFrontNode() {
			if (size == 0) throw true;

			Node<T> * temp = head;

			head = head->next;

			size--;
		}

		//remove a node at any position without deleting, no specific one for popping from back because it has no performance change
		virtual void unlinkNode(unsigned int pos) {
			if (pos >= size) throw false;

			Node<T> * prev = nullptr;
			Node<T> * cur = head;

			for (unsigned int i = 0; i < pos; i++) {
				prev = cur;
				cur = cur->next;
			}

			prev->next = cur->next;

			size--;
		}

		virtual T getVal(unsigned int _i) {
			Node<T> * temp = head;

			for (unsigned int i = 0; i < _i; i++) {
				temp = (*temp).next;
			}

			return (*temp).obj;
		}

		virtual T operator[](unsigned int _i) {
			Node<T> * temp = head;

			for (unsigned int i = 0; i < _i; i++) {
				temp = (*temp).next;
			}

			return (*temp).obj;
		}

		void display() {
			Node<T> * temp = head;

			while (temp) {
				std::cout << temp->obj << std::endl;

				temp = temp->next;
			}
		}

		/*RULE OF FIVE*/
		LinkedList(const LinkedList& old) {
			Node<T> * temp = old.head;

			while (temp) {
				Node<T> * tempNode = DBG_NEW Node<T>(temp);

				this->pushBackNode(tempNode);

				temp = temp->next;
			}
		}

		LinkedList operator=(const LinkedList&) = delete;

		LinkedList(LinkedList&& other) {
			head = other.head;
			tail = other.tail;

			size = other.size;

			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
		}

		LinkedList operator=(const LinkedList&&) = delete;

		~LinkedList() {
			if (head) {
				while (head->next) {
					Node<T> * temp = head->next;
					delete head;
					head = temp;
				} delete head; //to make sure tail gets deleted
			}
		}
	};

}

//inherits from linked list
namespace DoublyLinkedList {

	template <class T>
	struct Node{
		T obj;

		Node<T> * next;
		Node<T> * prev;

		Node(T _obj) : obj(_obj), next(nullptr), prev(nullptr) {};
		Node(Node<T> * _node) : obj(_node->obj), next(nullptr), prev(nullptr) {};
	};

	template<class T>
	struct LinkedList{
	public:
		Node<T> * head = nullptr;
		Node<T> * tail = nullptr;

		unsigned int size = 0;

		LinkedList() { 	}

		//add node at end of list by value
		void pushBackNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->next = temp; //covers the case when head == tail too
				temp->prev = tail;
				tail = temp;
			}

			size++;
		}

		//add to the front by value
		void pushForwardsNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			temp->next = head;
			head->prev = temp;

			head = temp;

			size++;
		}

		//insert a node by value
		void insertNode(T val, int pos) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			if (!size) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else if (!pos) {//first?
				temp->next = head;
				head->prev = temp;

				head = temp;
			}
			else if (pos < size) {
				if (pos < size / 2) {
					Node<T> * cur = head;
					Node<T> * pre = nullptr;

					for (int i = 0; i < pos - 1; i++) {
						pre = cur;
						cur = cur->next;
					}

					temp->next = cur->next;
					temp->prev = cur;

					cur->next = temp;
					cur->next->prev = temp;
				}
				else {
					Node<T> * cur = tail;
					Node<T> * post = nullptr;

					auto sd = size;
					auto df = pos;
					auto dfdf = size - pos;
					for (int i = 0; i < size - pos; i++) {
						post = cur;
						cur = cur->prev;
					}

					temp->prev = cur;
					temp->next = cur->next;

					cur->next->prev = temp;
					cur->next = temp;
				}
			}
			else {//if its past the end, just do push back node, copied for memory leak reasons
				if (head == nullptr) {
					head = temp;
					tail = temp;
					temp = nullptr;
				}
				else {
					tail->next = temp; //covers the case when head == tail too
					temp->prev = tail;
					tail = temp;
				}
			}

			size++;
		}

		//add node at end of list by pointer
		void pushBackNode(Node<T> * temp) {
			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->next = temp; //covers the case when head == tail too
				temp->prev = tail;

				tail = temp;
			}

			size++;
		}

		//add to the front by pointer
		void pushForwardsNode(Node<T> * temp) {
			temp->next = head;
			head->prev = temp;

			head = temp;

			size++;
		}

		//insert a node by pointer
		void insertNode(Node<T> * temp, int pos) {
			if (!size) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else if (!pos) {//first?
				temp->next = head;
				head->prev = temp;

				head = temp;
			}
			else if (pos < size) {
				if (pos < size / 2) {
					Node<T> * cur = head;
					Node<T> * pre = nullptr;

					for (int i = 0; i < pos - 1; i++) {
						pre = cur;
						cur = cur->next;
					}

					temp->next = cur->next;
					temp->prev = cur;

					cur->next = temp;
					cur->next->prev = temp;
				}
				else {
					Node<T> * cur = tail;
					Node<T> * post = nullptr;

					auto sd = size;
					auto df = pos;
					auto dfdf = size - pos;
					for (int i = 0; i < size - pos; i++) {
						post = cur;
						cur = cur->prev;
					}

					temp->prev = cur;
					temp->next = cur->next;

					cur->next->prev = temp;
					cur->next = temp;
				}
			}
			else {//if its past the end, just do push back node, copied for memory leak reasons
				if (head == nullptr) {
					head = temp;
					tail = temp;
					temp = nullptr;
				}
				else {
					tail->next = temp; //covers the case when head == tail too
					temp->prev = tail;
					tail = temp;
				}
			}

			size++;
		}

		//delete front node
		T popFrontNode() {
			if (size == 0) throw true;

			Node<T> * temp = head;

			head = head->next;

			T obj = temp->obj;

			delete temp;

			size--;

			return obj;
		}

		T popBackNode() {
			if (size == 0) throw true;

			Node<T> * temp = tail;

			tail = tail->prev;

			T obj = temp->obj;

			delete temp;

			size--;

			return obj;
		}

		//deleting a node at any position
		T popNode(int pos) {
			if (pos >= size) throw false;

			if (pos < size / 2) {
				Node<T> * pre = nullptr;
				Node<T> * cur = head;

				for (int i = 0; i < pos; i++) {
					pre = cur;
					cur = cur->next;
				}

				pre->next = cur->next;
				cur->next->prev = pre;

				T obj = cur->obj;

				delete cur;

				size--;

				return obj;
			}
			else {
				Node<T> * cur = tail;

				for (int i = 0; i < size - pos - 1; i++) {
					cur = cur->prev;
				}

				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;

				T obj = cur->obj;

				delete cur;
	
				size--;

				return obj;
			}
		}
		
		//remove front node without deleting
		void unlinkFrontNode() {
			if (size == 0) throw true;

			Node<T> * temp = head;

			head = head->next;

			size--;
		}

		//remove back node without deleting
		void unlinkBackNode() {
			if (size == 0) throw true;

			Node<T> * temp = tail;

			tail = tail->prev;

			size--;
		}

		//remove a node at any position without deleting, no specific one for popping from back because it has no performance change
		void unlinkNode(int pos) {
			if (pos >= size) throw false;

			if (pos < size / 2) {
				Node<T> * pre = nullptr;
				Node<T> * cur = head;

				for (int i = 0; i < pos; i++) {
					pre = cur;
					cur = cur->next;
				}

				pre->next = cur->next;
				cur->next->prev = pre;
			}
			else {
				Node<T> * cur = tail;

				for (int i = 0; i < size - pos - 1; i++) {
					cur = cur->prev;
				}

				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
			}

			size--;
		}

		T getVal(unsigned int _i) {
			if (_i < size / 2) {
				Node<T> * temp = head;

				for (int i = 0; i < _i; i++) {
					temp = (*temp).next;
				}

				return (*temp).obj;
			}
			else {
				Node<T> * temp = tail;

				for (int i = 0; i < size - _i - 1; i++) {
					temp = (*temp).prev;
				}

				return (*temp).obj;
			}
		}

		T operator[](unsigned int _i) {
			if (_i < size / 2) {
				Node<T> * temp = head;

				for (int i = 0; i < _i; i++) {
					temp = (*temp).next;
				}

				return (*temp).obj;
			}
			else {
				Node<T> * temp = tail;

				for (int i = 0; i < size - _i - 1; i++) {
					temp = (*temp).prev;
				}

				return (*temp).obj;
			}
		}

		void display() {
			Node<T> * temp = head;

			while (temp) {
				std::cout << temp->obj << std::endl;

				temp = temp->next;
			}
		}

		/*RULE OF FIVE*/
		LinkedList(const LinkedList& old) {
			Node<T> * temp = old.head;

			while (temp) {
				Node<T> * tempNode = DBG_NEW Node<T>(temp);

				this->pushBackNode(tempNode);

				temp = temp->next;
			}
		}

		LinkedList operator=(const LinkedList&) = delete;

		LinkedList(LinkedList&& other) {
			head = other.head;
			tail = other.tail;

			size = other.size;

			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
		}

		LinkedList operator=(const LinkedList&&) = delete;

		~LinkedList() {
			if (head) {
				while (head->next) {
					Node<T> * temp = head->next;
					delete head;
					head = temp;
				} delete head; //to make sure tail gets deleted
			}
		}
	};

}
/*
namespace XORDoublyLinkedList {
	template <class T>
	struct Node {
		T obj;
		Node<T> * ptr;

		Node(T _obj) : obj(_obj), ptr(nullptr) {};
		Node(Node<T> * _node) : obj(_node->obj), ptr(nullptr) {};
	};

	template<class T>
	struct LinkedList : DoublyLinkedList::LinkedList<T> {
	public:
		LinkedList() {  }
		/*
		//add node at end of list by value
		void pushBackNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->ptr = (Node *)((uintptr_t)tail->ptr ^ (uintptr_t)temp); //covers the case when head == tail too
				temp->ptr = tail;
				tail = temp;
			}

			size++;
		}
		/*
		//add to the front by value
		void pushForwardsNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			temp->next = head;
			head->prev = temp;

			head = temp;

			size++;
		}
		/*
		//insert a node by value
		void insertNode(T val, int pos) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			if (!size) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else if (!pos) {//first?
				temp->next = head;
				head->prev = temp;

				head = temp;
			}
			else if (pos < size) {
				if (pos < size / 2) {
					Node<T> * cur = head;
					Node<T> * pre = nullptr;

					for (int i = 0; i < pos - 1; i++) {
						pre = cur;
						cur = cur->next;
					}

					temp->next = cur->next;
					temp->prev = cur;

					cur->next = temp;
					cur->next->prev = temp;
				}
				else {
					Node<T> * cur = tail;
					Node<T> * post = nullptr;

					auto sd = size;
					auto df = pos;
					auto dfdf = size - pos;
					for (int i = 0; i < size - pos; i++) {
						post = cur;
						cur = cur->prev;
					}

					temp->prev = cur;
					temp->next = cur->next;

					cur->next->prev = temp;
					cur->next = temp;
				}
			}
			else {//if its past the end, just do push back node, copied for memory leak reasons
				if (head == nullptr) {
					head = temp;
					tail = temp;
					temp = nullptr;
				}
				else {
					tail->next = temp; //covers the case when head == tail too
					temp->prev = tail;
					tail = temp;
				}
			}

			size++;
		}

		//add node at end of list by pointer
		void pushBackNode(Node<T> * temp) {
			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->next = temp; //covers the case when head == tail too
				temp->prev = tail;

				tail = temp;
			}

			size++;
		}

		//add to the front by pointer
		void pushForwardsNode(Node<T> * temp) {
			temp->next = head;
			head->prev = temp;

			head = temp;

			size++;
		}

		//insert a node by pointer
		void insertNode(Node<T> * temp, int pos) {
			if (!size) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else if (!pos) {//first?
				temp->next = head;
				head->prev = temp;

				head = temp;
			}
			else if (pos < size) {
				if (pos < size / 2) {
					Node<T> * cur = head;
					Node<T> * pre = nullptr;

					for (int i = 0; i < pos - 1; i++) {
						pre = cur;
						cur = cur->next;
					}

					temp->next = cur->next;
					temp->prev = cur;

					cur->next = temp;
					cur->next->prev = temp;
				}
				else {
					Node<T> * cur = tail;
					Node<T> * post = nullptr;

					auto sd = size;
					auto df = pos;
					auto dfdf = size - pos;
					for (int i = 0; i < size - pos; i++) {
						post = cur;
						cur = cur->prev;
					}

					temp->prev = cur;
					temp->next = cur->next;

					cur->next->prev = temp;
					cur->next = temp;
				}
			}
			else {//if its past the end, just do push back node, copied for memory leak reasons
				if (head == nullptr) {
					head = temp;
					tail = temp;
					temp = nullptr;
				}
				else {
					tail->next = temp; //covers the case when head == tail too
					temp->prev = tail;
					tail = temp;
				}
			}

			size++;
		}

		T popBackNode() {
			if (size == 0) throw true;

			Node<T> * temp = tail;

			tail = tail->prev;

			T obj = temp->obj;

			delete temp;

			size--;

			return obj;
		}

		//deleting a node at any position
		T popNode(int pos) {
			if (pos >= size) throw false;

			if (pos < size / 2) {
				Node<T> * pre = nullptr;
				Node<T> * cur = head;

				for (int i = 0; i < pos; i++) {
					pre = cur;
					cur = cur->next;
				}

				pre->next = cur->next;
				cur->next->prev = pre;

				T obj = cur->obj;

				delete cur;

				size--;

				return obj;
			}
			else {
				Node<T> * cur = tail;

				for (int i = 0; i < size - pos - 1; i++) {
					cur = cur->prev;
				}

				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;

				T obj = cur->obj;

				delete cur;

				size--;

				return obj;
			}
		}

		//remove back node without deleting
		void unlinkBackNode() {
			if (size == 0) throw true;

			Node<T> * temp = tail;

			tail = tail->prev;

			size--;
		}

		//remove a node at any position without deleting, no specific one for popping from back because it has no performance change
		void unlinkNode(int pos) {
			if (pos >= size) throw false;

			if (pos < size / 2) {
				Node<T> * pre = nullptr;
				Node<T> * cur = head;

				for (int i = 0; i < pos; i++) {
					pre = cur;
					cur = cur->next;
				}

				pre->next = cur->next;
				cur->next->prev = pre;
			}
			else {
				Node<T> * cur = tail;

				for (int i = 0; i < size - pos - 1; i++) {
					cur = cur->prev;
				}

				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
			}

			size--;
		}

		T getVal(unsigned int _i) {
			if (_i < size / 2) {
				Node<T> * temp = head;

				for (int i = 0; i < _i; i++) {
					temp = (*temp).next;
				}

				return (*temp).obj;
			}
			else {
				Node<T> * temp = tail;

				for (int i = 0; i < size - _i - 1; i++) {
					temp = (*temp).prev;
				}

				return (*temp).obj;
			}
		}

		T operator[](unsigned int _i) {
			if (_i < size / 2) {
				Node<T> * temp = head;

				for (int i = 0; i < _i; i++) {
					temp = (*temp).next;
				}

				return (*temp).obj;
			}
			else {
				Node<T> * temp = tail;

				for (int i = 0; i < size - _i - 1; i++) {
					temp = (*temp).prev;
				}

				return (*temp).obj;
			}
		}*/
//	};
//}