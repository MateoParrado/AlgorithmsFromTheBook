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

	template<class T>
	struct XORLinkedList{
		Node<T> * head = nullptr;
		Node<T> * tail = nullptr;

		unsigned int size = 0;

		XORLinkedList() {};

		//add node at end of list by value
		virtual void pushBackNode(T val) {
			Node<T> * temp = DBG_NEW Node<T>(val);

			if (head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				Node<T> * tempTail = tail;

				tail->next = (Node<T> *) ((uintptr_t)(tail->next) ^ (uintptr_t)(temp)); //covers the case when head == tail too
				
				tail = temp;
				tail->next = tempTail;
			}

			size++;
		}

		void pushForwardsNode(T val) {
			if (this->head) {
				Node<T> * temp = DBG_NEW Node<T>(val);

				temp->next = head;

				head->next = (Node<T> *) ((uintptr_t)(temp) ^ (uintptr_t)(head->next));

				head = temp;
			}
			else {
				head = DBG_NEW Node<T>(val);
				tail = head;
			}

			size++;
		}

		virtual void insertNode(T val, unsigned int pos) {
			if (pos > size) {
				throw 1;
			}
			if (pos == size || !head) {
				pushBackNode(val);
				return;
			}

			Node<T> * cur = (pos < size/2)? head : tail;

			unsigned int bound = (pos < size / 2) ? pos : size - pos - 1;

			Node<T> * prev = nullptr;
			Node<T> * next; //dummy variable

			for (unsigned int i = 0; i < bound; i++) {
				next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

				prev = cur;
				cur = next;
			}

			Node<T> * temp = DBG_NEW Node<T>(val);
			temp->next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur));

			prev->next = (Node<T> *)((uintptr_t)(prev->next) ^ (uintptr_t)(cur) ^ (uintptr_t)(temp));

			cur->next = (Node<T> *)((uintptr_t)(cur->next) ^ (uintptr_t)(prev) ^ (uintptr_t)(temp));
				
			size++;
		}

		//add node at end of list by pointer
		virtual void pushBackNode(Node<T> * temp) {
			if (head == nullptr) {
				head = temp;
				tail = temp;
				head->next = nullptr;
			}
			else {
				tail->next = (Node<T> *)((uintptr_t)(temp) ^ (uintptr_t)(tail->next)); //covers the case when head == tail too
				temp->next = tail;
				tail = temp;
			}

			size++;
		}

		virtual void pushForwardsNode(Node<T> * temp) {
			if (head == nullptr) {
				head = temp;
				tail = temp;
				head->next = nullptr;
			}
			else {
				head->next = (Node<T> *)((uintptr_t)(temp) ^ (uintptr_t)(head->next)); //covers the case when head == tail too
				temp->next = head;
				head = temp;
			}

			size++;
		}

		virtual void insertNode(Node<T> * temp, unsigned int pos) {
			if (pos > size) {
				throw 3;
			}
			if (head == nullptr || !pos) {
				pushForwardsNode(temp);
			}
			else if (pos == size) {
				pushBackNode(temp);
			}
			else {
				Node<T> * cur = (pos < size / 2) ? head : tail;

				unsigned int bound = (pos < size / 2) ? pos : size - pos - 1;

				Node<T> * prev = nullptr;
				Node<T> * next; //dummy variable

				for (unsigned int i = 0; i < bound; i++) {
					next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

					prev = cur;
					cur = next;
				}

				temp->next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur));

				prev->next = (Node<T> *)((uintptr_t)(prev->next) ^ (uintptr_t)(cur) ^ (uintptr_t)(temp));

				cur->next = (Node<T> *)((uintptr_t)(cur->next) ^ (uintptr_t)(prev) ^ (uintptr_t)(temp));

				size++;
			}
		}

		T popBackNode() {
			Node<T> * temp = tail;

			tail = tail->next;

			tail->next = (Node<T> *)((uintptr_t)(tail->next) ^ (uintptr_t)(temp));

			T t = temp->obj;

			delete temp;

			size--;

			return t;

		}

		T popFrontNode() {
			if (size == 1) {
				T t = head->obj;

				tail = nullptr;

				delete head;

				head = nullptr;

				size--;

				return t;
			}

			Node<T> * temp = head;

			head = head->next;

			head->next = (Node<T> *)((uintptr_t)(head->next) ^ (uintptr_t)(temp));

			T t = temp->obj;

			delete temp;

			size--;

			return t;
		}

		T popNode(unsigned int pos) {
			if (pos == size - 1) {
				return popBackNode();
			}

			Node<T> * cur = (pos < size / 2) ? head : tail;

			unsigned int bound = (pos < size / 2) ? pos : size - pos - 1;

			Node<T> * prev = nullptr;
			Node<T> * next = nullptr; //dummy variable

			for (unsigned int i = 0; i < bound; i++) {
				next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

				prev = cur;
				cur = next;
			}

			next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

			T t = cur->obj;

			prev->next = (Node<T> *) ((uintptr_t)(prev->next) ^ (uintptr_t)(cur) ^ (uintptr_t)(next));
			next->next = (Node<T> *) ((uintptr_t)(next->next) ^ (uintptr_t)(cur) ^ (uintptr_t)(prev));

			delete cur;

			size--;

			return t;
		}

		virtual T getVal(unsigned int _i) {
			if (_i >= size) {
				throw 4;
			}

			Node<T> * cur = (_i < size/2)? head : tail;

			unsigned int upperBound = (_i < size / 2) ? _i : size - _i - 1;

			Node<T> * prev = nullptr;
			Node<T> * next; //dummy variable

			for (unsigned int i = 0; i < upperBound; i++) {
				next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

				prev = cur;
				cur = next;
			}

			return cur->obj;
		}

		virtual T operator[](unsigned int _i) {
			if (_i >= size) {
				throw 4;
			}

			Node<T> * cur = (_i < size / 2) ? head : tail;

			unsigned int upperBound = (_i < size / 2) ? _i : size - _i - 1;

			Node<T> * prev = nullptr;
			Node<T> * next; //dummy variable

			for (unsigned int i = 0; i < upperBound; i++) {
				next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

				prev = cur;
				cur = next;
			}

			return cur->obj;
		}

		void display() {
			if (head) {
				Node<T> * cur = head;
				Node<T> * prev = nullptr;
				Node<T> * next; //dummy variable

				while (cur) {
					std::cout << cur->obj << " ";

					next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

					prev = cur;
					cur = next;
				}
			}
		}

		/*RULE OF FIVE*/
		XORLinkedList(const XORLinkedList& old) {
			Node<T> * cur = old.head;
			Node<T> * prev = nullptr;
			Node<T> * next; //dummy variable

			while (cur) {
				next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

				pushBackNode(cur);

				prev = cur;
				cur = next;
			}
		}

		XORLinkedList operator=(const XORLinkedList&) = delete;

		XORLinkedList(XORLinkedList&& other) {
			head = other.head;
			tail = other.tail;

			size = other.size;

			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
		}

		XORLinkedList operator=(const XORLinkedList&&) = delete;

		//this is janky and ugly, but doesnt throw a segfault because the parent gets passed a null head ptr in the destructor
		~XORLinkedList() {
			if (head) {
				Node<T> * cur = head;
				Node<T> * prev = nullptr;
				Node<T> * next; //dummy variable

				while(cur) {
					next = (Node<T> *)((uintptr_t)(prev) ^ (uintptr_t)(cur->next));

					prev = cur;

					delete cur;

					cur = next;
				}
			}

			//stop the segfault
			head = nullptr;
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

			head->prev = nullptr;

			size--;
		}

		//remove back node without deleting
		void unlinkBackNode() {
			if (size == 0) throw true;

			Node<T> * temp = tail;

			tail = tail->prev;

			tail->next = nullptr;

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