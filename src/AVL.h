#pragma once
#include <algorithm>
#include <iostream>
#include <string>

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
        this->left = nullptr;
        this->right = nullptr;
        this->height=0; //height of a leaf node is 0
      }

      //Check whether this node is a leaf node or not
      bool isLeaf() const
      { 
        return !left && !right; 
      }
};

template <class T> 
class AVLTree
{
  public:
    AVLTree() //default constructor
    {
      this->root = nullptr;
    }

    ~AVLTree()
    {
      clear();
    }

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;

    void clear()
    {
      destroyTree(root);
    }

    void destroyTree(AVLNode<T>* &node) // destroys an AVL tree (all nodes) using recursion in psot order
    {
      if(node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
      }
      node = nullptr;
    }

    bool isEmpty() const { //function to check if the tree is empty
      return root == nullptr; //checks if tree is empty
    }

    AVLNode<T>* rootNode()
    {
      return root;
    }

    const AVLNode<T>* rootNode() const
    {
      return root;
    }

    void inOrderTraverse(AVLNode<T>* node) { //prints the tree using InOrderTraversal
      if(node) {
        inOrderTraverse(node->left);
        std::cout << node->data << std::endl;
        inOrderTraverse(node->right);
      }
      if (node == root)
        std::cout << std::endl;
    }

    void preOrderTraverse(AVLNode<T>* node) { //prints the tree using InOrderTraversal
      if(node) {
        std::cout << node->data << " ";
        preOrderTraverse(node->left);
        preOrderTraverse(node->right);
      }
      if (node == root) 
        std::cout << std::endl;
    }

    void postOrderTraverse(AVLNode<T>* node) //pass address of root node to print
    {
      if(node) {
        postOrderTraverse(node->left);
        postOrderTraverse(node->right);
        std::cout << node->data << " ";
      }
      if (node == root) 
        std::cout << std::endl;
    }

    void findNodeAndParent(AVLNode<T>* node, T value, AVLNode<T>*& foundNode, AVLNode<T>*& parentNode) const
    {
      foundNode = nullptr;
      parentNode = nullptr;

      AVLNode<T>* current = node;
      AVLNode<T>* parent = nullptr;
      while(current != nullptr)
      {
        if(current->data == value)
        {
          foundNode = current;
          parentNode = parent;
          return;
        }

        parent = current;
        if(value < current->data)
          current = current->left;
        else
          current = current->right;
      }
    }

    AVLNode<T>* find(T value)
    {
      AVLNode<T>* foundNode = nullptr;
      AVLNode<T>* parentNode = nullptr;
      findNodeAndParent(this->root, value, foundNode, parentNode);
      return foundNode;
    }

    const AVLNode<T>* find(T value) const
    {
      AVLNode<T>* foundNode = nullptr;
      AVLNode<T>* parentNode = nullptr;
      findNodeAndParent(this->root, value, foundNode, parentNode);
      return foundNode;
    }

    bool search(T value)
    {
      return find(value) != nullptr;
    }

    AVLNode<T>* balance(AVLNode<T> *node) //balances a node if it is unbalanced
    {
      node->height = std::max(height(node->left), height(node->right))+1; //updates the height of the node;
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
    AVLNode<T>* insert(AVLNode<T> *node, T key, AVLNode<T>*& insertedNode)
    {
      if(node == nullptr) { //when logical position is found, insert the new node with the given data and return it to parent
        AVLNode<T> *newNode = new AVLNode<T>(key);
        insertedNode = newNode;
        return newNode;
      }
      if( key < node->data ) //if key is smaller, explore left branch
        node->left = insert(node->left, key, insertedNode);
      else if( key > node->data ) //if key is geater, explore right branch
        node->right = insert(node->right, key, insertedNode);
      else {
        // Duplicate policy: keep one canonical node per key.
        insertedNode = node;
        return node;
      }

      node = balance(node); //balances the node
      return node; //returns the node
    }

    AVLNode<T>* insertAndGet(T key)
    {
      AVLNode<T>* insertedNode = nullptr;
      root = insert(root, key, insertedNode);
      return insertedNode;
    }

    void insert(T key) //insert function with only 1 paramter
    {
      (void)insertAndGet(key);
    } 

    //removes a node in the avl tree using recursion
    //it first finds the node in the tree
    //if node is not a leaf node, its successor is determined and unlinked
    //the left and right child of the deleted node are now assigned to the successor node
    //then the successor is returned to the parent function and its address is replaced
    //height and balancing are done as the function calls are popped
    AVLNode<T>* remove(AVLNode<T>* node, AVLNode<T>* pnode, T key)
    {
      if(node == nullptr) {
        return nullptr;
      }

      if( key < node->data )
        node->left = remove(node->left, node, key);
      else if( key > node->data )
        node->right = remove(node->right, node, key);
      else {
        AVLNode<T> *self = node; //declare 2 new pointers for naming and readability purposes only
        AVLNode<T> *parentNode = pnode;
        if( self->isLeaf() ) { // See if it's a leaf node
          if( self==root ) // If it is a leaf and there is no parent, then the tree will be emptied
            root = nullptr;
          // If it's a leaf node, simply remove it
          delete self; //delete the node
          node = nullptr; //and set it to null
        }
        else {
          AVLNode<T> *successor = maximumNode(self->left);
          if(!successor) // See if there's a left-most node
            successor = minimumNode(self->right);  // Get the smallest node on the right (because the left doesn't exist)
          unlinkSuccessor(successor); // Disconnect the replacement node's branch
          
          AVLNode<T> *leftChild = self->left; // Get the current node's left and right branches
          AVLNode<T> *rightChild = self->right;
          delete self; //delete the node as it is not needed anymore
          node = nullptr; //set it to null

          successor->left = leftChild; //as successor will replace that node, update the branches
          successor->right = rightChild;
          if( !parentNode ) //check if we deleted root node, then update root
            root = successor;
          successor = balance(successor);
          return successor;
        }
      }
      if(node != nullptr) //balance the node if it is not null
        node = balance(node);
  
      return node; //return the node to its invoking function
    }

    // Unlinks a node from its parent and children
    void unlinkSuccessor(AVLNode<T>* node)
    {
      AVLNode<T>* foundNode = nullptr;
      AVLNode<T>* parentNode = nullptr;
      findNodeAndParent(root, node->data, foundNode, parentNode);
      if(parentNode == nullptr || foundNode == nullptr)
        return;

      //unlink the node from its parent
      //2 cases whethe its a left child or a right child
      if ( parentNode->left == foundNode ) {
        parentNode->left = nullptr;
        if(foundNode->right)
          parentNode->left = foundNode->right;
      }
      else if ( parentNode->right == foundNode ) {
        parentNode->right = nullptr;
        if( foundNode->left )
          parentNode->right = foundNode->left;
      }
      //update the height of the parent as its child was unlinked
      parentNode->height = std::max(height(parentNode->left), height(parentNode->right))+1;
    }

    int height(const AVLNode<T> *node) const //returns height of a node
    {
      if(node == nullptr) //returns -1 for a non existent node
        return -1;
      else
        return node->height; //simply return the node height
    }

    //finds the node with the minimum node data
    //recursively finds the left most node
    AVLNode<T> *minimumNode(AVLNode<T>* node)
    {
      if(node == nullptr) //base case
        return nullptr;
      else if(node->left == nullptr) //2nd base case, vlaue found
        return node;
      else
        return minimumNode(node->left);
    }

    //finds the node with the maximum node data
    //recursively finds the right most node
    AVLNode<T> *maximumNode(AVLNode<T>* node)
    {
      if(node == nullptr) //base case
        return nullptr;
      else if(node->right == nullptr) //2nd base case, value found
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
      x->height = std::max(height(x->left), height(x->right)) + 1;
      y->height = std::max(height(y->left), height(y->right)) + 1;
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
      x->height = std::max(height(x->left), height(x->right)) + 1;
      y->height = std::max(height(y->left), height(y->right)) + 1;
      //Return new root
      return y;
    }

    int getBalance(const AVLNode<T> *node) const // Get Balance factor of a node
    {
      if(node == nullptr) //return 0 if node is invalid
        return 0;
      return ( height(node->left) - height(node->right) ); //else call the getter function
    }

    void remove(T value) //remove function with only 1 parameter that calls the function frm the root node
    {
      root = remove(root, nullptr, value);
    }

    void printHelper(AVLNode<T> *node, std::string indent, bool last) {
		// print the tree structure on the screen
	   	if (node != nullptr) {
		   std::cout << indent;
		   if (last) {
		      std::cout<<"R----";
		      indent += "     ";
		   } 
       else {
		      std::cout<<"L----";
		      indent += "|    ";
		   }

		   std::cout << node->data << "( H = " << node->height << ")" << std::endl;

		   printHelper(node->left, indent, false);
		   printHelper(node->right, indent, true);
		}
	}

	void prettyPrint() { // print the tree structure on the screen
		printHelper(this->root, "", true);
	}

  private:
    AVLNode<T>* root; //identifies the root node of AVL
};
