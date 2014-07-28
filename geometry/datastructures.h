#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "common.h"
#include <iostream>
#include <queue>
#include "point.h"
#include "line.h"
//#include "arrangement.h"


namespace DDAD{

template <class T>
class SplayTree;

template <class T>
/**
 * @brief Node in a generic binary tree.  The comparable element of type T in
 *  the left node is < and the element in the right node is >.
 */
struct BinaryNode
{
    T element;
    BinaryNode<T> *left;
    BinaryNode<T> *right;

    BinaryNode<T>( ) : left( nullptr ), right( nullptr ) { }
    BinaryNode<T>( const T & rhs_element, BinaryNode<T>* rhs_left,
                   BinaryNode<T>* rhs_right ) :
        element( rhs_element ), left( rhs_left ), right( rhs_right ) { }
};


template <class T>
/**
 * @brief Splay tree class for a generic comparable type T.
 */
class SplayTree
{
  public:
    /**
     * @brief The root of the current tree
     */
    BinaryNode<T>* root_;

    /**
     * Construct the tree.
     */
    explicit SplayTree<T>();

    /**
     * Copy constructors.
     */
    SplayTree<T>( BinaryNode<T>* new_root );
    SplayTree<T>( const SplayTree& rhs );
    const SplayTree & operator=( const SplayTree & rhs );

    /**
     * Destructor.
     */
    ~SplayTree<T>( );

    /**
     * @brief Test if the tree is logically empty
     * @return true if empty, false otherwise.
     */
    bool is_empty( ) const { return root_ == nullptr; }

    /**
     * @brief Internal method to perform a top-down Splay.
     *  The last accessed node becomes the new root. If x is not
     *  contained in the tree, then the root of the resulting tree is the
     *  greatest element < x or, if no such element exists in the tree, the
     *  least element > x.
     * @param x : The target item to Splay around
     * @param t : the root of the subtree to Splay
     */
    void Splay(const T & x, BinaryNode<T> *t );

    /**
     * @brief Find item x in the tree and splay it to the root.
     * @param x : Item to be found
     */
    void Find( const T & x );

    /**
     * @brief Insert an element x into the tree.  It will be splayed to the root.
     * @param x : Element to insert into the tree
     */
    void Insert( const T & x );

    /**
     * @brief Remove an element from the tree if it exists
     * @param x : element to be removed
     * @return : true if element found and removed, false if element not found
     */
    bool Remove( const T & x );

    /**
     * @brief Find the minimum element in the tree in log(n) time and splay it
     *  the root of the tree.  Not the most efficient implementation (uses two
     *  passes), but has the correct amortized behavior.  A good alternative is
     *  to first call Find with parameter smaller than any item in the tree,
     *  then call FindMin.
     * @return : The minimum node in the tree, or nullptr if the tree is empty
     */
    BinaryNode<T>* FindMin( );

    /**
     * @brief Find the maximum element in the tree in log(n) time and splay it
     *  the root of the tree.  Not the most efficient implementation (uses two
     *  passes), but has the correct amortized behavior.  A good alternative is
     *  to first call Find with parameter greater than any item in the tree,
     *  then call FindMax.
     * @return : The maximum node in the tree, or nullptr if the tree is empty
     */
    BinaryNode<T>* FindMax( );

    /**
     * @brief Make the tree logically empty.
     */
    void MakeEmpty( );

    /**
     * @brief Split the tree at x into two subtrees self and R such that r >= x
     *  for all nodes r in R and l < x for all nodes l in self.
     * @param x : the comparable element to split about.
     * @return : subtree R such that r >= x for all nodes r in R
     */
    SplayTree<T> SplitTree( const T& x);

    /**
     * @brief Merge the current tree with another tree R in constant time
     * @param R : for all nodes l in self and r in R, require r > l.
     */
    void MergeTree(SplayTree<T>* R);

    /**
     * @brief Check whether the value x is contained in any of the nodes
     *  in the tree
     * @param x : Value to check for
     * @return : true if x in tree, false otherwise
     */
    bool ContainsValue( const T & x );

    /**
     * @brief Get the number of nodes in the tree (takes n time) using a
     *  recursive method.
     * @return : int equal to the number of nodes in the tree
     */
    int Size();
    int Size(BinaryNode<T>* root);

    /**
     * @brief Assuming node elements are printable (in c-style strings),
     *  displays a visualization of the tree structure using a recursive
     *  method
     */
    void PrintTree();
    void PrintTree(BinaryNode<T>* node, std::string space);
};
} //namespace DDAD

#endif // DATASTRUCTURES_H




























