//Zach Akin-Amland
//CSCI 2270- Data Structures Final
//April 19,2021 - April 27,2021
#ifndef MINIGIT_H
#define MINIGIT_H
#include<vector>
#include<iostream>
using namespace std;


struct singlyNode{
    string fileName;                // Name of file in current directory
    string fileVersion;             // Name of file in .minigit sub directory
    singlyNode *next;               // Pointer to the next file in the current commit
};

struct Branch;

struct doublyNode{
    double commitNumber;
    singlyNode *head;               // single linked list of files in commit
    vector<doublyNode*> branches;   // vector of pointers to other commits
    Branch* branch;                 // Branch the commit is apart of
};

struct Branch{              
    doublyNode* branchHead;         // Pointer to first commit in the branch
    vector<doublyNode*> commits;    // Vector of all commits in the branch
    string name;                    // Name of the branch
};

struct stackNode{                   // Commit node in the stack
    doublyNode* node;               // Pointer to the commit
    stackNode *next;                // Pointer to next stackNode
};


class StackLL                       // Stack of commits
{
  private:
    stackNode* stackHead;
  public:
    StackLL();
    ~StackLL();
    doublyNode* returnStackHeadNode(){
        return stackHead->node;
    }
    stackNode* returnStackHead(){
        return stackHead;
    }
    bool isEmpty();
    stackNode* push(doublyNode* node);
    void pop();
    int peek();
};

class minigit {                                                             // Minigit class
    public:
        void runMinigit();                                                  // Runs program
        doublyNode* init();                                                 // Initializes new repository (Not used in this version)
        void add(doublyNode* currNode);                                     // Adds file to current commit
        void remove(doublyNode* currNode);                                  // Removes file from current commit
        doublyNode* commit(doublyNode* currNode,Branch* currBranch);        // Commits current commit, creates new commit node
        bool checkout(doublyNode* currNode);                                // checkout previous commits
        doublyNode* newBranch(doublyNode* currNode,string newBranchName);   // Creates new branch, sets current commit to head of branch
        doublyNode* switchBranch(doublyNode* currNode);                     // Switch between branches
        void diff();                                                        // Check if a file has been changed since last commit
        void status();                                                      // List all files changed since last commit
        void serialize();                                                   // Serializes data into .xml
        doublyNode* deserialize();                                          // Deserializes .xml into data when program starts
    private:
        vector<doublyNode*> commits;                                        // vector of all commits
        vector<Branch*> branchVector;                                       // vector of all branches
        doublyNode *currentDirectory;                                       // pointer to current commit node
        Branch* currentBranch;                                              // pointer to current branch node
        StackLL commitStack;                                                // stack of commit nodes
        doublyNode* previousCommit;                                         // pointer to most recent commit node
        stackNode* stackh;                                                  // top node of stack of commit nodes
        
};

#endif