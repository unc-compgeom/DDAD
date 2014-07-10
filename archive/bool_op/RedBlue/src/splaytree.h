

#include <iostream>  
#include <cmath>
#include <sstream>

// For NULL

// SplayTree class
// Author: http://users.cis.fiu.edu/~weiss/dsaa_c++/Code/
// Modified by: Sajal Dash to work in windows environment
// CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// Comparable find( x )   --> Return item that matches x
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
/****
	void merge(SplayTree &rhs);
	SplayTree& split(void);
****/
// Node and forward declaration because g++ does
// not understand nested classes.
template <class Comparable>
class SplayTree;

template <class Comparable>
class BinaryNode
{
public:
	Comparable *element;
	BinaryNode *left;
	BinaryNode *right;


	BinaryNode( ) : left( NULL ), right( NULL ) { }
	BinaryNode( Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
		: element( &theElement ), left( lt ), right( rt ) { }

	friend class SplayTree<Comparable>;
};


template <class Comparable>
class SplayTree
{
protected:
	BinaryNode<Comparable> *root;
	BinaryNode<Comparable> *nullNode;
	Comparable ITEM_NOT_FOUND;

	const Comparable & elementAt( BinaryNode<Comparable> *t ) const;

	void reclaimMemory( BinaryNode<Comparable> * t ) const
	{
		if( t != t->left )
		{
			reclaimMemory( t->left );
			reclaimMemory( t->right );
			delete t;
		}
	}

	BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const
	{
		if( t == t->left )  // Cannot test against nullNode!!!
			return nullNode;
		else
			return new BinaryNode<Comparable>( *t->element, clone( t->left ), clone( t->right ) );
	}

	// Tree manipulations
	void rotateWithLeftChild( BinaryNode<Comparable> * & k2 ) const
	{
		BinaryNode<Comparable> *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2 = k1;
	}
	void rotateWithRightChild( BinaryNode<Comparable> * & k1 ) const
	{
		BinaryNode<Comparable> *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1 = k2;
	}
	void Splay( Comparable & x, BinaryNode<Comparable> * & t ) const
	{
		BinaryNode<Comparable> *leftTreeMax, *rightTreeMin;
		static BinaryNode<Comparable> header;

		header.left = header.right = nullNode;
		leftTreeMax = rightTreeMin = &header;

		nullNode->element = &x;   // Guarantee a match

		for( ; ; )
			if( x < *t->element )
			{
				if( x < *t->left->element )
					rotateWithLeftChild( t );
				if( t->left == nullNode )
					break;
				// Link Right
				rightTreeMin->left = t;
				rightTreeMin = t;
				t = t->left;
			}
			else if( *t->element < x )
			{
				if( *t->right->element < x )
					rotateWithRightChild( t );
				if( t->right == nullNode )
					break;
				// Link Left
				leftTreeMax->right = t;
				leftTreeMax = t;
				t = t->right;
			}
			else
				break;

			leftTreeMax->right = t->left;
			rightTreeMin->left = t->right;
			t->left = header.right;
			t->right = header.left;
	}

	
	void printTree( BinaryNode<Comparable> *t ) const
	{

		if( t != t->left )
		{
			printTree( t->left );
			cout << *t->element << " ";
	
			printTree( t->right );
		}
	}

	void to_string( BinaryNode<Comparable> *t ) const
	{

		std::ostringstream os;
		if( t != t->left )
		{
			os << to_string( t->left );
			os << *t->element << " ";
	
			os << to_string( t->right );
		}

		return os.
	}

	public:
	SplayTree()
	{
		nullNode = new BinaryNode<Comparable>;
		nullNode->left = nullNode->right = nullNode;
		root = nullNode;

	}
	SplayTree( Comparable & notFound ): ITEM_NOT_FOUND( notFound )
	{
		nullNode = new BinaryNode<Comparable>;
		nullNode->left = nullNode->right = nullNode;
		nullNode->element = &notFound;
		root = nullNode;
	}

	SplayTree(Comparable & node, Comparable & notFound): ITEM_NOT_FOUND(notFound)
	{
		nullNode = new BinaryNode<Comparable>;
		nullNode->left = nullNode->right = nullNode;
		nullNode->element = &notFound;
		root = node;
	}

	SplayTree(BinaryNode<Comparable> * node, Comparable notFound): ITEM_NOT_FOUND(notFound)
	{
		nullNode = new BinaryNode<Comparable>;
		nullNode->left = nullNode->right = nullNode;
		nullNode->element = &notFound;
		root = node;
	}

	SplayTree( const SplayTree & rhs )
	{

		nullNode = new BinaryNode<Comparable>;
		nullNode->left = nullNode->right = nullNode;
		*nullNode->element = ITEM_NOT_FOUND;
		root = nullNode;
		*this = rhs;

	}
	~SplayTree( )
	{
		//makeEmpty( );
		//delete nullNode;
		//delete root;
		//cout << "Destroying" << endl;
	}

	const Comparable & findMin( )
	{
		if( isEmpty( ) )
			return ITEM_NOT_FOUND;

		BinaryNode<Comparable> *ptr = root;

		while( ptr->left != nullNode )
			ptr = ptr->left;

		Splay( *ptr->element, root );
		return *ptr->element;
	}



	const Comparable & findMax( )
	{
		if( isEmpty( ) )
			return ITEM_NOT_FOUND;

		BinaryNode<Comparable> *ptr = root;

		while( ptr->right != nullNode){
			if(ptr->right == NULL) cout << "Null pointer found" << endl;		
			ptr = ptr->right;
		}
		Splay( *ptr->element, root );
		return *ptr->element;
	}

	const Comparable & find( Comparable & x )
	{
		if( isEmpty( ) )
			return ITEM_NOT_FOUND;
		Splay( x, root );
		if( *root->element != x )
			return ITEM_NOT_FOUND;

		return *root->element;
	}

	BinaryNode<Comparable> * findNode( const Comparable & x )
	{
		if( isEmpty( ) )
			return NULL;
		Splay( x, root );
		if( root->element != x )
			return NULL;

		return root;
	}

	bool isEmpty( ) 
	{
		return root == nullNode;
	}

	void printTree( ) 
	{
		if( isEmpty( ) )
			cout << "Empty tree" << endl;
		else{
			printTree( root );
		}
	}

	std::string to_string()
	{
		std::ostringstream os;
		if(isEmpty())
			os << " Emptly tree" << endl;
		else
		{	
			os << to_string(root);
		}
		return os.

	}

	void makeEmpty( )
	{
		/******************************
		* Comment this out, because it is prone to excessive
		* recursion on degenerate trees. Use alternate algorithm.

		reclaimMemory( root );
		root = nullNode;
		*******************************/
		findMax( );        // Splay max item to root
		while( !isEmpty( ) )
			remove( *root->element );
	}

	BinaryNode<Comparable> * insert( Comparable & x )
	{
		static BinaryNode<Comparable> *newNode = NULL;
		BinaryNode<Comparable> *rv = NULL;
		if( newNode == NULL )
			newNode = new BinaryNode<Comparable>;
		newNode->element = &x;

		if( root == nullNode )
		{
			newNode->left = newNode->right = nullNode;
			root = newNode;
		}
		else
		{
			Splay( x, root );
			if( x < *root->element )
			{
				newNode->left = root->left;
				newNode->right = root;
				root->left = nullNode;
				root = newNode;
			}
			else
				if( *root->element < x )
				{
					newNode->right = root->right;
					newNode->left = root;
					root->right = nullNode;
					root = newNode;
				}
				else{
					rv = newNode;
					return rv;
				}
		}

		rv = newNode;
		newNode = NULL;   // So next insert will call new
		return rv;
	}

	void remove( Comparable & x )
	{
		BinaryNode<Comparable> *newTree;

		// If x is found, it will be at the root
		Splay( x, root );
		if( !(*root->element == x ))
			return;   // Item not found; do nothing

		if( root->left == nullNode )
			newTree = root->right;
		else
		{
			// Find the maximum in the left subtree
			// Splay it to the root; and then attach right child
			newTree = root->left;
			Splay( x, newTree );
			newTree->right = root->right;
		}
		//needs revision, this version is a preserving one, never destroys anything
		//delete root;
		root = newTree;
	}



	const SplayTree & operator=( const SplayTree & rhs )
	{
		if( this != &rhs )
		{
			makeEmpty( );
			root = clone( rhs.root );
		}

		return *this;
	}

	/****
	*** this tree is smaller than rightTree
	*** so, always call with a larger tree
	****/

	void merge(SplayTree<Comparable> &rightTree)
	{
		findMax();
		root->right = rightTree.root;
		rightTree.root = rightTree.nullNode;
	}


	SplayTree<Comparable> split(Comparable &value, Comparable notFound)
	{
		find(value);
		BinaryNode<Comparable> *temp = root->left;
		root->left = nullNode;

		return SplayTree(temp, notFound);
	}

	SplayTree split_right(Comparable &value, Comparable notFound)
	{
		find(value);
		BinaryNode<Comparable> *temp = root;
		root = root->left;

		return SplayTree(temp, notFound);
	}

	SplayTree split_left(Comparable &value, Comparable notFound)
	{
		find(value);
		BinaryNode<Comparable> *temp = root->left;
		root->left = nullNode;

		return SplayTree(temp, notFound);
	}
	void diagnoseRoot()
	{
		if(root == nullNode) cout << "Pointing to nullNode" << endl;
		if(root == NULL) cout << "pointing to NULL" << endl;
		if(root->left == nullNode) cout << "Pointing to nullNode" << endl;
		cout << root->left->element << " " << root->right->element << endl;
	}

};

