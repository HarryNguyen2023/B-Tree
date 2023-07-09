template <class T>
class BTreeNode;

// Define the class of the B Tree
template <class T>
class BTree
{
    private:
    BTreeNode<T>* root;     // Root node of the tree
    int t;                  // Minimum degree of each node
    long long size;

    public:
    BTree(int _t);
    void Traverse();
    bool searchTree(T data);
    void insertNode(T data);
    long long getSize();
    void delNode(T data);
};

// Define the class of the node of the B Tree
template <class T>
class BTreeNode
{
    private:
    T* keys;                // Array of keys in each node
    int t;                  // Minimum degree of a node = n/2, where n is the hard disk's block size
    BTreeNode<T>** Child;   // Array of node childs
    int n;                  // Number of current key in the node
    bool leaf;              // Check if the node is a leaf node

    public:
    BTreeNode(int _t, bool _leaf);
    void Traverse();
    bool searchTree(T data);
    void insertNode(T data);
    void splitChild(int num, BTreeNode* y);
    bool delNode(T data);
    void delFromLeaf(int index);
    void delFromNonLeaf(int index);
    T getPred(int index);
    T getSucc(int index);
    void fill(int index);
    void borrowFromPrev(int index);
    void borrowFromNext(int index);
    void merge(int index);
    int findIndex(T key);
    // Declare the Btree as the friend class of this node class
    friend class BTree<T>;
};

