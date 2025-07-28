#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int year;

public:
    Book() : id(0), year(0) {}
    Book(int i, const string& t, const string& a, int y) : id(i), title(t), author(a), year(y) {}

    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    int getYear() const { return year; }

    void setDetails(const string& t, const string& a, int y) {
        title = t;
        author = a;
        year = y;
    }

    void display() const {
        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\nYear: " << year << "\n";
    }

    string serialize() const {
        return to_string(id) + ";" + title + ";" + author + ";" + to_string(year);
    }

    static Book deserialize(const string& data) {
        stringstream ss(data);
        string segment;
        vector<string> parts;
        while (getline(ss, segment, ';')) parts.push_back(segment);
        return Book(stoi(parts[0]), parts[1], parts[2], stoi(parts[3]));
    }
};

vector<Book> loadBooks(const string& filename) {
    vector<Book> books;
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) books.push_back(Book::deserialize(line));
    }
    return books;
}

bool saveBooks(const vector<Book>& books, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) return false;
    for (const Book& book : books) {
        outFile << book.serialize() << "\n";
    }
    return true;
}

void addRecord(const string& filename) {
    int id, year;
    string title, author;

    cout << "Enter new book details:\n";
    cout << "ID: "; cin >> id; cin.ignore();
    cout << "Title: "; getline(cin, title);
    cout << "Author: "; getline(cin, author);
    cout << "Year: "; cin >> year; cin.ignore();

    vector<Book> books = loadBooks(filename);
    for (const Book& b : books) {
        if (b.getId() == id) {
            cout << "A book with this ID already exists.\n";
            return;
        }
    }

    books.emplace_back(id, title, author, year);
    if (saveBooks(books, filename)) cout << "Book added successfully.\n";
    else cout << "Failed to save the book.\n";
}

void displayAllRecords(const string& filename) {
    vector<Book> books = loadBooks(filename);
    if (books.empty()) {
        cout << "No records found.\n";
        return;
    }

    cout << "\nAll Book Records:\n";
    for (const Book& b : books) {
        b.display();
        cout << "------------------------\n";
    }
}

void searchRecordByID(const string& filename) {
    int id;
    cout << "Enter ID to search: "; cin >> id; cin.ignore();

    for (const Book& b : loadBooks(filename)) {
        if (b.getId() == id) {
            cout << "\nBook Found:\n";
            b.display();
            return;
        }
    }
    cout << "Book with ID " << id << " not found.\n";
}

void updateRecord(const string& filename) {
    int id;
    cout << "Enter ID to update: "; cin >> id; cin.ignore();

    vector<Book> books = loadBooks(filename);
    bool updated = false;

    for (Book& b : books) {
        if (b.getId() == id) {
            string newTitle, newAuthor;
            int newYear;

            cout << "Enter new title: "; getline(cin, newTitle);
            cout << "Enter new author: "; getline(cin, newAuthor);
            cout << "Enter new year: "; cin >> newYear; cin.ignore();

            b.setDetails(newTitle, newAuthor, newYear);
            updated = true;
            break;
        }
    }

    if (updated && saveBooks(books, filename))
        cout << "Book updated successfully.\n";
    else
        cout << "Update failed or book not found.\n";
}

void deleteRecord(const string& filename) {
    int id;
    cout << "Enter ID to delete: "; cin >> id; cin.ignore();

    vector<Book> books = loadBooks(filename);
    auto it = remove_if(books.begin(), books.end(), [id](const Book& b) { return b.getId() == id; });

    if (it == books.end()) {
        cout << "No book found with ID " << id << ".\n";
        return;
    }

    books.erase(it, books.end());

    if (saveBooks(books, filename))
        cout << "Book deleted successfully.\n";
    else
        cout << "Failed to update the file.\n";
}

void sortRecords(const string& filename) {
    vector<Book> books = loadBooks(filename);
    if (books.empty()) {
        cout << "No records to sort.\n";
        return;
    }

    int choice;
    cout << "Sort by: 1 - Title, 2 - Year\nYour choice: ";
    cin >> choice; cin.ignore();

    if (choice == 1) {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.getTitle() < b.getTitle();
        });
    } else if (choice == 2) {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.getYear() < b.getYear();
        });
    } else {
        cout << "Invalid sort option.\n";
        return;
    }

    cout << "\nSorted Books:\n";
    for (const Book& b : books) {
        b.display();
        cout << "------------------------\n";
    }
}

void exportToCSV(const string& filename, const string& csvFilename) {
    vector<Book> books = loadBooks(filename);
    ofstream out(csvFilename);
    if (!out) {
        cout << "Error writing to CSV file.\n";
        return;
    }

    out << "ID,Title,Author,Year\n";
    for (const Book& b : books) {
        out << b.getId() << ",\"" << b.getTitle() << "\",\"" << b.getAuthor() << "\"," << b.getYear() << "\n";
    }

    cout << "Exported successfully to " << csvFilename << "\n";
}

int main() {
    const string filename = "books.txt";
    int option;

    do {
        cout << "\n=== Book Management System ===\n";
        cout << "1. Add Book\n2. Display All Books\n3. Search by ID\n4. Update Book\n";
        cout << "5. Delete Book\n6. Sort Books\n7. Export to CSV\n0. Exit\nSelect an option: ";
        cin >> option; cin.ignore();

        switch (option) {
            case 1: addRecord(filename); break;
            case 2: displayAllRecords(filename); break;
            case 3: searchRecordByID(filename); break;
            case 4: updateRecord(filename); break;
            case 5: deleteRecord(filename); break;
            case 6: sortRecords(filename); break;
            case 7: exportToCSV(filename, "books.csv"); break;
            case 0: cout << "Exiting... Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (option != 0);

    return 0;
}
