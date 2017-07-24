/*******************************************************************************
*
*	tablearray.hpp
*
*
*	|-----|
*	| API |
*	|-----|
*
*		class Table<key_type, value_type>
*		class Table#N#D<key_type, value_type>
*
*		- value of N: 2 -> 10
*		- 'key_type' must be a data type with working relational operators
*
*		Constructors:
*
*			Table(#N#D)(unsigned int (*hash_function)(const key_type&, unsigned int))
*				- Initializes a TableArray with 2^8 buckets per dimension
*			Table(#N#D)(unsigned int (*hash_function)(const key_type&, unsigned int), unsigned int bucket_count)
*				- Initializes a TableArray with a certain bucket count per dimension
*			Table(#N#D)(const Table(#N#D)<key_type, value_type> &table)
*				- Copy constructor
*
*		Methods:
*
*			operator[](const key_type &key)
*			- Accesses a cetain position in the given dimension
*
*			Table(#N#D)<key_type, value_type>& operator=(const Table(#N#)D<key_type, value_type> &table)
*			- Copy assignment operator
*
*			bool empty()
*			- Checks if there are no value stored within the key's descendants
*
*			bool has(const key_type &key)
*			- Checks whether the key or any of its descendants has any
*			  value
*
*			void remove(const key_type &key)
*			- Removes the key and all its descendant keys and all their
*			  corresponding values
*
*			void clear()
*			- Removes all the keys (including their descendants) and their
*			  corresponding values
*
*
*	|---------|
*	| Example |
*	|---------|
*
*		Table4D<int, std::string> tablearray;
*		tablearray[100][200][5][0x2000] = "String";
*		std::cout<<tablearray[100][200][5][0x2000]<<"\n";	(Dislays "String")
*
*
*******************************************************************************/
#ifndef __TABLE_ARRAY_HPP__
#define __TABLE_ARRAY_HPP__


#include <hashtable.hpp>
#include <linkedlist.hpp>


template <class key_type, class value_type> class Table {


	template <class, class> friend class AVL;

	typedef Table<key_type, value_type> thistype;

	HashTable<key_type, value_type> hashtable;

	inline Table()
	: hashtable(NULL, 1) {}

public:

	value_type& operator[](const key_type &key) {
		if(!this->hashtable.has(key))
			this->hashtable.insert(key, value_type());
		return this->hashtable.load(key);
	}
	thistype& operator=(const thistype &table) {
		this->hashtable = table.hashtable;
		return *this;
	}

	inline bool empty() {
		return this->hashtable.empty();
	}
	inline bool has(const key_type &key) {
		return this->hashtable.has(key);
	}
	inline void remove(const key_type &key) {
		this->hashtable.remove(key);
	}
	inline void clear() {
		this->hashtable.clear();
	}

	inline Table(const thistype &table)
	: hashtable(table.hashtable) {}

	inline Table(unsigned int (*hash_function)(const key_type&, unsigned int))
	: hashtable(hash_function) {}

	inline Table(unsigned int (*hash_function)(const key_type&, unsigned int), unsigned int bucket_count)
	: hashtable(hash_function, bucket_count) {}

	inline ~Table() {}


};


#define TABLE_ARRAY(NAME, SUBCLASS)                                        									\
template <class key_type, class value_type> class NAME {													\
																											\
																											\
	template <class, class> friend class AVL;																\
																											\
	typedef NAME<key_type, value_type> thistype;															\
	typedef SUBCLASS<key_type, value_type> subclass;														\
																											\
	HashTable<key_type, subclass> hashtable;																\
	List<key_type> list;																					\
	unsigned int (*hasher)(const key_type&, unsigned int);													\
	unsigned int buckets;																					\
																											\
	inline NAME() 																							\
	: hashtable(NULL, 1) {}																					\
																											\
public:																										\
																											\
	subclass& operator[](const key_type &key) {																\
		if(!this->hashtable.has(key)) {																		\
			this->hashtable.insert(key, subclass(this->hasher, this->buckets));								\
			this->list.enqueue(key);																		\
		}																									\
		return this->hashtable.load(key);																	\
	}																										\
																											\
	thistype& operator=(const thistype &table) {															\
		this->hashtable = table.hashtable;																	\
		this->list = table.list;																			\
		this->hasher = table.hasher;																		\
		this->buckets = table.buckets;																		\
		return *this;																						\
	}																										\
																											\
	bool empty() {																							\
		for(typename List<key_type>::Node node = this->list.first(); node != this->list.head(); ++node)		\
			if(!this->hashtable.load(node.data()).empty())													\
				return false;																				\
		return true;																						\
	}																										\
																											\
	inline bool has(const key_type &key) {																	\
		return !this->hashtable.load(key).empty();															\
	}																										\
																											\
	void remove(const key_type &key) {																		\
		this->hashtable.load(key).clear();																	\
		this->hashtable.remove(key);																		\
	}																										\
																											\
	void clear() {																							\
		for(typename List<key_type>::Node node = this->list.first(); node != this->list.head(); ++node)		\
			this->hashtable.load(node.data()).clear();														\
		this->hashtable.clear();																			\
		this->list.clear();																					\
	}																										\
																											\
	inline NAME(const thistype &table)																		\
	: hashtable(table.hashtable), list(table.list), hasher(table.hasher), buckets(table.buckets) {}			\
																											\
	inline NAME(unsigned int (*hash_function)(const key_type&, unsigned int))								\
	: hashtable(hash_function, 0x100), hasher(hash_function), buckets(0x100) {}								\
																											\
	inline NAME(unsigned int (*hash_function)(const key_type&, unsigned int), unsigned int bucket_count)  	\
	: hashtable(hash_function, bucket_count), hasher(hash_function), buckets(bucket_count) {}				\
																											\
	inline ~NAME() {}																						\
																											\
																											\
};																							 			 	 

TABLE_ARRAY(Table2D, Table)
TABLE_ARRAY(Table3D, Table2D)
TABLE_ARRAY(Table4D, Table3D)
TABLE_ARRAY(Table5D, Table4D)
TABLE_ARRAY(Table6D, Table5D)
TABLE_ARRAY(Table7D, Table6D)
TABLE_ARRAY(Table8D, Table7D)
TABLE_ARRAY(Table9D, Table8D)
TABLE_ARRAY(Table10D, Table9D)

#undef TABLE_ARRAY


#endif
