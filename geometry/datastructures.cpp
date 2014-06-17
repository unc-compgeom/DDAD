#include "common.h"
#include "datastructures.h"

namespace DDAD{



/**
 * Insert x into the tree.
 */

void SplayNode::InsertSegment( SharedSegment x )
{
    SharedNode new_node = std::make_shared<SplayNode>(x);

    if( root == nullptr ) root = new_node;
    else
    {
        new_node->left_root = root->left_root;
        new_node->right_root = root->right_root;
        new_node->top_seg = root->top_seg;
        new_node->bot_seg = root->bot_seg;

        SplaySegment( x, root );

        if( x < root->segment )
        {
            new_node->left = root->left;
            new_node->right = root;

            root->left = nullptr;
            root = new_node;
        }
        else if( root->segment < x )
        {
            new_node->right = root->right;
            new_node->left = root;

            root->right = nullptr;
            root = new_node;
        }
        else return;
    }
}

/**
 * Remove x from the tree.
 */

void SplayNode::Remove( SharedSegment x )
{
    if(root == nullptr) return; // Can't remove from an empty tree

    SharedNode new_node;


    // If x is found, it will be at the root
    SplaySegment( x, root );
    if( root->segment != x ) return;   // Item not found; do nothing
    if( root->left == nullptr ) new_node = root->right;
    else
    {
        // Find the maximum in the left subtree
        // Splay it to the root; and then attach right child
        new_node = root->left;
        SplaySegment( x, new_node );
        new_node->right = root->right;
    }
    root = new_node;
}

/**
 Splays the smallest element to the root
 */

void SplayNode::FindMin( )
{
    if( is_empty( ) )  return;
    SharedNode ptr = root;
    while( ptr->left != nullptr )
        ptr = ptr->left;
    SplaySegment( ptr->segment, root );
    return;
}

/**
 * Splay the largest element to the root
 */

void SplayNode::FindMax( )
{
    if( is_empty( ) )
        return;

    SharedNode ptr = root;

    while( ptr->right != nullptr )
        ptr = ptr->right;

    SplaySegment( ptr->segment, root );
    return;
}

/**
 * Splay the largest elt < x in the tree to the root, or the smallest elt in
 * the tree if no such element exists
 */
void SplayNode::Find( SharedSegment x )
{
    if( is_empty( ) )
        return;
    SplaySegment( x, root );
    return;
}

/**
 * Test if the tree is logically empty.
 * @return true if empty, false otherwise.
 */
bool SplayNode::is_empty( ) const
{
    return root == nullptr;
}

/**
 * Make the tree logically empty.
 */
void SplayNode::MakeEmpty( )
{
    FindMax( );        // Splay max item to root
    while( !is_empty( ) )
        Remove( root->segment );
}



/**
  * Split the tree at elt into two subtrees such that subtree L is <= elt
  * and all remaining nodes in the tree are > elt
  */
SharedNode SplayNode::SplitTree(SharedSegment x){
    Find(x);  // Indexed elements are splayed to the top of the tree
    SharedNode R = std::make_shared<SplayNode>(root->right);
    root->right = nullptr;
    return R;
}


/**
  * Merge tree R into tree L (current tree). L must be < R.
  */

void SplayNode::MergeTree(SharedNode R){
    FindMax();  // Splay max(L) to top
    root->right = R->get_root();
}


/**
 * Deep copy.
 */
const SplayNode&
SplayNode::operator=( const SplayNode& rhs )
{
    if( this != &rhs )
    {
        MakeEmpty( );
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

void SplayNode::SplayBundle(SharedSegment pivot, SharedNode t){
    SharedNode N, left_subtree, right_subtree, y;
    if(t == nullptr) return;
    N->left_root = N->right_root = nullptr;
    left_subtree = right_subtree = N;

    while(true){
        if(pivot < t->segment){
            if(t->left_root == nullptr) break;
            if(pivot < t->left_root->segment){
                y = t->left_root;
                t->left_root = y->right_root;
                y->right_root = t;
                t = y;
                if(t->left_root == nullptr) break;
            }
            right_subtree->left_root = t;
            right_subtree = t;
            t = t->left_root;
        }
        else if(pivot > t->segment){
            if(t->right_root == nullptr) break;
            if(pivot > t->right_root->segment){
                y = t->right_root;
                t->right_root = y->left_root;
                y->left_root = t;
                t = y;
                if(t->right_root == nullptr) break;
            }
            left_subtree->right_root = t;
            left_subtree = t;
            t = t->right_root;
        }
        else break;
    }

    left_subtree->right_root = t->left_root;
    right_subtree->left_root = t->right_root;
    t->left_root = N->right_root;
    t->right_root = N->left_root;
}

void SplayNode::SplaySegment( SharedSegment pivot, SharedNode t )
{
    SharedNode N, left_subtree, right_subtree, y;
    if(t == nullptr) return;
    N->left = N->right = nullptr;
    left_subtree = right_subtree = N;

    while(true){
        if(pivot < t->segment){
            if(t->left == nullptr) break;
            if(pivot < t->left->segment){
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            right_subtree->left = t;
            right_subtree = t;
            t = t->left;
        }
        else if(pivot > t->segment){
            if(t->right == nullptr) break;
            if(pivot > t->right->segment){
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            left_subtree->right = t;
            left_subtree = t;
            t = t->right;
        }
        else break;
    }

    left_subtree->right = t->left;
    right_subtree->left = t->right;
    t->left = N->right;
    t->right = N->left;
}



/**
 * Internal method to clone subtree.
 * WARNING: This is prone to running out of stack space.
 */

SharedNode SplayNode::clone(SharedNode t ) const
{
    if(t == nullptr)
        return nullptr;
    else
        return std::make_shared<SplayNode>(t->get_segment(),
                                           clone(t->get_left()),
                                           clone(t->get_right()));
}

} // namespace DDAD
