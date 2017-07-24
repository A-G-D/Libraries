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
*			- 'key_type' must be a data type with working relational operators
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
*			bool operator==(HashTable<key_type, value_type> &table)
*			bool operator!=(HashTable<key_type, value_type> &table)
*			bool operator<=(HashTable<key_type, value_type> &table)
*			bool operator>=(HashTable<key_type, value_type> &table)
*			bool operator<(HashTable<key_type, value_type> &table)
*			bool operator>(HashTable<key_type, value_type> &table)
*				- Compares the size of two HashTables
*
*
*		class Hasher
*
*			- Provides pre-made hash functions
*
*			static unsigned int character(const char &key, unsigned int bucket_count)
*			static unsigned int short_integer(const short &key, unsigned int bucket_count)
*			static unsigned int integer(const int &key, unsigned int bucket_count)
*			static unsigned int long_integer(const long &key, unsigned int bucket_count)
*			static unsigned int long_long_integer(const long long &key, unsigned int bucket_count)
*
*			static unsigned int unsigned_character(const unsigned char &key, unsigned int bucket_count)
*			static unsigned int unsigned_short_integer(const unsigned short &key, unsigned int bucket_count)
*			static unsigned int unsigned_integer(const unsigned int &key, unsigned int bucket_count)
*			static unsigned int unsigned_long_integer(const unsigned long &key, unsigned int bucket_count)
*			static unsigned int unsigned_long_long_integer(const unsigned long long &key, unsigned int bucket_count)
*
*			static unsigned int string(const std::string key&, unsigned int bucket_count)
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

	inline HashTable() : buckets(1) {
		this->tree = new container[1];
	}

public:

	int size() const {
		int count = 0;
		for(int i = 0; i < this->buckets; count += this->tree[i++].size());
		return count;
	}

	bool empty() {
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

	HashTable(const thistype &table) {
		this->buckets = table.buckets;
		this->tree = new container[this->buckets];
		this->hasher = table.hasher;
		for(int i = 0; i < this->buckets; this->tree[i++] += table.tree[i]);
	}
	HashTable(unsigned int (*hash_function)(const key_type&, unsigned int), unsigned int bucket_count) {
		this->tree = new container[bucket_count];
		this->buckets = bucket_count;
		this->hasher = hash_function;
	}
	HashTable(unsigned int (*hash_function)(const key_type&, unsigned int)) {
		this->tree = new container[0x100];
		this->buckets = 0x100;
		this->hasher = hash_function;
	}
	inline ~HashTable() {
		delete[] this->tree;
	}


};


class Hasher {


	Hasher() {}
	~Hasher() {}

public:

	static unsigned int character(const char &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	static unsigned int short_integer(const short &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	static unsigned int integer(const int &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	static unsigned int long_integer(const long &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}
	static unsigned int long_long_integer(const long long &key, unsigned int bucket_count) {
		if(key < 0)
			return -key%bucket_count;
		return key%bucket_count;
	}

	inline static unsigned int unsigned_character(const unsigned char &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline static unsigned int unsigned_short_integer(const unsigned short &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline static unsigned int unsigned_integer(const unsigned int &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline static unsigned int unsigned_long_integer(const unsigned long &key, unsigned int bucket_count) {
		return key%bucket_count;
	}
	inline static unsigned int unsigned_long_long_integer(const unsigned long long &key, unsigned int bucket_count) {
		return key%bucket_count;
	}

	static unsigned int string(const std::string &key, unsigned int bucket_count) {
		long long int_key = 0;
		for(int i = 0; i < key.length(); int_key += key[i]*i++);
		return integer(int_key, bucket_count);
	}


};
#endif
