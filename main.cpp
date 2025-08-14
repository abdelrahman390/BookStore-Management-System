#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>
#include <any>

using namespace std;

/*
All instance:
    ispn: int
    Title: str
    PublishYear: int
    stock: int
    price: float
    FileType: str

class Book(abstract):
    ispn: int
    Title: str
    PublishYear: int

    returnOutdatedBooks(int years)

class ForSaleBook(abstract):
    price: float

class PhysicalBook(abstract):
    stock: int

class PaperBook -> Book, -> ForSaleBook, -> PhysicalBook:
    pass

class EBook -> Book, -> ForSaleBook:
    FileType: str

class DemoBook -> Book:
    pass


class ShippingService:
    int shipId;
    int totalPrice;
    str date;

    pair<Book*, int> order; // <Book ptr, quantity>

class PhysicalShippingService -> ShippingService:
    str address

class MailService -> ShippingService:
    str email

class BooksStore:
    container<Book*> books;
    container<ShippingService*> orders;

    buy(int ispn, int  quantity, str email)

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
                throw invalid_argument("this quntity is not available");
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
    int shipId = 1;
    vector<Book*> books;
    vector<ShippingService*> orders;

    public:
        PaperBook* addPaperBook(string _title, int _publishYear, float _price, int _stock){
            PaperBook *ptr = NULL;
            ptr = new PaperBook(_ispn++, _title, _publishYear, _price, _stock);
            books.push_back(ptr);
            return ptr;
        }

        EBook* addEBook(string _title, int _publishYear, float _price, string _fileType){
            EBook *ptr = NULL;
            ptr = new EBook(_ispn++, _title, _publishYear, _price, _fileType);
            books.push_back(ptr);
            return ptr;
        }

        DemoBook* addDemoBook(string _title, int _publishYear){
            DemoBook *ptr = NULL;
            ptr = new DemoBook(_ispn++, _title, _publishYear);
            books.push_back(ptr);
            return ptr;
        }

        void handleShipping(Book* product, string type, float totalPrice ,string email ,string addresss){
            if(type == "Paper"){
                PhysicalShippingService *ShipService = new PhysicalShippingService(dynamic_cast<PaperBook *>(product), totalPrice, "15/8/2025", addresss);
                orders.push_back(ShipService);
            } else if(type == "E") {
                MailService *ShipService = new MailService(dynamic_cast<EBook *>(product), totalPrice, "15/8/2025", email);
                orders.push_back(ShipService);
            } else {
                throw invalid_argument("Invaled book book for shepping.");
            }
        }

        float buy(int ispn, int  quantity, string email, string addresss){
            for(auto& b: books){
                if(ispn == b->getIspn()){
                    string type = b->getType();
                    if(type == "Paper"){
                        PaperBook *book = dynamic_cast<PaperBook *>(b);
                        if(book->isAvailable(quantity)){
                            book->removeSalledQnt(quantity);
                            float totalPrice = book->getPrice() * quantity;
                            handleShipping(book, type, totalPrice,email, addresss);
                            return totalPrice;
                        } else {
                            throw invalid_argument("The book is not available.");
                        }
                    } else if(type == "E"){
                        EBook *book = dynamic_cast<EBook *>(b);
                        float totalPrice = book->getPrice();
                        handleShipping(book, type, totalPrice, email, addresss);
                        return totalPrice;
                    } else if(type == "Demo"){
                        throw invalid_argument("This Book not for sale.");
                    }
                }
            }
            throw invalid_argument("This Book not founded.");
        }

        void viewBooks(){
            for(auto& b: books){
                cout << "Book: " << b->getTitle() << endl;
                if(b->getType() == "Paper"){
                    cout << "Stock: " << dynamic_cast<PhysicalBook *>(b)->getStock() << endl;
                }
            }
        }

        void removeBook(int ispn){
            int i = 0;
            for(auto& b: books){
                if(b->getIspn() == ispn){
                    cout << "Deleting Book: " << b->getTitle()  << endl;
                    books.erase(books.begin() + i);
                }
            }
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