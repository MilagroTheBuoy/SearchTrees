#include <bits/stdc++.h> 
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

const int NUM_EXPERIMENTS = 10;

class Node{
    public:
        Node(int newKey);
        ~Node();
        int key;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
};

class Tree{
    public:
        Tree();
        Tree(Node* newRoot);
        ~Tree();
        Node* root = nullptr;
        Node* treeMin(Node* root);
        Node* treeMax(Node* root);
        Node* findNode(Node* node, int key);
        Node* successor(Node* node);
        int getTreeHeight(Node* node);
        void treeInsert(int key);
        void treeDelete(Node* newValue);
        void preorderTraversal(Node* node);
        void inorderTraversal(Node* node);
        void postorderTraversal(Node* node);

    private:
        void transplant(Node* u, Node* v);
};

Node::Node(int newKey){
    key = newKey;
}

Node::~Node(){}

Tree::Tree(){}

Tree::Tree(Node* newRoot){
    root = newRoot;
}

Tree::~Tree(){
    while (root != nullptr){
        treeDelete(root);
    }
}

Node* Tree::treeMin(Node* root){ 
    while(root->left != nullptr){
        root = root->left;
    }
    return root;
}

Node* Tree::treeMax(Node* root){ 
    while(root->right != nullptr){
        root = root->right;
    }
    return root;
}

Node* Tree::findNode(Node* node, int key){
    while (node != nullptr && key != node->key){
        if(node->key < key){
            node = node->left;
        }
        else{
            node = node->right;
        }
    }
    return node;
}

Node* Tree::successor(Node* node){
    if(node->right != nullptr){
        return treeMin(node->right);
    }
    Node* y = node->parent;
    while(y != nullptr && node==y->right){
        node = y;
        y = y->parent;
    }
    return y;
}

int Tree::getTreeHeight(Node* node){
    if(node == nullptr){
        return -1;
    }

    int leftHeight = getTreeHeight(node->left);
    int rightHeight = getTreeHeight(node->right);

    return std::max(leftHeight, rightHeight) + 1;
}

void Tree::treeInsert(int key){
    Node* newValue = new Node(key);
    Node* y = nullptr;
    Node* x = root;
    while (x != nullptr){
        y = x;
        if(newValue->key < x->key){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    newValue->parent = y;
    if(y == nullptr){
        root = newValue;
    }
    else if(newValue->key < y->key){
        y->left = newValue;
    }
    else{
        y->right = newValue;
    }
}

void Tree::treeDelete(Node* newValue){ 
    if(newValue->left == nullptr){
        transplant(newValue, newValue->right);
    }
    else if(newValue->right == nullptr){
        transplant(newValue, newValue->left);
    }
    else{
        Node* y = treeMin(newValue->right);
        if(y->parent != newValue){
            transplant(y, y->right);
            y->right = newValue->right;
            y->right->parent = y;
        }
        transplant(newValue, y);
        y->left = newValue->left;
        y->left->parent = y;
    }
    delete newValue;
}

void Tree::preorderTraversal(Node* node){
    if(node != nullptr){
        std::cout << node->key << " ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
}

void Tree::inorderTraversal(Node* node){
    if(node != nullptr){
        inorderTraversal(node->left);
        std::cout << node->key << " ";
        inorderTraversal(node->right);
    }
}

void Tree::postorderTraversal(Node* node){
    if(node != nullptr){
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        std::cout << node->key << " ";
    }
}

void Tree::transplant(Node* u, Node* v){  
    if(u->parent == nullptr){
        root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    if(v != nullptr){
        v->parent = u->parent;
    }
}


/*=========================================================================================*/
//END OF TREE CODE
std::vector<int> generateRandomVector(int vectorSize){
    std::set<int> mySet;
    while(mySet.size() != vectorSize){
        mySet.insert(rand() % 10000);
    }
    std::vector<int> vec(mySet.begin(), mySet.end());
    std::random_shuffle(vec.begin(), vec.end());
    return vec;
}

Tree* createTree(std::vector<int> &vec){
    Tree* newTree = new Tree();
    for(int nodeNum = 0; nodeNum < vec.size(); nodeNum++){
        newTree->treeInsert(vec[nodeNum]);
    }
    return newTree;
}

void printCSV(std::vector<float> times, std::string fileName){
    std::vector<std::string> labels = {"300", "600", "900", "1200", "1500", "1800", "2100", 
                                        "2400", "2700", "3000", "3300", "3600", "3900", "4200", 
                                        "4500", "4800", "5100", "5400", "5700", "6000"};
    std::ofstream csv;
    csv.open(fileName);
    csv << "input,time\n";
    csv <<"0,0\n";
    for (size_t i = 0; i < labels.size(); i++){
        csv << labels[i] << "," << times[i] << "\n";
    }
    csv.close();
}

void partOneExperiment(){
    int treeHeight, averageTreeHeight;
    float averageCreateTime, averageDestroyTime;
    std::vector<int> nValues = {300, 600, 900, 1200, 1500, 1800, 2100, 2400, 2700, 3000, 
                                3300, 3600, 3900, 4200, 4500, 4800, 5100, 5400, 5700, 6000};
    
    std::vector<float> averageHeightsVec;
    std::vector<float> averageCreateVec;
    std::vector<float> averageDestroyVec;

    for(auto nValue: nValues){
        
        averageTreeHeight = 0;
        averageCreateTime = 0.0;
        averageDestroyTime = 0.0;
        for(int numTrials = 0; numTrials < NUM_EXPERIMENTS; numTrials++){
            //create tree, measure height and measure times
    
            std::vector<int> keyVector = generateRandomVector(nValue);
            auto startCreate = std::chrono::high_resolution_clock::now();
            Tree* experimentTree = createTree(keyVector);
            auto endCreate = std::chrono::high_resolution_clock::now();
            averageCreateTime += std::chrono::duration_cast<std::chrono::nanoseconds>
                            (endCreate - startCreate).count();
            treeHeight = experimentTree->getTreeHeight(experimentTree->root);
            averageTreeHeight += treeHeight;
            auto startDestroy = std::chrono::high_resolution_clock::now();
            delete experimentTree;
            auto endDestroy = std::chrono::high_resolution_clock::now();

            averageDestroyTime += std::chrono::duration_cast<std::chrono::nanoseconds>
                            (endDestroy - startDestroy).count();
        }

        averageHeightsVec.push_back(averageTreeHeight/NUM_EXPERIMENTS);
        averageCreateVec.push_back(averageCreateTime/NUM_EXPERIMENTS);
        averageDestroyVec.push_back(averageDestroyTime/NUM_EXPERIMENTS);
        std::cout << nValue << "\n";
    }

    printCSV(averageHeightsVec, "AverageTreeHeights.csv");
    printCSV(averageCreateVec, "AverageTreeCreateTime.csv");
    printCSV(averageDestroyVec, "AverageTreeDestroyTime.csv");
}

int main(){
    partOneExperiment();    
    return 0;
};