/*******************************************************************************
*
*	sequence.hpp
*
*	Sequence<T>
*
*		Sequence is a data type that holds a list of elements. You can decode
*		the pattern of the sequence by inputing a data. If the input is correct,
*		the current sequence position will move on to the next position. If you
*		reach the end of the sequence and the input is correct, the sequence is
*		marked as solved.
*
*
*	Author: AGD
*
*
*	API:
*
*		class Sequence<type>
*
*			- <type> must have an overloaded operator==()
*
*			Constructors:
*
*				Sequence()
*					- Default Constructor
*				Sequence(const Sequence<type> &sequence)
*					- Copy Constructor
*				Sequence(bool auto_reset_flag)
*					- if the argument is true, the sequence will automatically call
*					  reset() every time the input in the decode() method in incorrect
*
*			Methods:
*
*				bool empty()
*				bool solved()
*
*				Sequence<type>& enqueue(const type &data)
*					- Adds a new element to the end of the sequence
*				Sequence<type>& enqueue(const type &data, bool stretchable)
*					- If <stretchable> is true, decode() will return true if the input
*					  data matches with the element in the previous position and the
*					  current position will not move on to the next
*				type dequeue()
*					- Removes the first element in the sequence
*				Sequence<type>& clear()
*					- Removes all the elements in the sequence
*
*				bool decode(const type &data)
*					- If the input data matches with the element in the current position in
*					  the sequence, the next element becomes the new current element and
*					  the function returns true
*					- If the input data matches with the element in the current position and
*					  it is the last position, then the sequence is marked as solved
*				void reset()
*					- Resets the current position of the sequence back to the beginning
*					- Solved sequences will be marked again as unsolved
*
*				Sequence<type>& operator=(const Sequence<type> &other)
*					- Copy assignment operator
*
*
*******************************************************************************/
#ifndef __SEQUENCE_HPP__
#define __SEQUENCE_HPP__


template <class type> class Sequence {


	struct Node {

		Node *__next;
		type __data;
		bool __stretchable;

	};


protected:

	Node *__iterator,
		 *__head,
		 *__last;
	bool auto_reset;

private:

	void init() {
		Node *head = new Node;
		this->__head = head;
		head->__next = head;
		this->__last = head;
		this->reset();
	}

public:

	inline bool empty() const {
		return this->__head == this->__head->__next;
	}
	inline bool solved() const {
		return !this->empty() && this->__iterator == this->__last;
	}

	Sequence<type>& enqueue(const type &data, bool stretchable) {
		Node *node = new Node,
			 *last = this->__last;
		node->__next = last->__next;
		last->__next = node;
		this->__last = node;
		node->__data = data;
		node->__stretchable = stretchable;
		return *this;
	}
	inline Sequence<type>& enqueue(const type &data) {
		return this->enqueue(data, false);
	}
	type dequeue() {
		Node *first = this->__head->__next;
		this->__head->__next = first->__next;
		type &data = first->__data;
		delete first;
		return data;
	}

	Sequence<type>& clear() {
		while(!this->empty())
			this->dequeue();
		this->reset();
		return *this;
	}

	bool decode(const type &data) {
		if(data == this->__iterator->__next->__data) {
			this->__iterator = this->__iterator->__next;
			return true;
		}else if(this->__iterator->__stretchable && data == this->__iterator->__data)
			return true;
		else if(this->auto_reset) {
			this->reset();
			if(data == this->__head->__next->__data)
				this->__iterator = this->__iterator->__next;
		}
		return false;
	}

	inline void reset() {
		this->__iterator = this->__head;
	}

	Sequence<type>& operator=(const Sequence<type> &other) {
		this->clear();
		this->auto_reset = other.auto_reset;
		for(Node *node = other.__head->__next; node != other.__head; node = node->__next)
			this->enqueue(node->__data, node->__stretchable);
		return *this;
	}

	inline Sequence()
	: auto_reset(true) {
		this->init();
	}
	inline Sequence(bool auto_reset_flag)
	: auto_reset(auto_reset_flag) {
		this->init();
	}
	Sequence(const Sequence<type> &sequence)
	: auto_reset(sequence.auto_reset) {
		this->init();
		for(Node *node = sequence.__head->__next; node != sequence.__head; node = node->__next)
			this->enqueue(node->__data, node->__stretchable);
	}

	~Sequence() {
		this->clear();
		delete this->__head;
	}


};
#endif
