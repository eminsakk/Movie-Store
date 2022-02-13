#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <functional>
#include <cstddef>
#include <ostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <utility>

#include <iostream>


class DefaultBalanceCondition
{
public:
    bool operator() (int current_height, int ideal_height) const
    {
        return true;
    }
};


template <typename Key, typename Object,
    typename BalanceCondition = DefaultBalanceCondition,
    typename Comparator = std::less<Key> >
    class BinarySearchTree
{
public:
    struct Node   // core structure of BinarySearchTree
    {
        Key key;         // unique key
        Object data;     // data to be stored, characterized by key
        Node* left;     // pointer to left subtree
        Node* right;    // pointer to right subtree
        size_t height;   // height of the node
        size_t subsize;  // size of the subtree node roots including itself

        Node(const Key&, const Object&,
            Node*, Node*, size_t = 0, size_t = 1);   // Node constructor
    };

public:
    BinarySearchTree();  // zero-parameter constructor
    BinarySearchTree(const std::list<std::pair<Key, Object> >&); // list is always sorted!
    ~BinarySearchTree();  // destructor

public:
    void insert(const Key&, const Object&);  // insert new Key-Object
    void remove(const Key&);  // remove Node characterized by Key
    void toCompleteBST();  // convert into a complete BST

public: 
    Node* find(const Key&) const; // single item
    std::list<Node*> find(const Key&, const Key&) const; // range queries
    int height() const;  // return the height of the tree
    size_t size() const; // return the number of items in the tree
    bool empty() const;  //return whether the tree is empty or not
    Node* getRoot() const; // return a pointer to the root of the tree
    void print(std::ostream&) const;  // print tree structure into an output stream

private:
    Node* root;                     // designated root
    size_t numNodes;                 // size
    Comparator isLessThan;           // operator upon which BST nodes are arranged
    BalanceCondition isBalanced;     // signals whether the BST is balanced or not

private: 
    Node* find(const Key&, Node*) const;
    int height(Node*) const;
    int subsize(Node*) const;
    void print(Node*, std::ostream&) const;

    void helperForCompleteBst(typename  std::vector<std::pair<Key, Object>>::iterator headI, typename  std::vector<std::pair<Key, Object>>::iterator endI, Node*& copyRoot, const int& size);
    void inorderTraversal(std::vector<Node*>& address, Node* copyRoot);
    void helperForToComplete(std::vector<Node*> myAdress, int left, int right, Node*& myRoot);
    void helperForInsertion(const Key myKey, const Object myObj, Node*& myRoot);
    Node* helperForRemoval(Node* myRoot, const Key myKey);
    Node* findMinimum(Node* myRoot);
    Node* fuu(Node* myRoot);
    void findHelper(std::list<Node*>& adressList, Key firstKey, Key secondKey,Node * myRoot) const;
    


private:
    void makeEmpty(Node*&);       // utility for destructor



private: /* compiler defaults are blocked */
    BinarySearchTree(const BinarySearchTree&);
    const BinarySearchTree& operator=(const BinarySearchTree&);

private:  // static utility functions
    template <typename T> //static utility function
    static const T& max(const T&, const T&);


};

#endif



template <typename K, typename O, typename B, typename C>
BinarySearchTree<K, O, B, C>::Node::
Node(const K& _k, const O& _d, Node* _l, Node* _r, size_t _h, size_t _s)
    : key(_k), data(_d), left(_l), right(_r), height(_h), subsize(_s)
{
}



template <typename K, typename O, typename B, typename C>
BinarySearchTree<K, O, B, C>::BinarySearchTree()
    : root(NULL), numNodes(0)  //comparator(C() ??)
{
}
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::helperForCompleteBst(typename std::vector<std::pair<K, O>>::iterator headI, typename std::vector<std::pair<K, O>>::iterator endI, Node*& copyRoot, const int& size) {

    if (!size) 
        return;
    

    int minHeight = log2(size);
    int lastLevelNumbers = pow(2, minHeight + 1) - pow(2, minHeight);
    int remainder = size - pow(2, minHeight) + 1;
    int midKey;
    if (remainder > (lastLevelNumbers / 2)) {
        midKey = (pow(2, ceil(log2(size + 1))) - 2) / 2;
    }
    else {
        midKey = size - 1 - (pow(2, floor(log2(size + 1))) - 2) / 2;
    }
    auto iter = headI + midKey;
    copyRoot = new Node((*iter).first, (*iter).second, NULL, NULL, minHeight, size);

    helperForCompleteBst(headI, iter, copyRoot->left, iter - headI);
    helperForCompleteBst(iter + 1, endI, copyRoot->right, (endI - iter - 1));

}


template <typename K, typename O, typename B, typename C>
BinarySearchTree<K, O, B, C>::BinarySearchTree(const std::list<std::pair<K, O> >& datalist)
    : root(NULL), numNodes(0) 
{

    auto iter = datalist.begin();
    std::vector<std::pair<K, O>> myVect;
    for (; iter != datalist.end(); iter++) {
        myVect.push_back(*iter);

    }
    auto vectEnd = myVect.end();
    auto vectHead = myVect.begin();

    helperForCompleteBst(vectHead, vectEnd, root, myVect.size());
}

// HELPER FUNCTIONS FOR toCompleteBST()
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::inorderTraversal(std::vector<Node*>& address, Node* copyRoot) {
    if (copyRoot) {
        inorderTraversal(address, copyRoot->left);
        address.push_back(copyRoot);
        inorderTraversal(address, copyRoot->right);
        copyRoot->left = NULL;
        copyRoot->right = NULL;
        copyRoot->height = 0;
        copyRoot->subsize = 1;
    }
}
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::helperForToComplete(std::vector<Node*> myAdress, int left, int right, Node*& myRoot) {

    int size = right - left + 1;

    if (size <= 0) {
        myRoot = NULL;
        return;
    }
    if (size == 1) {
        myRoot = myAdress[left];
        myRoot->left = NULL;
        myRoot->right = NULL;
        myRoot->height = 0;
        myRoot->subsize = 1;
        return;
    }
    int minHeight = floor(log2(size));
    int lastLevelNumbers = pow(2, minHeight + 1) - pow(2, minHeight);
    int artik = size - pow(2, minHeight) + 1;
    int rootIndex = 0;
    if (artik > (lastLevelNumbers / 2)) {
        rootIndex = (pow(2, ceil(log2(size + 1))) - 2) / 2;
    }
    else {
        rootIndex = size - 1 - (pow(2, floor(log2(size + 1))) - 2) / 2;
    }




    myRoot = myAdress[left + rootIndex];
    myRoot->height = minHeight;
    myRoot->subsize = size;

    helperForToComplete(myAdress, left, left + rootIndex - 1, myRoot->left);
    helperForToComplete(myAdress, left + rootIndex + 1, right, myRoot->right);


}

template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::toCompleteBST()
{
    std::vector<Node*> nodeAdressess;
    inorderTraversal(nodeAdressess, root);

    helperForToComplete(nodeAdressess, 0, nodeAdressess.size() - 1, root);
}
// HELPER FOR INSERTION
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::helperForInsertion(const K myKey, const O myObj, Node*& myRoot) {
    if (!myRoot) {
        myRoot = new Node(myKey, myObj, NULL, NULL);
        return;
    }
    else if (isLessThan(myKey, myRoot->key)) {
        (myRoot->subsize)++;
        helperForInsertion(myKey, myObj, myRoot->left);
    }
    else if (isLessThan(myRoot->key, myKey)) {
        (myRoot->subsize)++;
        helperForInsertion(myKey, myObj, myRoot->right);
    }
    
    
    if (myRoot->left && myRoot->right) {
        myRoot->height = max(myRoot->left->height, myRoot->right->height) + 1;
    }
    else if (myRoot->left) {
        myRoot->height = myRoot->left->height + 1;
    }
    else if (myRoot->right) {
        myRoot->height = myRoot->right->height + 1;
    }
    else{
        myRoot->height = 0;
    }
    if ((myRoot->subsize) > 1) {
        int _val = log2(myRoot->subsize);
        if (!isBalanced(myRoot->height, _val)) {
            std::vector<Node*> myVect;
            inorderTraversal(myVect, myRoot);


            helperForToComplete(myVect, 0, myVect.size() - 1, myRoot);

        }
    }

}


template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::insert(const K& k, const O& x)
{
    Node* inOrNot = find(k);
    if (inOrNot) {
        inOrNot->data = x;
    }
    else {
        numNodes++;
        helperForInsertion(k, x, root);
    }
}

// HELPERS FOR REMOVE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K, O, B, C>::Node*
BinarySearchTree<K, O, B, C>::helperForRemoval(Node* myRoot, const K myKey) {
    if (!myRoot) {
        return NULL;
    }
    else if (isLessThan(myRoot->key, myKey)) {
        myRoot->right = helperForRemoval(myRoot->right, myKey);
    }
    else if (isLessThan(myKey, myRoot->key)) { 
        myRoot->left = helperForRemoval(myRoot->left, myKey);
    }
    else {
        
        if (!myRoot->right && !myRoot->left) { 
            delete myRoot;
            return NULL;
        }
        else if (myRoot->right && myRoot->left) {
            Node* minimum = findMinimum(myRoot->right);
            
            minimum ->right = fuu(myRoot->right);
            minimum->left = myRoot->left;
            delete myRoot;
            myRoot = minimum;
        }
        else if (myRoot->right) { 
            Node* tmp = myRoot;
            myRoot = myRoot->right;
            delete tmp;
            return myRoot;
        }
        else if (myRoot->left) { 
            Node* tmp = myRoot;
            myRoot = myRoot->left;
            delete tmp;
            return myRoot;
        }

    }

    if (myRoot) {
        myRoot->subsize = subsize(myRoot->left) + subsize(myRoot->right) + 1;
        

        if (myRoot->left && myRoot->right) {
            myRoot->height = max(myRoot->left->height, myRoot->right->height) + 1;
        }

        else if (myRoot->left) {
            myRoot->height = myRoot->left->height + 1;
        }

        else if (myRoot->right) {
            myRoot->height = myRoot->right->height + 1;
        }
        else{
            myRoot->height = 0;
        }

        if ((myRoot->subsize) > 1) {
            int _val = log2(myRoot->subsize);
            if (!isBalanced(myRoot->height, _val)) {
                std::vector<Node*> myVect;
                inorderTraversal(myVect, myRoot);
                helperForToComplete(myVect, 0, myVect.size() - 1, myRoot);
                return myRoot;
            }
        }
    }
    return myRoot;
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K, O, B, C>::Node*
BinarySearchTree<K, O, B, C>::fuu(Node* myRoot) {
    if (!myRoot->left) { 
       
        return myRoot->right;
    }
    
    myRoot->left = fuu(myRoot->left);
    
    myRoot->subsize = subsize(myRoot->left) + subsize(myRoot->right) + 1;

    if (myRoot->left && myRoot->right) {
        myRoot->height = max(myRoot->left->height, myRoot->right->height) + 1;
    }

    else if (myRoot->left) {
        myRoot->height = myRoot->left->height + 1;
    }

    else if (myRoot->right) {
        myRoot->height = myRoot->right->height + 1;
    }
    else{
        myRoot ->height = 0;
    }

    if ((myRoot->subsize) > 1) {
        int _val = log2(myRoot->subsize);
        if (!isBalanced(myRoot->height, _val)) {
            std::vector<Node*> myVect;
            inorderTraversal(myVect, myRoot);
            helperForToComplete(myVect, 0, myVect.size() - 1, myRoot);
            return myRoot;
        }
        
    }
    return myRoot;
}


template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K, O, B, C>::Node*
BinarySearchTree<K, O, B, C>::findMinimum(Node* myRoot) {
    Node* min = myRoot;
    if (!myRoot) {

        return NULL;
    }
    else {

        if (!myRoot->left) {
            return min;
        }

        else if (myRoot->left) {
            findMinimum(myRoot->left);
        }
    }

}





template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::remove(const K& k)
{
    Node* inOrNot = find(k);


    if (!inOrNot) {
        return;
    }
    else {
        numNodes--;
        root = helperForRemoval(root, k);
    }
}




template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K, O, B, C>::Node*
BinarySearchTree<K, O, B, C>::find(const K& key) const
{
    return find(key, root);
}



template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K, O, B, C>::Node*
BinarySearchTree<K, O, B, C>::find(const K& key, Node* t) const
{
    if (t == NULL)
    {
        return NULL;
    }
    else if (isLessThan(key, t->key))
    {
        return find(key, t->left);
    }
    else if (isLessThan(t->key, key))
    {
        return find(key, t->right);
    }
    else //found
    {
        return t;
    }
}
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::findHelper(std::list<Node*>& adressList, K firstKey, K secondKey, Node* myRoot) const {
    
    if(myRoot== NULL){
        return;
    }
    
    else {
        if(isLessThan(firstKey,myRoot->key)){
            findHelper(adressList,firstKey,secondKey,myRoot->left);
        }
        if(!(isLessThan(myRoot->key,firstKey)) && !(isLessThan(secondKey,myRoot->key))){
            adressList.push_back(myRoot);
        }
        
        if(isLessThan(myRoot->key,secondKey)){
            findHelper(adressList,firstKey,secondKey,myRoot->right);
        }
    }
        
}


template <typename K, typename O, typename B, typename C>
std::list<typename BinarySearchTree<K, O, B, C>::Node*>
BinarySearchTree<K, O, B, C>::find(const K& lower, const K& upper) const
{
    std::list<Node*> ranges;

    findHelper(ranges, lower, upper, root);
    

    return ranges;
}



template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K, O, B, C>::Node*
BinarySearchTree<K, O, B, C>::getRoot() const
{
    return root;
}



template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K, O, B, C>::height() const
{
    return height(root);
}



template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K, O, B, C>::height(Node* t) const
{
    return (t == NULL) ? -1 : t->height;
}


// private utility
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K, O, B, C>::subsize(Node* t) const
{
    return (t == NULL) ? 0 : t->subsize;
}



template <typename K, typename O, typename B, typename C>
size_t
BinarySearchTree<K, O, B, C>::size() const
{
    return numNodes;
}


template <typename K, typename O, typename B, typename C>
bool
BinarySearchTree<K, O, B, C>::empty() const
{
    return numNodes == 0;
}



template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::makeEmpty(Node*& t)
{
    if (t != NULL)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;

        --numNodes;
    }

    t = NULL;
}



template <typename K, typename O, typename B, typename C>
BinarySearchTree<K, O, B, C>::~BinarySearchTree()
{
    makeEmpty(root);
}



template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::print(std::ostream& out) const
{
    print(root, out);
    out << '\n';
}



template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K, O, B, C>::print(Node* t, std::ostream& out) const
{
    if (t != NULL && t->left != NULL)
    {
        out << '[';
        print(t->left, out);
    }
    else if (t != NULL && t->left == NULL && t->right != NULL)
    {
        out << "[";
    }

    if (t != NULL)
    {
        if (t->left == NULL && t->right == NULL)
        {
            out << '(' << (t->key) << ')';
        }
        else if (t->left != NULL || t->right != NULL)
        {
            out << '{' << (t->key) << ",H" << t->height << ",S" << t->subsize << '}';
        }
    }

    if (t != NULL && t->right != NULL)
    {
        print(t->right, out);
        out << ']';
    }
    else if (t != NULL && t->left != NULL && t->right == NULL)
    {
        out << "]";
    }
}



template <typename K, typename O, typename B, typename C>
template <typename T>
const T&
BinarySearchTree<K, O, B, C>::max(const T& el1, const T& el2)
{
    return el1 > el2 ? el1 : el2;
}

