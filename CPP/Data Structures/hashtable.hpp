/*******************************************************************************
*
*	hashtable.hpp
*
*
*	Hash Table library
*
*		Used for storing large amount of records and is designed to handle
*		fast data insertions and retrievals. This hash table uses AVL trees
*		(height-balanced trees) to resolve collisions, therefore, the insert
*		and load operations run in O(1) time on the average and O(log n)
*		(base 2) in worst-case scenarios.
*
*
*	|-----|
*	| API |
*	|-----|
*
*		class HashTable<key_type, value_type>
*
*			- 'key_type' must have working relational operators
*			- 'value_type' must have a default constructor
*
*		Constructors:
*
*			HashTable(unsigned int (*hash_function)(const key_type&, unsigned int))
*				- Initializes a hashtable with 2^8 buckets
*			HashTable(unsigned int (*hash_function)(const key_type&, unsigned int), unsigned int bucket_count)
*				- Initializes a hashtable with a certain bucket count
*			HashTable(const HashTable<key_type, value_type> &table)
*				- Copy constructor
*
*		Methods:
*
*			int size()
*				- The number of elements in the HashTable
*			bool empty()
*				- Checks if there are no elements in the HashTable
*
*			bool has(const key_type &key)
*				- Checks if there is data stored in a certain key
*
*			value_type& load(const key_type &key)
*				- Loads the data stored in a certain key
*
*			void insert(const key_type &key, value_type data)
*				- Inserts a new data with a specific key into the HashTable
*
*			void remove(const key_type &key)
*				- Removes the data stored in a certain key
*
*			void clear()
*				- Removes all data stored in the HashTable
*
*			HashTable<key_type, value_type>& operator=(const HashTable<key_type, value_type> &table)
*				- Overwrites the data of this HashTable with the data of another
*				  HashTable
*
*			bool operator==(const HashTable<key_type, value_type> &table)
*			bool operator!=(const HashTable<key_type, value_type> &table)
*			bool operator<=(const HashTable<key_type, value_type> &table)
*			bool operator>=(const HashTable<key_type, value_type> &table)
*			bool operator<(const HashTable<key_type, value_type> &table)
*			bool operator>(const HashTable<key_type, value_type> &table)
*				- Compares the size of two HashTables
*
*
*		namespace Hasher
*
*			- Provides pre-made hash functions
*
*			unsigned int character(const char &key, unsigned int bucket_count)
*			unsigned int short_integer(const short &key, unsigned int bucket_count)
*			unsigned int integer(const int &key, unsigned int bucket_count)
*			unsigned int long_integer(const long &key, unsigned int bucket_count)
*			unsigned int long_long_integer(const long long &key, unsigned int bucket_count)
*
*			unsigned int unsigned_character(const unsigned char &key, unsigned int bucket_count)
*			unsigned int unsigned_short_integer(const unsigned short &key, unsigned int bucket_count)
*			unsigned int unsigned_integer(const unsigned int &key, unsigned int bucket_count)
*			unsigned int unsigned_long_integer(const unsigned long &key, unsigned int bucket_count)
*			unsigned int unsigned_long_long_integer(const unsigned long long &key, unsigned int bucket_count)
*
*			unsigned int string(const std::string key&, unsigned int bucket_count)
*
*
*******************************************************************************/
#ifndef __HASHTABLE_HPP__
#define __HASHTABLE_HPP__


#include <avl.hpp>
#include <string>


template <class key_type, class value_type> class HashTable {


	typedef HashTable<key_type, value_type> thistype;
	typedef AVL<key_type, value_type> container;

	template <class, class> friend class AVL;

	container *tree;
	unsigned int buckets;
	unsigned int (*hasher)(const key_type&, unsigned int);

	inline HashTable()
	: buckets(1), tree(new container[1]) {}

public:

	int size() const {
		int count = 0;
		for(int i = 0; i < this->buckets; count += this->tree[i++].size());
		return count;
	}

	bool empty() const {
		for(int i = 0; i < this->buckets; ++i)
			if(!this->tree[i].empty())
				return false;
		return true;
	}

	inline bool has(const key_type &key) {
		return this->tree[hasher(key, this->buckets)].has(key);
	}

	inline value_type& load(const key_type &key) {
		return this->tree[hasher(key, this->buckets)].search(key).data();
	}

	inline void insert(const key_type &key, const value_type &data) {
		this->tree[hasher(key, this->buckets)].insert_unique(key).data() = data;
	}

	inline void remove(const key_type &key) {
		this->tree[hasher(key, this->buckets)].remove(key);
	}

	inline void clear() {
		for(int i = 0; i < this->buckets; this->tree[i++].destroy());
	}

	thistype& operator=(const thistype &table) {
		delete[] this->tree;
		this->buckets = table.buckets;
		this->hasher = table.hasher;
		this->tree = new container[this->buckets];
		for(int i = 0; i < this->buckets; this->tree[i++] += table.tree[i]);
		return *this;
	}

	inline bool operator==(const thistype &table) const {
		return this->size() == table.size();
	}
	inline bool operator!=(const thistype &table) const {
		return this->size() != table.size();
	}

	inline bool operator<(const thistype &table) const {
		return this->size() < table.size();
	}
	inline bool operator>(const thistype &table) const {
		return this->size() > table.size();
	}

	inline bool operator<=(const thistype &table) const {
		return this->size() <= table.size();
	}
	inline bool operator>=(const thistype &table) const {
		return this->size() >= table.size();
	}

	inline HashTable(const thistype &table)
	: buckets(table.buckets), tree(new container[table.buckets]), hasher(table.hasher) {
		for(int i = 0; i < this->buckets; this->tree[i++] += table.tree[i]);
	}

	inline HashTable(unsigned int (*hash_function)(const key_type&, unsigned int), unsigned int bucket_count)
	: tree(new container[bucket_count]), buckets(bucket_count), hasher(hash_function) {}

	inline HashTable(unsigned int (*hash_function)(const key_type&, unsigned int))
	: tree(new container[0x100]), buckets(0x100), hasher(hash_function) {}

	inline ~HashTable() {
		delete[] this->tree;
	}


};


namespace Hasher {


	unsigned int character(const char &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	unsigned int short_integer(const short &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	unsigned int integer(const int &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	unsigned int long_integer(const long &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	unsigned int long_long_integer(const long long &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}

	inline unsigned int unsigned_character(const unsigned char &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline unsigned int unsigned_short_integer(const unsigned short &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline unsigned int unsigned_integer(const unsigned int &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline unsigned int unsigned_long_integer(const unsigned long &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline unsigned int unsigned_long_long_integer(const unsigned long long &key, unsigned int bucket_count) {
		return key%bucket_count;
	}

	unsigned int string(const std::string &key, unsigned int bucket_count) {
		long long int_key = 0;
		for(int i = 0; i < key.length(); int_key += key[i]*i++);
		return integer(int_key, bucket_count);
	}


};
#endif
