#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>
#include <any>
#include <map>
#include <queue>

using namespace std;

/*
==================== Documentation ====================

Entities & Attributes
--------------------------------------------------------
Book (abstract):
    - ispn: int
    - title: string
    - publishYear: int
    + returnOutdatedBooks(int years): void
    + getIspn(): int
    + getTitle(): string
    + getType(): string (abstract)

ForSaleBook (abstract):
    - price: float
    + getPrice(): float

PhysicalBook (abstract, extends Book):
    - stock: int
    + isAvailable(int quantity): bool
    + removeSalledQnt(int qnt): void
    + getStock(): int

PaperBook (concrete, extends Book, ForSaleBook, PhysicalBook):
    + getType(): string ("Paper")

EBook (concrete, extends Book, ForSaleBook):
    - fileType: string
    + getType(): string ("E")

DemoBook (concrete, extends Book):
    + getType(): string ("Demo")


ShippingService (abstract):
    - totalPrice: int
    - date: string
    - order: pair<Book*, int>

PhysicalShippingService (extends ShippingService):
    - address: string

MailService (extends ShippingService):
    - email: string


BooksStore:
    - books: map<int, Book*>
    - orders: queue<ShippingService*>
    - _ispn: int (auto-increment for books)
    + addPaperBook(string title, int publishYear, float price, int stock): PaperBook*
    + addEBook(string title, int publishYear, float price, string fileType): EBook*
    + addDemoBook(string title, int publishYear): DemoBook*
    + buy(int ispn, int quantity, string email, string address): float
    + handleShipping(Book* product, string type, float totalPrice, string email, string address): void
    + viewBooks(): void
    + removeBook(int ispn): void
========================================================
*/



// Abstract
class Book{
    int ispn;
    string title;
    int publishYear;
    public:
        Book(int _ispn, string _title, int _publishYear){
            ispn = _ispn;
            title = _title;
            publishYear = _publishYear;
        }

        void returnOutdatedBooks(int years){

        }

        int getIspn(){
            return ispn;
        }

        string getTitle(){
            return title;
        }

        virtual string getType() = 0;
};

// Abstract
class ForSaleBook{
    float price;
    public:
        ForSaleBook(float _price){
            price = _price;
        }

        float getPrice(){
            return price;
        }
};

// Abstract
class PhysicalBook: public virtual Book{
    int stock;
    public:
        PhysicalBook(int _ispn, string _title, int _publishYear, int _stock): Book(_ispn, _title, _publishYear){
            stock = _stock;
        }

        bool isAvailable(int quantity){
            if(quantity <= stock){
                return 1;
            }
            return 0;
        }

        void removeSalledQnt(int qnt){
            if(qnt <= stock){
                stock -= qnt;
            } else {
                throw invalid_argument("this quantity is not available");
            }
        }

        int getStock(){
            return stock;
        }
};

class PaperBook: public virtual  Book, public ForSaleBook, public PhysicalBook{
    public:
        PaperBook(int _ispn, string _title, int _publishYear, float _price, int _stock) : Book(_ispn, _title, _publishYear), ForSaleBook(_price), PhysicalBook(_ispn, _title, _publishYear,_stock) {};
        string getType(){
            return "Paper";
        }
};

// class EBook -> Book, -> ForSaleBook:
class EBook: public Book, public ForSaleBook{
    string fileType;
    public:
        EBook(int _ispn, string _title, int _publishYear, float _price, string _fileType) : Book(_ispn, _title, _publishYear), ForSaleBook(_price) {
            fileType = _fileType;
        };

        string getType(){
            return "E";
        }
};

class DemoBook: public Book{
    public:
        DemoBook(int _ispn, string _title, int _publishYear) : Book(_ispn, _title, _publishYear) {};

    string getType(){
        return "Demo";
    }
};

// ---------------------- Shipping Services ----------------------
class ShippingService{
    protected:
        int totalPrice;
        string date;

        pair<Book*, int> order; // <Book ptr, quantity>
    
    public:
        ShippingService(int _totalPrice, string _date) {
            totalPrice = _totalPrice;
            date = _date;
        };
};

class PhysicalShippingService: public ShippingService{
    string address;
    PhysicalBook * book;
    public:
        PhysicalShippingService(PhysicalBook * book, int _totalPrice, string _date, string _address): ShippingService(_totalPrice, _date) {
            address = _address;
            cout << "--- Physical Books Shipping service ---" << "\n";
            cout << "Book: " << book->getTitle() << " Will send to: " << _address << endl;
            cout << "Total Price: " << _totalPrice << endl << endl;
        };
};

class MailService: public ShippingService {
    string email;
    EBook * book ;

public:
    MailService(EBook * book, int _totalPrice, string _date, string _email) : ShippingService(_totalPrice, _date)
    {
        email = _email;
        cout << "--- EBooks Shipping service ---" << "\n";
        cout << "Book: " << book->getTitle() << " Will send to: " << _email << endl;
        cout << "Total Price: " << _totalPrice << endl << endl;
    };
};

class BooksStore{
    int _ispn = 1;
    map<int, Book*> books;
    queue<ShippingService*> orders;

    public:
        PaperBook* addPaperBook(string _title, int _publishYear, float _price, int _stock){
            PaperBook *ptr = NULL;
            int tempIspn = _ispn++;
            ptr = new PaperBook(tempIspn, _title, _publishYear, _price, _stock);
            books[tempIspn] = ptr;
            return ptr;
        }

        EBook* addEBook(string _title, int _publishYear, float _price, string _fileType){
            EBook *ptr = NULL;
            int tempIspn = _ispn++;
            ptr = new EBook(tempIspn, _title, _publishYear, _price, _fileType);
            books[tempIspn] = ptr;
            return ptr;
        }

        DemoBook* addDemoBook(string _title, int _publishYear){
            DemoBook *ptr = NULL;
            int tempIspn = _ispn++;
            ptr = new DemoBook(tempIspn, _title, _publishYear);
            books[tempIspn] = ptr;
            return ptr;
        }

        void handleShipping(Book* product, string type, float totalPrice ,string email , string addresss){
            if(type == "Paper"){
                PhysicalShippingService *ShipService = new PhysicalShippingService(dynamic_cast<PaperBook *>(product), totalPrice, "15/8/2025", addresss);
                orders.push(ShipService);
            } else if(type == "E") {
                MailService *ShipService = new MailService(dynamic_cast<EBook *>(product), totalPrice, "15/8/2025", email);
                orders.push(ShipService);
            } else {
                throw invalid_argument("Invaled book book for shepping.");
            }
        }

        float buy(int ispn, int  quantity, string email, string addresss){
            string type = books[ispn]->getType();
            if(type == "Paper"){
                PaperBook *book = dynamic_cast<PaperBook *>(books[ispn]);
                if(book->isAvailable(quantity)){
                    book->removeSalledQnt(quantity);
                    float totalPrice = book->getPrice() * quantity;
                    handleShipping(book, type, totalPrice, email, addresss);
                    return totalPrice;
                } else {
                    throw invalid_argument("The book is not available.");
                }
            } else if(type == "E"){
                EBook *book = dynamic_cast<EBook *>(books[ispn]);
                float totalPrice = book->getPrice();
                handleShipping(book, type, totalPrice, email, addresss);
                return totalPrice;
            } else if(type == "Demo"){
                throw invalid_argument("This Book not for sale.");
            }
            throw invalid_argument("This Book not founded.");
        }

        void viewBooks(){
            for(auto& b: books){
                cout << "Book: " << b.second->getTitle() << endl;
                if(b.second->getType() == "Paper"){
                    cout << "Stock: " << dynamic_cast<PhysicalBook *>(b.second)->getStock() << endl;
                }
            }
        }

        void removeBook(int ispn){
            int i = 0;
            cout << "Deleting Book: " << books[ispn]->getTitle()  << endl;
            delete books[ispn];
        }
};

int main() {
    BooksStore store;

    PaperBook* HarryPotter = store.addPaperBook("Harry Potter", 2001, 50, 20); // ispn = 1
    EBook* LearnCpp = store.addEBook("Learn C++", 2024, 40, "pdf"); // ispn = 2
    DemoBook* Salad = store.addDemoBook("how to cook Salad", 2001); // ispn = 3

    store.buy(1, 2, "test@gmail", "cairo");
    store.buy(2, 2, "test@gmail", "cairo");

    store.viewBooks();
    store.removeBook(1);
    // cout << store.buy(3, 2, "test@gmail", "cairo") << endl; // Error: This Book not for sale
}