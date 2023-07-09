#include "BTree.h"
#include <iostream>
#include <vector>

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
    size = 0;
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
    // Update the size of the tree
    size++;
}

// Function to generally get the size of the tree
template <typename T>
long long BTree<T>::getSize()
{
    if(root == NULL)
        return 0;
    return size;
}

// Function to get the inorder predecessor - the largest key in the left child of the node
template <typename T>
T BTreeNode<T>::getPred(int index)
{
    // Create a temporary node to traverse the left branches of the node
    BTreeNode<T>* temp = Child[index];
    // Traverse the right-most branches of the left child
    while(temp->leaf == false)
        temp = temp->Child[temp->n];
    return temp->keys[(temp->n) - 1];
}

// Function to get the inorder successor - the smallest key in the right child of the node
template <typename T>
T BTreeNode<T>::getSucc(int index)
{
    // Create a temporary node
    BTreeNode<T>* temp = Child[index + 1];
    // Traverse the left-most branches of the right child
    while (temp->leaf == false)
        temp = temp->Child[0];
    return temp->keys[0];  
}

// Function to borrow a key from the C[index - 1] to the current node and insert keys[index - 1] to Child[i]
template <typename T>
void BTreeNode<T>::borrowFromPrev(int index)
{
    // Initiate the pointers for the left and the right child of the current node
    BTreeNode<T>* child = Child[index];
    BTreeNode<T>* sibling = Child[index - 1];

    // Move all the keys of the child one step a head to spare space for the new key
    for(int i = (child->n) - 1; i >=0; --i)
        child->keys[i + 1] = child->keys[i];
    // Move all the childs of the child node one step ahead if it not a leaf node
    if(child->leaf == false)
    {
        for(int i = child->n; i >=0; --i)
            child->Child[i + 1] = child->Child[i];
    }

    // Move the key of the current node to be the first key of the child node
    child->keys[0] = keys[index - 1];
    // Move the right-most key of the sibling child to the current node
    keys[index - 1] = sibling->keys[(sibling->n) - 1];
    // Move the right-most child of siblling to be the first child of the child node
    if(sibling->leaf == false)
        child->Child[0] = sibling->Child[sibling->n];
    
    // Update the number of keys in each node
    child->n += 1;
    sibling->n -= 1;
    return;
}

// Function to borrow a key from the right child C[index +1] to the current node, and insert a key from current node to the left child C[index]
template <typename T>
void BTreeNode<T>::borrowFromNext(int index)
{
    // Initiate the pointers for 2 childs node
    BTreeNode<T>* child = Child[index];
    BTreeNode<T>* sibling = Child[index + 1];

    // Move the keys[index] to be the right most key of the Child[index]
    child->keys[child->n] = keys[index];
    // Move the first keys of Child[index + 1] to be the keys[index]
    keys[index] = sibling->keys[0];
    // Move the first Child of the sibling to be the right-most child of the child node
    if(child->leaf == false)
        child->Child[(child->n) + 1] = sibling->Child[0];

    // Move all the keys of the sibbling one step back
    for(int i = 1; i <= (sibling->n) - 1; ++i)
        sibling->keys[i - 1] = sibling->keys[i];
    // Move all the child of the sibling one step back if not a leaf node
    if(sibling->leaf == false)
    {
        for(int i = 1; i <= (sibling->n); ++i)
            sibling->Child[i - 1] = sibling->Child[i];
    }
    // Update the number of keys in each node
    child->n += 1;
    sibling->n -= 1;
    return;
}

// Function to merge a node from a parent node into the sibling of the deleted node
template <typename T>
void BTreeNode<T>::merge(int index)
{
    // Initiate 2 pointers for deleted node and its sibling node
    BTreeNode<T>* sibling = Child[index];
    BTreeNode<T>* delnode = Child[index + 1];
    // Move a key from the current node and insert it into the sibling
    sibling->keys[t - 1] = keys[index];

    // Move all the keys of the delnode to the right of the sibling node
    for(int i = 0; i < delnode->n; ++i)
        sibling->keys[i + t] = delnode->keys[i];
    // Move all the childs of the delnode to the right of the sibling node
    if(delnode->leaf == false)
    {
        for(int i = 0; i <= delnode->n; ++i)
            sibling->Child[i + t] = delnode->Child[i];
    }

    // Move all the right keys of keys[index] one step back
    for(int i = index + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    // Move all the child pointer at the right of keys[index] one step back
    for(int i = index + 2; i <= n; ++i)
        Child[i - 1] = Child[i];
    
    // Update the number of keys in each node
    n--;
    sibling->n += (delnode->n) + 1;

    // Delete the delnode
    delete delnode;
    return;  
}

// Function to fill Child[index] which has less than t - 1 keys after the deletion
template <typename T>
void BTreeNode<T>::fill(int index)
{
    // Check if the left child sibling has more than the minimum key and borrow from it
    if(index != 0 && Child[index - 1]->n > t - 1)
        borrowFromPrev(index);
    // Check if the right child has more than the minimum number of key and borrow from it
    else if(index != n && Child[index + 1]->n > t - 1)
        borrowFromNext(index);
    // If both sibling child have only the minimum number of key, then merge from the parent node
    else
    {
        if(index != n)
            merge(index);
        else
            merge(index - 1);
    }
}

// Function to remove a key from a leaf node
template <typename T>
void BTreeNode<T>::delFromLeaf(int index)
{
    // Move all the keys at the right a the deleted key one step back
    for(int i = index + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    // Update the number of keys in the node
    n--;
    return;
}

// Function to delete a key from an internal node
template <typename T>
void BTreeNode<T>::delFromNonLeaf(int index)
{
    // Get the key to be deleted
    T k = keys[index];
    // Replace the key to be deleted by the inorder predecessor and delete the old predecessor
    if(Child[index]->n >= t)
    {
        T pred = getPred(index);
        keys[index] = pred;
        Child[index]->delNode(pred);
    }
    // else replace the key to be deleted by the inorder succesor and delete the old successor node
    else if(Child[index + 1]->n >= t)
    {
        T succ = getSucc(index);
        keys[index] = succ;
        Child[index + 1]->delNode(succ);
    }
    // if both child node has the minimum number of key, then merge them and remove k
    else
    {
        merge(index);
        Child[index]->delNode(k);
    }
    return;
}

// Function to find the index of the key to be deleted in the current node
template <typename T>
int BTreeNode<T>::findIndex(T key)
{
    int index = 0;
    while(index < n && key > keys[index])
        index++;
    return index;
}

// Function to remove a key from the B Tree
template <typename T>
bool BTreeNode<T>::delNode(T data)
{
    // Get the index of the deleted key
    int index = findIndex(data);
    // Remove the key if it is presented in the current node
    if(index < n && keys[index] == data)
    {
        // If the current node is at leaf
        if(leaf)
            delFromLeaf(index);
        else
            delFromNonLeaf(index);
    }
    else
    {
        // If the current node is at leaf the key is not in the tree
        if(leaf)
        {
            std::cout<<"The key "<<data<<" is not presented in the tree"<<std::endl;
            return false;
        }
        // Check if the key is presented in the last child of this node
        bool last = ((index == n) ? true : false);
        // If the child contain the key has less then t keys, fill it
        if(Child[index]->n < t)
            fill(index);
        // The last child is merge with its index-1 sibling
        if(last && index > n)
            return Child[index - 1]->delNode(data);
        else
            return Child[index]->delNode(data);
    }
    return true;
}

// Function to generally delete a key from the B Tree
template <typename T>
void BTree<T>::delNode(T data)
{
    // Check if the tree is empty
    if(root == NULL)
    {
        std::cout<<"The tree is empty!"<<std::endl;
        return;
    }
    // Call the delete function
    if(root->delNode(data))
        size--;
    // Case the root is shrink the height
    if(root->n == 0)
    {
        // Replace the root with its child node if it not already a leaf node
        BTreeNode<T>* temp = root;
        if(root->leaf)
            root = NULL;
        else
            root = root->Child[0];
        delete temp;
    }
    return;
}

int main()
{
    // Integer version
    // BTree<int> btree(3);
    // // Insert a few node into the tree
    // int random[] = {15, 13, 2, 5, 7, 0, 6, 9, 25, 78, 65, 45, 33, 110, 330};
    // for(int i = 0; i < sizeof(random)/sizeof(int); ++i)
    //     btree.insertNode(random[i]);
    // // Display the elements of the tree
    // btree.Traverse();
    // // Search for a few node in the tree
    // std::cout<<"Node 7 is "<<(btree.searchTree(7) ? "" : "not ")<<"in the tree"<<std::endl;
    // std::cout<<"Node 13 is "<<(btree.searchTree(13) ? "" : "not ")<<"in the tree"<<std::endl;
    // std::cout<<"Node 102 is "<<(btree.searchTree(102) ? "" : "not ")<<"in the tree"<<std::endl;
    // // Get the size of the tree
    // std::cout<<"The size of the tree is: "<<btree.getSize()<<std::endl;
    // // Delete a few node in the tree
    // btree.delNode(45);
    // btree.delNode(78);
    // btree.delNode(33);
    // btree.delNode(250);
    // // Seach for the deleted nodes
    // std::cout<<"Node 45 is "<<(btree.searchTree(45) ? "" : "not ")<<"in the tree"<<std::endl;
    // std::cout<<"Node 78 is "<<(btree.searchTree(78) ? "" : "not ")<<"in the tree"<<std::endl;
    // std::cout<<"Node 33 is "<<(btree.searchTree(33) ? "" : "not ")<<"in the tree"<<std::endl;
    // // Get the new size of the tree
    // std::cout<<"The size of the tree is: "<<btree.getSize()<<std::endl;
    // // Travese the tree
    // btree.Traverse();

    // String version 
    BTree<std::string> btree(3);
    std::vector<std::string> random = {"Gia", "Tran", "Phu", "Ngoc", "Khoa", "Khoi", "Chinh", "Duy", "Khang", "Huy"};
    // Insert a few node
    for(int i = 0; i < random.size(); ++i)
        btree.insertNode(random[i]);
    // Traverse the tree
    btree.Traverse();
    // Searach for a few node
    std::cout<<"Node Gia is "<<(btree.searchTree("Gia") ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node Tran is "<<(btree.searchTree("Tran") ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node Thinh is "<<(btree.searchTree("Thinh") ? "" : "not ")<<"in the tree"<<std::endl;
    // Delete a few node 
    btree.delNode("Ngoc");
    btree.delNode("Phu");
    // Seach for deleted node
    std::cout<<"Node Ngoc is "<<(btree.searchTree("Ngoc") ? "" : "not ")<<"in the tree"<<std::endl;
    std::cout<<"Node Phu is "<<(btree.searchTree("Phu") ? "" : "not ")<<"in the tree"<<std::endl;
    // Traverse the tree
    btree.Traverse();
    // Get the size of the tree
    std::cout<<"The size of the tree is: "<<btree.getSize()<<std::endl;

    return 0;
}