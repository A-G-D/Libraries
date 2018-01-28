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
*			Constructors:
*
*				List()
*					- Time Complexity: O(1)
*					- Default constructor
*				List(const List<type> &list)
*					- Time Complexity: O(n)
*					- Copy constructor
*
*			Methods:
*
*				List<type>::Node first()
*					- Time Complexity: O(1)
*					- First element of the list
*				List<type>::Node last()
*					- Time Complexity: O(1)
*					- Last element of the list
*				List<type>::Node head()
*					- Time Complexity: O(1)
*					- Head node - usually used to mark the end of iteration
*				  	  i.e., when the current node equals this (head) node
*
*				unsigned int size()
*					- Time Complexity: O(n)
*					- Number of elements in the list
*
*				bool empty()
*					- Time Complexity: O(1)
*					- Checks if the list is empty or not
*
*				List<type>::Node push(const type &value)
*					- Time Complexity: O(1)
*					- Adds a new element to the front of the list and returns a new
*				  	  node correspoding to the added element
*				List<type>::Node push(const List<type> &list)
*					- Time Complexity: O(1)
*					- Transfers all the elements from the input list to the front of
*				  	  this list and returns the first element of the input list
*				type pop()
*					- Time Complexity: O(1)
*					- Removes the front element of the list and returns a copy of its
*					  stored value
*
*				List<type>::Node enqueue(const type &value)
*					- Time Complexity: O(1)
*					- Adds a new element to the back of the list and returns a new
*				  	  node corresponding to the added element
*				List<type>::Node enqueue(const List<type> &list)
*					- Time Complexity: O(1)
*					- Transfers all the elements from the input list to the back of
*				  	  this list and returns the first element of the input list
*				type eject()
*					- Time Complexity: O(1)
*					- Removes the back element of the list and returns a copy of its
*					  stored value
*
*				void rotate_left()
*				void rotate_right()
*					- Time Complexity: O(1)
*					- Rotates the list clockwise/counter-clockwise
*					- left -> counter-clockwise | right -> clockwise
*
*				void shift_left()
*				void shift_right()
*					- Time Complexity: O(1)
*					- Shifts the elements of the list to the left/right by 1 bit
*
*				List<type>::Node random_sample()
*					- Time Complexity: O(1) - best case, O(n) - worst case
*					- Returns a random node sample from the list
*				List<type> random_subset(unsigned int count)
*					- Time Complexity: O(count) - best case, O(count*n) - worst case
*					- Returns a random subset from the list as a new list
*
*				List<type> split_front()
*					- Time Complexity: O(n)
*					- Splits the first half of the list and returns it as a new list
*				List<type> split_back()
*					- Time Complexity: O(n)
*					- Splits the last half of the list and returns it as a new list
*				List<type> split_shuffled()
*					- Time Complexity: O(n)
*					- Returns all elements in the even slots as a new list
*
*				void reverse()
*					- Time Complexity: O(n)
*					- Reverses the order of the elements of the list
*
*				void shuffle()
*					- Time Complexity: O(n)
*					- Performs a 'riffle shuffle' on the list
*
*				void randomize()
*					- Time Complexity: O(n)
*					- Randomizes the order of the elements of the list
*
*				void sort(bool (*handler)(const type&, const type&))
*					- Time Complexity: O(n log n)
*					- Sorts the order of the elements based on the boolean expression
*				  	  of the callback function (Uses merge sort algorithm)
*
*				void traverse(void (*callback)(const List<type>::Node&), bool clockwise)
*					- Runs the callback function for every node on the list
*					- The boolean parameter determines the direction of traversal
*				void traverse(bool (*handler)(const List<type>::Node&), bool clockwise)
*					- Cycles through the list while running the handler function for
*				  	  each node it traverses
*					- The cycling only stops when the handler function returns false
*					- The boolean parameter determines the direction of traversal
*
*				void clear()
*					- Time Complexity: O(n)
*					- Removes all the elements of the list
*
*				List<type>& operator<<(int bits)
*				List<type>& operator>>(int bits)
*					- Time Complexity: O(bits)
*					- Shifts the list to the left/right by a certain number of bits
*
*				type operator[](unsigned int index)
*					- Time Complexity: O(index)
*					- Returns the element found in a certain position in the list
*
*				List<type>& operator=(const List<type> &list)
*					- Time Complexity: O(n1 + n2)
*					- Copies the properties of the input list into this list
*
*				List<type>& operator+(const type &data)
*					- Time Complexity: O(n)
*					- Returns a new list composed of this list's elements plus the
*				  	  input data
*				List<type>& operator+(const List<type> &list)
*					- Time Complexity: O(n1 + n2)
*					- Returns the sum of this list and the input list as a new list
*
*				List<type>& operator+=(const type &data)
*					- Time Complexity: O(1)
*					- Enqueues a new element to the list
*				List<type>& operator+=(const List<type> &list)
*					- Time Complexity: O(1)
*					- Enqueues the input list into this list
*
*				bool operator==(const List<type> &list)
*				bool operator!=(const List<type> &list)
*				bool operator<=(const List<type> &list)
*				bool operator>=(const List<type> &list)
*				bool operator<(const List<type> &list)
*				bool operator>(const List<type> &list)
*					- Time Complexity: O(n1 + n2)
*					- Compares the sizes of two lists
*
*
*		class List<type>::Node
*
*			Constructors:
*
*				List<type>::Node()
*					- Time Complexity: O(1)
*					- Default constructor
*				List<type>::Node(const List<type>::Node &node)
*					- Time Complexity: O(1)
*					- Copy constructor
*
*			Methods:
*
*				static const List<type>::Node& null()
*					- Time Complexity: O(1)
*					- NULL node constant
*
*				type& data()
*					- Time Complexity: O(1)
*					- reference to node data
*
*				List<type>::Node prev()
*				List<type>::Node next()
*					- Time Complexity: O(1)
*					- The previous/next node of this node
*
*				List<type>::Node insert(const type &value)
*					- Time Complexity: O(1)
*					- Appends a new element next to this node
*				List<type>::Node insert(const List<type> &list)
*					- Time Complexity: O(1)
*					- Appends all the elements of the input list next to this node
*					- Elements of the input list are not copied but transfered
*				type remove()
*					- Time Complexity: O(1)
*					- Removes this node from its list and returns a copy of its
*					  stored value
*
*				void swap(const List<type>::Node &node)
*					- Time Complexity: O(1)
*					- Swaps the position of two nodes
*
*				void move(const List<type>::Node &node)
*					- Time Complexity: O(1)
*					- Moves this node next to the input node
*
*				void move_left()
*				void move_right()
*					- Time Complexity: O(1)
*					- Moves this node clockwise/counter-clockwise by 1 bit
*					- left -> counter-clockwise | right -> clockwise
*
*				List<type>::Node& operator<<(int bits)
*				List<type>::Node& operator>>(int bits)
*					- Time Complexity: O(bits)
*					- Sets the node pointer to a cetain number of bits before or after
*				  	  the current node
*					- << -> before | >> -> after
*
*				List<type>::Node& operator++()
*				List<type>::Node operator++(int)
*					- Time Complexity: O(1)
*					- Moves on to the next node
*
*				List<type>::Node& operator--()
*				List<type>::Node operator--(int)
*					- Time Complexity: O(1)
*					- Moves on to the previous node
*
*				List<type>::Node& operator=(const List<type>::Node &node)
*					- Time Complexity: O(1)
*					- Creates a shallow copy of the input node
*
*				bool operator==(const List<type>::Node &node)
*				bool operator!=(const List<type>::Node &node)
*					- Time Complexity: O(1)
*					- Checks the equality of two nodes
*
*******************************************************************************/
#ifndef __LINKEDLIST_HPP__
#define __LINKEDLIST_HPP__


#include <cstdlib>
#include <ctime>


#ifndef NULL
#define NULL 0
#endif


template <class T> struct List {


	class Node {


		friend struct List<T>;

		Node *__node,
			 *__prev,
			 *__next;
		T __data;

		inline Node(Node *node)
		: __node(node) {}

		void link(Node *node) const {
			Node *ptr = this->__node,
				 *next = ptr->__next;
			next->__prev = node;
			ptr->__next = node;
			node->__prev = ptr;
			node->__next = next;
		}
		void unlink() const {
			Node *ptr = this->__node,
				 *prev = ptr->__prev,
				 *next = ptr->__next;
			next->__prev = prev;
			prev->__next = next;
		}

		inline Node(const T &data)
		: __data(data) {}

	public:

		static const Node& null() {
			static Node null_node(NULL);
			return null_node;
		}

		inline T& data() const {
			return this->__node->__data;
		}
		inline Node prev() const {
			return this->__node->__prev;
		}
		inline Node next() const {
			return this->__node->__next;
		}

		Node insert(const T &data) const {
			Node *node = new Node(data);
			this->link(node);
			return node;
		}
		Node insert(const List<T> &list) const {
			Node *ptr = this->__node,
				 *next = ptr->__next,
				 *head = list.__head,
				 *first = head->__next,
				 *last = head->__prev;
			first->__prev = ptr;
			ptr->__next = first;
			next->__prev = last;
			last->__next = next;
			head->__prev = head;
			head->__next = head;
			return first;
		}
		T remove() const {
			T data = this->__node->__data;
			this->unlink();
			delete this->__node;
			return data;
		}

		void swap(const Node &node) const {
			Node *ptr = this->__node,
				 *other = node.__node,
				 *this_prev = ptr->__prev,
				 *this_next = ptr->__next,
				 *node_prev = other->__prev,
				 *node_next = other->__next;
			ptr->__prev = node_prev;
			ptr->__next = node_next;
			other->__prev = this_prev;
			other->__next = this_next;
			this_next->__prev = other;
			this_prev->__next = other;
			node_next->__prev = ptr;
			node_prev->__next = ptr;
		}

		void move(const Node &node) const {
			if(this != node.__node) {
				this->unlink();
				node.link(this->__node);
			}
		}
		inline void move_left() const {
			this->move(this->prev().prev());
		}
		inline void move_right() const {
			this->move(this->next());
		}

		Node& operator<<(int bits) {
			if(bits > 0)
				while(bits-- > 0)
					this->__node = this->__node->__prev;
			else
				while(bits++ < 0)
					this->__node = this->__node->__next;
			return *this;
		}
		Node& operator>>(int bits) {
			if(bits > 0)
				while(bits-- > 0)
					this->__node = this->__node->__next;
			else
				while(bits++ < 0)
					this->__node = this->__node->__prev;
			return *this;
		}

		Node& operator++() {
			this->__node = this->__node->__next;
			return *this;
		}
		Node operator++(int) {
			Node temp(*this);
			this->__node = this->__node->__next;
			return temp;
		}

		Node& operator--() {
			this->__node = this->__node->__prev;
			return *this;
		}
		Node operator--(int) {
			Node temp(*this);
			this->__node = this->__node->__prev;
			return temp;
		}

		Node& operator=(const Node &node) {
			this->__node = node.__node;
			return *this;
		}

		inline bool operator==(const Node &node) const {
			return this->__node == node.__node;
		}
		inline bool operator!=(const Node &node) const {
			return this->__node != node.__node;
		}

		inline Node()
		: __node(NULL) {}
		inline Node(const Node &node)
		: __node(node.__node) {}


	};

	friend class Node;

protected:

	Node *__head;

public:

	inline Node head() const {
		return this->__head;
	}
	inline Node first() const {
		return this->__head->__next;
	}
	inline Node last() const {
		return this->__head->__prev;
	}

	inline bool empty() const {
		return this->__head == this->__head->__next;
	}

	unsigned int size() const {
		unsigned int count = 0;
		for(Node *node = this->__head->__next; node != this->__head; node = node->__next)
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
		Node *head = this->__head,
			 *first = head->__next,
			 *last = head->__prev,
			 *new_first = first->__next;
		head->__prev = first;
		head->__next = new_first;
		first->__prev = last;
		first->__next = head;
		new_first->__prev = head;
		last->__next = first;
	}
	void rotate_right() const {
		Node *head = this->__head,
			 *first = head->__next,
			 *last = head->__prev,
			 *new_last = last->__prev;
		head->__prev = new_last;
		head->__next = last;
		last->__prev = head;
		last->__next = first;
		first->__prev = last;
		new_last->__next = head;
	}

	void shift_left() const {
		this->pop();
		this->enqueue(T());
	}
	void shift_right() const {
		this->eject();
		this->push(T());
	}

private:

	static int random_int(int lowbound, int upperbound) {
		int n = rand();
		time_t t = time(0);
		srand(n + (localtime(&t)->tm_sec));
		return rand()%(upperbound - lowbound + 1) + lowbound;
	}

public:

	Node random_sample() const {
		Node node(this->__head);
		for(int i = random_int(1, this->size()); i-- > 0; ++node);
		return node;
	}
	List random_subset(unsigned int size) const {
		List<T> temp, copy(*this);
		const Node &head(temp.__head);
		while(size-- > 0)
			copy.random_sample().move(head);
		return temp;
	}

	List<T> split_front() const {
		List<T> list;
		Node *list_head = list.__head,
			 *this_head = this->__head,
			 *this_first,
			 *node = this_head,
			 *first = node->__next;
		for(int i = this->size()/2; i-- > 0; node = node->__next);
		this_first = node->__next;
		this_first->__prev = this_head;
		this_head->__next = this_first;
		list_head->__prev = node;
		list_head->__next = first;
		first->__prev = list_head;
		node->__next = list_head;
		return list;
	}
	List<T> split_back() const {
		List<T> list;
		Node *list_head = list.__head,
			 *this_head = this->__head,
			 *list_last = this_head->__prev,
			 *list_first,
			 *node = this_head;
		for(int i = int(this->size()/2.0 + 0.5); i-- > 0; node = node->__next);
		list_first = node->__next;
		list_head->__prev = list_last;
		list_head->__next = list_first;
		list_first->__prev = list_head;
		list_last->__next = list_head;
		this_head->__prev = node;
		node->__next = this_head;
		return list;
	}
	List<T> split_shuffled() const {
		List<T> list;
		const Node &head(this->__head),
			 	   &first(head.next());
		Node node(first.next()), next;
		while(node != head && node != first) {
			next = node.next().next();
			node.move(list.last());
			node = next;
		}
		return list;
	}

	void reverse() const {
		Node *head = this->__head,
			 *first = head->__next,
			 *node = first->__next,
			 *last = first,
			 *next;
		while(node != head) {
			next = node->__next;
			node->__prev = head;
			node->__next = first;
			first->__prev = node;
			first = node;
			node = next;
		}
		head->__prev = last;
		head->__next = first;
		last->__next = head;
	}

	void shuffle() const {
		List<T> a, b;
		a.push(*this);
		b.enqueue(a.split_back());
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
		b.enqueue(a.split_back());
		time_t t = time(0);
		srand((localtime(&t))->tm_sec);
		while(!a.empty() || !b.empty()) {
			this->random_distribution(a);
			this->random_distribution(b);
		}
	}

	void sort(bool (*handler)(const T&, const T&)) const {
		List<T> a, b;
		a.enqueue(*this);
		b.enqueue(a.split_back());
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
		const Node &head(this->__head);
		if(clockwise) {
			for(Node node(head.next()); handler(node); ++node)
				if(node.next() == head)
					node = head;
		}else {
			for(Node node(head.prev()); handler(node); --node)
				if(node.prev() == head)
					node = head;
		}
	}
	void traverse(void (*callback)(const Node&), bool clockwise) const {
		const Node &head(this->__head);
		if(clockwise)
			for(Node node(head.next()); node != head; callback(node++));
		else
			for(Node node(head.prev()); node != head; callback(node--));
	}

	void clear() const {
		while(!this->empty())
			this->pop();
	}

	List<T>& operator<<(int bits) {
		if(bits > 0)
			while(bits-- > 0)
				this->shift_left();
		else
			while(bits++ < 0)
				this->shift_right();
		return *this;
	}
	List<T>& operator>>(int bits) {
		if(bits > 0)
			while(bits-- > 0)
				this->shift_right();
		else
			while(bits++ < 0)
				this->shift_left();
		return *this;
	}

	T operator[](unsigned int index) const {
		Node *head = this->__head,
			 *node = head->__next;
		for(int count = 0; node != head && count++ < index; node = node->__next);
		if(node == head || index < 0)
			return T();
		return node->__data;
	}

private:

	void add(const List<T> &list) {
		const Node &head(list.__head),
				   &last(this->last());
		for(Node node(head.prev()); node != head; --node)
			last.insert(node.data());
	}

public:

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
		Node *head = static_cast<Node*>(::operator new(sizeof(Node)));
		this->__head = head;
		head->__prev = head;
		head->__next = head;
	}

public:

	inline List() {
		this->init();
	}
	List(const List<T> &list) {
		this->init();
		this->add(list);
	}
	~List() {
		this->clear();
		::operator delete(this->__head);
	}


};
#endif
