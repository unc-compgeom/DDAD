#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "common.h"
#include <iostream>
#include <queue>

namespace DDAD{

//=============================================================================
// Interfaces
//=============================================================================


//=============================================================================
// Interface: Binary search tree
//=============================================================================
template <typename T>
struct TreeNode{
    TreeNode() {
        left_ = NULL;
        right_ = NULL;
        parent_ = NULL;
    }

    TreeNode(T inival){
        left_ = NULL;
        right_ = NULL;
        parent_ = NULL;
        val_ = inival;
    }

    TreeNode(TreeNode<T> &theNode){
        TreeNode<T> *copy = new TreeNode();
        copy->parent_ = theNode.parent_;
        copy->left_ = theNode.left_;
        copy->right_ = theNode.right_;
        copy->val_ = theNode.val_;
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

    TreeNode* left_;
    TreeNode* right_;
    TreeNode* parent_;
    T val_;
};

template <typename T>
class Binary_ST{

public:
    Binary_ST(){
        root_ = NULL;
    }

    void Insert(TreeNode<T> *newNode){
        // Insert a new node
        if(root_ == NULL) root_ = newNode; // Check if tree is empty
        TreeNode<T>* parentNode = Binary_ST<T>::SearchTree(newNode->val_ ,root_);
        std::cout << "\nInserting " << newNode->val_ << ", found parent node " << parentNode->val_;

        if(newNode->val_ > parentNode->val_){
            //insert new node as right child of parent
            newNode->parent_ = parentNode;
            parentNode->right_ = newNode;

        }
        else if(newNode->val_ < parentNode->val_){
            //insert new node as left child of parent
            newNode->parent_ = parentNode;
            parentNode->left_ = newNode;
        }
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

    void Rotate(TreeNode<T> *pivotNode){
        // Rotate node v and its parent, then repair the tree
        TreeNode<T>* parent = pivotNode->parent_;
        if(parent == NULL) { // Identity transformation
            std::cout << "\nCan't rotate the root node!";
            return;
        }
        else if(parent->left_ == pivotNode) { // Right rotation
            parent->left_ = pivotNode->right_;
            pivotNode->right_->parent_= parent;
            pivotNode->right_ = parent;
        }
        else if(parent->right_ == pivotNode) { // Left rotation
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
        // Return a pointer to the node equal to val, or, if val not in tree, return the node that would be its parent
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

private:
    TreeNode<T>* root_;
};


} //namespace DDAD

#endif // DATASTRUCTURES_H
