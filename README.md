# B-Tree
## Author Harry Nguyen (Created 07/13/2023)

B tree different from the BST contains multiple key data in the same node. This is because secondary storage devices such as hard disk is extremely time-expensive in aceesing to read and write operations. Furthermore, every disk space allow much more storage capacity than a speicfic data type in most of the programming languages, containing multiple keys at a nodes will be better.

Each B tree has s special variable t called the degree of the B tree, which is mostly chosen by the disk space property, and B tree nodes must follow the following conditions:
- All the node except root node must have at least t - 1 keys and at most 2t -1 keys.
- All the node except root node must have at least t child and at most 2t child.
- Keys in the node are arranged in increasing order.

### Due to the above conditions, the B tree is quite complex in insetion and deletion algorithms
- Insertion in B tree requires a techniques called proactive approach, that going to split the node, which have the maximum allowable number of keys, on the way traversing to the leaf node for insertion of new key.
- Deletion in B tree is even more complex as it may violate the minimum number of allowable keys in each node. Therefore, on the wat traversing to the node having the key to be deleted, we must apply borrow or merge algorithm for nodes that have only the minimum number os keys.

### References
[1]. B Tree, https://www.programiz.com/dsa/b-tree.

[2]. Introduction of B Tree, https://www.geeksforgeeks.org/introduction-of-b-tree-2/.
