#include <iostream>
#include <vector>
#include <string>

class Book {
public:
    std::string isbn;
    std::string title;
    std::string author;
    double price;
    int copies;

    Book(std::string isbn, std::string title, std::string author, double price, int copies)
        : isbn(isbn), title(title), author(author), price(price), copies(copies) {}

    void update_inventory(int quantity) {
        if (quantity <= copies) {
            copies -= quantity;
        } else {
            std::cout << "Not enough copies of '" << title << "' available." << std::endl;
        }
    }

    void display_info() const {
        std::cout << "ISBN: " << isbn << ", Title: " << title 
                  << ", Author: " << author << ", Price: $" << price 
                  << ", Copies available: " << copies << std::endl;
    }
};

class Cart {
public:
    std::vector<std::pair<Book*,
	 int items;
    double total_cost = 0.0;

    void add_book(Book& book, int quantity) {
        if (quantity <= book.copies) {
            book.update_inventory(quantity);
            bool found = false;
            for (auto& item : items) {
                if (item.first->isbn == book.isbn) {
                    item.second += quantity;
                    found = true;
                    break;
                }
            }
            if (!found) {
                items.push_back({&book, quantity});
            }
            total_cost += book.price * quantity;
            std::cout << "Added " << quantity << " copy(ies) of '" << book.title << "' to cart." << std::endl;
        } else {
            std::cout << "Not enough copies of '" << book.title << "' available." << std::endl;
        }
    }

    void remove_book(Book& book, int quantity) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->first->isbn == book.isbn) {
                if (it->second >= quantity) {
                    it->second -= quantity;
                    total_cost -= book.price * quantity;
                    book.copies += quantity;
                    std::cout << "Removed " << quantity << " copy(ies) of '" << book.title << "' from cart." << std::endl;
                    if (it->second == 0) {
                        items.erase(it);
                    }
                    return;
                } else {
                    std::cout << "You don't have that many copies of '" << book.title << "' in your cart." << std::endl;
                }
            }
        }
        std::cout << "'" << book.title << "' is not in your cart." << std::endl;
    }

    void display_cart() const {
        if (items.empty()) {
            std::cout << "Your cart is empty." << std::endl;
        } else {
            for (const auto& item : items) {
                std::cout << "Title: " << item.first->title << ", Quantity: " << item.second 
                          << ", Price per unit: $" << item.first->price << std::endl;
            }
            std::cout << "Total cost: $" << total_cost << std::endl;
        }
    }
};

class Store {
public:
    std::vector<Book> inventory;

    void add_book(const Book& book) {
        inventory.push_back(book);
    }

    Book* search_book(const std::string& isbn) {
        for (auto& book : inventory) {
            if (book.isbn == isbn) {
                return &book;
            }
        }
        std::cout << "Book not found!" << std::endl;
        return nullptr;
    }

    void display_books() const {
        if (inventory.empty()) {
            std::cout << "No books available in the store." << std::endl;
        } else {
            for (const auto& book : inventory) {
                book.display_info();
            }
        }
    }

    void checkout(Cart& cart) {
        std::cout << "Your total cost is: $" << cart.total_cost << std::endl;
        cart.display_cart();
        std::cout << "Thank you for your purchase!" << std::endl;
    }
};

// Example usage:
int main() {
    Store store;
    store.add_book(Book("123456", "C++ Programming", "John Doe", 29.99, 10));
    store.add_book(Book("654321", "Data Structures", "Jane Smith", 39.99, 5));

    Cart cart;
    // Display available books
    store.display_books();

    // Add books to cart
    Book* book1 = store.search_book("123456");
    if (book1) cart.add_book(*book1, 2);

    // Remove a book from the cart
    Book* book2 = store.search_book("123456");
    if (book2) cart.remove_book(*book2, 1);

    // Display cart contents
    cart.display_cart();

    // Checkout
    store.checkout(cart);

    return 0;
}

