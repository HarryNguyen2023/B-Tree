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
    Child = new BTreeNode<T>*[2*t];
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
    while(i < n && data > keys[i])
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
    return root->searchTree(data);
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
    std::cout<<"The elements of the tree: ";
    root->Traverse();
    std::cout<<std::endl;
}

// Function to proactively split a full node in the insertion of the B Tree
template <typename T>
void BTreeNode<T>::splitChild(int index, BTreeNode<T>* y)
{
    // Create a new node for splitting node y into 2 part
    BTreeNode<T>* z = new BTreeNode<T>(y->t, y->leaf);
    // Copy a larger half keys of node y into node z
    z->n = t - 1;
    for(int i = 0; i < z->n; ++i)
        z->keys[i] = y->keys[i + t];

    // If a node leaf, then copy a half child nodes from y to z
    if(y->leaf == false)
    {
        for(int i = 0; i < t; ++i)
            z->Child[i] = y->Child[i + t];
    }
    // Change values of parameters of y
    y->n = t - 1;

    // As the current node is going to have a new child, we are going to spare a space for this child
    for(int i = n; i >= index + 1; i--)
        Child[i + 1] = Child[i];
    Child[index + 1] = z;
    
    // Add the median key of node y to the current node
    for(int i = n - 1; i >= index; --i)
        keys[i + 1] = keys[i];
    keys[index] = y->keys[t - 1];

    // Update the number of keys at the current node
    n++;
}

// Function to insert the new key into the current node
template <typename T>
void BTreeNode<T>::insertNode(T data)
{
    int i = n - 1;
    // Case inset at leaf node only care about keys
    if(leaf == true)
    {
        // Move the larger keys to the right and insert the new key
        while(i >= 0 && data < keys[i])
        {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = data;
        n++;
    }
    else
    {
        // Find the child that is appropriate to insert the new key
        while(i >= 0 && data < keys[i])
            i--;
        // Check if that child is full or node
        if(Child[i + 1]->n == 2*t - 1)
        {
            // Split the child before move the current node to that node
            splitChild(i + 1, Child[i + 1]);
            // Check which node of the 2 new splitting node we have the new key
            if(keys[i + 1] < data)
                i++;
        }
        // Move to the child node and continue for insertion
        Child[i + 1]->insertNode(data);
    } 
}

// Function to generally insert a new node into the B Tree
template <typename T>
void BTree<T>::insertNode(T data)
{
    // Case the tree is empty
    if(root == NULL)
    {
        root = new BTreeNode<T>(t, true);
        root->keys[0] = data;
        root->n = 1;
    }
    else
    {
        // Case root is full, split and enhance the height of the root
        if(root->n == 2*t-1)
        {
            // Create new root node and move the median of the old root up
            BTreeNode<T>* newroot = new BTreeNode<T>(t, false);
            // Add old root as child of the new root
            newroot->Child[0] = root;
            // Split and old root and move the median key up to the new root
            newroot->splitChild(0, root);
            // Decide which child will have the new key
            int i = 0;
            if(newroot->keys[0] < data)
                i++;
            newroot->Child[i]->insertNode(data);
            // Update the new root
            root = newroot;
        }
        else
        {
            root->insertNode(data);
        }
    }
}

// // Function to get the size of the tree
// template <typename T>
// int BTreeNode<T>::getSize()
// {
//     int i;
//     for(i = 0; i < n; ++i)
//     {
//         if(leaf != false)
//             return n + Child[i]->getSize();
//     }
//     if(leaf != false)
//         return n + Child[i]->getSize();
// }

// // Function to generally get the size of the tree
// template <typename T>
// int BTree<T>::getSize()
// {
//     if(root == NULL)
//         return 0;
//     return root->getSize();
// }

int main()
{
    // Integer version
    BTree<int> btree(3);
    // Insert a few node into the tree
    int random[] = {15, 13, 2, 5, 7, 0, 6, 9, 25, 78, 65, 45, 33, 110, 330};
    for(int i = 0; i < sizeof(random)/sizeof(int); ++i)
        btree.insertNode(random[i]);
    // Display the elements of the tree
    btree.Traverse();
    // Search for a few node in the tree
    std::cout<<"Node 7 is "<<(btree.searchTree(7) ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node 13 is "<<(btree.searchTree(13) ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node 102 is "<<(btree.searchTree(102) ? "" : "not ")<<"in the tree"<<std::endl;
    // Get the size of the tree
    // std::cout<<"The size of the tree is: "<<btree.getSize()<<std::endl;

    return 0;
}