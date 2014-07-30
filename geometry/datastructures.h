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
    /**
     * @brief Deep copy of a splay tree with root t
     * @param t
     * @return
     */
    BinaryNode<T>* Clone(BinaryNode<T> * t ) const;
};

} //namespace DDAD

//IMPLEMENTATION

namespace DDAD{

template <class T>
SplayTree<T>::SplayTree()
{
    root_ = nullptr;
}

template <class T>
SplayTree<T>::SplayTree(BinaryNode<T> *new_root)
{
    root_ = new_root;
}

template <class T>
SplayTree<T>::SplayTree( const SplayTree<T> & rhs )
{
//    root_ = nullptr;
    *this = rhs;
}

template <class T>
SplayTree<T>::~SplayTree( )
{
    MakeEmpty( );
}

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
    root_ = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root_->left != nullptr){
        if(x < root_->element && root_->left->element < x){
            t = root_->left;
            root_->left = t->right;
            t->right = root_;
            root_ = t;
        }
    }
}

template <class T>
void SplayTree<T>::Find( const T & x )
{
    if( is_empty( ) )
        return;
    Splay( x, root_ );
    return;
}

template <class T>
void SplayTree<T>::Insert( const T & x )
{
    BinaryNode<T> *newNode = new BinaryNode<T>;
    newNode->element = x;

    if( root_ == nullptr )
    {
        newNode->left = newNode->right = nullptr;
        root_ = newNode;
    }
    else
    {
        BinaryNode<T> *tmp_root = root_;
        while(tmp_root != nullptr){
            if(x < tmp_root->element)
            {
                if(tmp_root->left == nullptr){
                    tmp_root->left = newNode;
                    break;
                }
                tmp_root = tmp_root->left;
            }
            else if(x > tmp_root->element)
            {
                if(tmp_root->right == nullptr){
                    tmp_root->right = newNode;
                    break;
                }
                tmp_root = tmp_root->right;
            }
            else return; // No duplicates
        }
        Splay(x, root_);
    }
}

template <class T>
bool SplayTree<T>::Remove( const T & x )
{
    if(root_ == nullptr) return false; // Can't remove from an empty tree
    BinaryNode<T> *new_node;

    if( !ContainsValue(x) )
        return false;   // Item not found; do nothing
        // If x is found, it will be at the root
    Splay( x, root_ );

    if( root_->left == nullptr )
        new_node = root_->right;
    else
    {
        // Find the maximum in the left subtree
        SplayTree<T>* new_tree = new SplayTree<T>(root_->left);
        new_tree->Splay(x, new_tree->root_);
        new_node = new_tree->root_;
        new_node->right = root_->right;
    }
//    delete root;
    root_ = new_node;
    return true;
}

template <class T>
BinaryNode<T>* SplayTree<T>::FindMin( )
{
    if( is_empty( ) )
        return nullptr;
    BinaryNode<T> *ptr = root_;
    while( ptr->left != nullptr )
        ptr = ptr->left;

    Splay( ptr->element, root_ );
    return root_;
}

template <class T>
BinaryNode<T>* SplayTree<T>::FindMax( )
{
    if( is_empty( ) )
        return nullptr;

    BinaryNode<T>* ptr = root_;

    while( ptr->right != nullptr )
        ptr = ptr->right;

    Splay( ptr->element, root_ );
    return root_;
}

template <class T>
void SplayTree<T>::MakeEmpty( )
{
    FindMax( );        // Splay max item to root
    while( !is_empty( ) )
        Remove( root_->element );
}

template <class T>
SplayTree<T> SplayTree<T>::SplitTree(const T &x)
{
    // Splitting at a minimum/maximum value should always leave at least that
    // value in the tree
    SplayTree<T>* R;
    Splay(x, root_);
    if(x == root_->element && root_->right == nullptr)
    {
        R = new SplayTree();
        R->Insert(root_->element);
        root_ = root_->left;
    }
    else
    {
        R = new SplayTree(root_->right);
        root_->right = nullptr;
    }
    return *R;
}

template <class T>
void SplayTree<T>::MergeTree(SplayTree<T>* R){
    if(is_empty()){
        root_ = R->root_;
        return;
    }
    FindMax();  // Splay max(L) to top
    root_->right = R->root_;
}

template <class T>
bool SplayTree<T>::ContainsValue(const T &x){
    if (is_empty()) return false;
    BinaryNode<T>* tmp_root = root_;
    while(tmp_root != nullptr){
        if(x < tmp_root->element) tmp_root = tmp_root->left;
        else if(x > tmp_root->element) tmp_root = tmp_root->right;
        else return true;
    }
    return false;
}

template <class T>
int SplayTree<T>::Size()
{
    return Size(root_);
}

template <class T>
int SplayTree<T>::Size(BinaryNode<T> *root)
{
    if(root == nullptr) return 0;
    int tree_size = 1;
    if(root->right != nullptr)
        tree_size += Size(root->right);
    if(root->left != nullptr)
        tree_size += Size(root->left);

    return tree_size;
}

template <class T>
void SplayTree<T>::PrintTree(){
    PrintTree(root_, std::string(""));
}

template <class T>
void SplayTree<T>::PrintTree(BinaryNode<T>* node,
                                  std::string space){
    if(node != nullptr){
        space.append("    ");
        PrintTree(node->right, space);
        std::cout << space <<node->element << "\n";
        PrintTree(node->left, space);
    }
}


/**
 * Internal method to clone subtree.
 * WARNING: This is prone to running out of stack space.
 */
template <class T>
BinaryNode<T> *
SplayTree<T>::Clone( BinaryNode<T> * t ) const
{
//    if( t == t->left )  // Cannot test against nullptr!!!
    if(t == nullptr)
        return nullptr;
    else
        return new BinaryNode<T>( t->element, Clone( t->left ),
                                           Clone( t->right ) );
}
} // namespace DDAD


#endif // DATASTRUCTURES_H




























