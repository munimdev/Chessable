#pragma once
#include <iostream>
using namespace std;

template <class T> 
class AVLNode //definition for template node class for AVL
{
    public:
      T data; //node of data type T
      int height; //store height of a node
      AVLNode* left;
      AVLNode* right;

      AVLNode(T value) //default constructor
      {
        this->data=value; //sets data equal to passed value
        this->left=NULL;
        this->right=NULL;
        this->height=0; //height of a leaf node is 0
      }

      //Check whether this node is a leaf node or not
      bool isLeaf() 
      { 
        return !left && !right; 
      }
};

template <class T> 
class AVLTree
{
  public:
    AVLNode<T>* root; //identifies the root node of AVL
    AVLNode<T>* loc; //loc and ploc help in searching nodes
    AVLNode<T>* ploc;

    AVLTree() //default constructor
    {
      this->root = NULL;
      this->loc = NULL;
      this->ploc = NULL;
    }

    void destroyTree(AVLNode<T>* &node) // destroys an AVL tree (all nodes) using recursion in psot order
    {
      if(node != NULL) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
      }
      node = NULL;
    }

    bool isEmpty() { //function to check if the tree is empty
      return root == NULL; //checks if tree is empty
    }

    void inOrderTraverse(AVLNode<T>* node) { //prints the tree using InOrderTraversal
      if(node) {
        inOrderTraverse(node->left);
        cout << node->data << endl;
        inOrderTraverse(node->right);
      }
      if (node == root)
        cout << endl;
    }

    void preOrderTraverse(AVLNode<T>* node) { //prints the tree using InOrderTraversal
      if(node) {
        cout << node->data << " ";
        preOrderTraverse(node->left);
        preOrderTraverse(node->right);
      }
      if (node == root) 
        cout << endl;
    }

    void postOrderTraverse(AVLNode<T>* node) //pass address of root node to print
    {
      if(node) {
        postOrderTraverse(node->left);
        postOrderTraverse(node->right);
        cout << node->data << " ";
      }
      if (node == root) 
        cout << endl;
    }

    void search(AVLNode<T>* node, T value) //recursively searches a value in the avl tree. if found, loc stores its address and ploc stores its parent's address
    {
      if(node == NULL) { //if value was not found, simply return
        loc = NULL;
        return;
      }
      else if(node->data == value) {
        loc = node;
        if(node == root)
          ploc = NULL;
        return;
      }
      else {
        if(value < node->data) { //search left branch
          loc = node->left;
          ploc = node;
          search(node->left, value);
        }
        else if(value > node->data) { //search right branch
          loc = node->right;
          ploc = node;
          search(node->right, value);
        }
      }
    }

    bool search(T value)
    {
      search(this->root, value);
      if(loc)
        return true;
      else
        return false;
    }

    AVLNode<T>* balance(AVLNode<T> *node) //balances a node if it is unbalanced
    {
      node->height = max(height(node->left), height(node->right))+1; //updates the height of the node;
      int balanceFactor = getBalance(node); //save the balance factor of the node. if it is >1 or <-1, we need to perform balancing operations

      AVLNode<T> *temp;
      //If this node is unbalanced then handle 4 cases
      if( balanceFactor>1 ) //left branch is heavier
      {
        //Left Left case
        if(getBalance(node->left) > 0 )
          node = rotateRight(node);
        else { //left right case 
          temp = node->left;
          node->left = rotateLeft(temp);
          node = rotateRight(node);
        }
      }
      else if(balanceFactor<-1) { //right branch is heavier
        //right left case
        if(getBalance(node->right) > 0 ) {
          temp = node->right;
          node->right = rotateRight(temp);
          node = rotateLeft(node);
        }
        else //right right case
          node = rotateLeft(node);
      }
      return node;
    }

    //inserts a node in the AVL tree with the given key value
    //uses recursion
    AVLNode<T>* insert(AVLNode<T> *node, T key)
    {
      if(node == NULL) { //when logical position is found, insert the new node with the given data and return it to parent
        AVLNode<T> *newNode = new AVLNode<T>(key);
        loc = newNode; //save the address of the newly insrted node in loc
        return newNode;
      }
      if( key < node->data ) //if key is smaller, explore left branch
        node->left = insert(node->left, key);
      else //if key is geater, explore right branch
        node->right = insert(node->right, key);

      node = balance(node); //balances the node
      return node; //returns the node
    }

    void insert(T key) //insert function with only 1 paramter
    {
      root = insert(root, key);
    } 

    //removes a node in the avl tree using recursion
    //it first finds the node in the tree
    //if node is not a leaf node, its successor is determined and unlinked
    //the left and right child of the deleted node are now assigned to the successor node
    //then the successor is returned to the parent function and its address is replaced
    //height and balancing are done as the function calls are popped
    AVLNode<T>* remove(AVLNode<T>* node, AVLNode<T>* pnode, T key)
    {
      if( key < node->data )
        node->left = remove(node->left, node, key);
      else if( key > node->data )
        node->right = remove(node->right, node, key);
      else {
        AVLNode<T> *self = node; //declare 2 new pointers for naming and readability purposes only
        AVLNode<T> *parentNode = pnode;
        if( self->isLeaf() ) { // See if it's a leaf node
          if( self==root ) // If it is a leaf and there is no parent, then the tree will be emptied
            root = NULL;
          // If it's a leaf node, simply remove it
          delete self; //delete the node
          node = NULL; //and set it to null
        }
        else {
          AVLNode<T> *successor = maximumNode(self->left);
          if(!successor) // See if there's a left-most node
            successor = minimumNode(self->right);  // Get the smallest node on the right (because the left doesn't exist)
          unlinkSuccessor(successor); // Disconnect the replacement node's branch
          
          AVLNode<T> *leftChild = self->left; // Get the current node's left and right branches
          AVLNode<T> *rightChild = self->right;
          delete self; //delete the node as it is not needed anymore
          node = NULL; //set it to null

          successor->left = leftChild; //as successor will replace that node, update the branches
          successor->right = rightChild;
          if( !parentNode ) //check if we deleted root node, then update root
            root = successor;
          successor = balance(successor);
          return successor;
        }
      }
      if(node != NULL) //balance the node if it is not null
        node = balance(node);
  
      return node; //return the node to its invoking function
    }

    // Unlinks a node from its parent and children
    void unlinkSuccessor(AVLNode<T>* node)
    {
      search(node->data); //calls the recursive search function to find the node's parent
      if(ploc == NULL)
        return;
      //unlink the node from its parent
      //2 cases whethe its a left child or a right child
      if ( ploc->left == loc ) {
        ploc->left = NULL;
        if(loc->right)
          ploc->left = loc->right;
      }
      else if ( ploc->right == loc ) {
        ploc->right = NULL;
        if( loc->left )
          ploc->right = loc->left;
      }
      //update the height of the parent as its child was unlinked
      ploc->height = max(height(ploc->left), height(ploc->right))+1;
    }

    int height(AVLNode<T> *node) //returns height of a node 
    {
      if(node == NULL) //returns -1 for a non existent node
        return -1;
      else
        return node->height; //simply return the node height
    }

    //finds the node with the minimum node data
    //recursively finds the left most node
    AVLNode<T> *minimumNode(AVLNode<T>* node)
    {
      if(node == NULL) //base case
        return NULL;
      else if(node->left == NULL) //2nd base case, vlaue found
        return node;
      else
        return minimumNode(node->left);
    }

    //finds the node with the maximum node data
    //recursively finds the right most node
    AVLNode<T> *maximumNode(AVLNode<T>* node)
    {
      if(node == NULL) //base case
        return NULL;
      else if(node->right == NULL) //2nd base case, value found
        return node;
      else
        return maximumNode(node->right);
    }

    //rotates a subtree right that is rooted at x
    AVLNode<T>* rotateRight(AVLNode<T>* x)
    {
      AVLNode<T> *y = x->left;
      AVLNode<T> *temp = y->right;
      //perform rotation
      y->right = x;
      x->left = temp;
      // Update heights
      x->height = max(height(x->left), height(x->right)) + 1;
      y->height = max(height(y->left), height(y->right)) + 1;
      //Return new root
      return y;
    }

    //rotates a subtree left that is rooted at x
    AVLNode<T>* rotateLeft(AVLNode<T>* x)
    {
      AVLNode<T> *y = x->right;
      AVLNode<T> *temp = y->left;
      //perform rotation
      y->left = x;
      x->right = temp;
      // Update heights
      x->height = max(height(x->left), height(x->right)) + 1;
      y->height = max(height(y->left), height(y->right)) + 1;
      //Return new root
      return y;
    }

    int getBalance(AVLNode<T> *node) // Get Balance factor of a node
    {
      if(node == NULL) //return 0 if node is invalid
        return 0;
      return ( height(node->left) - height(node->right) ); //else call the getter function
    }

    void remove(T value) //remove function with only 1 parameter that calls the function frm the root node
    {
      root = remove(root, NULL, value);
    }

    void printHelper(AVLNode<T> *root, string indent, bool last) {
		// print the tree structure on the screen
	   	if (root != nullptr) {
		   cout << indent;
		   if (last) {
		      cout<<"R----";
		      indent += "     ";
		   } 
       else {
		      cout<<"L----";
		      indent += "|    ";
		   }

		   cout << root->data << "( H = " << root->height << ")" << endl;

		   printHelper(root->left, indent, false);
		   printHelper(root->right, indent, true);
		}
	}

	void prettyPrint() { // print the tree structure on the screen
		printHelper(this->root, "", true);
	}
};
