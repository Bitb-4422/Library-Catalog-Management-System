# Library-Catalog-Management-System

This project is a Library Catalog Management System built using C++ and a custom general tree data structure. It is designed to help librarians manage books by organizing them into categories and subcategories. The system allows users to add, search, edit, borrow, and return books, and track the borrowing history of each book.

Features

- Tree-Based Catalog Structure: Books are categorized in a hierarchical tree, where each node represents a category or subcategory.
- Book Management:
  - Add, edit, and remove books from the catalog.
  - Search for books by title.
  - Update book details, such as title, author, ISBN, and publication year.
  - Track availability status.
- Category Management:
  - Add and remove categories/subcategories.
  - Organize books into different categories.
- Borrowing System:
  - Borrow and return books.
  - Track borrowers and their borrowing periods.
  - List current borrowers for any given book.
- Export/Import Data: Export and import library data to/from a file.
- Visualization Component: A simple graphical component for displaying the library's tree structure.

Requirements

- C++ Compiler: The program must be compiled using a C++ compiler that supports C++11 standard or higher.
- Makefile: The project includes a Makefile to manage the build process.

 Project Structure

The following files are included in the project:
- `book.h`, `book.cpp`: Defines the `Book` class and its functionality.
- `borrower.h`, `borrower.cpp`: Defines the `Borrower` class, which manages borrower details.
- `tree.h`, `tree.cpp`: Implements the general tree data structure for managing categories and subcategories.
- `lcms.h`, `lcms.cpp`: Contains the main logic for the Library Catalog Management System.
- `myvector.h`: Defines a custom vector class for managing lists of books and borrowers.
- `main.cpp`: The entry point for the program, where user interactions are handled.
- `makefile`: Handles compilation and linking of the source files.

How to Run

1. Clone the repository to your local machine:
2. Build the project using the provided Makefile:
   ```
   make
   ```
3. Run the program:
   ```
   ./lcms
   ```
4. Available Commands:
   - `import <file_name>`: Import a catalog from a file.
   - `export <file_name>`: Export the catalog to a file.
   - `findBook <title>`: Search for a book by title.
   - `findAll <category/subcategory>`: List all books in a category or subcategory.
   - `addBook`: Add a new book to the catalog.
   - `editBook <title>`: Edit the details of a book.
   - `removeBook <title>`: Remove a book from the catalog.
   - `addCategory <category/subcategory>`: Add a new category or subcategory.
   - `removeCategory <category/subcategory>`: Remove a category or subcategory.
   - `borrowBook <title>`: Borrow a book by title.
   - `returnBook <title>`: Return a borrowed book.
   - `listCurrentBorrowers <title>`: List all borrowers of a book.
   - `listBooks <borrower’s name>`: List all books borrowed by a specific user.


