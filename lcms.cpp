 #include<iostream>
#include<string>
#include<fstream>

#include "lcms.h"

using namespace std;

// Constructor to initialize LCMS with a name
LCMS::LCMS(string name) {
    libTree = new Tree(name);
}

LCMS::~LCMS() 
{
    if (libTree != nullptr) {
        delete libTree;
        cout << "Tree has been deleted successfully." << endl;
    }
}

int LCMS::import(string path)
{
    ifstream read_books(path);

    if (!read_books.is_open()) 
    {
        cout << "Unable to open file for reading." << endl;
        return -1;
    }

    // Skip the first line (header line)
    string header;
    getline(read_books, header);

    string book_info;
    while (getline(read_books, book_info)) 
    {
        stringstream ss(book_info);
        string item;

        string title, authors, isbn, publication_year, category, total_copies, available_copies;

        // Extracting title (which may contain commas)
        getline(ss, title, ',');  // Assuming titles are enclosed within double quotes
        if (title.front() == '"') 
        {
            // If title starts with a quote, it means it contains commas
            // Keep extracting until the ending quote is found
            while (getline(ss, item, ',')) 
            {
                title += ',' + item;
                if (item.back() == '"') 
                {
                    // Remove surrounding quotes from the title
                    title.erase(0, 1);
                    title.pop_back();
                    break;
                }
            }
        }

        // Extracting authors (which may contain commas and multiple names)
        getline(ss, authors, ',');
        if (authors.front() == '"') 
        {
            // If authors start with a quote, it means they contain commas
            // Keep extracting until the ending quote is found
            while (getline(ss, item, ',')) 
            {
                authors += ',' + item;
                if (item.back() == '"') 
                {
                    // Remove surrounding quotes from the authors
                    authors.erase(0, 1);
                    authors.pop_back();
                    break;
                }
            }
        }

        // Extracting other fields
        getline(ss, isbn, ',');
        getline(ss, publication_year, ',');
        getline(ss, category, ',');
        getline(ss, total_copies, ',');
        getline(ss, available_copies, ',');

        // Convert string arguments to integers
        int publication_year_int = (publication_year.empty()) ? 0 : stoi(publication_year);
        int total_copies_int = (total_copies.empty()) ? 0 : stoi(total_copies);
        int available_copies_int = (available_copies.empty()) ? 0 : stoi(available_copies);

        // Create the new book with converted integer arguments
        Book* newBook = new Book(title, authors, isbn, publication_year_int, total_copies_int, available_copies_int);

        // Add the book to the appropriate category node
        Node* node = libTree->createNode(category);
        node->books.push_back(newBook);
        libTree->updateBookCount(node, 1);
    }

    read_books.close();
    cout << libTree->getRoot()->bookCount <<" records have been imported successfully" << endl;
    return 0;
}


void LCMS::exportData(string path)
{
    ofstream write_books(path);

    if (!write_books.is_open()) {
        cout << "Unable to open file for writing" << endl;
        return;
    }

    // Start exporting from the root node
    Node* rootNode = libTree->getRoot();

    // Recursively export data starting from the root node
    exportNode(rootNode, write_books);

    write_books.close();
    cout << libTree->getRoot()->bookCount << " records have been successfully exported to " << path << endl;
}

void LCMS::exportNode(Node* node, ofstream& outputFile)
{
    // Export books stored in the current node
    for (int i = 0; i < node->books.size(); ++i) 
    {
        outputFile << '"' << node->books[i]->title << "\",\"" << node->books[i]->author << "\",\""
                    << node->books[i]->isbn << "\"," << node->books[i]->publication_year << ","
                    << node->getCategory(node) << "," << node->books[i]->total_copies << ","
                    << node->books[i]->available_copies << endl;
    }

    // Recursively export data for child nodes
    for (int i = 0; i < node->children.size(); ++i) 
    {
        exportNode(node->children[i], outputFile);
    }
}

// Find all books in a given category
void LCMS::findAll(string category)
{
    // Get the node corresponding to the given category
    Node* node = libTree->getNode(category);
    if (node != nullptr) 
    {
        // Print all books in the category and display the total count of books
        libTree->printAll(node);
        cout << node->bookCount << " records found" << endl;
    } 
    else 
    {
        // If the category is not found, print a message
        cout << "Category not found" << endl;
    }
}

// Find a book by its title
void LCMS::findBook(string bookTitle) 
{
    // Start searching from the root node
    Node* node = libTree->getRoot();

    // Search for the book recursively
    Book* book = findBookHelper(node, bookTitle);

    // If book is found, display its details; otherwise, print a message
    if (book != nullptr) {
        book->display();
    } 
    else 
    {
        cout << "Book not found" << endl;
    }
}


Book* LCMS::findBookHelper(Node* node, string bookTitle) 
{
    // Search for the book in the current node
    for (int i = 0; i < node->books.size(); i++) 
    {
        if (node->books[i]->title == bookTitle) {
            return node->books[i]; // Book found in the current node
        }
    }

    // Search for the book recursively in children nodes
    for (int i = 0; i < node->children.size(); i++) {
        Book* book = findBookHelper(node->children[i], bookTitle);
        if (book != nullptr) {
            return book; // Book found in a child node
        }
    }

    // Book not found in the current node or its children
    return nullptr;
}


void LCMS::addBook() 
{
    string title, author, isbn, category;
    int publication_year, total_copies, available_copies;

    cout << "Enter Title: ";
    getline(cin, title);

    cout << "Enter author(s): ";
    getline(cin, author);

    cout << "Enter ISBN: ";
    getline(cin, isbn);

    cout << "Enter Publication Year: ";
    cin >> publication_year;
    cin.ignore();

    cout << "Enter the total copies: ";
    cin >> total_copies;
    cin.ignore();

    cout << "Enter the available copies: ";
    cin >> available_copies;
    cin.ignore();

    cout << "Enter Category: ";
    getline(cin,category);
    cout << endl;

    // Check if the category exists, if not, add it
    Node* node = libTree->getNode(category);
    if (node == nullptr) 
    {
        addCategory(category);
        node = libTree->getNode(category);
    }

    // Create the new book and insert it into the appropriate category node
    Book* newBook = new Book(title, author, isbn, publication_year, total_copies, available_copies);

	node->books.push_back(newBook);
    libTree->updateBookCount(node, 1);
}

// Edit the details of a book
void LCMS::editBook(string bookTitle) 
{
    // Start searching from the root node
    Node* curr = libTree->getRoot();
    Book* book = nullptr;
    
    // Find the book by its title
    book = findBookHelper(curr, bookTitle);

    // If the book is not found, print a message and return
    if (book == nullptr) 
    {
        cout << "Book not found" << endl;
        return;
    } 
    else
    {
        int choice;
        do{
            // Variables to store new values for book details
            int publication_year, total_copies, available_copies;
            string title, author, isbn, category;

            // Display options for editing
            cout << "1: Title" << endl;
            cout << "2: Author(s)" << endl;
            cout << "3: ISBN" << endl;
            cout << "4: Publication Year" << endl;
            cout << "5: Total Copies" << endl;
            cout << "6: Available Copies" << endl;
            cout << "7: Exit" << endl;
            cout << "Choose the field you want to edit: ";
            cin >> choice;

            // Clear input buffer
            cin.ignore();

            // Switch case for different editing options
            switch (choice) 
            {
                case 1:
                    cout << "Enter new title: ";
                    getline(cin ,title);
                    book->title = title;
                    break;
                case 2:
                    cout << "Enter new author(s): ";
                    getline(cin,author);
                    book->author = author;
                    break;
                case 3:
                    cout << "Enter new ISBN: ";
                    getline(cin,isbn);
                    book->isbn = isbn;
                    break;
                case 4:
                    cout << "Enter new publication year: ";
                    cin >> publication_year;
                    book->publication_year = publication_year;
                    break;
                case 5:
                    cout << "Enter new total copies: ";
                    cin >> total_copies;
                    book->total_copies = total_copies;
                    break;
                case 6:
                    cout << "Enter new available copies: ";
                    cin >> available_copies;
                    book->available_copies = available_copies;
                    break;
                case 7:
                    cout << "Exiting" << endl;
                    return;
                default:
                    cout << "Invalid choice" << endl;
                    break;

            }
        
        } while(choice != 7); // Continue editing until the user chooses to exit
    }
}



// Borrow a book from the library
void LCMS::borrowBook(string bookTitle) 
{
    // Variables to store borrower's name and ID
    string name, id;

    // Prompt user to enter borrower's name
    cout << "Enter Borrower's name: ";
    getline(cin,name);

    // Prompt user to enter borrower's ID
    cout << "Enter borrower's ID: ";
    getline(cin,id);

    // Search for the book
    Node* curr = libTree->getRoot();
    Book* book = nullptr;

    // Traverse the library tree to find the book
    while (curr != nullptr) 
    {
        // Search for the book in the current node
        book = libTree->findBook(curr, bookTitle);
        if (book != nullptr) 
        {
            break; // Book found, exit the loop
        }

        // Traverse through the children nodes
        for (int i = 0; i < curr->children.size(); ++i) 
        {
            book = findBookHelper(curr->children[i], bookTitle);
            if (book != nullptr) 
            {
                curr = curr->children[i];
                break; // Book found in child node, exit the loop
            }
        }
    }

    // If the book is not found, print a message and return
    if (book == nullptr) 
    {
        cout << "Book not found" << endl;
        return;
    }

    // Add borrower to current borrowers
    book->currentBorrowers.push_back(new Borrower(name, id));

    // Add borrower to all borrowers
    book->allBorrowers.push_back(new Borrower(name, id));

    // Print a confirmation message
    cout << "Book " << bookTitle << " has been issued to " << name << endl;
}

// Return a borrowed book to the library
void LCMS::returnBook(string bookTitle) 
{
    // Variables to store borrower's name and ID
    string name, id;

    // Prompt user to enter borrower's name
    cout << "Enter Borrower's name: ";
    getline(cin,name);

    // Prompt user to enter borrower's ID
    cout << "Enter borrower's ID: ";
    getline(cin,id);

    // Search for the book
    Node* curr = libTree->getRoot();
    Book* book = nullptr;

    // Traverse the library tree to find the book
    while (curr != nullptr) 
    {
        // Search for the book in the current node
        book = libTree->findBook(curr, bookTitle);
        if (book != nullptr) 
        {
            break; // Book found, exit the loop
        }

        // Traverse through the children nodes
        for (int i = 0; i < curr->children.size(); ++i) 
        {
            book = findBookHelper(curr->children[i], bookTitle);
            if (book != nullptr) 
            {
                curr = curr->children[i];
                break; // Book found in child node, exit the loop
            }
        }
    }

    // If the book is not found, print a message and return
    if (book == nullptr) 
    {
        cout << "Book not found" << endl;
        return;
    }

    // Search for the borrower in current borrowers
    bool borrowerFound = false;
    for (int i = 0; i < book->currentBorrowers.size(); ++i) 
    {
        if (book->currentBorrowers[i]->name == name && book->currentBorrowers[i]->id == id) 
        {
            book->currentBorrowers.erase(i);
            borrowerFound = true;
            break;
        }
    }

    // Print appropriate message based on whether the borrower was found or not
    if (borrowerFound) 
    {
        cout << "Book " << bookTitle << " has been successfully returned" << endl;
    } 
    else 
    {
        cout << "Borrower not found for book " << bookTitle << endl;
    }
}


// List current borrowers of a book
void LCMS::listCurrentBorrowers(string bookTitle) 
{
    // Search for the book
    Node* curr = libTree->getRoot();
    Book* book = nullptr;

    // Traverse the library tree to find the book
    while (curr != nullptr) 
    {
        // Search for the book in the current node
        book = libTree->findBook(curr, bookTitle);
        if (book != nullptr) 
        {
            break; // Book found, exit the loop
        }

        // Traverse through the children nodes
        for (int i = 0; i < curr->children.size(); ++i) 
        {
            book = findBookHelper(curr->children[i], bookTitle);
            if (book != nullptr) 
            {
                curr = curr->children[i];
                break; // Book found in child node, exit the loop
            }
        }
    }

    // If the book is not found, print a message and return
    if (book == nullptr) 
    {
        cout << "Book not found" << endl;
        return;
    }

    // List current borrowers
    cout << "Current borrowers of book \"" << bookTitle << "\":" << endl;
    for (int i = 0; i < book->currentBorrowers.size(); ++i) 
    {
        cout << i + 1 << ": " << book->currentBorrowers[i]->name << endl;
    }
}

// List all borrowers of a book
void LCMS::listAllBorrowers(string bookTitle) 
{
    // Search for the book
    Node* curr = libTree->getRoot();
    Book* book = nullptr;

    // Traverse the library tree to find the book
    while (curr != nullptr) 
    {
        // Search for the book in the current node
        book = libTree->findBook(curr, bookTitle);
        if (book != nullptr) 
        {
            break; // Book found, exit the loop
        }

        // Traverse through the children nodes
        for (int i = 0; i < curr->children.size(); ++i) 
        {
            book = findBookHelper(curr->children[i], bookTitle);
            if (book != nullptr) 
            {
                curr = curr->children[i];
                break; // Book found in child node, exit the loop
            }
        }
    }

    // If the book is not found, print a message and return
    if (book == nullptr) 
    {
        cout << "Book not found" << endl;
        return;
    }

    // List all borrowers
    cout << "All borrowers of book \"" << bookTitle << "\":" << endl;
    for (int i = 0; i < book->allBorrowers.size(); ++i) 
    {
        cout << i + 1 << ": " << book->allBorrowers[i]->name << endl;
    }
}


// List books borrowed by a specific borrower
void LCMS::listBooks(string borrower_name, string id) 
{
    cout << "Books borrowed by " << borrower_name << " (" << id << "):" << endl;
    // Start DFS traversal from the root node
    listBooksDFS(libTree->getRoot(), borrower_name, id);
}

// Depth-first search (DFS) traversal to list books borrowed by a specific borrower
void LCMS::listBooksDFS(Node* node, string borrower_name, string id) 
{
    // Base case: If the current node is null, return
    if (node == nullptr) return;

    // Iterate over the books borrowed by the borrower in the current node
    for (int i = 0; i < node->books.size(); ++i) 
    {
        Book* book = node->books[i];
        for (int i = 0; i < book->allBorrowers.size(); ++i) 
        {
            Borrower* currentBorrower = book->allBorrowers[i];
            if (currentBorrower->name == borrower_name && currentBorrower->id == id) 
            {
                // Print the title of the book borrowed by the specific borrower
                cout << book->title << endl;
                return; // Stop searching once borrower is found
            }
        }
    }

    // Recursively traverse the children nodes
    for (int i = 0; i < node->children.size(); ++i) 
    {
        listBooksDFS(node->children[i], borrower_name, id);
    }
}

// Remove a book from the library
void LCMS::removeBook(string bookTitle) 
{
    string choice;

    // Prompt the user for confirmation before deleting the book
    cout << "Are you sure you want to delete the book " << bookTitle << " (yes/no): ";
    cin >> choice;

    if (choice == "yes")
    {
        // Get the root node of the library tree
        Node* curr = libTree->getRoot();

        // Initialize a flag to check if the book was found and removed
        int flag = 0;

        // Call the helper function to remove the book
        removeBookHelper(curr, bookTitle, flag);

        // Check if the book was successfully removed
        if(flag)
        {
            cout << "Book " << bookTitle << " has been successfully removed" << endl;
        }
        else
        {
            cout << "Book not found, remove unsuccessful" << endl;
        }
    }
    else
    {
        // If the user doesn't confirm deletion, return without performing any action
        return;
    }
}



// Helper function to remove a book from the library tree
void LCMS::removeBookHelper(Node* node, string bookTitle, int& flag)
{
    // If the current node is null, return
    if (node == nullptr){
        return;
    }

    // Find the book to remove in the current node
    Book* bookToRemove = libTree->findBook(node, bookTitle);
    if (bookToRemove != nullptr)
    {
        // If the book is found, remove it from the node's book list
        for (int i = 0; i < node->books.size(); ++i){
            if (node->books[i] == bookToRemove)
            {
                node->books.erase(i); // Erase the book from the node's books list
                libTree->updateBookCount(node, -1); // Update the book count in the node
                flag = 1; // Set flag to indicate successful removal
                return;
            }
        }
    }

    // Recursively call removeBookHelper for each child node
    for(int i = 0; i< node->children.size() ;++i)
    {
        removeBookHelper(node->children[i], bookTitle, flag);
    }
}

// Add a new category to the library tree
void LCMS::addCategory(string category) 
{
    libTree->createNode(category); // Create a new node for the category
    cout << category << " has been successfully created " << endl; // Inform the user
}

// Find a category in the library tree
void LCMS::findCategory(string category) 
{
    // Get the node corresponding to the category
    Node* node = libTree->getNode(category);
    
    // Check if the node exists
    if (node != nullptr) {
        cout << "Category \"" << category << "\" was found in the catalog" << endl; // Inform the user
    } else {
        cout << "No such category \"" << category << "\" found in the catalog" << endl; // Inform the user
    }
}



// Remove a category from the library tree
void LCMS::removeCategory(string category) 
{
    // Get the node corresponding to the category
    Node* node = libTree->getNode(category);

    // Check if the category node exists
    if (node == nullptr) 
    {
        cout << "Category \"" << category << "\" not found" << endl; // Inform the user
        return;
    }

    int offset = 0;

    // Remove the category node from the library tree
    Node* parentNode = node->parent;
    if (parentNode != nullptr) 
    {
        // Search for the category node in the parent's children
        for (int i = 0; i < parentNode->children.size(); ++i) 
        {
            if (parentNode->children[i] == node) 
            {
                removeCategoryHelper(node, offset); // Call helper function to remove the category and its books
                libTree->updateBookCount(parentNode, -1 * offset); // Update book counts in parent nodes
                cout << "Category \"" << category << "\" removed successfully" << endl; // Inform the user
                return;
            }
        }
    }
    else
    {
        // If parentNode is null, indicating the node is the root
        removeCategoryHelper(node, offset); // Call helper function to remove the category and its books
    }

    // If parentNode is null or node is not found in parent's children
    cout << "Failed to remove category \"" << category << "\"" << endl; // Inform the user
}


// Helper function to remove a category node and its children from the library tree
void LCMS::removeCategoryHelper(Node* node, int& offset) 
{
    // Check if the node is null
    if (node == nullptr) {
        return; // If so, return without performing any action
    }
   
    MyVector<Node*> nodeChildren = node->children; // Get the children of the current node
    
    // If the node has no children (leaf node) and contains books
    if (nodeChildren.empty() && !node->books.empty()) {
        offset += node->books.size(); // Update the offset by the number of books in the node
    }

    // Recursively remove subcategories and update offset
    for (int i = 0; i < nodeChildren.size(); ++i) {
        removeCategoryHelper(nodeChildren[i], offset); // Call this function recursively for each child node
    }

    // Remove the node from its parent's children vector
    if (node->parent != nullptr) {
        for (int i = 0; i < node->parent->children.size(); ++i) 
        {
            if (node->parent->children[i] == node) {
                node->parent->children.erase(i); // Remove the node from the parent's children vector
                break; // Exit the loop once the node is removed
            }
        }
    }

    delete node; // Delete the node from memory
}


