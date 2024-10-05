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
        Tree(std::vector<int> &vec);
        ~Tree();

        Node* root = nullptr;
        Node* treeMin(Node* root);
        Node* treeMax(Node* root);
        Node* findNode(Node* node, int key);
        Node* successor(Node* node);

        int getTreeHeight(Node* node);

        void treeInsert(int key);
        void treeDelete(Node* targetNode);
        void preorderTraversal(Node* node);
        void inorderTraversal(Node* node);
        void postorderTraversal(Node* node);

    private:
        void transplant(Node* nodeToReplace, Node* replacementNode);
};

Node::Node(int newKey){
    key = newKey;
}

Node::~Node(){}

Tree::Tree(){}

Tree::Tree(Node* newRoot){
    root = newRoot;
}

Tree::Tree(std::vector<int> &vec){
    for(int number: vec){
        treeInsert(number);
    }
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
        if(key < node->key){
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

    Node* curr = node->parent;

    while(curr != nullptr && node == curr->right){
        node = curr;
        curr = curr->parent;
    }

    return curr;
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
    Node* newValueParent = nullptr;
    Node* curr = root;

    while (curr != nullptr){
        newValueParent = curr;

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

void Tree::treeDelete(Node* targetNode){ 
    if(targetNode == nullptr){
        return;
    }

    if(targetNode->left == nullptr){
        transplant(targetNode, targetNode->right);
    }
    else if(targetNode->right == nullptr){
        transplant(targetNode, targetNode->left);
    }
    else{
        Node* replacementNode = treeMin(targetNode->right);
        if(replacementNode->parent != targetNode){
            transplant(replacementNode, replacementNode->right);
            replacementNode->right = targetNode->right;
            replacementNode->right->parent = replacementNode;
        }
        transplant(targetNode, replacementNode);
        replacementNode->left = targetNode->left;
        replacementNode->left->parent = replacementNode;
    }

    delete targetNode;
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

void Tree::transplant(Node* nodeToReplace, Node* replacementNode){  
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

/*==================================END OF TREE CODE================================*/

std::vector<int> generateRandomVector(int vectorSize){
    std::set<int> mySet;

    while(mySet.size() != vectorSize){
        int randInt = rand() % 100000;
        if(mySet.find(randInt) == mySet.end()){
            mySet.insert(randInt);
        }
        
    }

    std::vector<int> vec(mySet.begin(), mySet.end());
    std::random_shuffle(vec.begin(), vec.end());

    return vec;
}

void printCSV(std::vector<float> times, std::string fileName){
    std::vector<std::string> labels = {"3000", "6000", "9000", "12000", "15000", "18000", "21000", 
                                        "24000", "27000", "30000", "33000", "36000", "39000", "42000", 
                                        "45000", "48000", "51000", "54000", "57000", "60000",
                                        "63000", "66000", "69000", "72000", "75000", "78000", "81000", 
                                        "84000", "87000", "90000"};
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
    std::vector<int> nValues = {3000, 6000, 9000, 12000, 15000, 18000, 21000, 24000, 27000, 30000, 
                                33000, 36000, 39000, 42000, 45000, 48000, 51000, 54000, 57000, 60000,
                                63000, 66000, 69000, 72000, 75000, 78000, 81000, 84000, 87000, 90000};
    
    
    std::vector<float> averageHeightsVec;
    std::vector<float> averageCreateVec;
    std::vector<float> averageDestroyVec;

    for(auto nValue: nValues){
        
        averageTreeHeight = 0;
        averageCreateTime = 0.0;
        averageDestroyTime = 0.0;
        for(int numTrials = 0; numTrials < NUM_EXPERIMENTS; numTrials++){
    
            std::vector<int> keyVector = generateRandomVector(nValue);
            auto startCreate = std::chrono::high_resolution_clock::now();
            Tree* experimentTree = new Tree(keyVector);
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

        std::cout <<nValue << '\n';
    }

    printCSV(averageHeightsVec, "AverageTreeHeights.csv");
    printCSV(averageCreateVec, "AverageTreeCreateTime.csv");
    printCSV(averageDestroyVec, "AverageTreeDestroyTime.csv");
}

int main(){
    partOneExperiment();    
    return 0;
};