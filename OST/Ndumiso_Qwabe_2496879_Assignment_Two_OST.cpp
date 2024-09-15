#include <bits/stdc++.h> 
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Node{
    public:
        Node(int newKey);
        ~Node();

        int key, size = 1;

        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
};

class OSTree{
    public:
        OSTree();
        OSTree(Node* newRoot);
        OSTree(std::vector<int> &vec);
        ~OSTree();

        Node* root = nullptr;
        Node* treeMin(Node* root);
        Node* treeMax(Node* root);
        Node* findNode(Node* node, int key);
        Node* orderFindNode(Node* node, int key);
        Node* successor(Node* node);

        int rank(Node* node);
        int getTreeHeight(Node* node);

        void treeInsert(int key);
        void treeDelete(Node* targetNode);
        void preorderTraversal(Node* node);
        void inorderTraversal(Node* node, bool printMode);
        void postorderTraversal(Node* node);
        

    private:
        void transplant(Node* nodeToReplace, Node* replacementNode);
        void traverseDecrement(Node* targetNode);
};

Node::Node(int newKey){
    key = newKey;
}

Node::~Node(){}

OSTree::OSTree(){}

OSTree::OSTree(Node* newRoot){
    root = newRoot;
}

OSTree::OSTree(std::vector<int> &vec){
    for(int number: vec){
        treeInsert(number);
    }
}

OSTree::~OSTree(){
    while (root != nullptr){
        treeDelete(root);
    }
}

Node* OSTree::treeMin(Node* root){ 
    while(root->left != nullptr){
        root = root->left;
    }

    return root;
}

Node* OSTree::treeMax(Node* root){ 
    while(root->right != nullptr){
        root = root->right;
    }

    return root;
}

Node* OSTree::findNode(Node* node, int key){
    while (node != nullptr && key != node->key){
        if(key < node->key){
            node = node->left;
        }
        else{
            node = node->right;
        }
    }

    return node;
}

Node* OSTree::orderFindNode(Node* node, int rank){
    int rankOfRoot = 0;

    if(node->left != nullptr){
        rankOfRoot = node->left->size;
    }
    rankOfRoot++;

    if(rank == rankOfRoot){
        return node;
    }
    else if(rank < rankOfRoot){
        return orderFindNode(node->left, rank);
    }

    return orderFindNode(node->right, rank - rankOfRoot);
}

Node* OSTree::successor(Node* node){
    if(node->right != nullptr){
        return treeMin(node->right);
    }

    Node* curr = node->parent;

    while(curr != nullptr && node==curr->right){
        node = curr;
        curr = curr->parent;
    }

    return curr;
}

int OSTree::rank(Node* node){
    int rank = 0;

    if(node->left != nullptr){
        rank = node->left->size;
    }
    rank++;

    Node* y = node;

    while (y != root){
        if(y == y->parent->right){
            if(y->parent->left != nullptr){
                rank = rank + y->parent->left->size;
            }
            rank++;
        }
        y = y->parent;
    }

    return rank;
}

int OSTree::getTreeHeight(Node* node){
    if(node == nullptr){
        return -1;
    }

    int leftHeight = getTreeHeight(node->left);
    int rightHeight = getTreeHeight(node->right);

    return std::max(leftHeight, rightHeight) + 1;
}

void OSTree::treeInsert(int key){
    Node* newValue = new Node(key);
    Node* newValueParent = nullptr;
    Node* curr = root;

    while (curr != nullptr){
        newValueParent = curr;

        //since newValue is going to be in curr's subtree, increase curr's size
        curr->size++;

        if(newValue->key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }

    newValue->parent = newValueParent;
    
    if(newValueParent == nullptr){
        root = newValue;
    }
    else if(newValue->key < newValueParent->key){
        newValueParent->left = newValue;
    }
    else{
        newValueParent->right = newValue;
    }
}

/*The traverseDecrement function decrements the size of all of the ancestors of the
node that is going to be transplanted*/
void OSTree::traverseDecrement(Node* targetNode){
    Node* curr = root;

    if(targetNode == nullptr){
        int f = 0;
    }

    while (curr != nullptr && targetNode != nullptr && curr != targetNode){
        curr->size--;

        if(targetNode->key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
}

void OSTree::treeDelete(Node* targetNode){

    if(targetNode == nullptr){
        return;
    }
    
    Node* transplantNode;
    if(targetNode->left == nullptr){
        transplantNode = targetNode->right;
        traverseDecrement(targetNode->right);
        transplant(targetNode, targetNode->right);
    }
    else if(targetNode->right == nullptr){
        transplantNode = targetNode->left;
        traverseDecrement(targetNode->left);
        transplant(targetNode, targetNode->left);
    }
    else{
        Node* replacementNode = treeMin(targetNode->right);
        transplantNode = replacementNode;
        traverseDecrement(replacementNode);

        if(replacementNode->parent != targetNode){
            transplant(replacementNode, replacementNode->right);
            replacementNode->right = targetNode->right;
            replacementNode->right->parent = replacementNode;
        }

        transplant(targetNode, replacementNode);
        replacementNode->left = targetNode->left;
        replacementNode->left->parent = replacementNode;
    }

    //update transplantNode's size to be the sum of the size of its children + 1
    if(transplantNode != nullptr){
        transplantNode->size = 1;

        if(transplantNode->left != nullptr){
            transplantNode->size += transplantNode->left->size;
        }

        if(transplantNode->right != nullptr){
            transplantNode->size += transplantNode->right->size;
        }
    }
    
    delete targetNode;
}

void OSTree::preorderTraversal(Node* node){
    if(node != nullptr){
        std::cout << node->key << " ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
}

void OSTree::inorderTraversal(Node* node, bool printMode){
    if(node != nullptr){

        inorderTraversal(node->left, printMode);

        if(printMode){
            std::cout << node->key << ":" << rank(node) << " ";
        }
        else{
            node->key -= 1;
        }

        inorderTraversal(node->right, printMode);
    }
}

void OSTree::postorderTraversal(Node* node){
    if(node != nullptr){
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        std::cout << node->key << " ";
    }
}

void OSTree::transplant(Node* nodeToReplace, Node* replacementNode){
    if(nodeToReplace->parent == nullptr){
        root = replacementNode;
    }
    else if(nodeToReplace == nodeToReplace->parent->left){
        nodeToReplace->parent->left = replacementNode;
    }
    else{
        nodeToReplace->parent->right = replacementNode;
    }

    if(replacementNode != nullptr){
        replacementNode->parent = nodeToReplace->parent;
    }
}

/*==================================END OF OSTREE CODE===============================*/

int main(){
    std::vector<int> vec = {27, 30, 25, 28, 50, 40, 60, 42, 65};
    OSTree* tree = new OSTree(vec);
    tree->inorderTraversal(tree->root, true);
    tree->treeDelete(tree->findNode(tree->root, 40));
    delete tree;
    return 0;
};