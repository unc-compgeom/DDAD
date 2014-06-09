//#include "datastructures.h"
//#include <iostream>

//namespace DDAD{


////=============================================================================
//// Implementations
////=============================================================================

////=============================================================================
//// Implementation: Binary search tree
////=============================================================================

//template <typename T>
//TreeNode<T>::TreeNode(){
//    left_ = NULL;
//    right_ = NULL;
//    parent_ = NULL;
//}

//template <typename T>
//TreeNode<T>::TreeNode(T inival){
//    left_ = NULL;
//    right_ = NULL;
//    parent_ = NULL;
//    val_ = inival;
//}

//template <typename T>
//TreeNode<T>::~TreeNode(){
//    if(parent_ != NULL){
//        if(parent_->left_ == this){
//            parent_->left = NULL;
//        }
//        else if(parent_->right == this){
//            parent_->right = NULL;
//        }
//    }
//}

//template <typename T>
//Binary_ST<T>::Binary_ST(){
//    root_ = NULL;
//}

//template <typename T>
//void Binary_ST<T>::Insert(TreeNode<T> &newNode){
//    // Insert a new node
//    TreeNode<T> parentNode = Binary_ST<T>::SearchTree(newNode.val_,root_);
//    if(newNode.val_ > parentNode.val_){
//        //insert new node as right child of parent
//        parentNode.right_ = newNode;
//    }
//    else if(newNode.val_ < parentNode.val_){
//        //insert new node as left child of parent
//        parentNode.left_ = newNode;
//    }

//}

//template <typename T>
//void Binary_ST<T>::Insert(T &newVal){
//    Binary_ST::Insert(Binary_ST::SearchTree(newVal, root_));
//}

//template <typename T>
//void Binary_ST<T>::Delete(TreeNode<T>& node){

//    //case 1: node to delete has no children
//    if((node.left_ == NULL) && (node.right_ == NULL)){
//        ~node;
//    }
//    //case 2: node to delete has 1 child
//    if(node.left_ == NULL){ // Only has a right child
//        node.left_ = node.right_->right_;
//        node.val_ = node.right_->val_;
//        node.right_ = node.right_->right_;
//    }
//    else if(node.right_ == NULL){ // Only has a left child
//        node.right_ = node.left_->right_;
//        node.val_ = node.left_->val_;
//        node.left_ = node.left_->left_;
//    }
//    //case 3: node to delete has 2 children
//    TreeNode<T> *N = node.left_;
//    while(N->right_ != NULL) N = N->right_;
//    node.val_ = N->val_;
//    ~N;
//}

//template <typename T>
//void Binary_ST<T>::Rotate(T &val){
//    Binary_ST::Rotate(Binary_ST::SearchTree(newVal, root_));
//}

//template <typename T>
//void Binary_ST<T>::Rotate(TreeNode<T> &v){
//    // Rotate node v and its parent, then repair the tree
//    break;
//}

//template <typename T>
//TreeNode<T> Binary_ST<T>::SearchTree(T &val, TreeNode<T>* root){
//    // Return the node equal to val, or, if val not in tree, return the node that would be its parent
//    if(val < root->val_){
//        // Look at left child
//        if(root->left_ == NULL) return root;
//        else return Binary_ST<T>::SearchTree(val, root->left_);
//    }
//    else if(val > root->val_){
//        // Look at right child
//        if(root->right_ == NULL) return root;
//        else return Binary_ST<T>::SearchTree(val, root->right_);
//    }
//    else{
//        // Found val;
//        return root;
//    }
//}

//template <typename T>
//bool Binary_ST<T>::NodeInTree(T &val){
//    return Binary_ST<T>::SearchTree(val, root_)->val_ == val;
//}

//template <typename T>
//bool Binary_ST<T>::NodeInTree(TreeNode<T> &node){
//    return Binary_ST<T>::SearchTree(node.val_, root_) == node;
//}

//template <typename T>
//void Binary_ST<T>::PrintTree(){
//    std::queue<TreeNode<T>*> q;
//    if (root_) {
//        q.push(root_);
//        std::cout << root_->val_ << " ";
//    }
//    while (!q.empty()){
//        const TreeNode<T>* const temp_node = q.front();
//        q.pop();

//        if(temp_node->left_){
//            q.push(temp_node->left_);
//            std::cout << temp_node->left_->val_ << " ";
//        } else {
//            std::cout << "NULL";
//        }
//        if (temp_node->right_){
//            q.push(temp_node->right_);
//            std::cout << temp_node->left_->val_ << " ";
//        } else {
//            std::cout << "NULL";
//        }
//    }

//}


//} // Namespace DDAD
