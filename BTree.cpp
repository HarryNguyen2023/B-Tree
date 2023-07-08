#include "BTree.h"
#include <iostream>

// Constructor to initiate the node of the B Tree
template <typename T>
BTreeNode<T>::BTreeNode(int _t, bool _leaf)
{
    t = _t;
    n = 0;
    leaf = _leaf;
    // Initiate the dynamic arrays
    keys = new T[2*t - 1];
    Child = new BTreeNode*[2*t];
}

// Function to initiate the B Tree
template <typename T>
BTree<T>::BTree(int _t)
{
    t = _t;
    root = NULL;
}

// Function to search for the node of the tree
template <typename T>
bool BTreeNode<T>::searchTree(T data)
{
    int i = 0;
    // Loop through the keys of the node
    while(i < n && data < keys[i])
        i++;
    if(data == keys[i])
        return true;
    // Case at leaf
    if(leaf == true)
        return false;
    return Child[i]->searchTree(data);
}

// Function to generally search for the node of the tree
template <typename T>
bool BTree<T>::searchTree(T data)
{
    // Check if the tree is empty
    if(root == NULL)
        return false;
    // Get to the function in the base class
    return root->searchTree();
}

// Function to traverse the tree and display the value of the nodes
template <typename T>
void BTreeNode<T>::Traverse()
{
    int i;
    // Loop through the child of the current node
    for(i = 0; i < n; ++i)
    {
        if(leaf == false)
            Child[i]->Traverse();
        std::cout<<keys[i]<<" ";
    }
    // Print the subtree rooted with the last child of the current node
    if(leaf == false)
        Child[i]->Traverse();
}

// Function to generally traverse the B Tree
template <typename T>
void BTree<T>::Traverse()
{
    // Check if the tree is empty
    if(root == NULL)
        return;
    root->Traverse();
}

// Function to create a new node for a B Tree
template <typename T>
BTreeNode<T>* BTreeNode<T>::newNode(T data, int _t, bool _leaf)
{
    BTreeNode<T>* node = new BTreeNode(_t, _leaf);
    node->n = 1;
    node->keys[0] = data;
    return node;
}