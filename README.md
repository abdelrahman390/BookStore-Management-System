## Books object-oriented bookstore hierarchy:
                            ┌────────────────────┐
                            │      Book          │ (abstract)
                            │- ispn              │
                            │- title             │
                            │- publishYear       │
                            │+ getType() = 0     │
                            └────────┬───────────┘
                                     │
                      ┌──────────────┼─────────────────────┐
                      │              │   (abstract)        │
                  ┌───▼────┐    ┌────▼───────┐        ┌────▼───────┐ 
                  │DemoBook│    │ForSaleBook │        │PhysicalBook│ (abstract)
                  └────────┘    │- price     │        │- stock     │
                                │+ getPrice()│        │+ getStock()│
                                └────┬───────┘        └────┬───────┘
                                     │                     │
                   ┌─────────────────┼─────────────────────┼
                   │                 │                     │     
               ┌───▼───────┐     ┌───▼─────┐               │
               │EBook      │     │PaperBook│<──────────────┘
               │- fileType │     └─────────┘ 
               └───────────┘       

 ─────────────────────────────────────────────────────────────

                        ┌───────────────────────────┐
                        │     ShippingService       │ (abstract)
                        │ - totalPrice              │
                        │ - date                    │
                        │ - pair<Book*, int> order; │
                        └───────────┬───────────────┘
                                    │
                   ┌────────────────┼──────────────────┐
                   │                                    │
         ┌─────────▼─────────┐                ┌────────▼──────────┐
         │   PostalService   │                │   EmailService    │
         │ - address         │                │   - email         │
         └───────────────────┘                └───────────────────┘
 ─────────────────────────────────────────────────────────────

                        ┌─────────────────────────┐
                        │       BookStore         │
                        │- books                  │
                        │- shippingService        │
                        │+ addBook()              │
                        │+ removeBookByIspn()     │
                        │+ listAllBooks()         │
                        │+ findBookByIspn()       │
                        │+ purchaseBook()         │
                        │+ setShippingService()   │
                        └─────────────────────────┘

```cpp
# ------------------------------
# BOOK HIERARCHY
# ------------------------------

class Book(abstract):
    ispn: int
    title: str
    publishYear: int

    getIspn() -> int
    getTitle() -> str
    getPublishYear() -> int
    returnOutdatedBooks(int years) -> bool


class ForSaleBook(abstract):
    price: float

    getPrice() -> float
    setPrice(float newPrice)


class PhysicalBook(abstract):
    stock: int

    getStock() -> int
    setStock(int newStock)
    decreaseStock(int amount)
    increaseStock(int amount)


class PaperBook -> Book, ForSaleBook, PhysicalBook:
    # Inherits all attributes & methods
    pass


class EBook -> Book, ForSaleBook:
    fileType: str

    getFileType() -> str
    setFileType(str newType)


class DemoBook -> Book:
    # Inherits base Book behavior
    pass


# ------------------------------
# SHIPPING SERVICE HIERARCHY
# ------------------------------

class ShippingService(abstract):
    totalPrice: int
    date: string 
    pair<Book*, int> order; // <Book ptr, quantity>


class PostalService -> ShippingService:
    address: string 
    PhysicalBook * book


class EmailService -> ShippingService:
    email: string 
    EBook * book


# ------------------------------
# SHOP / STORE MANAGEMENT
# ------------------------------

class BookStore:
    books: vector<Book*>
    shippingService: ShippingService*

    addPaperBook(string _title, int _publishYear, float _price, int _stock)
    addEBook(string _title, int _publishYear, float _price, string _fileType)
    addDemoBook(string _title, int _publishYear)
    removeBookByIspn(int ispn)
    viewBooks()
    purchaseBook(int ispn, str customerName)
    handleShipping()
