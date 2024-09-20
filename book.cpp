#include<iostream>
#include<iomanip>
#include<string>

#include "book.h"

using namespace std;

Book::Book(string title, string author, string isbn, int publication_year, 
           int total_copies, int available_copies)
    : title(title), author(author), isbn(isbn), publication_year(publication_year),
      total_copies(total_copies), available_copies(available_copies) {}

// Method to display detailed information about the book
void Book::display()
{
    cout << setw(18) << left << "Title: " << title << endl;
    cout << setw(18) << left << "Author(s): " << author << endl;
    cout << setw(18) << left << "ISBN: " << isbn << endl;
    cout << setw(18) << left << "Year: " << publication_year << endl;
    cout << setw(18) << left << "Total Copies: " << total_copies << endl;
    cout << setw(18) << left << "Available copies: " << available_copies << endl;
    cout << "------------------------------------------------------" << endl;
} 

// Method to display basic information about the book
void Book::display2()
{
    cout << setw(18) << left << "Title: " << title << endl;
    cout << setw(18) << left << "Author(s): " << author << endl;
    cout << setw(18) << left << "ISBN: " << isbn << endl;
    cout << setw(18) << left << "Year: " << publication_year << endl;
    cout << "------------------------------------------------------" << endl;
} 