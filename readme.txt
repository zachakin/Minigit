CSCI 2270 - Data Stuctures Final Project
Zach Akin-Amland
Phase 2

-----Header File-----
"miniGit.hpp"
#include <iostream>
#include <vector>

-Single Linked List Struct 'singlyNode'
* file name
* file version
* next pointer

-Double Linked List Struct 'doublyNode'
* commit number
* single LL head
* vector of doublyNode pointers
* pointer to the Branch that the commit is apart of

-Branch Struct 'Branch'
* doublyNode* branch_head
* vector of commits apart of the Branch
* string name

-Stack Node Struct 'stackNode'
* doublyNode* node
* stackNode* next

StackLL Class

-Public
* Constuctor
* Destructor
* return top stack head node
* return stack head
* bool isEmpty()
* push
* pop
* peek

-Private
* stackNode stackHead - pointer to stackHead



Minigit Class

-Public
* void runMinigit() runs prints the menu and runs the program
* doublyNode* init(doublyNode* currNode) creates a new repository, returns new DLL node
* void add(doublyNode* currNode) creates new single linked list node and adds it to the current commits SLL
* void remove(doublyNode* currNode) removes SLL node from current commit SLL
* doublyNode* commit(doublyNode* currNode) commits the current double LL node's single LL by copying the files that have been changed to .minigit folder
* bool checkout(doublyNode* currNode) copies files in the .minigit folder from the commit the user checks out to the corresponding files in the current folder
* doublyNode* newBranch(doublyNode* currNode, string branchName) create a new Branch
* doublyNode* switchBranch(doublyNode* currNode) switches Branch
* void diff() takes user input and finds if the file has been changed since last commit
* void status() lists all files that have been changed since last commit
* void serialize() writes the data structures into a .xml file
* void deserialize() reads the .xml file into data structures on program start

-Private
* vector of all doublyNode* commits
* vector of all Branch* branches
* pointer to current commit node
* pointer to current Branch node
* Linked list stack- StackLL commitStack
* pointer to most recent commit
* pointer to the top of the commit stack

-----Implementation-----
"miniGit.cpp"

-init() //Not used in this version of minigit
* if .minigit folder can be created then
* A new doublyNode is created with next, previous, and head all pointing to null
* The node's commit number is set to 0 because it is the first commit
* The new node is returned in order to update the current commit pointer
* if .minigit folder cannot be created then the repository already exists and the function returns null

-add()
* The user inputs a file name to be added
* Substrings are used to check if the file type extension is valid
* If the file does not exist in the current directory then the function returns
* If the file exists already in the current commit node's single linked list then the function returns
* A new singlyNode is created and put at the front of the single linked list
* The new node is given the user input file name and file version 00

-remove()
* The user inputs a file name to be removed
* If the current commit node's single linked list is empty the function returns
* The single linked list is traversed to find the user input file name
* Once the input is found the node is removed from the single linked list
* If the node is not found the function returns

-commit()
* If the current commit node's single linked list head is empty then the function returns the current commit node
* A new doublyNode is created
* The new node's commit number is the current commit number +1
* The current commit is added to the new commits's vector of pointers
* The new commit is added to the current commit's vector of pointers
* The new commit's branch is set to the current Branch
* While the current commit node's single linked list is traversed
* New single linked list nodes are created
* The new nodes are copies of the current commits single linked list nodes
* New file versions are copied to .minigit based on the current commit's single linked list nodes
* If no versions of a file exist the file is copied to .minigit with version 00, current node version = 00, new node version = 00
* If versions of a file do exist then if the most recent version of a file does not equal the file in the current folder: file is copied with version 01, current node version 00, new node version 01
* If the most recent version of a file in .minigit is equal to the file in the current folder then the file has not been changed and gets the current nodes file version
* The new commit node is added to the current Branch's vector of commits
* The current commit is added to the vector of commits
* the stack head is set to the current commit node
* the most recent commit is set to the current commit
* The function returns the newly created node to update the current commit pointer

-checkout()
* Menu is displayed asking user to checkout HEAD, previous commit, i steps before HEAD, or custom commit
* Traverses all commits to find the most recent commit
* Lists all commit numbers except the most recent then the user chooses one to checkout
* The commits are all then traversed until the user input commit number is found
* If the user input commit number is not found the function returns false
* For each file in the current folder the found commit node's single linked list is traversed
* If the file is found in the commit's single linked list then
* the user input commit's file version is copied from .minigit to the file in the current folder
* the function returns false when the user input commit number is the most recent commit
* the function returns true when the user input commit number is not the most recent commit's
* This function is put in a while(true) loop so that the user only has the option to checkout or quit while the function returns true

-newBranch()
* Takes in user input branch name parameter
* Creates new branch sub folder in .minigit
* Creates new commit node with empty single LL and commit number 0
* Creates new branch with branch head as new commit node
* Branch name is the user input branch name
* The new commit node is added to the new branch vector of commits
* The new commit's branch pointer is set to the new branch
* Returns newly creates commit node
* deserializes .xml file is master branch already exists

-switchBranch()
* Displays all branches in branch vector
* Asks user to input which branch to switch to
* Check if user input branch name exists
* If it exists the found branch's head commit node is returned to update the current commit pointer

-diff()
* User inputs file name
* Check if file exists in the current folder
* Check if file exists in most recent commit
* ifstream both files line by line
* if files are not equal it outputs the first list not equal to each other
* function returns if the files are the same

-status()
* For each file in the folder the most recent commit single LL is traversed
* checks if the file is different than the version in the last commit
* outputs the file name if it has been changed

-serialize()
* Loops through each branch in the vector of branches
* Loops through each commit in the branches vector of commits
* Loops through each file in each of the commits single LL
* Each branch, commit, and file have a certain outputs
* The output is sent into a .xml file in the .minigit folder

-deserialize()
* Uses split function to read in .xml file in .minigit
* Rebuilds data structure based on strings that are on each line
* Creates a new commit node to be the current once the data structure is remade


-----Driver-----
"driver.cpp"

* Includes <iostream> and "miniGit.hpp"
* int main()
* minigit object 'm' is initialized
* m.runMinigit()
* return 0
