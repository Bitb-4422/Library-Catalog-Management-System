#include <iostream>
#include<string>

#include "borrower.h"


using namespace std;
// Constructor to initialize Borrower object with name and ID
Borrower::Borrower(string name, string id) : name(name), id(id) {}

// Method to list the books borrowed by the borrower
void Borrower::listBooks() 
{
    if (books_borrowed.empty()) 
    {
        // Print message if no books are borrowed by the borrower
        std::cout << "No books borrowed by " << name << std::endl;
    } 
    else 
    {
        // Iterate over the borrowed books and print their titles
        for (int i = 0; i < books_borrowed.size(); ++i)
        {
            cout << i + 1 << ": " << books_borrowed[i]->title << endl;
        }
    }
}
