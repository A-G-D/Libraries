/*******************************************************************************
*
*	avl.hpp
*
*
*	AVL Tree
*		- a height-balanced binary search tree
*
*
*	|-----|
*	| API |
*	|-----|
*
*		Note: 'key_type' must be a fundamental data type or object with working
*			  relational operators
*
*
*		class AVL<key_type, value_type>
*
*		Constructors:
*
*			AVL()
*				- Default Constructor
*			AVL(const AVL<key_type, value_type> &tree)
*				- Copy Constructor
*
*		Methods:
*
*			AVL<key_type, value_type>::Node root()
*				- The root node of the tree
*
*			int size()
*				- Returns the number of elements in the tree
*			bool empty()
*				- Checks if there are no elements in the tree
*
*			bool has(key_type key)
*				- Checks if the tree contains a certain key
*
*			AVL<key_type, value_type>::Node insert(key_type key)
*				- Inserts a new key into the tree regardless of whether the tree already
*				  contains a similar key
*			void insert(const AVL<key_type, value_type> &tree)
*				- Inserts each key from a given tree into this tree
*
*			AVL<key_type, value_type>::Node insert_unique(key_type key)
*				- Inserts a new key into the tree is the tree does not already contain a
*				  similar key
*				- If the tree already has the key, it returns the node corresponding to
*				  that existing key
*			void insert_unique(const AVL<key_type, value_type> &tree)
*				- Inserts each key from a given tree into this tree if this tree does not
*				  already contain a similar key
*
*			AVL<key_type, value_type>::Node search(key_type key)
*				- Seaches for the first node matching the given key
*			AVL<key_type, value_type>::Node search_close(key_type key)
*				- Returns the node first matching the given key if the tree contains the
*				  key
*				- Returns the lowest node that has a key closest to the given key 
*
*			value_type remove(key_type key)
*				- Removes the node first matching the given key from the tree and returns
*				  the value stored on the certain key
*
*			void destroy()
*				- Destroys the root node of this tree and its descendants
*
*			void traverse_inorder(void (*callback)(const AVL<key_type, value_type>::Node&))
*			void traverse_preorder(void (*callback)(const AVL<key_type, value_type>::Node&))
*			void traverse_postorder(void (*callback)(const AVL<key_type, value_type>::Node&))
*				- Traverses the tree in a certain order
*
*			AVL<key_type, value_type>& operator=(const AVL<key_type, value_type> &tree)
*				- Copy assignment operator
*
*			AVL<key_type, value_type>& operator+=(const AVL<key_type, value_type> &tree)
*			AVL<key_type, value_type>& operator+=(key_type key)
*				- Works similar to insert() but returns a reference to this tree
*
*			AVL<key_type, value_type> operator+(const AVL<key_type, value_type> &tree)
*			AVL<key_type, value_type> operator+(key_type key)
*				- Returns a new tree which is the sum of this tree and the input given
*
*			bool operator==(const AVL<key_type, value_type> &tree)
*			bool operator!=(const AVL<key_type, value_type> &tree)
*			bool operator<=(const AVL<key_type, value_type> &tree)
*			bool operator>=(const AVL<key_type, value_type> &tree)
*			bool operator<(const AVL<key_type, value_type> &tree)
*			bool operator>(const AVL<key_type, value_type> &tree)
*				- Tree size relational operators
*
*
*		class AVL<key_type, value_type>::Node
*
*		Constructors:
*
*			AVL<key_type, value_type>::Node()
*				- Default Constructor
*
*		Methods:
*
*			static AVL<key_type, value_type>::Node& null()
*				- Null node constant
*
*			AVL<key_type, value_type>::Node left()
*				- Left node of this node
*			AVL<key_type, value_type>::Node right()
*				- Right node of this node
*			AVL<key_type, value_type>::Node parent()
*				- Parent node of this node
*			AVL<key_type, value_type>::Node sibling()
*				- Other child of the parent of this node
*
*			key_type key()
*				- Key of this node
*			value_type& data()
*				- Date reference of this node
*
*			bool is_root()
*			bool is_leaf()
*				- Checks if the node is a root/leaf node
*
*			void remove()
*				- Removes this node from the tree
*
*			void destroy()
*				- Removes this node and its descendants from the tree
*
*			AVL<key_type, value_type>::Node& operator=(const AVL<key_type, value_type>::Node &node)
*				- Copy assignment operator
*
*			bool operator==(const AVL<key_type, value_type>::Node &node)
*			bool operator!=(const AVL<key_type, value_type>::Node &node)
*				- Checks the equality of two nodes
*
*
*******************************************************************************/
#ifndef __AVL_HPP__
#define __AVL_HPP__


#include <stddef.h>
#include <linkedlist.hpp>
#include <vector>


template <class key_type, class value_type> class AVL {


	typedef AVL<key_type, value_type> thistype;

public:

	class Node {


		friend class AVL<key_type, value_type>;

		thistype *p_tree;
		Node *p_ptr,
			 *p_left,
			 *p_right,
			 *p_parent;
		key_type p_key;
		value_type p_data;
		int	p_height;

		int size() const {
			int count = 1;
			if(this->left() != null())
				count += this->left().size();
			if(this->right() != null())
				count += this->right().size();
			return count;
		}

		Node insert(key_type key) {
			if(key < this->p_key)
				if(this->p_left == NULL) {
					this->p_left = new Node;
					this->p_left->p_left = NULL;
					this->p_left->p_right = NULL;
					this->p_left->p_parent = this;
					this->p_left->p_tree = this->p_tree;
					this->p_left->p_key = key;
					this->update_height();
					return Node(this->p_left);
				}else
					return this->p_left->insert(key);
			else
				if(this->p_right == NULL) {
					this->p_right = new Node;
					this->p_right->p_left = NULL;
					this->p_right->p_right = NULL;
					this->p_right->p_parent = this;
					this->p_right->p_tree = this->p_tree;
					this->p_right->p_key = key;
					this->update_height();
					return Node(this->p_right);
				}else
					return this->p_right->insert(key);
		}

		void traverse_inorder(void (*callback)(const Node&)) {
			if(this->p_left != NULL)
				this->p_left->traverse_inorder(callback);
			callback(Node(this));
			if(this->p_right != NULL)
				this->p_right->traverse_inorder(callback);
		}
		void traverse_preorder(void (*callback)(const Node&)) {
			callback(Node(this));
			if(this->p_left != NULL)
				this->p_left->traverse_preorder(callback);
			if(this->p_right != NULL)
				this->p_right->traverse_preorder(callback);
		}
		void traverse_postorder(void (*callback)(const Node&)) {
			if(this->p_left != NULL)
				this->p_left->traverse_postorder(callback);
			if(this->p_right != NULL)
				this->p_right->traverse_postorder(callback);
			callback(Node(this));
		}

		void ondestroy() {
			if(this->p_left != NULL)
				this->p_left->ondestroy();
			if(this->p_right != NULL)
				this->p_right->ondestroy();
			if(this->p_parent == this)
				this->p_tree->p_root.p_ptr = NULL;
			else if(this->p_parent->p_left == this)
				this->p_parent->p_left = NULL;
			else
				this->p_parent->p_right = NULL;
			delete this;
		}

		void update_height() {
			int left_height = 0, right_height = 0;
			if(this->p_left != NULL)
				left_height = this->p_left->p_height;
			if(this->p_right != NULL)
				right_height = this->p_right->p_height;
			if(left_height > right_height)
				this->p_height = left_height + 1;
			this->p_height = right_height + 1;
		}

		void finish_rotate(Node *pivot) {
			if(this->p_parent == this)
				this->p_tree->p_root.p_ptr = pivot;
			else if(this->p_parent->p_left == this)
				this->p_parent->p_left = pivot;
			else
				this->p_parent->p_right = pivot;
			if(this->p_parent == this)
				pivot->p_parent = pivot;
			else
				pivot->p_parent = this->p_parent;
			this->p_parent = pivot;
			this->update_height();
			pivot->update_height();
		}

		void rotate_left() {
			Node *pivot = this->p_ptr,
				 *parent = pivot->p_parent;
			if(pivot == NULL)
				return;
			else if(parent == pivot)
				this->right().rotate_left();
			else if(parent->p_right != pivot)
				return;
			else {
				parent->p_right = pivot->p_left;
				pivot->p_left = parent;
				parent->finish_rotate(pivot);
			}
		}
		void rotate_right() {
			Node *pivot = this->p_ptr,
				 *parent = pivot->p_parent;
			if(pivot == NULL)
				return;
			else if(parent == pivot)
				this->left().rotate_right();
			else if(parent->p_left != pivot)
				return;
			else {
				parent->p_left = pivot->p_right;
				pivot->p_right = parent;
				parent->finish_rotate(pivot);
			}
		}

		void balance() {
			Node *node = this->p_ptr;
			if(node != NULL) {
				while(true) {
					Node *left = node->p_left,
						 *right = node->p_right;
					node->update_height();
					int lheight = 0, rheight = 0;
					if(left != NULL) lheight = left->p_height;
					if(right != NULL) rheight = right->p_height;
					if(lheight - rheight == 2) {
						int llheight = 0, lrheight = 0;
						if(left->p_left != NULL) llheight = left->p_left->p_height;
						if(left->p_right != NULL) lrheight = left->p_right->p_height;
						if(llheight - lrheight == -1)
							Node(left).right().rotate_left();
						Node(left).rotate_right();
						return;
					}else if(lheight - rheight == -2) {
						int rlheight = 0, rrheight = 0;
						if(right->p_left != NULL) rlheight = right->p_left->p_height;
						if(right->p_right != NULL) rrheight = right->p_right->p_height;
						if(rlheight - rrheight == 1)
							Node(right).left().rotate_right();
						Node(right).rotate_left();
						return;
					}
					if(node == node->p_parent)
						break;
					node = node->p_parent;
				}
			}
		}

		void tree_to_list(List<Node> &list) {
			if(this->p_left != NULL)
				this->p_left->tree_to_list(list);
			list.insert(Node(this));
			if(this->p_right != NULL)
				this->p_right->tree_to_list(list);
		}
		void tree_to_vector(std::vector<Node> &vec, int &position) {
			if(this->p_left != NULL)
				this->p_left->tree_to_vector(vec, position);
			vec[position++] = Node(this);
			if(this->p_right != NULL)
				this->p_right->tree_to_vector(vec, position);
		}

		inline Node(Node *node) {
			this->p_ptr = node;
		}

	public:

		static Node& null() {
			static Node null_node(NULL);
			return null_node;
		}

		inline key_type key() const {
			return this->p_ptr->p_key;
		}
		inline Node left() const {
			return Node(this->p_ptr->p_left);
		}
		inline Node right() const {
			return Node(this->p_ptr->p_right);
		}
		inline Node parent() const {
			return Node(this->p_ptr->p_parent);
		}
		Node sibling() const {
			if(this->p_ptr->p_parent == this->p_ptr)
				return null();
			else if(this->p_ptr->p_parent->p_left == this->p_ptr)
				return Node(this->p_ptr->p_parent->p_right);
			else
				return Node(this->p_ptr->p_parent->p_left);
		}

		inline value_type& data() const {
			return this->p_ptr->p_data;
		}

		inline bool is_root() const {
			return this->p_ptr->p_parent == this->p_ptr;
		}
		inline bool is_leaf() const {
			return this->p_ptr->p_left == NULL && this->p_ptr->p_right == NULL;
		}

		void remove() {
			Node *node = this->p_ptr;
			if(node->p_parent == node && node->p_left == NULL && node->p_right == NULL)
				node->p_tree->p_root.p_ptr = NULL;
			else if(node->p_left != NULL && node->p_right != NULL) {
				Node *least = node->p_right;
				while(least->p_left != NULL)
					least = least->p_left;
				if(node->p_parent == node)
					node->p_tree->p_root.p_ptr = least;
				else if(node->p_parent->p_left == node)
					node->p_parent->p_left = least;
				else
					node->p_parent->p_right = least;
				if(least->p_parent->p_left == least)
					least->p_parent->p_left = least->p_right;
				else
					least->p_parent->p_right = least->p_right;
				if(node->p_right == least)
					least->p_right = NULL;
				else
					least->p_right = node->p_right;
				least->p_left = node->p_left;
				least->p_parent = node->p_parent;
				least->update_height();
			}else if(node->p_left != NULL) {
				if(node->p_parent == node)
					node->p_tree->p_root.p_ptr = node->p_left;
				else if(node->p_parent->p_left == node)
					node->p_parent->p_left = node->p_left;
				else
					node->p_parent->p_right = node->p_left;
				node->p_left->p_parent = node->p_parent;
			}else if(node->p_right != NULL) {
				if(node->p_parent == node)
					node->p_tree->p_root.p_ptr = node->p_right;
				else if(node->p_parent->p_left == node)
					node->p_parent->p_left = node->p_right;
				else
					node->p_parent->p_right = node->p_right;
				node->p_right->p_parent = node->p_parent;
			}else {
				if(node->p_parent->p_left == node)
					node->p_parent->p_left = NULL;
				else
					node->p_parent->p_right = NULL;
			}
			Node(node->p_parent).balance();
			delete node;
		}

		void destroy() {
			this->p_ptr->ondestroy();
			this->p_ptr = NULL;
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

		Node() : p_data() {
			this->p_ptr = NULL;
			this->p_tree = NULL;
			this->p_left = NULL;
			this->p_right = NULL;
			this->p_parent = NULL;
			this->p_height = 1;
		}
		~Node() {
			this->p_ptr = NULL;
			this->p_tree = NULL;
			this->p_left = NULL;
			this->p_right = NULL;
			this->p_parent = NULL;
		}


	};

private:

	friend class Node;

	Node p_root;

	void insert_to_tree(Node *node) const {
		if(node->p_left != NULL)
			this->insert_to_tree(node->p_left);
		this->insert(node->p_key).data() = node->p_data;
		if(node->p_right != NULL)
			this->insert_to_tree(node->p_right);
	}
	void insert_to_tree_unique(Node *node) const {
		if(node->p_left != NULL)
			this->insert_to_tree_unique(node->p_left);
		if(this->search(node->p_key) == Node::null())
			this->insert(node->p_key).data() = node->p_data;
		if(node->p_right != NULL)
			this->insert_to_tree_unique(node->p_right);
	}

public:

	inline Node root() const {
		return Node(this->p_root.p_ptr);
	}

	int size() const {
		if(this->p_root.p_ptr == NULL)
			return 0;
		return this->root().size();
	}

	inline bool empty() const {
		return this->p_root.p_ptr == NULL;
	}

	inline bool has(key_type key) {
		return this->search(key) != Node::null();
	}

	Node search(key_type key) {
		Node *node = this->p_root.p_ptr;
		while(node != NULL && key != node->p_key)
			if(key < node->p_key)
				node = node->p_left;
			else
				node = node->p_right;
		return Node(node);
	}
	Node search_close(key_type key) {
		Node *node = this->p_root.p_ptr;
		if(node == NULL)
			return Node::null();
		while(key != node->p_key)
			if(key < node->p_key)
				if(node->p_left == NULL)
					return Node(node);
				else
					node = node->p_left;
			else
				if(node->p_right == NULL)
					return Node(node);
				else
					node = node->p_right;
		return Node(node);
	}

	Node insert(key_type key) const {
		if(this->p_root.p_ptr == NULL) {
			Node *node = new Node;
			const_cast<thistype*>(this)->p_root.p_ptr = node;
			node->p_parent = node;
			node->p_key = key;
			node->p_tree = const_cast<thistype*>(this);
			return Node(node);
		}
		Node node = this->p_root.p_ptr->insert(key);
		node.parent().parent().balance();
		return node;
	}
	void insert(const thistype &tree) {
		if(tree.p_root.p_ptr != NULL)
			this->insert_to_tree(tree.p_root.p_ptr);
	}

	Node insert_unique(key_type key) {
		Node node = this->search(key);
		if(node == Node::null())
			return this->insert(key);
		return node;
	}
	void insert_unique(const thistype &tree) {
		if(tree.p_root.p_ptr != NULL)
			this->insert_to_tree_unique(tree.p_root.p_ptr);
	}

	value_type remove(key_type key) {
		Node node = this->search(key);
		if(node == Node::null())
			return value_type();
		value_type data = node.data();
		node.remove();
		return data;
	}

	void destroy() {
		if(this->p_root.p_ptr != NULL)
			this->p_root.destroy();
	}

	List<Node> to_list() {
		List<Node> list;
		this->p_root.p_ptr->tree_to_list(list);
		return list;
	}
	std::vector<Node> to_vector() {
		std::vector<Node> vec(this->size());
		int position = 0;
		this->p_root.p_ptr->tree_to_vector(vec, position);
		return vec;
	}

	void traverse_inorder(void (*callback)(const Node&)) const {
		if(this->p_root.p_ptr != NULL)
			this->p_root.p_ptr->traverse_inorder(callback);
	}
	void traverse_preorder(void (*callback)(const Node&)) const {
		if(this->p_root.p_ptr != NULL)
			this->p_root.p_ptr->traverse_preorder(callback);
	}
	void traverse_postorder(void (*callback)(const Node&)) const {
		if(this->p_root.p_ptr != NULL)
			this->p_root.p_ptr->traverse_postorder(callback);
	}

	thistype& operator=(const thistype &tree) {
		this->destroy();
		this->insert(tree);
		return *this;
	}

	thistype& operator+=(key_type key) {
		this->insert(key);
		return *this;
	}
	thistype& operator+=(const thistype &tree) {
		this->insert(tree);
		return *this;
	}

	thistype operator+(key_type key) {
		thistype temp(*this);
		this->insert(key);
		return temp;
	}
	thistype operator+(const thistype &tree) {
		thistype temp(*this);
		this->insert(tree);
		return temp;
	}

	inline bool operator==(const thistype &tree) const {
		return this->size() == tree.size();
	}
	inline bool operator!=(const thistype &tree) const {
		return this->size() != tree.size();
	}

	inline bool operator<=(const thistype &tree) const {
		return this->size() <= tree.size();
	}
	inline bool operator>=(const thistype &tree) const {
		return this->size() >= tree.size();
	}

	inline bool operator<(const thistype &tree) const {
		return this->size() < tree.size();
	}
	inline bool operator>(const thistype &tree) const {
		return this->size() > tree.size();
	}

	inline AVL() {}
	inline AVL(const thistype &tree) {
		this->insert(tree);
	}
	inline ~AVL() {
		this->destroy();
	}


};
#endif
