#include<iostream> 
#include<string>
#include<fstream>

#include "myvector.hpp"
#include "book.h"
#include "tree.h"


using namespace std;
//========================================================================
// Constructor to create an empty node (category/sub-category)
Node::Node(string name) : name(name), bookCount(0), parent(nullptr) {}

//========================================================================
string Node::getCategory(Node* node)
{
    string category = node->name;

    while(node->parent->name != "Library")
    {
        category = node->parent->name + "/" + category; // Prepend parent's name to category
        node = node->parent; // Move to the parent node
    }

    return category;
}

//========================================================================
Node::~Node()
{
    // Delete all children nodes recursively
    for (int i = 0; i < this->children.size(); ++i)
    {
        // Recursively delete child nodes
        delete this->children[i];
    }
}

//========================================================================
//========================================================================

// Constructor for the Tree class
Tree::Tree(string rootName)
{
	root = new Node(rootName);   // Initialize root node with name "root"
	curr_Node = root;    // Set the current node pointer to the root
}
//========================================================================
// Destructor for the Tree class
Tree::~Tree()
{
    if (root != nullptr)
    {
        delete root;   // Delete the root node, which triggers recursive deletion of all nodes in the tree
        root = nullptr; // Set root to nullptr to avoid dangling pointer
    }
}
//========================================================================
// Returns a pointer to the root of the tree
Node* Tree::getRoot()
{
	return root;
}

//========================================================================
bool Tree::isLastChild(Node *ptr)
{
	if(ptr!=root and ptr == ptr->parent->children[ptr->parent->children.size()-1])
		return true;
	return false;
}
//========================================================================
void Tree::insert(Node* node,string name)
{
	// Check if a node with the same name already exists as a child
    for(int i=0; i<node->children.size(); i++) 
    {
	    if(node->children[i]->name == name) 
	    {
		    // Node with the same name already exists, handle the error
		    throw runtime_error("A Book with the same name already exists.");
		}
	}

	Node* new_node = new Node(name);  // Create a new node with the given name
    new_node->parent = node;   // Set the parent of the new node
    node->children.push_back(new_node);  // Add the new node to the children of the parent node
}
//========================================================================
// Remove a child node with the specified name from the given parent node
// Throws a runtime error if the child node is not found
void Tree::remove(Node* node, string child_name)
{
    for (int i = 0; i < node->children.size(); i++)
    {
        if (node->children[i]->name == child_name)
        {
            // Delete the child node and remove it from the parent's children vector
            delete node->children[i];
            node->children.erase(i);
            cout << child_name << " has been deleted successfully." << endl;
            return;
        }
    }

    // Print error if child node not found
    throw runtime_error("Category not found");
}
//========================================================================
// Check if the given node is the root of the tree
bool Tree::isRoot(Node* node)
{
    return (node == root);
}
//========================================================================	
// Get the node corresponding to the given path
// Returns nullptr if the node is not found
Node* Tree::getNode(string path)
{
    // Start the search from the root node
    return getNodeHelper(root, path);
}

// Helper function to recursively search for a node with the given path
Node* Tree::getNodeHelper(Node* currentNode, string path)
{
    size_t pos = 0;
    string delimiter = "/";
    string token;

    // Loop through each token in the path
    if ((pos = path.find(delimiter)) != string::npos) 
    {
        token = path.substr(0, pos); // Extract the next token
        path.erase(0, pos + delimiter.length()); // Erase the token and the delimiter from the path
    }
    else 
    {
        // If no delimiter found, token is the remaining path
        token = path;
    }

    // Check if the current node matches the token
    if (currentNode->name == token || path.empty()) 
    {
        // If it matches and there are no more tokens, return the current node
        return currentNode;
    }
       
    // Otherwise, continue searching in the children of the current node
    for (int i = 0; i < currentNode->children.size(); ++i) 
    {
        Node* child = currentNode->children[i];
        Node* node = getNodeHelper(child, path);
        if (node != nullptr) 
        {
            return node; // Return the result if found in the subtree
        }
    }

    // If no matching node found in the subtree, return nullptr
    return nullptr;
}

//========================================================================
// Create a node corresponding to the given path and return it
// If the path already exists, return the existing node
Node* Tree::createNode(string path)
{
    // If the root node doesn't exist, create it
    if (root == nullptr) {
        root = new Node("Library");
    }

    Node* currentNode = root;
    size_t pos = 0;
    string delimiter = "/";

    // Loop through each token in the path
    while ((pos = path.find(delimiter)) != string::npos) 
    {
        // Extract the next token from the path
        string token = path.substr(0, pos);
        // Erase the token and the delimiter from the path
        path.erase(0, pos + delimiter.length());

        bool found = false;

        // Check if the current node has a child with the current token
        for (int i = 0; i < currentNode->children.size(); ++i) 
        {
            Node* child = currentNode->children[i];
            if (child->name == token) 
            {
                currentNode = child; // Move to the child node
                found = true;
                break;
            }
        }

        // If the current node doesn't have a child with the current token, create one
        if (!found) 
        {
            // Create a new node with the token
            Node* newNode = new Node(token);
            // Set the parent of the new node
            newNode->parent = currentNode;
            // Add the new node as a child of the current node
            currentNode->children.push_back(newNode);
            // Move to the new node
            currentNode = newNode;
        }
    }

    // If the path was empty, return the root node
    if (path.empty()) {
        return root;
    }

    // If there's a remaining token, create a node for it
    // Create a node for the last token
    Node* newNode = new Node(path);
    // Set the parent of the new node
    newNode->parent = currentNode;
    // Add the new node as a child of the current node
    currentNode->children.push_back(newNode);

    // Return the newly created node
    return newNode;
}
//========================================================================
// Get the child node with the specified name from the given parent node
Node* Tree::getChild(Node* ptr, string childname)
{
    // Loop through each child of the parent node
    for(int i = 0; i < ptr->children.size(); i++)
    {
        // Check if the name of the current child matches the specified name
        if (ptr->children[i]->name == childname)
        {
            // Return the matching child node
            return ptr->children[i];
        }
    }

    // If no matching child node is found, return nullptr
    return nullptr;
}


//========================================================================
// Update the book count for a node and its ancestors recursively
void Tree::updateBookCount(Node* ptr, int offset)
{
    // Increment or decrement the book count by the offset
    ptr->bookCount += offset;
    
    // Base case: If ptr's parent is nullptr, stop recursion
    if (ptr->parent != nullptr) 
    {
        // Recursive call with parent node and offset
        updateBookCount(ptr->parent, offset);
    }
}
//========================================================================
// Find a book with the given title in the subtree rooted at the specified node
Book* Tree::findBook(Node* node, string bookTitle)
{
    // Search for the book in the current node
    for (int i = 0; i < node->books.size(); i++)
    {
        if (node->books[i]->title == bookTitle)
        {
            return node->books[i];  // Book found, return it
        }
    }

    // Book not found in this node or its children
    return nullptr;
}



//========================================================================
// Remove a book with the given title from the node's books vector
void Tree::removeBook(Node* node, string bookTitle)
{
    for (int i = 0; i < node->books.size(); i++)
    {
        if (node->books[i]->title == bookTitle)
        {
            // Delete the book object, update book count, and remove it from the books vector
            updateBookCount(node, 1);
            delete node->books[i];
            node->books.erase(i);
            cout << bookTitle << " has been successfully removed." << endl;
            return;
        }
    }

    // Throw a runtime_error if the book is not found
    throw runtime_error("Book not found");
}
//========================================================================
// Print all books in the subtree rooted at the given node
void Tree::printAll(Node* node) 
{
    if (node == nullptr) return; // Return if the node is nullptr

    // Print books in the current node
    for (int i = 0; i < node->books.size(); i++)
    {
        node->books[i]->display2();
    }

    // Recursively print books in the children nodes
    for (int i = 0; i < node->children.size(); i++) 
    {
        printAll(node->children[i]);
    }
}
//========================================================================


// Print the tree structure
void Tree::print()
{
    print_helper("", "", root); // Start printing from the root node
}

// Helper function to print the tree structure recursively
void Tree::print_helper(string padding, string pointer, Node *node)
{
    if (node != nullptr) 
    {
        // Print the node name and book count with appropriate padding and pointers
        cout << padding << pointer << node->name << " (" << node->bookCount << ")" << endl;

        // Adjust padding for the next level of nodes
        if (node != root) 
        {
            padding += (isLastChild(node)) ? "   " : "│  ";
        }

        // Recursively print children nodes
        for (int i = 0; i < node->children.size(); i++) 
        {
            string marker = isLastChild(node->children[i]) ? "└──" : "├──";
            print_helper(padding, marker, node->children[i]);
        }
    }
}

// Export tree data to a file
int Tree::exportData(Node *node, ofstream& file)
{
    if (!file.is_open()) // Check if the file is open
        return -1; // Return -1 if the file is not open

    // Write node name and book count to the file
    file << node->name << " (" << node->bookCount << ")\n";

    // Recursively export data for children nodes
    for (int i = 0; i < node->children.size(); ++i) 
    {
        Node* child = node->children[i];
        exportData(child, file);
    }

    return 0; // Return 0 upon successful export
}

// Check if the tree is empty
bool Tree::isEmpty()
{
    return (root == nullptr); // Return true if the root node is nullptr, indicating an empty tree
}
