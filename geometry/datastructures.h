#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "common.h"
#include <iostream>
#include <queue>
#include "point.h"
#include "line.h"
//#include "arrangement.h"


namespace DDAD{

// SplayTree class
//
// CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// T find( x )   --> Return item that matches x
// T findMin( )  --> Return smallest item
// T findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void print( )          --> Print ascii tree


  // Node and forward declaration because g++ does
  // not understand nested classes.

template <class T>
class SplayTree;

template <class T>
class BinaryNode
{
public:
    T getElement() {return element;}

    T  element;
    BinaryNode<T> *left;
    BinaryNode<T> *right;

    BinaryNode<T>( ) : left( nullptr ), right( nullptr ) { }
    BinaryNode<T>( const T & theElement, BinaryNode<T> *lt, BinaryNode<T> *rt )
      : element( theElement ), left( lt ), right( rt ) { }

    friend class SplayTree<T>;
};


template <class T>
class SplayTree
{
  public:
    explicit SplayTree<T>();
    SplayTree<T>(BinaryNode<T>* newroot);
    SplayTree<T>( const SplayTree & rhs );

    ~SplayTree<T>( );

    void findMin( );
    void findMax( );
    void find( const T & x );
    bool isEmpty( ) const;
    SplayTree<T> SplitTree( const T & x);
    SplayTree<T> SplitTree( const Point_2r& split_here);
    void mergeTree(SplayTree<T> * R);
    int Size();

    void makeEmpty( );
    void insert( const T & x );
    bool remove( const T & x );

    void printBreadthFirst() const;
    void print();
    void print(BinaryNode<T>* node, std::string space);

    BinaryNode<T>* getRoot() {return root;}
    const BinaryNode<T>* getRoot() const {return root;}
    const SplayTree & operator=( const SplayTree & rhs );
    bool ContainsValue( const T & x );
    void Splay(const Point_2r& x, BinaryNode<T> *t);

  protected:
    BinaryNode<T> *root;
    int Size(BinaryNode<T> *root);
    const T & elementAt( BinaryNode<T> *t ) const;
    BinaryNode<T> * clone( BinaryNode<T> *t ) const;

    void Splay(const T & x, BinaryNode<T> *t );
};
/**
 * Construct the tree.
 */
template <class T>
SplayTree<T>::SplayTree()
{
    root = nullptr;
}
template <class T>
SplayTree<T>::SplayTree(BinaryNode<T> *newroot) {
    root = newroot;
}

/**
 * Copy constructor.
 */
template <class T>
SplayTree<T>::SplayTree( const SplayTree<T> & rhs )
{
    root = nullptr;
    *this = rhs;
}


/**
 * Destructor.
 */
template <class T>
SplayTree<T>::~SplayTree( )
{
    makeEmpty( );
}

/**
 * Insert x into the tree.
 */
template <class T>
void SplayTree<T>::insert( const T & x )
{
    BinaryNode<T> *newNode = new BinaryNode<T>;
    newNode->element = x;

    if( root == nullptr )
    {
        newNode->left = newNode->right = nullptr;
        root = newNode;
    }
    else
    {
        BinaryNode<T> *tmp_root = root;
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
template <class T>
bool SplayTree<T>::remove( const T & x )
{
    if(root == nullptr) return false; // Can't remove from an empty tree
    BinaryNode<T> *newTree;

    if( !ContainsValue(x) )
        return false;   // Item not found; do nothing
        // If x is found, it will be at the root
    Splay( x, root );

    if( root->left == nullptr )
        newTree = root->right;
    else
    {
        // Find the maximum in the left subtree
        SplayTree<T>* newSplay = new SplayTree<T>(root->left);
        newSplay->Splay(x, newSplay->getRoot());
        newTree = newSplay->getRoot();
        //assert(newTree->right == nullptr);
        newTree->right = root->right;
    }

//    delete root;
    root = newTree;
    return true;
}

/**
 * Find the smallest item in the tree.
 * Not the most efficient implementation (uses two passes), but has correct
 *     amortized behavior.
 * A good alternative is to first call Find with parameter
 *     smaller than any item in the tree, then call findMin.
 * Return the smallest item or ITEM_NOT_FOUND if empty.
 */
template <class T>
void SplayTree<T>::findMin( )
{
    if( isEmpty( ) )
        return;

    BinaryNode<T> *ptr = root;

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
template <class T>
void SplayTree<T>::findMax( )
{
    if( isEmpty( ) )
        return;

    BinaryNode<T> *ptr = root;

    while( ptr->right != nullptr )
        ptr = ptr->right;

    Splay( ptr->element, root );
    return;
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class T>
void SplayTree<T>::find( const T & x )
{
    if( isEmpty( ) )
        return;
    Splay( x, root );
    return;
}

/**
 * Make the tree logically empty.
 */
template <class T>
void SplayTree<T>::makeEmpty( )
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
template <class T>
bool SplayTree<T>::isEmpty( ) const
{
    return root == nullptr;
}

/**
  * Split the tree at elt into two subtrees such that subtree L is <= elt
  * and all remaining nodes in the tree are > elt
  */
template <class T>
SplayTree<T> SplayTree<T>::SplitTree(const T &x){
    // Splitting at a minimum/maximum value should always leave at least that
    // value in the tree
    SplayTree<T>* R;
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

template <class T>
SplayTree<T> SplayTree<T>::SplitTree(
        const Point_2r& split_here)
{
    // Splitting at a minimum/maximum value should always leave at least that
    // value in the tree
    SplayTree<T>* R;
    Splay(split_here, root);

    if(Predicate::PointAIsInB(split_here, root->getElement()) && root->right == nullptr)
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
template <class T>
void SplayTree<T>::mergeTree(SplayTree<T>* R){
    if(isEmpty()){
        root = R->getRoot();
        return;
    }
    findMax();  // Splay max(L) to top
    root->right = R->getRoot();
}

template <class T>
int SplayTree<T>::Size()
{
    return Size(root);
}


/**
 * Deep copy.
 */
template <class T>
const SplayTree<T> &
SplayTree<T>::operator=( const SplayTree<T> & rhs )
{
    if( this != &rhs )
    {
        makeEmpty( );
        root = clone( rhs.root );
    }

    return *this;
}

template <class T>
bool SplayTree<T>::ContainsValue(const T &x){
    if (isEmpty()) return false;
    BinaryNode<T>* tmp_root = root;
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
template <class T>
void SplayTree<T>::Splay( const T & x,
                                   BinaryNode<T> * t )
{

    BinaryNode<T> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    if((t->left == nullptr) && (t->right == nullptr)) return;

    while(true){
        if(DDAD::AIsBelowB(x, t->element)){
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
        else if(DDAD::AIsAboveB(x, t->element)){
            if(t->right == nullptr) break;
            if(DDAD::AIsAboveB(x, t->right->element)){
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
        if(DDAD::AIsBelowB(x, root->element) && DDAD::AIsBelowB(root->left->element, x)){
            t = root->left;
            root->left = t->right;
            t->right = root;
            root = t;
        }
    }
}

template <class T>
void SplayTree<T>::Splay( const Point_2r & x,
                                   BinaryNode<T> * t )
{

    BinaryNode<T> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    while(true){
        if(DDAD::AIsBelowB(x, t->element)){
            if(t->left == nullptr) break;
            if(DDAD::AIsBelowB(x, t->left->element)){
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
        else if(DDAD::AIsAboveB(x, t->element)){
            if(t->right == nullptr) break;
            if(DDAD::AIsAboveB(x, t->right->element)){
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
        if(DDAD::AIsBelowB(x, root->element) && DDAD::AIsAboveB(x, root->left->element)){
            t = root->left;
            root->left = t->right;
            t->right = root;
            root = t;
        }
    }
}


/*
 * Display Tree Elements
 */
template <class T>
void SplayTree<T>::printBreadthFirst() const{
    int depth = 0;
    std::queue<BinaryNode<T>*> q;
    if (root != nullptr) {
        q.push(root);
        std::cout << "\n" << root->element << " ";
    }
    while (!q.empty()){
        depth++;
        const BinaryNode<T>* const temp_node = q.front();
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

template <class T>
void SplayTree<T>::print(){
    print(root, std::string(""));
}

template <class T>
void SplayTree<T>::print(BinaryNode<T>* node,
                                  std::string space){
    if(node != nullptr){
        space.append("    ");
        print(node->right, space);
        std::cout << space <<node->element << "\n";
        print(node->left, space);
    }

}

template <class T>
int SplayTree<T>::Size(BinaryNode<T> *root)
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
template <class T>
BinaryNode<T> *
SplayTree<T>::clone( BinaryNode<T> * t ) const
{
//    if( t == t->left )  // Cannot test against nullptr!!!
    if(t == nullptr)
        return nullptr;
    else
        return new BinaryNode<T>( t->element, clone( t->left ),
                                           clone( t->right ) );
}


} //namespace DDAD

#endif // DATASTRUCTURES_H




























