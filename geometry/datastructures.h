#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "common.h"
#include <iostream>
#include <queue>

namespace DDAD{


//=============================================================================
// Binary search tree
//=============================================================================
template <typename T>
struct TreeNode{

    TreeNode* left_;
    TreeNode* right_;
    TreeNode* parent_;
    T val_;
    int level_;

    TreeNode() {
        left_ = NULL;
        right_ = NULL;
        parent_ = NULL;
        val_ = NULL;
        level_ = 1;
    }

    TreeNode(T inival){
        left_ = NULL;
        right_ = NULL;
        parent_ = NULL;
        val_ = inival;
        level_ = 1;
    }

    TreeNode(TreeNode<T> &theNode){
        TreeNode<T> *copy = new TreeNode();
        copy->parent_ = theNode.parent_;
        copy->left_ = theNode.left_;
        copy->right_ = theNode.right_;
        copy->val_ = theNode.val_;
        copy->level_ = theNode.level_;
        return *copy;
    }

    ~TreeNode(){
        if(parent_ != NULL){
            if(parent_->left_ == this){
                parent_->left_ = NULL;
            }
            else if(parent_->right_ == this){
                parent_->right_ = NULL;
            }
        }
    }

};

template <typename T>
class Binary_ST{

public:
    Binary_ST(){
        root_ = NULL;
    }

    void Insert(TreeNode<T> *newNode, TreeNode<T> *root){
        // Insert a new node
        if(root_ == NULL) root_ = newNode;
        else if(newNode->val_ < root->val_){
            if(root->left_ == NULL) {
                root->left_ = newNode;
                newNode->parent_ = root;
            }
            else Insert(newNode, root->left_);
        }
        else if(newNode->val_ > root->val_) {
            if(root->right_ == NULL) {
                root->right_ = newNode;
                newNode->parent_ = root;
            }
            else Insert(newNode, root->right_);
        }
        else std::cout << "Duplicate node found; node not added";



//        if(root_ == NULL) root_ = newNode; // Check if tree is empty
//        TreeNode<T>* parentNode = Binary_ST<T>::SearchTree(newNode->val_ ,root_);
//        std::cout << "\nInserting " << newNode->val_ << ", found parent node " << parentNode->val_;

//        if(newNode->val_ > parentNode->val_){
//            //insert new node as right child of parent
//            newNode->parent_ = parentNode;
//            parentNode->right_ = newNode;

//        }
//        else if(newNode->val_ < parentNode->val_){
//            //insert new node as left child of parent
//            newNode->parent_ = parentNode;
//            parentNode->left_ = newNode;
//        }
    }

    TreeNode<T>* getRoot(){
        return root_;
    }

    void Insert(T& newVal){
        Binary_ST::Insert(Binary_ST::SearchTree(newVal, root_));
    }

    void Delete(TreeNode<T> &node){
        //case 1: node to delete has no children
        if((node.left_ == NULL) && (node.right_ == NULL)){
            node.~TreeNode();
        }
        //case 2: node to delete has 1 child
        if(node.left_ == NULL){ // Only has a right child
            node.left_ = node.right_->right_;
            node.val_ = node.right_->val_;
            node.right_ = node.right_->right_;
        }
        else if(node.right_ == NULL){ // Only has a left child
            node.right_ = node.left_->right_;
            node.val_ = node.left_->val_;
            node.left_ = node.left_->left_;
        }
        //case 3: node to delete has 2 children
        TreeNode<T> *N = node.left_;
        while(N->right_ != NULL) N = N->right_;
        node.val_ = N->val_;
        N->~TreeNode();
    }


    void RotateRight(TreeNode<T> *pivotNode){
        TreeNode<T>* parent = pivotNode->parent_;

        if(parent == NULL) { // Identity transformation
            std::cout << "\nCan't rotate the root node!";
            return;
        }
        else{ // Right rotation
            parent->left_ = pivotNode->right_;
            pivotNode->right_->parent_= parent;
            pivotNode->right_ = parent;
        }

        // Deal with parents farther up the tree
        if(parent->parent_ != NULL){
            if(parent->parent_->left_ == parent) parent->parent_->left_ = pivotNode;
            else parent->parent_->right_ = pivotNode;
        }
        else{ // Pivot is the new root of the tree
            root_ = pivotNode;
        }

        pivotNode->parent_ = parent->parent_;
        parent->parent_ = pivotNode;

    }

    void RotateLeft(TreeNode<T> *pivotNode){
        TreeNode<T>* parent = pivotNode->parent_;

        if(parent == NULL) { // Identity transformation
            std::cout << "\nCan't rotate the root node!";
            return;
        }
        else{ // Left rotation
            parent->right_ = pivotNode->left_;
            pivotNode->left_->parent_ = parent;
            pivotNode->left_ = parent;
        }

        // Deal with parents farther up the tree
        if(parent->parent_ != NULL){
            if(parent->parent_->left_ == parent) parent->parent_->left_ = pivotNode;
            else parent->parent_->right_ = pivotNode;
        }
        else{ // Pivot is the new root of the tree
            root_ = pivotNode;
        }

        pivotNode->parent_ = parent->parent_;
        parent->parent_ = pivotNode;

    }


    void Rotate(T& val){
        Binary_ST::Rotate(Binary_ST::SearchTree(newVal, root_));
    }

    TreeNode<T>* SearchTree(T& val, TreeNode<T> *root){
        // Return a pointer to the node equal to val, or, if val not in tree,
        //   return the node that would be its parent
        if(val < root->val_){
            // Look at left child
            if(root->left_ == NULL) return root;
            else return Binary_ST<T>::SearchTree(val, root->left_);
        }
        else if(val > root->val_){
            // Look at right child
            if(root->right_ == NULL) return root;
            else return Binary_ST<T>::SearchTree(val, root->right_);
        }
        else{
            // Found val;
            return root;
        }
    }

    bool NodeInTree(T& val){
        return Binary_ST<T>::SearchTree(val, root_)->val_ == val;
    }

    bool NodeInTree(TreeNode<T> &node){
        return Binary_ST<T>::SearchTree(node.val_, root_) == *node;
    }

    void PrintTree(){
        int depth = 0;
        std::queue<TreeNode<T>*> q;
        if (root_ != NULL) {
            q.push(root_);
            std::cout << "\n" << root_->val_ << " ";
        }
        while (!q.empty()){
            depth++;
            const TreeNode<T>* const temp_node = q.front();
            q.pop();

            if(temp_node->left_ != NULL){
                q.push(temp_node->left_);
                std::cout << "\n" << (temp_node->left_)->val_ << " ";
            } else {
                std::cout << "\n" << "NULL";
            }
            if (temp_node->right_ != NULL){
                q.push(temp_node->right_);
                std::cout << "\n" << (temp_node->right_)->val_ << " ";
            } else {
                std::cout << "\n" << "NULL";
            }
        }
    }
protected:
    TreeNode<T>* root_;
};


//=============================================================================
// AA Tree
//=============================================================================

// Every node is either red or black
// The root is black
// External nodes are black
// If a node is red, then its children must be black
// All paths from any node to a descendent leaf must contain the same number of
//   black nodes
// Left children may not be red
template <class Comparable>
class AATree;

template <class Comparable>
class AANode
{
    Comparable element;
    AANode    *left;
    AANode    *right;
    int        level;

    AANode( ) : left( NULL ), right( NULL ), level( 1 ) { }
    AANode( const Comparable & e, AANode *lt, AANode *rt, int lv = 1 )
      : element( e ), left( lt ), right( rt ), level( lv ) { }

    friend class AATree<Comparable>;
};

template <class Comparable>
class AATree
{
  public:
    explicit AATree( const Comparable & notFound );
    AATree( const AATree & rhs );
    ~AATree( );

    const Comparable & findMin( ) const;
    const Comparable & findMax( ) const;
    const Comparable & find( const Comparable & x ) const;
    bool isEmpty( ) const;
    void printTree( ) const;
    void printBreadthFirst() const;

    void makeEmpty( );
    void insert( const Comparable & x );
    void remove( const Comparable & x );

    const AATree & operator=( const AATree & rhs );
  private:
    AANode<Comparable> *root;
    const Comparable ITEM_NOT_FOUND;
    AANode<Comparable> *nullNode;

      // Recursive routines
    void insert( const Comparable & x, AANode<Comparable> * & t );
    void remove( const Comparable & x, AANode<Comparable> * & t );
    void makeEmpty( AANode<Comparable> * & t );
    void printTree( AANode<Comparable> *t ) const;

      // Rotations
    void skew( AANode<Comparable> * & t ) const;
    void split( AANode<Comparable> * & t ) const;
    void rotateWithLeftChild( AANode<Comparable> * & t ) const;
    void rotateWithRightChild( AANode<Comparable> * & t ) const;
    AANode<Comparable> * clone( AANode<Comparable> * t ) const;
};

/**
 * Construct the tree.
 */
template <class Comparable>
AATree<Comparable>::AATree( const Comparable & notFound ) :
  ITEM_NOT_FOUND( notFound )
{
    nullNode = new AANode<Comparable>;
    nullNode->left = nullNode->right = nullNode;
    nullNode->level = 0;
    root = nullNode;
}

/**
 * Copy constructor.
 */
template <class Comparable>
AATree<Comparable>::AATree( const AATree<Comparable> & rhs ) :
  ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
{
    nullNode = new AANode<Comparable>;
    nullNode->left = nullNode->right = nullNode;
    nullNode->level = 0;
    root = clone( rhs.root );
}

/**
 * Destructor for the tree.
 */
template <class Comparable>
AATree<Comparable>::~AATree( )
{
    makeEmpty( );
    delete nullNode;
}

/**
 * Insert x into the tree; duplicates are ignored.
 */
template <class Comparable>
void AATree<Comparable>::insert( const Comparable & x )
{
    insert( x, root );
}

/**
 * Remove x from the tree. Nothing is done if x is not found.
 */
template <class Comparable>
void AATree<Comparable>::remove( const Comparable & x )
{
    remove( x, root );
}

/**
 * Find the smallest item in the tree.
 * Return smallest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & AATree<Comparable>::findMin( ) const
{
    if( isEmpty( ) )
        return ITEM_NOT_FOUND;

    AANode<Comparable> *ptr = root;
    while( ptr->left != nullNode )
        ptr = ptr->left;

    return ptr->element;
}

/**
 * Find the largest item in the tree.
 * Return the largest item of ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & AATree<Comparable>::findMax( ) const
{
    if( isEmpty( ) )
        return ITEM_NOT_FOUND;

    AANode<Comparable> *ptr = root;
    while( ptr->right != nullNode )
        ptr = ptr->right;

    return ptr->element;
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class Comparable>
const Comparable & AATree<Comparable>::
find( const Comparable & x ) const
{
    AANode<Comparable> *current = root;
    nullNode->element = x;

    for( ; ; )
    {
        if( x < current->element )
            current = current->left;
        else if( current->element < x )
            current = current->right;
        else if( current != nullNode )
            return current->element;
        else
            return ITEM_NOT_FOUND;
    }
}

/**
 * Make the tree logically empty.
 */
template <class Comparable>
void AATree<Comparable>::makeEmpty( )
{
    makeEmpty( root );
}

/**
 * Test if the tree is logically empty.
 * Return true if empty, false otherwise.
 */
template <class Comparable>
bool AATree<Comparable>::isEmpty( ) const
{
    return root == nullNode;
}

/**
 * Print the tree contents in sorted order.
 */
template <class Comparable>
void AATree<Comparable>::printTree( ) const
{
    if( root == nullNode )
        std::cout << "Empty tree" << endl;
    else
        printTree( root );
}
template <class Comparable>
/*
 * Display Tree Elements
 */
void AATree<Comparable>::printBreadthFirst() const{
    int depth = 0;
    std::queue<AANode<Comparable>*> q;
    if (root != nullNode) {
        q.push(root);
        std::cout << "\n" << root->element << " ";
    }
    while (!q.empty()){
        depth++;
        const AANode<Comparable>* const temp_node = q.front();
        q.pop();

        if(temp_node->left != nullNode){
            q.push(temp_node->left);
            std::cout << "\n" << (temp_node->left)->element << " ";
        } else {
            std::cout << "\n" << "NULL";
        }
        if (temp_node->right != nullNode){
            q.push(temp_node->right);
            std::cout << "\n" << (temp_node->right)->element << " ";
        } else {
            std::cout << "\n" << "NULL";
        }
    }
}

/**
 * Deep copy.
 */
template <class Comparable>
const AATree<Comparable> &
AATree<Comparable>::operator=( const AATree<Comparable> & rhs )
{
    if( this != &rhs )
    {
        makeEmpty( );
        root = clone( rhs.root );
    }

    return *this;
}

/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the tree.
 * Set the new root.
 */
template <class Comparable>
void AATree<Comparable>::
insert( const Comparable & x, AANode<Comparable> * & t )
{
    if( t == nullNode )
        t = new AANode<Comparable>( x, nullNode, nullNode );
    else if( x < t->element )
        insert( x, t->left );
    else if( t->element < x )
        insert( x, t->right );
    else
        return;  // Duplicate; do nothing

    skew( t );
    split( t );
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the tree.
 * Set the new root.
 */
template <class Comparable>
void AATree<Comparable>::
remove( const Comparable & x, AANode<Comparable> * & t )
{
    static AANode<Comparable> *lastNode, *deletedNode = nullNode;

    if( t != nullNode )
    {
        // Step 1: Search down the tree and set lastNode and deletedNode
        lastNode = t;
        if( x < t->element )
            remove( x, t->left );
        else
        {
            deletedNode = t;
            remove( x, t->right );
        }

        // Step 2: If at the bottom of the tree and
        //         x is present, we remove it
        if( t == lastNode )
        {
            if( deletedNode == nullNode || x != deletedNode->element )
                return;   // Item not found; do nothing
            deletedNode->element = t->element;
            deletedNode = nullNode;
            t = t->right;
            delete lastNode;
        }

        // Step 3: Otherwise, we are not at the bottom; rebalance
        else
            if( t->left->level < t->level - 1 ||
                t->right->level < t->level - 1 )
            {
                if( t->right->level > --t->level )
                    t->right->level = t->level;
                skew( t );
                skew( t->right );
                skew( t->right->right );
                split( t );
                split( t->right );
            }
    }
}

/**
 * Internal method to make subtree empty.
 */
template <class Comparable>
void AATree<Comparable>::makeEmpty( AANode<Comparable> * & t )
{
    if( t != nullNode )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullNode;
}

/**
 * Internal method to print a subtree in sorted order.
 * @param t the node that roots the tree.
 */
template <class Comparable>
void AATree<Comparable>::printTree( AANode<Comparable> *t ) const
{
    if( t != nullNode )
    {
        printTree( t->left );
        std::cout << t->element << endl;
        printTree( t->right );
    }
}


/**
 * Rotate binary tree node with left child.
 */
template <class Comparable>
void AATree<Comparable>::rotateWithLeftChild( AANode<Comparable> * & k2 ) const
{
    AANode<Comparable> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2 = k1;
}

/**
 * Rotate binary tree node with right child.
 */
template <class Comparable>
void AATree<Comparable>::rotateWithRightChild( AANode<Comparable> * & k1 ) const
{
    AANode<Comparable> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1 = k2;
}

/**
 * Skew primitive for AA-trees.
 * t is the node that roots the tree.
 */
template <class Comparable>
void AATree<Comparable>::skew( AANode<Comparable> * & t ) const
{
    if( t->left->level == t->level )
        rotateWithLeftChild( t );
}

/**
 * Split primitive for AA-trees.
 * t is the node that roots the tree.
 */
template <class Comparable>
void AATree<Comparable>::split( AANode<Comparable> * & t ) const
{
    if( t->right->right->level == t->level )
    {
        rotateWithRightChild( t );
        t->level++;
    }
}
/**
 * Internal method to clone subtree.
 */
template <class Comparable>
AANode<Comparable> *
AATree<Comparable>::clone( AANode<Comparable> * t ) const
{
    if( t == t->left )  // Cannot test against nullNode!!!
        return nullNode;
    else
        return new AANode<Comparable>( t->element, clone( t->left ),
                                       clone( t->right ), t->level );
}

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
// void printTree( )      --> Print tree in sorted order


  // Node and forward declaration because g++ does
  // not understand nested classes.

template <class Comparable>
class SplayTree;

template <class Comparable>
class BinaryNode
{
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
    explicit SplayTree( const Comparable & notFound );
    SplayTree(const Comparable & notFound, BinaryNode<Comparable>* newroot);
    SplayTree( const SplayTree & rhs );

    ~SplayTree( );

    const Comparable & findMin( );
    const Comparable & findMax( );
    const Comparable & find( const Comparable & x );
    bool isEmpty( ) const;
    SplayTree<Comparable> splitTree( const Comparable & x);

    void makeEmpty( );
    void insert( const Comparable & x );
    void remove( const Comparable & x );

    void printBreadthFirst() const;

    const SplayTree & operator=( const SplayTree & rhs );

  private:
    BinaryNode<Comparable> *root;
    const Comparable ITEM_NOT_FOUND;

    const Comparable & elementAt( BinaryNode<Comparable> *t ) const;
    BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;

        // Tree manipulations
    void rotateWithLeftChild( BinaryNode<Comparable> * & k2 ) const;
    void rotateWithRightChild( BinaryNode<Comparable> * & k1 ) const;
    void splay( const Comparable & x, BinaryNode<Comparable> * & t ) const;
};
/**
 * Construct the tree.
 */
template <class Comparable>
SplayTree<Comparable>::SplayTree( const Comparable & notFound )
  : ITEM_NOT_FOUND( notFound )
{
    root = nullptr;
}
template <class Comparable>
SplayTree<Comparable>::SplayTree(const Comparable & notFound, BinaryNode<Comparable> *newroot) : ITEM_NOT_FOUND(notFound) {
    root = newroot;
}

/**
 * Copy constructor.
 */
template <class Comparable>
SplayTree<Comparable>::SplayTree( const SplayTree<Comparable> & rhs )
  : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
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
    static BinaryNode<Comparable> *newNode = nullptr;

    if( newNode == nullptr )
        newNode = new BinaryNode<Comparable>;
    newNode->element = x;

    if( root == nullptr )
    {
        newNode->left = newNode->right = nullptr;
        root = newNode;
    }
    else
    {
        splay( x, root );
        if( x < root->element )
        {
            newNode->left = root->left;
            newNode->right = root;
            root->left = nullptr;
            root = newNode;
        }
        else
        if( root->element < x )
        {
            newNode->right = root->right;
            newNode->left = root;
            root->right = nullptr;
            root = newNode;
        }
        else
            return;
    }
    newNode = nullptr;   // So next insert will call new
}

/**
 * Remove x from the tree.
 */
template <class Comparable>
void SplayTree<Comparable>::remove( const Comparable & x )
{
    if(root == nullptr) return; // Can't remove from an empty tree
    BinaryNode<Comparable> *newTree;

        // If x is found, it will be at the root
    splay( x, root );
    if( root->element != x )
        return;   // Item not found; do nothing

    if( root->left == nullptr )
        newTree = root->right;
    else
    {
        // Find the maximum in the left subtree
        // Splay it to the root; and then attach right child
        newTree = root->left;
        splay( x, newTree );
        newTree->right = root->right;
    }
    delete root;
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
const Comparable & SplayTree<Comparable>::findMin( )
{
    if( isEmpty( ) )
        return ITEM_NOT_FOUND;

    BinaryNode<Comparable> *ptr = root;

    while( ptr->left != nullptr )
        ptr = ptr->left;

    splay( ptr->element, root );
    return ptr->element;
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
const Comparable & SplayTree<Comparable>::findMax( )
{
    if( isEmpty( ) )
        return ITEM_NOT_FOUND;

    BinaryNode<Comparable> *ptr = root;

    while( ptr->right != nullptr )
        ptr = ptr->right;

    splay( ptr->element, root );
    return ptr->element;
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class Comparable>
const Comparable & SplayTree<Comparable>::find( const Comparable & x )
{
    if( isEmpty( ) )
        return ITEM_NOT_FOUND;
    splay( x, root );
    if( root->element != x )
        return ITEM_NOT_FOUND;

    return root->element;
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
  * Split the tree at elt into two subtrees such that subtree L is < elt
  * and all remaining nodes in the tree are >= elt
  */
template <class Comparable>
SplayTree<Comparable> SplayTree<Comparable>::splitTree(const Comparable &x){
    find(x);  // Indexed elements are splayed to the top of the tree
    SplayTree<Comparable>* R = new SplayTree(ITEM_NOT_FOUND, root->right);
    root->right = nullptr;
    return *R;
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

/**
 * Internal method to perform a top-down splay.
 * The last accessed node becomes the new root.
 * This method may be overridden to use a different
 * splaying algorithm, however, the splay tree code
 * depends on the accessed item going to the root.
 * x is the target item to splay around.
 * t is the root of the subtree to splay.
 */
template <class Comparable>
void SplayTree<Comparable>::splay( const Comparable & x,
                                   BinaryNode<Comparable> * & t ) const
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
}

/**
 * Rotate binary tree node with left child.
 */
template <class Comparable>
void SplayTree<Comparable>::rotateWithLeftChild( BinaryNode<Comparable> * & k2 ) const
{
    BinaryNode<Comparable> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2 = k1;
}

/**
 * Rotate binary tree node with right child.
 */
template <class Comparable>
void SplayTree<Comparable>::rotateWithRightChild( BinaryNode<Comparable> * & k1 ) const
{
    BinaryNode<Comparable> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1 = k2;
}

/*
 * Display Tree Elements
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




























