# BST-AVL-Red-Black-Tree-CLI
A perfect Project to learn about algorithms, logic and operations of BST, AVL &amp; Red-Black Tree Through Command Line Interface 2D Visualization


Binary Search Tree Project Documentation 


1. Introduction 

Data structures are a fundamental part of computer science and software engineering. They provide systematic ways to organize, store, and manage data efficiently. Among all data structures, tree-based structures are especially powerful because they represent hierarchical relationships naturally. 

This project focuses on the implementation of Binary Search Trees (BST) and two of their balanced variants: AVL Trees and Red-Black Trees, using the C++ programming language. The system allows users to interactively perform operations such as insertion, deletion, searching, traversal, visualization, and persistent storage. 

The goal of this project is not only to implement these data structures but also to demonstrate their differences in balancing strategies, performance, and real-world usage through a single unified interface. 

 

2. Overview of Tree Data Structures 

A tree is a non-linear data structure consisting of nodes connected by edges. Each tree has: 

A root node 

Zero or more child nodes 

Leaf nodes that have no children 

A binary tree is a special type of tree in which each node has at most two children, commonly referred to as the left child and right child. 

Tree structures are widely used in: 

Searching algorithms 

Database indexing 

Compiler design 

File systems 

Artificial intelligence 

Binary trees form the foundation for more advanced tree structures such as Binary Search Trees, AVL Trees, and Red-Black Trees. 

 

3. Binary Search Tree (BST) Concept 

A Binary Search Tree (BST) is a binary tree that follows specific ordering rules: 

The value of every node in the left subtree is smaller than the root node. 

The value of every node in the right subtree is greater than the root node. 

Both left and right subtrees are themselves Binary Search Trees. 

These properties allow BSTs to perform search operations efficiently. On average, insertion, deletion, and search operations take O(log n) time. However, if the tree becomes skewed, performance can degrade to O(n). 

 

4. BST Implementation Details 

In this project, the BST is implemented using a BSTNode structure. Each node contains: 

An integer key 

An integer value 

A pointer to the left child 

A pointer to the right child 

A pointer to the parent node 

The BST class provides the following core functionalities: 

Insertion of nodes based on BST rules 

Deletion using standard BST deletion logic 

Searching with depth tracking 

Traversals (Inorder, Preorder, Postorder) 

Tree visualization using a 2D matrix 

File storage and loading using preorder traversal 

The parent pointer simplifies node replacement during deletion and restructuring. 

 

5. Tree Traversals 

Tree traversal refers to visiting all nodes in a specific order. This project supports three traversal techniques: 

Inorder Traversal 

Left → Root → Right 

Produces sorted output for BSTs 

Preorder Traversal 

Root → Left → Right 

Useful for saving tree structure 

Postorder Traversal 

Left → Right → Root 

Used for deleting trees and memory cleanup 

Traversal results are displayed clearly for educational understanding. 

 

6. AVL Tree Concept 

An AVL Tree is a self-balancing Binary Search Tree. It maintains balance by ensuring that the height difference between the left and right subtrees of any node is at most 1. 

This height difference is known as the balance factor: 

Balance Factor = Height(Left Subtree) – Height(Right Subtree) 
 

If the balance factor exceeds the allowed range, the tree automatically performs rotations to restore balance. 

 

7. AVL Tree Implementation 

The AVL Tree is implemented as a subclass of the BST, allowing reuse of traversal and utility functions. 

Key AVL features include: 

Height calculation 

Balance factor computation 

Left rotation 

Right rotation 

Double rotations (Left-Right and Right-Left) 

After every insertion or deletion, the tree is rebalanced to ensure logarithmic height. This guarantees O(log n) performance even in the worst case. 

 

8. Red-Black Tree Concept 

A Red-Black Tree is another type of self-balancing Binary Search Tree. Each node is assigned a color: Red or Black. 

The Red-Black Tree follows these rules: 

Every node is either red or black 

The root is always black 

Red nodes cannot have red children 

Every path from a node to its descendant null nodes has the same number of black nodes 

These rules ensure that the tree remains approximately balanced. 

 

9. Red-Black Tree Implementation 

The Red-Black Tree is implemented using RBNode and RBTree classes. 

Key operations include: 

Color-based insertion fix-up 

Rotation operations 

Recoloring during insertions and deletions 

Complex delete fix-up logic to preserve Red-Black properties 

Compared to AVL Trees, Red-Black Trees perform fewer rotations, making them more efficient in systems with frequent insertions and deletions. 

 

10. Tree Visualization and File Handling 

This project includes advanced visualization features: 

Trees are printed in a 2D structured format 

Red-Black nodes show color indicators (R) or (B) 

File handling is implemented using: 

Preorder traversal for saving tree structure 

Recursive loading to rebuild trees from files 

Each tree type is stored separately: 

bst.txt 

avl.txt 

rb.txt 

This ensures data persistence between program executions. 

 

11. TreeManager Class 

The TreeManager class acts as a controller that integrates all tree structures. 

Responsibilities include: 

Switching between BST, AVL, and Red-Black Trees 

Managing insert, delete, search, and traversal operations 

Handling screen clearing and user interaction 

Coordinating file saving and loading 

This design follows good object-oriented principles such as abstraction and separation of concerns. 

 

12. Main Function and User Interface 

The main() function provides a menu-driven interface that: 

Allows users to select the tree type 

Provides operation choices 

Handles user input validation 

Maintains program flow 

This interactive design makes the project user-friendly and suitable for demonstrations and academic evaluation. 

 

13. Conclusion 

This project successfully demonstrates the implementation of Binary Search Trees, AVL Trees, and Red-Black Trees using C++. It highlights: 

Structural differences 

Balancing strategies 

Performance trade-offs 

Practical applications 

By combining theoretical concepts with practical implementation, this project serves as a strong educational resource for understanding tree-based data structures. 

 

End of Documentation 
