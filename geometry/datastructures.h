#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "common.h"
#include <iostream>
#include <queue>
#include "point.h"
#include "line.h"


namespace DDAD{

// SplayTree class
//
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
// void print( )          --> Print ascii tree


  // Node and forward declaration because g++ does
  // not understand nested classes.

template <class Comparable>
class SplayTree;

template <class Comparable>
class BinaryNode
{
public:
    Comparable getElement() {return element;}

    Comparable  element;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode( ) : left( nullptr ), right( nullptr ) { }
    BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
      : element( theElement ), left( lt ), right( rt ) { }

    friend class SplayTree<Comparable>;
};


template <class Comparable>
class SplayTree
{
  public:
    explicit SplayTree();
    SplayTree(BinaryNode<Comparable>* newroot);
    SplayTree( const SplayTree & rhs );

    ~SplayTree( );

    void findMin( );
    void findMax( );
    void find( const Comparable & x );
    bool isEmpty( ) const;
    SplayTree<Comparable> SplitTree( const Comparable & x);
    SplayTree<Comparable> SplitTree( const Point_2r& split_here);
    void mergeTree(SplayTree<Comparable> * R);
    int Size();

    void makeEmpty( );
    void insert( const Comparable & x );
    void remove( const Comparable & x );

    void printBreadthFirst() const;
    void print();
    void print(BinaryNode<Comparable>* node, std::string space);

    BinaryNode<Comparable>* getRoot() {return root;}
    const BinaryNode<Comparable>* getRoot() const {return root;}
    const SplayTree & operator=( const SplayTree & rhs );
    bool ContainsValue( const Comparable & x );
    void Splay(const Point_2r& x, BinaryNode<Comparable> *t);

  protected:
    BinaryNode<Comparable> *root;
    int Size(BinaryNode<Comparable> *root);
    const Comparable & elementAt( BinaryNode<Comparable> *t ) const;
    BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;

    void Splay(const Comparable & x, BinaryNode<Comparable> *t );
};
/**
 * Construct the tree.
 */
template <class Comparable>
SplayTree<Comparable>::SplayTree()
{
    root = nullptr;
}
template <class Comparable>
SplayTree<Comparable>::SplayTree(BinaryNode<Comparable> *newroot) {
    root = newroot;
}

/**
 * Copy constructor.
 */
template <class Comparable>
SplayTree<Comparable>::SplayTree( const SplayTree<Comparable> & rhs )
{
    root = nullptr;
    *this = rhs;
}


/**
 * Destructor.
 */
template <class Comparable>
SplayTree<Comparable>::~SplayTree( )
{
    makeEmpty( );
}

/**
 * Insert x into the tree.
 */
template <class Comparable>
void SplayTree<Comparable>::insert( const Comparable & x )
{
    BinaryNode<Comparable> *newNode = new BinaryNode<Comparable>;
    newNode->element = x;

    if( root == nullptr )
    {
        newNode->left = newNode->right = nullptr;
        root = newNode;
    }
    else
    {
        BinaryNode<Comparable> *tmp_root = root;
        while(tmp_root != nullptr){
            if(x < tmp_root->getElement())
            {
                if(tmp_root->left == nullptr){
                    tmp_root->left = newNode;
                    break;
                }
                tmp_root = tmp_root->left;
            }
            else if(x > tmp_root->getElement())
            {
                if(tmp_root->right == nullptr){
                    tmp_root->right = newNode;
                    break;
                }
                tmp_root = tmp_root->right;
            }
            else return; // No duplicates
        }
        Splay(x, root);
    }
}

/**
 * Remove x from the tree.
 */
template <class Comparable>
void SplayTree<Comparable>::remove( const Comparable & x )
{
    if(root == nullptr) return; // Can't remove from an empty tree
    BinaryNode<Comparable> *newTree;
    if( !ContainsValue(x) )
        return;   // Item not found; do nothing
        // If x is found, it will be at the root
    Splay( x, root );

    if( root->left == nullptr )
        newTree = root->right;
    else
    {
        // Find the maximum in the left subtree
        // Splay it to the root; and then attach right child
        newTree = root->left;
        Splay( x, newTree );
        newTree->right = root->right;
    }

//    delete root;
    root = newTree;
}

/**
 * Find the smallest item in the tree.
 * Not the most efficient implementation (uses two passes), but has correct
 *     amortized behavior.
 * A good alternative is to first call Find with parameter
 *     smaller than any item in the tree, then call findMin.
 * Return the smallest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
void SplayTree<Comparable>::findMin( )
{
    if( isEmpty( ) )
        return;

    BinaryNode<Comparable> *ptr = root;

    while( ptr->left != nullptr )
        ptr = ptr->left;

    Splay( ptr->element, root );
    return;
}

/**
 * Find the largest item in the tree.
 * Not the most efficient implementation (uses two passes), but has correct
 *     amortized behavior.
 * A good alternative is to first call Find with parameter
 *     larger than any item in the tree, then call findMax.
 * Return the largest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
void SplayTree<Comparable>::findMax( )
{
    if( isEmpty( ) )
        return;

    BinaryNode<Comparable> *ptr = root;

    while( ptr->right != nullptr )
        ptr = ptr->right;

    Splay( ptr->element, root );
    return;
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class Comparable>
void SplayTree<Comparable>::find( const Comparable & x )
{
    if( isEmpty( ) )
        return;
    Splay( x, root );
    return;
}

/**
 * Make the tree logically empty.
 */
template <class Comparable>
void SplayTree<Comparable>::makeEmpty( )
{
/******************************
 * Comment this out, because it is prone to excessive
 * recursion on degenerate trees. Use alternate algorithm.

    reclaimMemory( root );
    root = nullptr;
 *******************************/
    findMax( );        // Splay max item to root
    while( !isEmpty( ) )
        remove( root->element );
}

/**
 * Test if the tree is logically empty.
 * @return true if empty, false otherwise.
 */
template <class Comparable>
bool SplayTree<Comparable>::isEmpty( ) const
{
    return root == nullptr;
}

/**
  * Split the tree at elt into two subtrees such that subtree L is <= elt
  * and all remaining nodes in the tree are > elt
  */
template <class Comparable>
SplayTree<Comparable> SplayTree<Comparable>::SplitTree(const Comparable &x){
    // Splitting at a minimum/maximum value should always leave at least that
    // value in the tree
    SplayTree<Comparable>* R;
    find(x);  // Indexed elements are Splayed to the top of the tree
    if(x == root->getElement() && root->right == nullptr)
    {
        R = new SplayTree();
        R->insert(root->getElement());
        root = root->left;
    }
    else
    {
        R = new SplayTree(root->right);
        root->right = nullptr;
    }
    return *R;
}

template <class Comparable>
SplayTree<Comparable> SplayTree<Comparable>::SplitTree(const Point_2r& split_here)
{
    // Splitting at a minimum/maximum value should always leave at least that
    // value in the tree
    SplayTree<Comparable>* R;
    Splay(split_here, root);

    if(split_here == root->getElement() && root->right == nullptr)
    {
        R = new SplayTree();
        R->insert(root->getElement());
        root = root->left;
    }
    else
    {
        R = new SplayTree(root->right);
        root->right = nullptr;
    }
    return *R;
}


/**
  * Merge tree R into tree L (current tree). L must be < R.
  */
template <class Comparable>
void SplayTree<Comparable>::mergeTree(SplayTree<Comparable>* R){
    if(isEmpty()){
        root = R->getRoot();
        return;
    }
    findMax();  // Splay max(L) to top
    root->right = R->getRoot();
}

template <class Comparable>
int SplayTree<Comparable>::Size()
{
    return Size(root);
}


/**
 * Deep copy.
 */
template <class Comparable>
const SplayTree<Comparable> &
SplayTree<Comparable>::operator=( const SplayTree<Comparable> & rhs )
{
    if( this != &rhs )
    {
        makeEmpty( );
        root = clone( rhs.root );
    }

    return *this;
}

template <class Comparable>
bool SplayTree<Comparable>::ContainsValue(const Comparable &x){
    if (isEmpty()) return false;
    BinaryNode<Comparable>* tmp_root = root;
    while(tmp_root != nullptr){
        if(x < tmp_root->getElement()) tmp_root = tmp_root->left;
        else if(x > tmp_root->getElement()) tmp_root = tmp_root->right;
        else return true;
    }
    return false;
}

/**
 * Internal method to perform a top-down Splay.
 * The last accessed node becomes the new root.
 * This method may be overridden to use a different
 * Splaying algorithm, however, the Splay tree code
 * depends on the accessed item going to the root.
 * x is the target item to Splay around.
 * t is the root of the subtree to Splay.
 */
template <class Comparable>
void SplayTree<Comparable>::Splay( const Comparable & x,
                                   BinaryNode<Comparable> * t )
{

    BinaryNode<Comparable> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    while(true){
        if(x < t->element){
            if(t->left == nullptr) break;
            if(x < t->left->element){
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            R->left = t;
            R = t;
            t = t->left;
        }
        else if(x > t->element){
            if(t->right == nullptr) break;
            if(x > t->right->element){
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            L->right = t;
            L = t;
            t = t->right;
        }
        else break;
    }

    L->right = t->left;
    R->left = t->right;
    t->left = N.right;
    t->right = N.left;
    root = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root->left != nullptr){
        if(x < root->element && root->left->element < x){
            t = root->left;
            root->left = t->right;
            t->right = root;
            root = t;
        }
    }
}

template <class Comparable>
void SplayTree<Comparable>::Splay( const Point_2r & x,
                                   BinaryNode<Comparable> * t )
{

    BinaryNode<Comparable> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    while(true){
        if(x < t->element){
            if(t->left == nullptr) break;
            if(x < t->left->element){
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            R->left = t;
            R = t;
            t = t->left;
        }
        else if(x > t->element){
            if(t->right == nullptr) break;
            if(x > t->right->element){
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            L->right = t;
            L = t;
            t = t->right;
        }
        else break;
    }

    L->right = t->left;
    R->left = t->right;
    t->left = N.right;
    t->right = N.left;
    root = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root->left != nullptr){
        if(x < root->element && x > root->left->element){
            t = root->left;
            root->left = t->right;
            t->right = root;
            root = t;
        }
    }
}


/*
 * DiSplay Tree Elements
 */
template <class Comparable>
void SplayTree<Comparable>::printBreadthFirst() const{
    int depth = 0;
    std::queue<BinaryNode<Comparable>*> q;
    if (root != nullptr) {
        q.push(root);
        std::cout << "\n" << root->element << " ";
    }
    while (!q.empty()){
        depth++;
        const BinaryNode<Comparable>* const temp_node = q.front();
        q.pop();

        if(temp_node->left != nullptr){
            q.push(temp_node->left);
            std::cout << "\n" << (temp_node->left)->element << " ";
        } else {
            std::cout << "\n" << "NULL";
        }
        if (temp_node->right != nullptr){
            q.push(temp_node->right);
            std::cout << "\n" << (temp_node->right)->element << " ";
        } else {
            std::cout << "\n" << "NULL";
        }
    }
    std::cout << "\n";
}

template <class Comparable>
void SplayTree<Comparable>::print(){
    print(root, std::string(""));
}

template <class Comparable>
void SplayTree<Comparable>::print(BinaryNode<Comparable>* node, std::string space){
    if(node != nullptr){
        space.append("    ");
        print(node->right, space);
        std::cout << space <<node->element << "\n";
        print(node->left, space);
    }

}

template <class Comparable>
int SplayTree<Comparable>::Size(BinaryNode<Comparable> *root)
{
    if(root == nullptr) return 0;
    int tree_size = 1;
    if(root->right != nullptr) tree_size += Size(root->right);
    if(root->left != nullptr) tree_size += Size(root->left);

    return tree_size;
}

/**
 * Internal method to clone subtree.
 * WARNING: This is prone to running out of stack space.
 */
template <class Comparable>
BinaryNode<Comparable> *
SplayTree<Comparable>::clone( BinaryNode<Comparable> * t ) const
{
//    if( t == t->left )  // Cannot test against nullptr!!!
    if(t == nullptr)
        return nullptr;
    else
        return new BinaryNode<Comparable>( t->element, clone( t->left ), clone( t->right ) );
}


} //namespace DDAD

#endif // DATASTRUCTURES_H




























