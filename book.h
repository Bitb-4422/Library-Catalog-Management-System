//============================================================================
// Name         : 
// Author       : 
// Version      : 
// Date Created : 
// Date Modified:
// Description  : 
//============================================================================
#ifndef _BOOK_H
#define _BOOK_H
#include<string>
#include "myvector.hpp"

using namespace std;

class Borrower;
class Book
{
	private:
		string title;
		string author;
		string isbn;
		int publication_year;
		int total_copies;
		int available_copies;
		MyVector<Borrower*> currentBorrowers;		//current borrowers of the book
		MyVector<Borrower*> allBorrowers;   //history of all borrowers of the book

	public:
		Book(string title,string author,string isbn, int publication_year,int total_copies, int available_copies);
		void display(); // display details of a book (see output of command findbook)
		void display2();

	public:
		friend class Tree;
		friend class Node;
		friend class LCMS;
		friend class Borrower;
};
#endif