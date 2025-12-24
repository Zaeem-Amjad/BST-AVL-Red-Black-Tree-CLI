#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// ---------------------
// Optional<T> helper
// ---------------------
template<typename T>
struct Optional {
    bool has;
    T val;
    Optional(): has(false) {}
    Optional(const T &v): has(true), val(v) {}
};

// ---------------------
// BST
// ---------------------
struct BSTNode {
    int key, value;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
    BSTNode(int k=0,int v=0): key(k), value(v), left(nullptr), right(nullptr), parent(nullptr) {}
};

class BST {
public:
    BSTNode* root;
    BST(): root(nullptr) {}
    virtual ~BST() { clear(root); }

    void clear(BSTNode* n) {
        if(!n) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }

    struct SearchResult {
        bool found;
        int depth;
        SearchResult(): found(false), depth(0) {}
        SearchResult(bool f, int d): found(f), depth(d) {}
    };

    SearchResult search(int k) {
        BSTNode* n = root;
        int depth = 0;
        while(n) {
            if(n->key == k) return SearchResult(true, depth);
            n = (k < n->key) ? n->left : n->right;
            depth++;
        }
        return SearchResult(false, -1);
    }

    virtual void insert(int k, int v) {
        BSTNode* node = new BSTNode(k,v);
        if(!root) { root = node; return; }
        BSTNode* cur = root;
        BSTNode* par = nullptr;
        while(cur) { par = cur; cur = (k < cur->key) ? cur->left : cur->right; }
        node->parent = par;
        if(k < par->key) par->left = node;
        else par->right = node;
    }

    virtual bool remove(int k) {
        BSTNode* z = root;
        while(z && z->key != k) z = (k < z->key) ? z->left : z->right;
        if(!z) return false;

        if(!z->left) transplant(z, z->right);
        else if(!z->right) transplant(z, z->left);
        else {
            BSTNode* y = minimum(z->right);
            if(y->parent != z) {
                transplant(y, y->right);
                y->right = z->right;
                if(y->right) y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            if(y->left) y->left->parent = y;
        }
        delete z;
        return true;
    }

    void transplant(BSTNode* u, BSTNode* v) {
        if(!u->parent) root = v;
        else if(u->parent->left == u) u->parent->left = v;
        else u->parent->right = v;
        if(v) v->parent = u->parent;
    }

    BSTNode* minimum(BSTNode* n) {
        while(n && n->left) n = n->left;
        return n;
    }

    void inorder(BSTNode* n, vector<int>& out) {
        if(!n) return;
        inorder(n->left, out);
        out.push_back(n->key);
        inorder(n->right, out);
    }
    vector<int> inorderKeys() {
        vector<int> v; inorder(root,v); return v;
    }

    void preorder(BSTNode* n, vector<int>& out) {
        if(!n) return;
        out.push_back(n->key);
        preorder(n->left, out);
        preorder(n->right, out);
    }
    vector<int> preorderKeys() {
        vector<int> v; preorder(root,v); return v;
    }

    void postorder(BSTNode* n, vector<int>& out) {
        if(!n) return;
        postorder(n->left, out);
        postorder(n->right, out);
        out.push_back(n->key);
    }
    vector<int> postorderKeys() {
        vector<int> v; postorder(root,v); return v;
    }

    int getHeight(BSTNode* n) {
        if(!n) return 0;
        return 1 + max(getHeight(n->left), getHeight(n->right));
    }

    int getWidth(BSTNode* n) {
        if(!n) return 0;
        int h = getHeight(n);
        return (1 << h) - 1;
    }

    void printSpaces(int n) {
        for(int i = 0; i < n; i++) cout << " ";
    }

    void fillMatrix(BSTNode* n, vector<vector<string>>& matrix, int row, int left, int right) {
        if(!n) return;
        int mid = (left + right) / 2;
        matrix[row][mid] = to_string(n->key);
        
        if(n->left || n->right) {
            int branchRow = row + 1;
            if(n->left) {
                int leftMid = (left + mid - 1) / 2;
                matrix[branchRow][leftMid] = "/";
                fillMatrix(n->left, matrix, row + 2, left, mid - 1);
            }
            if(n->right) {
                int rightMid = (mid + 1 + right) / 2;
                matrix[branchRow][rightMid] = "\\";
                fillMatrix(n->right, matrix, row + 2, mid + 1, right);
            }
        }
    }

    void print2D() { 
        if(!root) { cout << "Tree is empty.\n"; return; }
        int h = getHeight(root);
        int w = getWidth(root);
        int rows = h * 2 - 1;
        
        vector<vector<string>> matrix(rows, vector<string>(w, " "));
        fillMatrix(root, matrix, 0, 0, w - 1);
        
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < w; j++) {
                cout << matrix[i][j];
            }
            cout << "\n";
        }
    }

    void saveToFile(const string& filename) {
        ofstream ofs(filename);
        savePre(root, ofs);
    }
    void savePre(BSTNode* n, ofstream& ofs) {
        if(!n) { ofs<<"# "; return; }
        ofs<<n->key<<" ";
        savePre(n->left, ofs);
        savePre(n->right, ofs);
    }
    void loadFromFile(const string& filename) {
        ifstream ifs(filename);
        clear(root);
        root = loadPre(ifs, nullptr);
    }
    BSTNode* loadPre(ifstream& ifs, BSTNode* parent) {
        string tok;
        if(!(ifs >> tok)) return nullptr;
        if(tok == "#") return nullptr;
        int k = stoi(tok);
        BSTNode* n = new BSTNode(k,k);
        n->parent = parent;
        n->left = loadPre(ifs, n);
        n->right = loadPre(ifs, n);
        return n;
    }

    void clearTree() {
        clear(root);
        root = nullptr;
    }
};

// ---------------------
// AVL
// ---------------------
class AVL : public BST {
public:
    int height(BSTNode* n) {
        if(!n) return 0;
        return 1 + max(height(n->left), height(n->right));
    }

    int balanceFactor(BSTNode* n) {
        return height(n->left) - height(n->right);
    }

    BSTNode* rightRotate(BSTNode* y) {
        BSTNode* x = y->left;
        BSTNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        x->parent = y->parent;
        y->parent = x;
        if(T2) T2->parent = y;

        if(!x->parent) root = x;
        else if(x->parent->left == y) x->parent->left = x;
        else x->parent->right = x;

        return x;
    }

    BSTNode* leftRotate(BSTNode* x) {
        BSTNode* y = x->right;
        BSTNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        y->parent = x->parent;
        x->parent = y;
        if(T2) T2->parent = x;

        if(!y->parent) root = y;
        else if(y->parent->left == x) y->parent->left = y;
        else y->parent->right = y;

        return y;
    }

    BSTNode* rebalance(BSTNode* node) {
        int bf = balanceFactor(node);
        if(bf > 1) {
            if(balanceFactor(node->left) >= 0) return rightRotate(node);
            else {
                leftRotate(node->left);
                return rightRotate(node);
            }
        }
        else if(bf < -1) {
            if(balanceFactor(node->right) <= 0) return leftRotate(node);
            else {
                rightRotate(node->right);
                return leftRotate(node);
            }
        }
        return node;
    }

    BSTNode* insertRec(BSTNode* node, int k, int v, BSTNode* parent) {
        if(!node) { BSTNode* n = new BSTNode(k,v); n->parent = parent; return n; }
        if(k < node->key) node->left = insertRec(node->left, k, v, node);
        else node->right = insertRec(node->right, k, v, node);
        return rebalance(node);
    }

    void insert(int k, int v) override {
        root = insertRec(root, k, v, nullptr);
        if(root) root->parent = nullptr;
    }

    BSTNode* removeRec(BSTNode* node, int k) {
        if(!node) return nullptr;
        if(k < node->key) node->left = removeRec(node->left, k);
        else if(k > node->key) node->right = removeRec(node->right, k);
        else {
            if(!node->left || !node->right) {
                BSTNode* tmp = node->left ? node->left : node->right;
                if(!tmp) { delete node; return nullptr; }
                else { tmp->parent = node->parent; delete node; return tmp; }
            } else {
                BSTNode* succ = minimum(node->right);
                node->key = succ->key;
                node->value = succ->value;
                node->right = removeRec(node->right, succ->key);
            }
        }
        return rebalance(node);
    }

    bool remove(int k) override {
        root = removeRec(root,k);
        if(root) root->parent=nullptr;
        return true;
    }
};

// ---------------------
// Red-Black
// ---------------------
class RBNode {
public:
    int key, value;
    RBNode *left, *right, *parent;
    bool red;
    RBNode(int k=0,int v=0): key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), red(true) {}
};

class RBTree {
public:
    RBNode* root;
    RBTree(): root(nullptr) {}
    ~RBTree() { clear(root); }

    void clear(RBNode* n) {
        if(!n) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }

    struct SearchResult {
        bool found;
        int depth;
        SearchResult(): found(false), depth(0) {}
        SearchResult(bool f, int d): found(f), depth(d) {}
    };

    SearchResult search(int k) {
        RBNode* cur = root;
        int depth = 0;
        while(cur) {
            if(cur->key==k) return SearchResult(true, depth);
            cur = (k < cur->key) ? cur->left : cur->right;
            depth++;
        }
        return SearchResult(false, -1);
    }

    void leftRotate(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if(y->left) y->left->parent = x;
        y->parent = x->parent;
        if(!x->parent) root = y;
        else if(x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode* y) {
        RBNode* x = y->left;
        y->left = x->right;
        if(x->right) x->right->parent = y;
        x->parent = y->parent;
        if(!y->parent) root = x;
        else if(y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    void insert(int k, int v) {
        RBNode* z = new RBNode(k,v);
        RBNode *y = nullptr, *x = root;
        while(x) { y=x; x=(z->key<x->key)?x->left:x->right; }
        z->parent=y;
        if(!y) root=z;
        else if(z->key<y->key) y->left=z;
        else y->right=z;
        z->left=z->right=nullptr; z->red=true;
        insertFixup(z);
    }

    void insertFixup(RBNode* z) {
        while(z->parent && z->parent->red) {
            if(z->parent==z->parent->parent->left) {
                RBNode* y = z->parent->parent->right;
                if(y && y->red) { z->parent->red=false; y->red=false; z->parent->parent->red=true; z=z->parent->parent; }
                else {
                    if(z==z->parent->right) { z=z->parent; leftRotate(z); }
                    z->parent->red=false; z->parent->parent->red=true; rightRotate(z->parent->parent);
                }
            } else {
                RBNode* y=z->parent->parent->left;
                if(y && y->red) { z->parent->red=false; y->red=false; z->parent->parent->red=true; z=z->parent->parent; }
                else {
                    if(z==z->parent->left) { z=z->parent; rightRotate(z); }
                    z->parent->red=false; z->parent->parent->red=true; leftRotate(z->parent->parent);
                }
            }
        }
        if(root) root->red=false;
    }

    RBNode* minimum(RBNode* n) {
        while(n && n->left) n = n->left;
        return n;
    }

    void transplant(RBNode* u, RBNode* v) {
        if(!u->parent) root = v;
        else if(u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if(v) v->parent = u->parent;
    }

    void deleteFixup(RBNode* x, RBNode* xParent) {
        while(x != root && (!x || !x->red)) {
            if(x == xParent->left) {
                RBNode* w = xParent->right;
                if(w && w->red) {
                    w->red = false;
                    xParent->red = true;
                    leftRotate(xParent);
                    w = xParent->right;
                }
                if(w && (!w->left || !w->left->red) && (!w->right || !w->right->red)) {
                    w->red = true;
                    x = xParent;
                    xParent = x ? x->parent : nullptr;
                } else if(w) {
                    if(!w->right || !w->right->red) {
                        if(w->left) w->left->red = false;
                        w->red = true;
                        rightRotate(w);
                        w = xParent->right;
                    }
                    w->red = xParent->red;
                    xParent->red = false;
                    if(w->right) w->right->red = false;
                    leftRotate(xParent);
                    x = root;
                }
            } else {
                RBNode* w = xParent->left;
                if(w && w->red) {
                    w->red = false;
                    xParent->red = true;
                    rightRotate(xParent);
                    w = xParent->left;
                }
                if(w && (!w->right || !w->right->red) && (!w->left || !w->left->red)) {
                    w->red = true;
                    x = xParent;
                    xParent = x ? x->parent : nullptr;
                } else if(w) {
                    if(!w->left || !w->left->red) {
                        if(w->right) w->right->red = false;
                        w->red = true;
                        leftRotate(w);
                        w = xParent->left;
                    }
                    w->red = xParent->red;
                    xParent->red = false;
                    if(w->left) w->left->red = false;
                    rightRotate(xParent);
                    x = root;
                }
            }
        }
        if(x) x->red = false;
    }

    bool remove(int k) {
        RBNode* z = root;
        while(z && z->key != k) z = (k < z->key) ? z->left : z->right;
        if(!z) return false;

        RBNode* y = z;
        RBNode* x;
        RBNode* xParent;
        bool yOriginalRed = y->red;

        if(!z->left) {
            x = z->right;
            xParent = z->parent;
            transplant(z, z->right);
        } else if(!z->right) {
            x = z->left;
            xParent = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalRed = y->red;
            x = y->right;
            xParent = y;
            if(y->parent == z) {
                if(x) x->parent = y;
                xParent = y;
            } else {
                xParent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                if(y->right) y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            if(y->left) y->left->parent = y;
            y->red = z->red;
        }
        delete z;
        if(!yOriginalRed) deleteFixup(x, xParent);
        return true;
    }

    void inorder(RBNode* n, vector<int>& out) {
        if(!n) return;
        inorder(n->left,out);
        out.push_back(n->key);
        inorder(n->right,out);
    }

    vector<int> inorderKeys() { vector<int> v; inorder(root,v); return v; }

    void preorder(RBNode* n, vector<int>& out) {
        if(!n) return;
        out.push_back(n->key);
        preorder(n->left,out);
        preorder(n->right,out);
    }

    vector<int> preorderKeys() { vector<int> v; preorder(root,v); return v; }

    void postorder(RBNode* n, vector<int>& out) {
        if(!n) return;
        postorder(n->left,out);
        postorder(n->right,out);
        out.push_back(n->key);
    }

    vector<int> postorderKeys() { vector<int> v; postorder(root,v); return v; }

    int getHeight(RBNode* n) {
        if(!n) return 0;
        return 1 + max(getHeight(n->left), getHeight(n->right));
    }

    int getWidth(RBNode* n) {
        if(!n) return 0;
        int h = getHeight(n);
        return (1 << h) - 1;
    }

    void printSpaces(int n) {
        for(int i = 0; i < n; i++) cout << " ";
    }

    void fillMatrix(RBNode* n, vector<vector<string>>& matrix, int row, int left, int right) {
        if(!n) return;
        int mid = (left + right) / 2;
        matrix[row][mid] = to_string(n->key) + (n->red ? "(R)" : "(B)");
        
        if(n->left || n->right) {
            int branchRow = row + 1;
            if(n->left) {
                int leftMid = (left + mid - 1) / 2;
                matrix[branchRow][leftMid] = "/";
                fillMatrix(n->left, matrix, row + 2, left, mid - 1);
            }
            if(n->right) {
                int rightMid = (mid + 1 + right) / 2;
                matrix[branchRow][rightMid] = "\\";
                fillMatrix(n->right, matrix, row + 2, mid + 1, right);
            }
        }
    }

    void print2D() { 
        if(!root) { cout << "Tree is empty.\n"; return; }
        int h = getHeight(root);
        int w = getWidth(root);
        int rows = h * 2 - 1;
        
        vector<vector<string>> matrix(rows, vector<string>(w, " "));
        fillMatrix(root, matrix, 0, 0, w - 1);
        
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < w; j++) {
                cout << matrix[i][j];
            }
            cout << "\n";
        }
    }

    void saveToFile(const string &filename) {
        ofstream ofs(filename);
        savePre(root,ofs);
    }
    void savePre(RBNode* n, ofstream &ofs) {
        if(!n) { ofs<<"# "; return; }
        ofs<<n->key<<" ";
        savePre(n->left,ofs);
        savePre(n->right,ofs);
    }
    void loadFromFile(const string &filename) {
        ifstream ifs(filename);
        clear(root);
        root = loadPre(ifs,nullptr);
        if(root) root->red=false;
    }
    RBNode* loadPre(ifstream &ifs,RBNode* parent) {
        string tok; if(!(ifs>>tok)) return nullptr;
        if(tok=="#") return nullptr;
        int k=stoi(tok);
        RBNode* n=new RBNode(k,k);
        n->parent=parent;
        n->left=loadPre(ifs,n);
        n->right=loadPre(ifs,n);
        return n;
    }

    void clearTree() {
        clear(root);
        root = nullptr;
    }
};

// ---------------------
// TreeManager
// ---------------------

class TreeManager {
public:
    BST bst;
    AVL avl;
    RBTree rb;

    enum TreeType { BSTType, AVLType, RBType };
    TreeType currentTree = BSTType;
    
    void setTree(TreeType t) {
        currentTree = t;
    }

    void insert(int key) {
        switch (currentTree) {
        case BSTType:
            bst.insert(key, key);
            bst.saveToFile("bst.txt");
            bst.print2D();
            break;
        case AVLType:
            avl.insert(key, key);
            avl.saveToFile("avl.txt");
            avl.print2D();
            break;
        case RBType:
            rb.insert(key, key);
            rb.saveToFile("rb.txt");
            rb.print2D();
            break;
        }
        cout<<"Press any key to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
    }

    void remove(int key) {
        switch (currentTree) {
        case BSTType:
            bst.remove(key);
            bst.saveToFile("bst.txt");
            bst.print2D();
            break;
        case AVLType:
            avl.remove(key);
            avl.saveToFile("avl.txt");
            avl.print2D();
            break;
        case RBType:
            rb.remove(key);
            rb.saveToFile("rb.txt");
            rb.print2D();
            break;
        }
        cout<<"Press any key to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
    }

    void search(int key) {
        BST::SearchResult result;
        AVL::SearchResult result2;
        RBTree::SearchResult result3;
        
        switch (currentTree) {
        case BSTType:
            result = bst.search(key);
            break;
        case AVLType:
            result2 = avl.search(key);
            result.found = result2.found;
            result.depth = result2.depth;
            break;
        case RBType:
            result3 = rb.search(key);
            result.found = result3.found;
            result.depth = result3.depth;
            break;
        }
        
        if(result.found) {
            cout << "Found at depth: " << result.depth << " (height from root: " << result.depth << ")\n";
        } else {
            cout << "Not found\n";
        }
        cout<<"Press any key to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
    }

    void traverse() {
        cout<<"\n====================\n";
        cout<<"Select Traversal Type:\n";
        cout<<"1. Preorder\n";
        cout<<"2. Inorder\n";
        cout<<"3. Postorder\n";
        cout<<"4. All Three Orders\n";
        cout<<"Enter number: ";
        int choice;
        cin >> choice;

        switch (currentTree) {
        case BSTType:
            if(choice==1) printVec(bst.preorderKeys(), "Preorder");
            else if(choice==2) printVec(bst.inorderKeys(), "Inorder");
            else if(choice==3) printVec(bst.postorderKeys(), "Postorder");
            else if(choice==4) {
                printVec(bst.preorderKeys(), "Preorder");
                printVec(bst.inorderKeys(), "Inorder");
                printVec(bst.postorderKeys(), "Postorder");
            }
            break;
        case AVLType:
            if(choice==1) printVec(avl.preorderKeys(), "Preorder");
            else if(choice==2) printVec(avl.inorderKeys(), "Inorder");
            else if(choice==3) printVec(avl.postorderKeys(), "Postorder");
            else if(choice==4) {
                printVec(avl.preorderKeys(), "Preorder");
                printVec(avl.inorderKeys(), "Inorder");
                printVec(avl.postorderKeys(), "Postorder");
            }
            break;
        case RBType:
            if(choice==1) printVec(rb.preorderKeys(), "Preorder");
            else if(choice==2) printVec(rb.inorderKeys(), "Inorder");
            else if(choice==3) printVec(rb.postorderKeys(), "Postorder");
            else if(choice==4) {
                printVec(rb.preorderKeys(), "Preorder");
                printVec(rb.inorderKeys(), "Inorder");
                printVec(rb.postorderKeys(), "Postorder");
            }
            break;
        }
        cout<<"Press any key to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
    }

    void clearTree() {
        string filename;
        switch (currentTree) {
        case BSTType:
            bst.clearTree();
            bst.saveToFile("bst.txt");
            filename = "bst.txt";
            cout<<"BST cleared from memory and file.\n";
            break;
        case AVLType:
            avl.clearTree();
            avl.saveToFile("avl.txt");
            filename = "avl.txt";
            cout<<"AVL Tree cleared from memory and file.\n";
            break;
        case RBType:
            rb.clearTree();
            rb.saveToFile("rb.txt");
            filename = "rb.txt";
            cout<<"Red-Black Tree cleared from memory and file.\n";
            break;
        }
        cout<<"Press any key to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
    }

    void loadAll() {
        bst.loadFromFile("bst.txt");
        avl.loadFromFile("avl.txt");
        rb.loadFromFile("rb.txt");
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

private:
    void printVec(const vector<int>& v, const string& label) {
        cout << label << ": ";
        for (int x : v) cout << x << " ";
        cout << "\n";
    }
};


// ---------------------
// Main
// ---------------------
int main() {
    TreeManager manager;
    manager.loadAll();

    while(true) {
        cout<<"=============================\n";
        cout<<" Welcome to Reactive Trees!  \n";
        cout<<"=============================\n";
        cout<<"Select a tree type:\n";
        cout<<"1. Binary Search Tree (BST)\n";
        cout<<"2. AVL Tree\n";
        cout<<"3. Red-Black Tree\n";
        cout<<"4. Exit Program\n";
        cout<<"Enter number: ";

        int treeChoice;
        cin >> treeChoice;
        cin.ignore(); // discard newline

        if(treeChoice == 4) {
            cout<<"Exiting program.\n";
            return 0;
        }

        switch(treeChoice) {
            case 1: manager.setTree(TreeManager::BSTType); break;
            case 2: manager.setTree(TreeManager::AVLType); break;
            case 3: manager.setTree(TreeManager::RBType); break;
            default: cout<<"Invalid choice, defaulting to BST.\n"; manager.setTree(TreeManager::BSTType); break;
        }

        manager.clearScreen();

        bool backToTreeSelection = false;
        while(!backToTreeSelection) {
            cout<<"\n====================\n";
            cout<<"Select Operation:\n";
            cout<<"1. Insert key\n";
            cout<<"2. Delete key\n";
            cout<<"3. Search key\n";
            cout<<"4. Traverse\n";
            cout<<"5. Clear Tree\n";
            cout<<"6. Back to Tree Selection\n";
            cout<<"7. Exit Program\n";
            cout<<"Enter number: ";

            int op;
            cin >> op;
            
            if(op==7) {
                cout<<"Exiting program.\n";
                return 0;
            }
            
            if(op==6) {
                backToTreeSelection = true;
                manager.clearScreen();
                continue;
            }

            int key;
            switch(op) {
                case 1: cout<<"Enter key to insert: "; cin>>key; manager.insert(key); break;
                case 2: cout<<"Enter key to delete: "; cin>>key; manager.remove(key); break;
                case 3: cout<<"Enter key to search: "; cin>>key; manager.search(key); break;
                case 4: manager.traverse(); break;
                case 5: manager.clearTree(); break;
                default: cout<<"Invalid option.\n"; break;
            }
        }
    }

    return 0;
}
