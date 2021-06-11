//Minigit Class Implementation File
#include "miniGit.hpp"
#include <iostream>
#include <filesystem>
#include <string>
// #include <regex>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

void minigit::runMinigit()
{
    cout << "Press 1 to enter repository" << endl;                              //Ask user to initialize new repository    
    cout << "Press any other button to quit" << endl;
    string option;  
    getline(cin,option);                                                        //Get user input
    if(option!="1") return;                                                     //Program quits if user does not enter 1
    fs::create_directory(".minigit");
    currentDirectory=newBranch(currentDirectory,"master");                      //Sets current doublyNode pointer to new node

    int x=0;                                                                    //variable used for switch statement
    if(currentDirectory!=NULL){                                                 //checks if initial doublyNode node was created while initializing
        while(x!=9){
            currentBranch=currentDirectory->branch;                             //Keeps current branch updated             
            x=0;
            while(x<1||x>9){                                                    //print menu while the user input is not 1-5
                cout << "Select a number option:" << endl;                      
                cout << "1. Add a file" << endl;
                cout << "2. Remove a file" << endl;
                cout << "3. Commit files" << endl;
                cout << "4. Checkout a commit" << endl;
                cout << "5. Add a branch" << endl;
                cout << "6. Switch branch" << endl;
                cout << "7. minigit diff" << endl;
                cout << "8. minigit status" << endl;
                cout << "9. Quit" << endl;
                getline(cin,option);
                x=stoi(option);
                if(x<1||x>9) cout << "Invalid option, please try again." << endl;    
            }
            string input;
            doublyNode* newBranchNode;
            switch(x){                                                          //switch with user input
                case 1:
                    add(currentDirectory);
                break;
                case 2:
                    remove(currentDirectory);
                break;
                case 3:
                    currentDirectory=commit(currentDirectory,currentBranch);    //committing updates current commit pointer
                break;
                case 4:
                    while(checkout(currentDirectory)==true){                    //checkout() returns true if current commit pointer is not the most recent commit
                        int y=0;                                                //while true, user only has option to checkout or quit
                        while(y!=1&&y!=2){                                                   
                            cout << "Select a number option:" << endl;
                            cout << "1. Checkout a commit" << endl;                                 
                            cout << "2. Quit" << endl;
                            getline(cin,option);
                            y=stoi(option);
                            if(y!=1&&y!=2) cout << "Invalid option, please try again." << endl;    
                        }
                        switch(y){
                            case 1:
                            break;
                            case 2:
                                fs::remove_all(".minigit");
                                return;
                            break;
                        }
                    }
                break;
                case 5:                                                         //Adding a new branch
                    cout << "Enter the name of the new branch: ";
                    getline(cin,input);
                    newBranchNode = newBranch(currentDirectory,input);
                    currentDirectory->branches.push_back(newBranchNode);
                    currentDirectory=newBranchNode;
                break;
                case 6:
                    currentDirectory=switchBranch(currentDirectory);            //switching branch updates current commit node
                break;
                case 7:
                    diff();                                                     //minigit diff
                break;
                case 8:
                    status();                                                   //minigit status
                break;
                case 9:
                    serialize();                                                //Quitting function serializes data
                    return;
                break;
            }
        }
    }
    return;
}

doublyNode* minigit::init(){                                                    //Initialize new repository function
    if(fs::create_directory(".minigit")){                                       //Not used in this version of minigit
        doublyNode *newRepo = new doublyNode;
        newRepo->head=NULL;
        newRepo->commitNumber=0;
        return newRepo;
    } else {
        cout << "Repository already exists" << endl;
        return NULL;
    }
}

void minigit::add(doublyNode* currNode){                                        //Add file to current commit function
   
    string repoPath=fs::current_path();
    string fileType;
    string input;
    string fileVersionNumber;
    
    cout << "Enter the name of the file: ";                                     //Get user input file name
    getline(cin,input);
    fileType = input.substr(input.length()-4,input.length());                   //sub string of last 4 characters of user input

    if(fileType!=".hpp"&&fileType!=".cpp"&&fileType!=".txt"){                   //if file is not .txt .cpp or .hpp    
        cout << "Invalid file type" << endl;                                    //file type is invalid and function runs recursively
        add(currNode);
        return;
    }

    string path = fs::current_path();                                           //current folder path
    bool found=false;
    for (const auto &entry : fs::directory_iterator(path)){                     //find if user input file exists in current folder
        if(entry.path()==path+"/"+input){
            found=true;
        }
    }
    if(found==false){                                                           //if file doesn't exist function returns
        cout << "File doesn't exist" << endl;
        return;
    }

    singlyNode *head;
    head=currNode->head;

    while(head!=NULL){                                                          //find if the file has been added to the single linked list already
        if(head->fileName==input){                                              //if it has been added before, the function returns
            cout << "File already exists" << endl;
            add(currNode);
            return;
        }
        head=head->next;
    }

    singlyNode* node= new singlyNode;                                           //if all previous tests have passed
    node->fileName=input;                                                       //new single LL node is created with user input file name
    node->fileVersion=input.substr(0,input.length()-4)+"00"+input.substr(input.length()-4,input.length());
    node->next=currentDirectory->head;                                          //file version starts at 00
    currentDirectory->head=node;                                                //adds new node to LL head like a stack
    cout << "Added: " << node->fileVersion << " to " << currNode->commitNumber << ". With the name: " << currNode->head->fileVersion << endl;
}

void minigit::remove(doublyNode* currNode){                                     //Remove file from current commit function

    string filename;
    cout<<"Enter the name of the file to be removed" << endl;                   //Get user input file name
    getline(cin,filename);

    singlyNode *head=currNode->head;                                            //Check if current commit single LL is empty
    
    if(head==NULL)                                                              //if empty, function returns
    {
        return;
    }
    bool found=false;
    singlyNode* prev=NULL;
    singlyNode* temp=NULL;
    while(head!=NULL)                                                           //traverse single LL to find user input file name
    {
        if(head->fileName==filename)            
        {
            if(prev==NULL)                                                      //Case to delete single LL head node
            {
                temp=currNode->head;
                currNode->head=currNode->head->next;
                delete temp;
                temp=0;                    
                found=true;
                cout << "Removed: " << filename << endl;
                break;
            }
            else                                                                //Case to delete all other nodes
            {
                prev->next=head->next;
                delete head;
                head=0;
                found=true;
                cout << "Removed: " << filename << endl;
                break;
            }
        }
        prev=head;
        head=head->next;
    }
    if(!found)                                                                  //Output if user input file name does not exist
    {   
        cout << "File does not exist." << endl;
    }
}

void copy(string f1name, string f2name){                                        //Copy file function
    ifstream f1(f1name);                                                        //Input file
    ofstream f2(f2name);                                                        //Output file
    char c1,c2; 

    if(!f1.is_open()){
        cout<<"input file cannot be opened"<< endl;
        return;
    }
    if(!f2.is_open()){
        cout<<"output file cannot be opened"<< endl;
        return;
    }
    while(1){                                                          
        c1=f1.get();
        if(c1==EOF){
            break;
        }
        f2 << c1;
    }
    f1.close();
    f2.close();
}

bool isEqual(string f1name,string f2name){                                      //Checking if file are equal
    ifstream f1(f1name), f2(f2name);
    char c1,c2;

    if(!f1.is_open()&&!f2.is_open()){
        return true;
    }
    if(!f1.is_open()||!f2.is_open()){
        return false;
    }
    while(1){
        c1=f1.get();
        c2=f2.get();
        if(c1!=c2){
            return false;
        }
        if(c1==EOF&&c2==EOF){
            return true;
        }
    }
    f1.close();
    f2.close();
    return true;
}

string findHighestVersion(singlyNode* node,string fileVersion,Branch* currBranch){  //Returns string of highest version +1 in .minigit sub folder
    string path = fs::current_path();
    path=path+"/.minigit/"+currBranch->name;

    string temp;
    int x=0;

    for (const auto &entry : fs::directory_iterator(path)){
        if(entry.path()==path+"/"+fileVersion){
            temp = fileVersion.substr(node->fileName.length()-4,2);
            x=stoi(temp);
            x++;
            if(x<10){
                fileVersion=node->fileName.substr(0,node->fileName.length()-4)+"0"+to_string(x)+node->fileName.substr(node->fileName.length()-4,4);
            } else {
                fileVersion=node->fileName.substr(0,node->fileName.length()-4)+to_string(x)+node->fileName.substr(node->fileName.length()-4,4);
            }
            fileVersion = findHighestVersion(node,fileVersion,currBranch);
            break;
        }
    }
    return fileVersion;
}

string findOneLessVersion(singlyNode *head,string newFileVersion){      //Returns one less file version string than passed in
    string prefix = newFileVersion.substr(0,head->fileName.length()-4);
    string version = newFileVersion.substr(head->fileName.length()-4,2);
    string suffix = newFileVersion.substr(head->fileName.length()-2,4);
    int oneLessVersion = stoi(version)-1;
    string minusVersion="";
    if(oneLessVersion<10){
        minusVersion=prefix+"0"+to_string(oneLessVersion)+suffix;
    } else {
        minusVersion=prefix+to_string(oneLessVersion)+suffix;
    }
    return minusVersion;
}



doublyNode* minigit::commit(doublyNode* currNode,Branch* currBranch){           //Commit current commit node function
    singlyNode* head=currNode->head;

    if(head==NULL){
        cout << "Nothing to commit" << endl;                                    //if current commit head is NULL nothing has been added to commit
        return currNode;
    }

    doublyNode *ptr = currNode;                                                 //Pointer to current commit node
    doublyNode *newNode = new doublyNode;                                       //Creating new commit node
    newNode->commitNumber=ptr->commitNumber+1;                                  //Increase commit number by 1
    ptr->branches.push_back(newNode);                                           //current commit points to new commit node
    newNode->branches.push_back(ptr);                                           //new node points to current node
    newNode->branch=currBranch;                                                 //sets new node branch

    head=currNode->head;                                                        //copying SLL
    singlyNode* sn = new singlyNode;                                            //new single LL node to create new single LL
    newNode->head=sn;                                                           //new commit node head is new single LL node
    while(head!=NULL){                                                          //Traverse current commit node single LL

        sn->fileName=head->fileName;                                            //copies current single LL node file name to new single LL node
        sn->next=NULL;          

        string path = fs::current_path();
        bool found=false;
        path=path+"/.minigit/"+currBranch->name;                                //path = currentFolder /.minigit / branchName

        string newFileVersion=findHighestVersion(head,head->fileVersion,currBranch);    //string of highest file version +1 in .minigit
        string minusVersion = findOneLessVersion(head,newFileVersion);                  //One less than highest version

        for (const auto &entry : fs::directory_iterator(path)){                         //traverse .minigit/ to find .minigit/ minusVersion
            if(entry.path()==path+"/"+minusVersion){                   
                found=true;
            }
        }
        if(found==false){                                                               //if file doesn't exist in .minigit/
            cout << "Committing first version of " << head->fileVersion << endl;
            copy(head->fileName,".minigit/"+currBranch->name+"/"+head->fileVersion);    //file is copied from current directory
            sn->fileVersion=head->fileVersion;                                          //new single LL node file version is 00
        } else {
            if(!isEqual(head->fileName,".minigit/"+currBranch->name+"/"+minusVersion)){ //Checks if current single LL node is equal to current highest version in .minigit
                copy(head->fileName,".minigit/"+currBranch->name+"/"+newFileVersion);   //if not equal, copies file from current folder to .minigit with incremented file version
                head->fileVersion=newFileVersion;                                       //updates current commit single LL node because the file was changed
                sn->fileVersion=newFileVersion;                                         //copies file version to new single LL node
            } else {
                cout << head->fileName << " was unchanged." << endl;                    //if files are equal, file to commit was unchanged
                sn->fileVersion=head->fileVersion;                                      //new single LL node has same file version as current commitm single LL node
            }
        }
        head=head->next;                                                //traverses single LL to copy over
        if(head!=NULL){                                                 //if it is not the end of the list
            sn->next=new singlyNode;                                    //new nodes are created so that there is something to copy into
            sn=sn->next;
        }
    }
    currBranch->commits.push_back(newNode);                             //Adds new commit to current branch commit pointer vector
    commits.push_back(currNode);                                        //Adds new commit to vector of all commits
    stackh=commitStack.push(currNode);                                  //Adds new commit to top of commit stack
    previousCommit=currNode;                                            //Setsc current commit to previous commit
    return newNode;                                                     //returns new commit node to update current commit pointer
}

bool minigit::checkout(doublyNode* currNode){                           //Checkout previous commits function

    cout << "Choose a numerical option:" << endl;           
    cout << "1. minigit checkout HEAD //checkout the most recent commit" << endl;   
    cout << "2. minigit checkout - //checkout previous commit" << endl;
    cout << "3. Head~i //checkout i steps before HEAD" << endl;
    cout << "4. Choose a branch and commit number" << endl;

    string choice;
    getline(cin,choice);                                                //Get user input
    int x = stoi(choice);

    string branchInput;
    Branch *foundBranch=NULL;
    doublyNode* foundNode = NULL;
    stackNode* sNode;
    string commitNumber;
    string steps;
    int j;

    switch(x){
        case 1:
            foundNode = stackh->node;                                   //returns commit from top of commit stack
        break;
        case 2:
            foundNode = previousCommit;                                 //returns previous commit set in commit()
        break;
        case 3:
            cout << "Enter number of steps to go back" << endl; 
            getline(cin,steps);
            j=stoi(steps);
            sNode = stackh;
            for(int i=0;i<j;i++){                                       //traverses stack to move back i steps
                sNode=sNode->next;
            }
            foundNode=sNode->node;
        break;
        case 4:
            cout << "Which branch do you want to checkout: ";           //Traverses branch vector to list all branches
            for(int i=0;i<branchVector.size();i++){ 
                cout << branchVector[i]->name << " ";
            }
            cout << endl;       
            getline(cin,branchInput);                                   //get user input of branch choice
            for(int i=0;i<branchVector.size();i++){
                if(branchInput==branchVector[i]->name){
                    foundBranch = branchVector[i];
                    break;
                }
            }
            if(foundBranch==NULL){
                cout << "Branch does not exist." << endl;
                return false;
            } else {
                cout << "Enter a commit number: ";                      //list commit numbers of branch
                for(int i=0;i<foundBranch->commits.size();i++){
                    if(foundBranch->commits[i]!=currentDirectory){
                        cout << foundBranch->commits[i]->commitNumber << " ";
                    }
                }
                cout << ": ";
                getline(cin,commitNumber);                              //get user input commit number
                for(int i=0;i<foundBranch->commits.size();i++){
                    if(foundBranch->commits[i]->commitNumber==stoi(commitNumber)){
                        foundNode = foundBranch->commits[i];
                        break;
                    }
                }
                if(foundNode==NULL){
                    cout<<"Commit number does not exist."<<endl;
                    return false;
                }

            }
        break;
        default:
            cout << "Invalid option" << endl;
            return false;
        break;
    }

    singlyNode* node;                                   
    string path = fs::current_path();
    for(const auto &entry : fs::directory_iterator(path)){              //For every file in the current folder
        node=foundNode->head;                                           //traverse single LL in the found double LL node
        while(node!=NULL){
            if(entry.path()==path+"/"+node->fileName){                  //if the current file's path == the current path / current single LL node file name
                copy(".minigit/"+foundNode->branch->name+"/"+node->fileVersion,node->fileName);    
                previousCommit=currNode;                                //copy file from .minigit sub folder to corresponding file name in current folder
                break;
            }
            node=node->next;
        }
    }                                                                   //if user input commit number is not the most recent commit
    if(foundNode->commitNumber!=foundNode->branch->commits[foundNode->branch->commits.size()-2]->commitNumber){        
        return true;                                                    //function returns false so that the while loop continues that
    } else {                                                            //only allows user to checkout or quit
        return false;
    }
    
}

doublyNode* minigit::newBranch(doublyNode* currNode,string newBranchName){  //Add new branch function takes in user input branch name

    string path = fs::current_path();       
    path=path+"/.minigit/"+newBranchName;                       
    if(fs::create_directory(path)){                                         //creates new branch sub folder

        doublyNode *newRepo = new doublyNode;                               //creates new commit node
        newRepo->head=NULL;                                                 //single LL is empty
        newRepo->commitNumber=0;                                            //commit number = 0

        Branch* newB = new Branch;                                          //new branch
        newB->branchHead=newRepo;                                           //branch head is new commit node
        newB->name=newBranchName;                                           //branch name
        newB->commits.push_back(newRepo);                                   //adds new node to new branch commit vector
        branchVector.push_back(newB);                                       //adds new branch to branch vector
        newRepo->branch=newB;                                               //sets new commit node branch to new branch
        cout << newB->name << " branch created successfully" << endl;
        return newRepo;                                                     //returns newly created commit node
    } else {
        return deserialize();                                               //deserializes file if master branch already exists
    }   
}

doublyNode* minigit::switchBranch(doublyNode* currNode){                    //swtich branch function
    cout << "Which branch do you want to switch to: ";
    for(int i=0;i<branchVector.size();i++){                                 //list names of all branches
        cout << branchVector[i]->name << " ";
    }
    cout << endl;

    string input;
    getline(cin,input);                                                     //get user input branch name
    Branch *foundBranch=NULL;
    

    for(int i=0;i<branchVector.size();i++){
        if(input==branchVector[i]->name){                                   //check if name exists in branches vector
            foundBranch = branchVector[i];
        }
    }

    if(foundBranch==NULL){
        cout << "Branch does not exist." << endl;
        return currNode;
    } else {
        return foundBranch->branchHead;                                     //returns found branch -> branchHead so current commit pointer can be set
    }

}

//stack implementation
StackLL::StackLL()
{
    stackHead = nullptr;
}

StackLL::~StackLL()
{
    while(!isEmpty())
        pop();
}

bool StackLL::isEmpty()
{
    return (stackHead == nullptr);
}

stackNode* StackLL::push(doublyNode* n)
{
    stackNode* nn = new stackNode;
    nn->node=n;
    nn->next = nullptr;
    nn->next = stackHead;
    stackHead = nn;
    return nn;
}

void StackLL::pop()
{
    if(!isEmpty())
    {
        stackNode* temp = stackHead;
        stackHead = stackHead->next;
        delete temp;
    }
    else
    {
        cout<<"empty stack. can not pop"<<endl;
    }
}

int StackLL::peek()
{
    if(!isEmpty())
        return stackHead->node->commitNumber;
    else{
        cout<<"empty stack. can not peek"<<endl;
        return -1;
    }
  return 0;
}
//

void minigit::diff(){                                                   //Check if file has been changed since last commit
    string input;
    cout << "Enter a file name: ";                                      //get user input file name
    getline(cin,input);

    string mainPath = fs::current_path();
    string mainFile = mainPath + "/" + input;
    bool found=false;
    for(const auto &entry : fs::directory_iterator(mainPath)){          //check if file exists in current folder
        if(entry.path()==mainFile){ 
            found=true;
        }
    }
    if(found==false){
        cout << "File does not exist in the current directory." << endl;
        return;
    }

    singlyNode* node = previousCommit->head;                            //find if file exists in most recent commit
    singlyNode* foundFile = NULL;
    while(node!=NULL){
        if(node->fileName==input){
            foundFile = node;
        }
        node=node->next;
    }
    if(!foundFile){
        cout << "File does not exist in most recent commit" << endl;
        return;
    } else {
        string commitFile = mainPath + "/.minigit/" + previousCommit->branch->name + "/" + foundFile->fileVersion;
        bool different=false;
        ifstream f1name(mainFile), f2name(commitFile);
        char s1[256], s2[256];
        int x = 0;
        
        while(!f1name.eof())                                        //ifstreams both files line by line
        {
            f1name.getline(s1,256);
            f2name.getline(s2,256);
            x++;
            if(strcmp(s1,s2)!=0)                                    //if the lines are not equal
            {
                cout << "The files are different on line " << x << ":" << endl;
                cout << s1 << endl;                                 //outputs the line
                different=true; 
                break;
            }
        }
        if(!different){
            cout << "The file " << input << " has not been changed." << endl;
            return;                                                 //returns if the files are the same
        }
        f1name.close();
        f2name.close();
    }
}

void minigit::status(){                                             //Lists all files that have been changed since last commit
    singlyNode* node;
    string path = fs::current_path();
    for(const auto &entry : fs::directory_iterator(path)){          //for each file in the current folder
        node=previousCommit->head;                                  //traverse the most recent commit single LL
        while(node!=NULL){
            if(entry.path()==path+"/"+node->fileName){              //if the file exists in the current folder and single LL
                string commitFile = path + "/.minigit/" + previousCommit->branch->name + "/" + node->fileVersion;
                ifstream f1name(entry.path()), f2name(commitFile);
                char s1[256], s2[256];
                
                while(!f1name.eof())                                //checks if file has been changed since last commit
                {
                    f1name.getline(s1,256);
                    f2name.getline(s2,256);
                    if(strcmp(s1,s2)!=0)
                    {
                        cout << node->fileName << " was changed" << endl;
                        break;                                      //outputs name if file has been changed
                    }
                }
            }
            node=node->next;
        }
    }
}

void minigit::serialize(){                                          //Serialize data structures
    ofstream outfile(".minigit/minigit.xml");                       //creates file to write out to
    for(int i=0;i<branchVector.size();i++){
        outfile << "<branch dir=\"" << branchVector[i]->name << "\">" << endl;          //<branch dir="branchName">
        for(int j=0;j<branchVector[i]->commits.size()-1;j++){
            outfile << "<commit id = " << branchVector[i]->commits[j]->commitNumber << " >" << endl;    //<commit id = 0 >
            outfile << "<files>" << endl;                                               //<files>
            singlyNode* node = branchVector[i]->commits[j]->head;
            while(node!=NULL){
                outfile << "<file src=\"" << node->fileName << "\">" << " " << node->fileVersion << " " << "</file>" << endl;
                node = node->next;                                                      //<file src="fileName"> fileVersion </file>
            }
            outfile << "</files>" << endl;                                              //</files>
            outfile << "</commit>" << endl;                                             //</commit
        }
        outfile << "</branch>" << endl;                                                 //</branch>
    }
    outfile.close();
}

int split(string line, char separtator, char separator2, string myLine[], int size)     //split function
{                                                                                       //splits string line into array
    int i=0;
    int j=0;
    string temp="";
    if(line=="")
    {
        return 0;
    }
    line = line + separtator;
    int lineSize = line.length();
    for(i=0;i<lineSize;i++)
    {
        if(line[i]!=separtator&&line[i]!=separator2)
        {
            temp+=line[i];
        }
        else
        {
            myLine[j]=temp;
            j++;
            temp="";
        }
    }
    return j;
}

doublyNode* minigit::deserialize(){                                         //deserialize .xml file
    ifstream infile(".minigit/minigit.xml");
    string line;
    int size=6;
    string arr[size];
    char s1=' ';
    char s2='"';

    Branch* currBr = NULL;
    doublyNode* currDn = NULL;

    while(getline(infile,line)){                                            //while file is being read in
        for(int i=0;i<size;i++){
            arr[i] = "";
        }
        split(line,s1,s2,arr,size);                                         //split line into array

        if(arr[0]=="<branch"){                                              //create new branch
            //arr[2] = branch name

            Branch* newB = new Branch;
            newB->name=arr[2];
            branchVector.push_back(newB);
            currBr = newB;
            currentBranch = currBr;



        } else if(arr[0]=="<commit"){                                       //create new commit
            //arr[3] = commit id

            doublyNode *newRepo = new doublyNode;
            // newRepo->head=NULL;
            newRepo->commitNumber=stoi(arr[3]);
            if(currBr->commits.size()==0){
                currBr->branchHead=newRepo;
            }
            newRepo->branch = currBr;
            currBr->commits.push_back(newRepo);
            if(currDn!=NULL){
                currDn->branches.push_back(newRepo);
                newRepo->branches.push_back(currDn);
            }
            currDn = newRepo;
            currDn->head=NULL;

            commits.push_back(newRepo);
            currentDirectory = newRepo;

        } else if(arr[0]=="<file"){                                         //add file to commit
            //arr[2] = file name
            //arr[4] = file version

            singlyNode *head;
            head=currDn->head;
            while(head!=NULL){
                head=head->next;
            }

            if(head==NULL){
                singlyNode* node= new singlyNode;
                node->fileName=arr[2];
                node->fileVersion=arr[4];
                node->next=currDn->head;
                currDn->head=node;
            } else {
                singlyNode* node= new singlyNode;
                node->fileName=arr[2];
                node->fileVersion=arr[4];
                node->next = head->next;
                head->next = node;
            }
        }
    }
    previousCommit=currDn;

    doublyNode* dn = branchVector[0]->branchHead;
    for(int i=0;branchVector[0]->commits[i]->branches[0]==NULL;i++){
        dn = branchVector[0]->commits[i];
    }

    doublyNode *newDn = new doublyNode;                                     //create new commit node
    dn->branches.push_back(newDn);
    newDn->branches.push_back(dn);
    newDn->head=NULL;
    newDn->commitNumber=dn->commitNumber+1;
    newDn->branch=branchVector[0];
    branchVector[0]->commits.push_back(newDn);

    return newDn;
}