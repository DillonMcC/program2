//COSC 2030
//Section 10
//Dillon McCardell
//Program 2

#include "timer.h"
#include<string>
#include<vector>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<istream>
#include<sstream>
#include<cctype>

using std::string;
using namespace std;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

//takes a string and returns a cleaned word
string cleanWord(string &word);
// Function to check the given word exist or not
bool iterativeSearch(struct Node* root, string word, int& checker);

class Node
{
    public:
    string key;
    Node *left;
    Node *right;
    int height;
};


// Return max of two integers
int max(int a, int b);


// Return height of the tree
int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum
// of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Helper function that allocates a
   new node with the given key and
   NULL left and right pointers. */
Node* newNode(string key)
{
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // new node is initially
                      // added at leaf
    return(node);
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;
  
    // Perform rotation
    x->right = y;
    y->left = T2;
  
    // Update heights
    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;
  
    // Return new root
    return x;
}


// left rotate subtree rooted with x
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;
  
    // Perform rotation
    y->left = x;
    x->right = T2;
  
    // Update heights
    x->height = max(height(x->left),
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;
  
    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

/* Recursive function to insert a key
 in the subtree rooted with node and
 returns the new root of the subtree. */
Node* insert(Node* node, string key)
{
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));
  
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Equal keys are not allowed in BST
        return node;
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                        height(node->right));
  
    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);
  
    // If this node becomes unbalanced, then
    // there are 4 cases
  
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
  
    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
  
    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
  
    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
  
    // return the (unchanged) node pointer 
    return node;
}

void deleteTree(Node* node)
{
    if (node == NULL) return;
  
    // first delete both subtrees
    deleteTree(node->left);
    deleteTree(node->right);
      
    // then delete the node
    free(node);
}

int main()
{
    //Declare Node root
    Node *root = NULL;
    
    int checker = 0;
    long numCompare1 = 0;
    long numCompare2 = 0;
    int dictionarySize = 0;
    int numFound = 0;
    int numNotFound = 0;
    long average1 = 0;
    long average2 = 0;
    int jumped = 0;
    vector <string> misspelledVector;
    
    Timer min;
    string fileName;
    string word;
    string line;
    ifstream dictionary;
    ifstream book;
    string bookName;
    ofstream misspelled;
    
    // Creates output file "misspelled.txt"
    ofstream outfile ("misspelled.txt");
    // Clears output file of existing data before data is written to it later in the program
    misspelled.open("misspelled.txt", std::ofstream::out | std::ofstream::trunc);
    misspelled.close();
    
    do
    {
        cout << "Enter a Dictionary: ";
        cin >> fileName;
        cout << endl;

        dictionary.open(fileName);
        
        if (!dictionary)
        {
            cout << "Cannot be found, try again!\n" << endl;
        }
    } while (!dictionary);

    cout << "Loading Dictionary" << endl;

    getline(dictionary, word);
    
    do
    {
        word = cleanWord(word);
        root = insert(root, word); //Insert each word into the tree
        dictionarySize++;
    } while (getline(dictionary, word));

    dictionary.close();

    cout << "Successfully loaded " << dictionarySize << " words from the Dictionary" << endl;
    
    do{
        cout << "\nEnter a Book: ";
        cin >> bookName;
       
        book.open(bookName);
       
        if (!book)
               {
                   cout << "\n - The book cannot be opened - " << endl;
               }
   }while(!book);
    
    word.clear();

    min.Start(); //Timer started

    cout << "\nTimer started, spellchecking..." << endl;

    while (book >> word)
    {
        word = cleanWord(word); //Pass word through CleanWord Method
        
        if(word.empty())
        {
            //If the word is blank, do nothing
        }
        else if(!isalpha(word[0])) //If word starts with a number
        {
            jumped++; //Skipped word Count
        }
        else if (!iterativeSearch(root, word, checker)) //If word is not found
        {
            numNotFound++;
            misspelledVector.push_back(word); //Add misspelled word to vector
            numCompare2 = checker + numCompare2;
        }
        else //Word was found
        {
            numCompare1 = checker + numCompare1;
            numFound++;
        }
        checker=0; //reset checker
        word.clear(); //reset word
    }

    min.Stop(); //Stop timer
    book.close(); //Close book
    
    //Opens misspelled.txt and prints all of the misspelled words to it
    misspelled.open("misspelled.txt");
    for (int i=0; i<misspelledVector.size(); i++)
    misspelled << misspelledVector[i] << "\n";
    misspelled.close();
    
    cout << "Book Closed" << endl << endl;

    //Calcualte Averages
    average1 = (numCompare1 / numFound);
    average2 = (numCompare2 / numNotFound);

    //Ouput Print Statements
    cout << "Finished" << endl << endl;
    cout << "Dictionary Size: " << dictionarySize << endl;
    cout << "Finished in time: " << setprecision(6) << min.Time() << endl;
    cout << "There are " << numFound << " words found in the dictionary" << endl;
    cout << "\t" << numCompare1 << " compares. Average: " << setprecision(1) << average1 << endl;
    cout << "There are " << numNotFound << " words NOT found in the dictionary" << endl;
    cout << "\t" << numCompare2 << " compares. Average: " << setprecision(1) << average2 << endl;
    cout << "There are " << jumped << " words not checked." << endl;
    
    deleteTree(root); //Clear the tree data
    cout << "\n --Tree Deleted--\n" << endl;
    
    return 0;
}

//takes a string a returns a cleaned word
string cleanWord(string &word)
{
  
   stringstream ss;
   char letter;
    //split word into letters
   for(int i=0; i<word.size(); i++){
       letter = word.at(i);
       //if uppercase, lower
       if(isalnum(letter)){
           letter = tolower(letter);
           ss<<letter; //store in string stream
       }
       //if these characters are included:
       else if(letter=='\''){
           ss<<letter; //store in string stream
       }
   }
   string cleanedWord;
   ss>>cleanedWord;

   return cleanedWord;
}


// Function to check the given key exist or not
bool iterativeSearch(struct Node* root, string word, int& checker)
{
     while(root != NULL){

       checker++; //a single compare
       if( word.compare(root->key) > 0)

       root = root->right;

       else if( word.compare(root->key) < 0)

       root = root->left;

       else

       return true;

       }

       return false;
}

