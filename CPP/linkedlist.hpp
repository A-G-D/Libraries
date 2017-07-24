/*******************************************************************************
*
*	linkedlist.hpp
*
*
*	List<T>
*		- Circular doubly-linked list template class
*
*
*	Author: AGD
*
*
*	API:
*
*		class List<type>
*
*			List()
*				- Default constructor
*			List(const List<type> &list)
*				- Copy constructor
*
*			List<type>::Node first()
*				- Front element of the list
*			List<type>::Node last()
*				- Back element of the list
*			List<type>::Node head()
*				- Head node - usually used to mark the end of iteration
*				  i.e., when the current node equals this (head) node
*
*			int size()
*				- Number of elements in the list
*
*			bool empty()
*				- Checks if the list is empty or not
*
*			List<type>::Node push(type value)
*				- Adds a new element to the front of the list and returns a new
*				  node correspoding to the added element
*			List<type>::Node push(List<type> &list)
*				- Transfers all the elements from the input list to the front of
*				  this list and returns the first element of the input list
*			type pop()
*				- Removes the front element of the list and returns its value
*
*			List<type>::Node enqueue(type value)
*				- Adds a new element to the back of the list and returns a new
*				  node corresponding to the added element
*			List<type>::Node enqueue(List<type> &list)
*				- Transfers all the elements from the input list to the back of
*				  this list and returns the first element of the input list
*			type eject()
*				- Removes the back element of the list and returns its value
*
*			void rotate_left()
*			void rotate_right()
*				- Rotates the list clockwise/counter-clockwise
*				- left -> counter-clockwise | right -> clockwise
*
*			void shift_left()
*			void shift_right()
*				- Shifts the elements of the list to the left/right by 1 bit
*
*			List<type>& split_front()
*				- Splits the first half of the list and returns it as a new list
*			List<type>& split_back()
*				- Splits the last half of the list and returns it as a new list
*			List<type>& split_shuffled()
*				- Returns all elements in the even slots as a new list
*
*			void reverse()
*				- Reverses the order of the elements of the list
*
*			void shuffle()
*				- Performs a 'riffle shuffle' on the list
*
*			void randomize()
*				- Randomizes the order of the elements of the list
*
*			void sort(bool (*handler)(type, type))
*				- Sorts the order of the elements based on the boolean expression
*				  of the callback function
*
*			void traverse(void (*callback)(List<type>::Node), bool clockwise)
*				- Runs the callback function for every node on the list
*				- The boolean parameter determines the direction of traversal
*			void traverse(bool (*handler)(List<type>::Node), bool clockwise)
*				- Cycles through the list while running the handler function for
*				  each node it traverses
*				- The cycling only stops when the handler function returns false
*				- The boolean parameter determines the direction of traversal
*
*			void clear()
*				- Removes all the elements of the list
*
*			List<type>& operator<<(int bits)
*			List<type>& operator>>(int bits)
*				- Shifts the list to the left/right by a certain number of bits
*
*			type operator[](int index)
*				- Returns the element found in a certain position in the list
*
*			List<type>& operator=(List<type> &list)
*				- Copies the properties of the input list into this list
*
*			List<type>& operator+(type data)
*				- Returns a new list composed of this list's elements plus the
*				  input data
*			List<type>& operator+(List<type> list)
*				- Returns the sum of this list and the input list as a new list
*
*			List<type>& operator+=(type data)
*				- Enqueues a new element to the list
*			List<type>& operator+=(List<type> list)
*				- Enqueues the input list into this list
*
*			bool operator==(List<type> &list)
*			bool operator!=(List<type> &list)
*			bool operator<(List<type> &list)
*			bool operator>(List<type> &list)
*			bool operator<=(List<type> &list)
*			bool operator>=(List<type> &list)
*				- Compares the sizes of two lists
*
*
*		class List<type>::Node
*
*			List<type>::Node(List<type>::Node *node)
*				- Constructs a node based on node pointer argument
*
*			static List<type>::Node& null()
*				- NULL node (constant)
*
*			type& data()
*				- Node data
*
*			List<type>::Node prev()
*			List<type>::Node next()
*				- The previous/next node of this node
*
*			List<type>::Node insert(type value)
*				- Appends a new element next to this node
*			List<type>::Node insert(List<type> &list)
*				- Appends all the elements of the input list next to this node
*				- Elements of the input list are not copied but transfered
*			type remove()
*				- Removes this node from its list and returns its value
*
*			void swap(List<type>::Node &node)
*				- Swaps the position of two nodes
*
*			void move(List<type>::Node &node)
*				- Moves this node next to the input node
*
*			void move_left()
*			void move_right()
*				- Moves this node clockwise/counter-clockwise by 1 bit
*				- left -> counter-clockwise | right -> clockwise
*
*			List<type>::Node& operator<<(int bits)
*			List<type>::Node& operator>>(int bits)
*				- Moves this node clockwise/counter-clockwise by a certain number
*				  of bits
*
*			List<type>::Node operator++()
*			List<type>::Node operator++(int)
*				- Moves on to the next node
*
*			List<type>::Node operator--()
*			List<type>::Node operator--(int)
*				- Moves on to the previous node
*
*			bool operator==(List<type>::Node &node)
*			bool operator!=(List<type>::Node &node)
*				- Checks the equality of two nodes
*
*******************************************************************************/
#ifndef __LINKEDLIST_HPP__
#define __LINKEDLIST_HPP__


#include <stdlib.h>
#include <time.h>


template <class T> class List {


public:

	class Node {


		friend class List<T>;

		Node *p_ptr,
			 *p_prev,
			 *p_next;
		T p_data;

		inline Node(Node *node) {
			this->p_ptr = node;
		}

		void link(Node *node) const {
			Node *ptr = this->p_ptr,
				 *next = ptr->p_next;
			next->p_prev = node;
			ptr->p_next = node;
			node->p_prev = ptr;
			node->p_next = next;
		}
		void unlink() const {
			Node *ptr = this->p_ptr,
				 *prev = ptr->p_prev,
				 *next = ptr->p_next;
			next->p_prev = prev;
			prev->p_next = next;
		}

	public:

		static Node& null() {
			static Node null_node(NULL);
			return null_node;
		}

		inline T& data() const {
			return this->p_ptr->p_data;
		}
		inline Node prev() const {
			return Node(this->p_ptr->p_prev);
		}
		inline Node next() const {
			return Node(this->p_ptr->p_next);
		}

		Node insert(const T &data) const {
			Node *node = new Node;
			this->link(node);
			node->p_data = data;
			return Node(node);
		}
		Node insert(const List<T> &list) const {
			Node *ptr = this->p_ptr,
				 *next = ptr->p_next,
				 *head = list.p_head.p_ptr,
				 *first = head->p_next,
				 *last = head->p_prev;
			first->p_prev = ptr;
			ptr->p_next = first;
			next->p_prev = last;
			last->p_next = next;
			head->p_prev = head;
			head->p_next = head;
			return Node(first);
		}
		T remove() const {
			T data = this->p_ptr->p_data;
			this->unlink();
			delete this->p_ptr;
			return data;
		}

		void swap(const Node &node) const {
			Node *ptr = this->p_ptr,
				 *other = node.p_ptr,
				 *this_prev = ptr->p_prev,
				 *this_next = ptr->p_next,
				 *node_prev = other->p_prev,
				 *node_next = other->p_next;
			ptr->p_prev = node_prev;
			ptr->p_next = node_next;
			other->p_prev = this_prev;
			other->p_next = this_next;
			this_next->p_prev = other;
			this_prev->p_next = other;
			node_next->p_prev = ptr;
			node_prev->p_next = ptr;
		}

		void move(const Node &node) const {
			this->unlink();
			node.link(this->p_ptr);
		}
		inline void move_left() const {
			this->move(this->prev().prev());
		}
		inline void move_right() const {
			this->move(this->next());
		}

		Node& operator<<(int bits) {
			while(bits-- > 0)
				this->move_left();
			return *this;
		}
		Node& operator>>(int bits) {
			while(bits-- > 0)
				this->move_right();
			return *this;
		}

		Node& operator++() {
			this->p_ptr = this->p_ptr->p_next;
			return *this;
		}
		Node operator++(int) {
			Node temp = *this;
			this->p_ptr = this->p_ptr->p_next;
			return temp;
		}

		Node& operator--() {
			this->p_ptr = this->p_ptr->p_prev;
			return *this;
		}
		Node operator--(int) {
			Node temp = *this;
			this->p_ptr = this->p_ptr->p_prev;
			return temp;
		}

		Node& operator=(const Node &node) {
			this->p_ptr = node.p_ptr;
			return *this;
		}

		inline bool operator==(const Node &node) const {
			return this->p_ptr == node.p_ptr;
		}
		inline bool operator!=(const Node &node) const {
			return this->p_ptr != node.p_ptr;
		}

		inline Node() {}


	};

private:

	friend class Node;

	Node p_head;

	static int random_int(int lowbound, int upperbound) {
		int n = rand();
		time_t t = time(0);
		srand(n + (localtime(&t)->tm_sec));
		return rand()%(upperbound - lowbound + 1) + lowbound;
	}

	void add(const List<T> &list) {
		Node head = list.head(),
			 last = this->last();
		for(Node node = head.prev(); node != head; --node)
			last.insert(node.data());
	}

public:

	inline Node head() const {
		return Node(this->p_head.p_ptr);
	}
	inline Node first() const {
		return this->head().next();
	}
	inline Node last() const {
		return this->head().prev();
	}

	inline bool empty() const {
		return this->head() == this->first();
	}

	int size() const {
		int count = 0;
		Node *head = this->p_head.p_ptr;
		for(Node *node = head->p_next; node != head; node = node->p_next)
			++count;
		return count;
	}

	inline Node push(const T &data) const {
		return this->head().insert(data);
	}
	inline Node push(const List<T> &list) const {
		return this->head().insert(list);
	}
	inline T pop() const {
		return this->first().remove();
	}

	inline Node enqueue(const T &data) const {
		return this->last().insert(data);
	}
	inline Node enqueue(const List<T> &list) const {
		return this->last().insert(list);
	}
	inline T eject() const {
		return this->last().remove();
	}

	void rotate_left() const {
		Node *head = this->p_head.p_ptr,
			 *first = head->p_next,
			 *last = head->p_prev,
			 *first_new = first->p_next;
		head->p_prev = first;
		head->p_next = first_new;
		first->p_prev = last;
		first->p_next = head;
		first_new->p_prev = head;
		last->p_next = first;
	}
	void rotate_right() const {
		Node *head = this->p_head.p_ptr,
			 *first = head->p_next,
			 *last = head->p_prev,
			 *last_new = last->p_prev;
		head->p_prev = last_new;
		head->p_next = last;
		last->p_prev = head;
		last->p_next = first;
		first->p_prev = last;
		last_new->p_next = head;
	}

	void shift_left() const {
		this->pop();
		this->enqueue(T());
	}
	void shift_right() const {
		this->eject();
		this->push(T());
	}

	Node random_sample() const {
		Node node = this->head();
		for(int i = random_int(1, this->size()); i-- > 0; ++node);
		return node;
	}
	List random_subset(int size) const {
		List<T> temp, copy(*this);
		Node head = temp.head();
		while(size-- > 0)
			copy.random_sample().move(head);
		return temp;
	}

	List<T> split_front() const {
		List<T> list;
		Node *list_head = list.p_head.p_ptr,
			 *this_head = this->p_head.p_ptr,
			 *this_first,
			 *node = this_head;
		for(int i = this->size()/2; i-- > 0; node = node->p_next);
		this_first = node->p_next;
		this_first->p_prev = this_head;
		this_head->p_next = this_first;
		list_head->p_prev = node;
		list_head->p_next = this_first;
		this_first->p_prev = list_head;
		node->p_next = list_head;
		return list;
	}
	List<T> split_back() const {
		List<T> list;
		Node *list_head = list.p_head.p_ptr,
			 *this_head = this->p_head.p_ptr,
			 *list_last = this_head->p_prev,
			 *list_first,
			 *node = this_head;
		for(int i = int(this->size()/2.0 + 0.5); i-- > 0; node = node->p_next);
		list_first = node->p_next;
		list_head->p_prev = list_last;
		list_head->p_next = list_first;
		list_first->p_prev = list_head;
		list_last->p_next = list_head;
		this_head->p_prev = node;
		node->p_next = this_head;
		return list;
	}
	List<T> split_shuffled() const {
		List<T> list;
		Node head = this->head(),
			 first = head.next(),
			 node = first.next(),
			 next;
		while(node != head && node != first) {
			next = node.next().next();
			node.move(list.last());
			node = next;
		}
		return list;
	}

	void reverse() const {
		Node *head = this->p_head.p_ptr,
			 *first = head->p_next,
			 *node = first->p_next,
			 *last = first,
			 *next;
		while(node != head) {
			next = node->p_next;
			node->p_prev = head;
			node->p_next = first;
			first->p_prev = node;
			first = node;
			node = next;
		}
		head->p_prev = last;
		head->p_next = first;
		last->p_next = head;
	}

	void shuffle() const {
		List<T> a, b;
		a.push(*this);
		b = a.split_back();
		while(!a.empty() || !b.empty()) {
			if(!b.empty())
				b.first().move(this->last());
			if(!a.empty())
				a.first().move(this->last());
		}
	}

private:

	void random_position(const Node &node) const {
		if(rand()%2)
			node.move(this->head());
		else
			node.move(this->last());
	}

	void random_distribution(const List<T> &list) const {
		if(!list.empty())
			if(rand()%2)
				this->random_position(list.first());
			else
				this->random_position(list.last());
	}

public:

	void randomize() const {
		List<T> a, b;
		a.push(*this);
		b = a.split_back();
		time_t t = time(0);
		srand((localtime(&t))->tm_sec);
		while(!a.empty() || !b.empty()) {
			this->random_distribution(a);
			this->random_distribution(b);
		}
	}

	void sort(bool (*handler)(T, T)) const {
		List<T> a, b;
		a.enqueue(*this);
		b = a.split_back();
		if(a.first().next() != a.head())
			a.sort(handler);
		if(b.first().next() != b.head())
			b.sort(handler);
		while(true)
			if(a.empty()) {
				this->enqueue(b);
				break;
			}else if(b.empty()) {
				this->enqueue(a);
				break;
			}else if(handler(a.first().data(), b.first().data()))
				a.first().move(this->last());
			else
				b.first().move(this->last());
	}

	void traverse(bool (*handler)(const Node&), bool clockwise) const {
		Node head = this->head(), node;
		if(clockwise)
			for(node = head.next(); handler(node); ++node) {
				if(node.next() == head)
					node = head;
			}
		else
			for(node = head.prev(); handler(node); --node) {
				if(node.prev() == head)
					node = head;
			}
	}
	void traverse(void (*callback)(const Node&), bool clockwise) const {
		Node head = this->head(), node;
		if(clockwise)
			for(node = head.next(); node != head; callback(node++));
		else
			for(node = head.prev(); node != head; callback(node--));
	}

	void clear() const {
		Node *head = this->p_head.p_ptr;
		while(head->p_next != head)
			this->pop();
	}

	List<T>& operator<<(int bits) {
		while(bits-- > 0)
			this->shift_left();
		return *this;
	}
	List<T>& operator>>(int bits) {
		while(bits-- > 0)
			this->shift_right();
		return *this;
	}

	T operator[](int index) const {
		Node *head = this->p_head.p_ptr,
			 *node = head->p_next;
		for(int count = 0; node != head && count++ < index; node = node->p_next);
		if(node == head || index < 0)
			return T();
		return node->p_data;
	}

	List<T>& operator=(const List<T> &list) {
		this->clear();
		this->add(list);
		return *this;
	}

	List<T> operator+(const T &data) {
		List<T> temp(*this);
		temp.enqueue(data);
		return temp;
	}
	List<T> operator+(const List<T> &list) {
		List<T> temp(*this);
		temp.add(list);
		return temp;
	}

	List<T>& operator+=(const T &data) {
		this->enqueue(data);
		return *this;
	}
	List<T>& operator+=(const List<T> &list) {
		this->add(list);
		return *this;
	}

	inline bool operator==(const List<T> &list) const {
		return this->size() == list.size();
	}
	inline bool operator!=(const List<T> &list) const {
		return this->size() != list.size();
	}

	inline bool operator<(const List<T> &list) const {
		return this->size() < list.size();
	}
	inline bool operator>(const List<T> &list) const {
		return this->size() > list.size();
	}

	inline bool operator<=(const List<T> &list) const {
		return this->size() <= list.size();
	}
	inline bool operator>=(const List<T> &list) const {
		return this->size() >= list.size();
	}

private:

	void init() {
		Node *ptr = new Node;
		this->p_head.p_ptr = ptr;
		ptr->p_prev = ptr;
		ptr->p_next = ptr;
	}

public:

    inline List() {
		this->init();
	}
	List(const List<T> &list) {
		Node *head = list.p_head.p_ptr;
		this->init();
		for(Node *node = head->p_next; node != head; node = node->p_next)
			this->enqueue(node->p_data);
	}
	~List() {
		this->clear();
		delete this->p_head.p_ptr;
	}


};
#endif
