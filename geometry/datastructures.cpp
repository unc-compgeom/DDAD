#include "datastructures.h"

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
    BinaryNode<T> *ptr = root;
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
void SplayTree<T>::print(BinaryNode<T>* node,
                                  std::string space){
    if(node != nullptr){
        space.append("    ");
        PrintTree(node->right, space);
        std::cout << space <<node->element << "\n";
        PrintTree(node->left, space);
    }
}
} // namespace DDAD
