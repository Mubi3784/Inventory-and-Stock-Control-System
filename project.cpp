#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>

using namespace std;

// Custom Exceptions
class FileException : public runtime_error {
public:
    FileException(const string& msg) : runtime_error(msg) {}
};

class DuplicateIDException : public runtime_error {
public:
    DuplicateIDException(const string& msg) : runtime_error(msg) {}
};

class NotFoundException : public runtime_error {
public:
    NotFoundException(const string& msg) : runtime_error(msg) {}
};

// --------- PRODUCT CLASS ---------
class Product {
public:
    int productID;
    string name;
    double price;
    string category;

    Product() : productID(0), price(0.0) {}
    Product(int id, const string& n, double p, const string& c)
        : productID(id), name(n), price(p), category(c) {}

    void display() const {
        cout << "Product ID: " << productID
             << ", Name: " << name
             << ", Price: $" << price
             << ", Category: " << category << endl;
    }

    string toString() const {
        stringstream ss;
        ss << productID << "," << name << "," << price << "," << category;
        return ss.str();
    }

    static Product fromString(const string& line) {
        stringstream ss(line);
        string token;
        int id;
        string name;
        double price;
        string category;

        getline(ss, token, ','); id = stoi(token);
        getline(ss, name, ',');
        getline(ss, token, ','); price = stod(token);
        getline(ss, category, ',');

        return Product(id, name, price, category);
    }
};

// --------- PRODUCT BST NODE ---------
class ProductNode {
public:
    Product data;
    ProductNode* left;
    ProductNode* right;

    ProductNode(const Product& p) : data(p), left(nullptr), right(nullptr) {}
};

// --------- PRODUCT BST ---------
class ProductBST {
private:
    ProductNode* root;

    ProductNode* insertHelper(ProductNode* node, const Product& p) {
        if (!node) return new ProductNode(p);

        if (p.productID < node->data.productID)
            node->left = insertHelper(node->left, p);
        else if (p.productID > node->data.productID)
            node->right = insertHelper(node->right, p);
        else
            throw DuplicateIDException("Duplicate Product ID: " + to_string(p.productID));

        return node;
    }

    ProductNode* findMin(ProductNode* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    ProductNode* removeHelper(ProductNode* node, int productID) {
        if (!node)
            throw NotFoundException("Product ID not found: " + to_string(productID));

        if (productID < node->data.productID)
            node->left = removeHelper(node->left, productID);
        else if (productID > node->data.productID)
            node->right = removeHelper(node->right, productID);
        else {
            // node to delete found
            if (!node->left) {
                ProductNode* rightChild = node->right;
                delete node;
                return rightChild;
            }
            else if (!node->right) {
                ProductNode* leftChild = node->left;
                delete node;
                return leftChild;
            }
            else {
                ProductNode* minRight = findMin(node->right);
                node->data = minRight->data;
                node->right = removeHelper(node->right, minRight->data.productID);
            }
        }
        return node;
    }

    ProductNode* searchHelper(ProductNode* node, int productID) {
        if (!node) return nullptr;
        if (productID == node->data.productID) return node;
        if (productID < node->data.productID) return searchHelper(node->left, productID);
        else return searchHelper(node->right, productID);
    }

    void inorderHelper(ProductNode* node) {
        if (!node) return;
        inorderHelper(node->left);
        node->data.display();
        inorderHelper(node->right);
    }

    void collectInorder(ProductNode* node, Product* arr[], int& idx, int capacity) {
        if (!node || idx >= capacity) return;
        collectInorder(node->left, arr, idx, capacity);
        arr[idx++] = &(node->data);
        collectInorder(node->right, arr, idx, capacity);
    }

    void clearHelper(ProductNode* node) {
        if (!node) return;
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }

public:
    ProductBST() : root(nullptr) {}

    ~ProductBST() {
        clearHelper(root);
    }

    void insert(const Product& p) {
        root = insertHelper(root, p);
    }

    void remove(int productID) {
        root = removeHelper(root, productID);
    }

    Product* search(int productID) {
        ProductNode* node = searchHelper(root, productID);
        return node ? &(node->data) : nullptr;
    }

    void displayAll() {
        if (!root) {
            cout << "No products to display." << endl;
            return;
        }
        cout << "--- Products List ---" << endl;
        inorderHelper(root);
        cout << "------------------------------" << endl;
    }

    // Get count of nodes for array allocation
    int countNodes(ProductNode* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    int getCount() {
        return countNodes(root);
    }

    // Get all products into an array (for sorting/searching)
    int getAllProducts(Product* arr[], int capacity) {
        int idx = 0;
        collectInorder(root, arr, idx, capacity);
        return idx;
    }
};

// --------- SUPPLIER CLASS ---------
class Supplier {
public:
    int supplierID;
    string name;
    string contactInfo;

    Supplier() : supplierID(0) {}
    Supplier(int id, const string& n, const string& c)
        : supplierID(id), name(n), contactInfo(c) {}

    void display() const {
        cout << "Supplier ID: " << supplierID
             << ", Name: " << name
             << ", Contact: " << contactInfo << endl;
    }

    string toString() const {
        stringstream ss;
        ss << supplierID << "," << name << "," << contactInfo;
        return ss.str();
    }

    static Supplier fromString(const string& line) {
        stringstream ss(line);
        string token;
        int id;
        string name;
        string contact;

        getline(ss, token, ','); id = stoi(token);
        getline(ss, name, ',');
        getline(ss, contact, ',');

        return Supplier(id, name, contact);
    }
};

// --------- SUPPLIER NODE & LINKED LIST ---------
class SupplierNode {
public:
    Supplier data;
    SupplierNode* next;

    SupplierNode(const Supplier& s) : data(s), next(nullptr) {}
};

class SupplierList {
private:
    SupplierNode* head;

public:
    SupplierList() : head(nullptr) {}

    ~SupplierList() {
        while (head) {
            SupplierNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addSupplier(const Supplier& s) {
        if (findSupplier(s.supplierID)) {
            throw DuplicateIDException("Duplicate Supplier ID: " + to_string(s.supplierID));
        }
        SupplierNode* newNode = new SupplierNode(s);
        newNode->next = head;
        head = newNode;
    }

    Supplier* findSupplier(int supplierID) {
        SupplierNode* current = head;
        while (current) {
            if (current->data.supplierID == supplierID)
                return &(current->data);
            current = current->next;
        }
        return nullptr;
    }

    void displayAll() {
        if (!head) {
            cout << "No suppliers to display." << endl;
            return;
        }
        cout << "--- Suppliers List ---" << endl;
        SupplierNode* current = head;
        while (current) {
            current->data.display();
            current = current->next;
        }
        cout << "----------------------" << endl;
    }

    // Count suppliers
    int count() {
        int cnt = 0;
        SupplierNode* current = head;
        while (current) {
            cnt++;
            current = current->next;
        }
        return cnt;
    }

    // Get all suppliers in array for sorting/searching
    int getAllSuppliers(Supplier* arr[], int capacity) {
        int idx = 0;
        SupplierNode* current = head;
        while (current && idx < capacity) {
            arr[idx++] = &(current->data);
            current = current->next;
        }
        return idx;
    }
};

// --------- STOCK CLASS ---------
class Stock {
public:
    int productID;
    int supplierID;
    int quantity;

    Stock() : productID(0), supplierID(0), quantity(0) {}
    Stock(int pID, int sID, int qty) : productID(pID), supplierID(sID), quantity(qty) {}

    void display() const {
        cout << "Product ID: " << productID
             << ", Supplier ID: " << supplierID
             << ", Quantity: " << quantity << endl;
    }

    string toString() const {
        stringstream ss;
        ss << productID << "," << supplierID << "," << quantity;
        return ss.str();
    }

    static Stock fromString(const string& line) {
        stringstream ss(line);
        string token;
        int pID, sID, qty;

        getline(ss, token, ','); pID = stoi(token);
        getline(ss, token, ','); sID = stoi(token);
        getline(ss, token, ','); qty = stoi(token);

        return Stock(pID, sID, qty);
    }
};

// --------- STOCK NODE & LINKED LIST ---------
class StockNode {
public:
    Stock data;
    StockNode* next;

    StockNode(const Stock& s) : data(s), next(nullptr) {}
};

class StockList {
private:
    StockNode* head;

public:
    StockList() : head(nullptr) {}

    ~StockList() {
        while (head) {
            StockNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addStock(const Stock& s) {
        // If product-supplier pair exists, update quantity instead of adding new
        StockNode* current = head;
        while (current) {
            if (current->data.productID == s.productID && current->data.supplierID == s.supplierID) {
                current->data.quantity += s.quantity;
                return;
            }
            current = current->next;
        }
        StockNode* newNode = new StockNode(s);
        newNode->next = head;
        head = newNode;
    }

    // Display all stocks
    void displayAll() {
        if (!head) {
            cout << "No stock records to display." << endl;
            return;
        }
        cout << "--- Stock List ---" << endl;
        StockNode* current = head;
        while (current) {
            current->data.display();
            current = current->next;
        }
        cout << "------------------" << endl;
    }

    // Find stock by product and supplier
    Stock* findStock(int productID, int supplierID) {
        StockNode* current = head;
        while (current) {
            if (current->data.productID == productID && current->data.supplierID == supplierID)
                return &(current->data);
            current = current->next;
        }
        return nullptr;
    }

    // Count stocks
    int count() {
        int cnt = 0;
        StockNode* current = head;
        while (current) {
            cnt++;
            current = current->next;
        }
        return cnt;
    }

    // Get all stocks in array for sorting/searching
    int getAllStocks(Stock* arr[], int capacity) {
        int idx = 0;
        StockNode* current = head;
        while (current && idx < capacity) {
            arr[idx++] = &(current->data);
            current = current->next;
        }
        return idx;
    }
};

// --------- Sorting Functions ---------

// Bubble Sort for Products by productID
void bubbleSortProducts(Product* arr[], int n) {
    bool swapped;
    for (int i = 0; i < n -1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i -1; ++j) {
            if (arr[j]->productID > arr[j+1]->productID) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// Bubble Sort for Suppliers by supplierID
void bubbleSortSuppliers(Supplier* arr[], int n) {
    bool swapped;
    for (int i = 0; i < n -1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i -1; ++j) {
            if (arr[j]->supplierID > arr[j+1]->supplierID) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// Merge Sort for Stocks by quantity (descending)
void merge(Stock* arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Stock* L[n1];
    Stock* R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i=0, j=0, k=l;
    while (i < n1 && j < n2) {
        if (L[i]->quantity >= R[j]->quantity) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortStocks(Stock* arr[], int l, int r) {
    if (l < r) {
        int m = l + (r-l)/2;
        mergeSortStocks(arr, l, m);
        mergeSortStocks(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// --------- Searching Functions ---------

// Linear Search for Product by productID in array
int linearSearchProduct(Product* arr[], int n, int productID) {
    for (int i=0; i<n; i++) {
        if (arr[i]->productID == productID) return i;
    }
    return -1;
}

// Binary Search for Product by productID in sorted array
int binarySearchProduct(Product* arr[], int n, int productID) {
    int low = 0, high = n -1;
    while (low <= high) {
        int mid = low + (high - low)/2;
        if (arr[mid]->productID == productID)
            return mid;
        else if (arr[mid]->productID < productID)
            low = mid + 1;
        else
            high = mid -1;
    }
    return -1;
}

// Linear Search for Supplier by supplierID in array
int linearSearchSupplier(Supplier* arr[], int n, int supplierID) {
    for (int i=0; i<n; i++) {
        if (arr[i]->supplierID == supplierID) return i;
    }
    return -1;
}

// Binary Search for Supplier by supplierID in sorted array
int binarySearchSupplier(Supplier* arr[], int n, int supplierID) {
    int low = 0, high = n -1;
    while (low <= high) {
        int mid = low + (high - low)/2;
        if (arr[mid]->supplierID == supplierID)
            return mid;
        else if (arr[mid]->supplierID < supplierID)
            low = mid + 1;
        else
            high = mid -1;
    }
    return -1;
}

// --------- File Handling ---------

void saveProductsToFile(ProductBST& bst, const string& filename) {
    ofstream ofs(filename);
    if (!ofs)
        throw FileException("Cannot open products file for writing.");

    int count = bst.getCount();
    Product* products[count];
    int n = bst.getAllProducts(products, count);

    for (int i=0; i<n; i++) {
        ofs << products[i]->toString() << "\n";
    }
    ofs.close();
}

void loadProductsFromFile(ProductBST& bst, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) throw FileException("Cannot open products file for reading.");

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        Product p = Product::fromString(line);
        try {
            bst.insert(p);
        } catch (const DuplicateIDException& e) {
            cout << "Warning: " << e.what() << endl;
        }
    }
    ifs.close();
}

void saveSuppliersToFile(SupplierList& list, const string& filename) {
    ofstream ofs(filename);
    if (!ofs)
        throw FileException("Cannot open suppliers file for writing.");

    int count = list.count();
    Supplier* suppliers[count];
    int n = list.getAllSuppliers(suppliers, count);

    for (int i=0; i<n; i++) {
        ofs << suppliers[i]->toString() << "\n";
    }
    ofs.close();
}

void loadSuppliersFromFile(SupplierList& list, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) throw FileException("Cannot open suppliers file for reading.");

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        Supplier s = Supplier::fromString(line);
        try {
            list.addSupplier(s);
        } catch (const DuplicateIDException& e) {
            cout << "Warning: " << e.what() << endl;
        }
    }
    ifs.close();
}

void saveStocksToFile(StockList& list, const string& filename) {
    ofstream ofs(filename);
    if (!ofs)
        throw FileException("Cannot open stocks file for writing.");

    int count = list.count();
    Stock* stocks[count];
    int n = list.getAllStocks(stocks, count);

    for (int i=0; i<n; i++) {
        ofs << stocks[i]->toString() << "\n";
    }
    ofs.close();
}

void loadStocksFromFile(StockList& list, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) throw FileException("Cannot open stocks file for reading.");

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        Stock s = Stock::fromString(line);
        list.addStock(s);
    }
    ifs.close();
}

// --------- Menu & Interaction ---------

void displayMainMenu() {
    cout << "\n--- Inventory Management System ---\n";
    cout << "1. Add Product\n";
    cout << "2. Display All Products\n";
    cout << "3. Search Product by ID\n";
    cout << "4. Remove Product\n";
    cout << "5. Add Supplier\n";
    cout << "6. Display All Suppliers\n";
    cout << "7. Search Supplier by ID\n";
    cout << "8. Add Stock\n";
    cout << "9. Display All Stocks\n";
    cout << "10. Sort Products\n";
    cout << "11. Sort Stocks\n";
    cout << "12. Save All Data\n";
    cout << "13. Load All Data\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    ProductBST products;
    SupplierList suppliers;
    StockList stocks;

    const string productFile = "products.txt";
    const string supplierFile = "suppliers.txt";
    const string stockFile = "stocks.txt";

    bool running = true;
    while (running) {
        displayMainMenu();
        int choice;
        cin >> choice;
        cin.ignore(); // clear newline

        try {
            switch (choice) {
                case 1: {
                    // Add Product
                    int id; string name, category;
                    double price;
                    cout << "Enter Product ID: "; cin >> id; cin.ignore();
                    cout << "Enter Product Name: "; getline(cin, name);
                    cout << "Enter Product Price: "; cin >> price; cin.ignore();
                    cout << "Enter Product Category: "; getline(cin, category);

                    products.insert(Product(id, name, price, category));
                    cout << "Product added successfully.\n";
                    break;
                }
                case 2:
                    products.displayAll();
                    break;

                case 3: {
                    // Search Product
                    int id;
                    cout << "Enter Product ID to search: ";
                    cin >> id; cin.ignore();

                    Product* p = products.search(id);
                    if (p) p->display();
                    else cout << "Product not found.\n";
                    break;
                }
                case 4: {
                    // Remove Product
                    int id;
                    cout << "Enter Product ID to remove: ";
                    cin >> id; cin.ignore();

                    products.remove(id);
                    cout << "Product removed successfully.\n";
                    break;
                }
                case 5: {
                    // Add Supplier
                    int id; string name, contact;
                    cout << "Enter Supplier ID: "; cin >> id; cin.ignore();
                    cout << "Enter Supplier Name: "; getline(cin, name);
                    cout << "Enter Supplier Contact Info: "; getline(cin, contact);

                    suppliers.addSupplier(Supplier(id, name, contact));
                    cout << "Supplier added successfully.\n";
                    break;
                }
                case 6:
                    suppliers.displayAll();
                    break;

                case 7: {
                    // Search Supplier
                    int id;
                    cout << "Enter Supplier ID to search: ";
                    cin >> id; cin.ignore();

                    Supplier* s = suppliers.findSupplier(id);
                    if (s) s->display();
                    else cout << "Supplier not found.\n";
                    break;
                }
                case 8: {
                    // Add Stock
                    int pID, sID, qty;
                    cout << "Enter Product ID: "; cin >> pID; cin.ignore();
                    cout << "Enter Supplier ID: "; cin >> sID; cin.ignore();
                    cout << "Enter Quantity: "; cin >> qty; cin.ignore();

                    // Check product and supplier existence
                    if (!products.search(pID)) {
                        cout << "Product ID not found.\n";
                        break;
                    }
                    if (!suppliers.findSupplier(sID)) {
                        cout << "Supplier ID not found.\n";
                        break;
                    }

                    stocks.addStock(Stock(pID, sID, qty));
                    cout << "Stock added/updated successfully.\n";
                    break;
                }
                case 9:
                    stocks.displayAll();
                    break;

                case 10: {
                    // Sort Products using bubble sort
                    int n = products.getCount();
                    if (n == 0) {
                        cout << "No products to sort.\n";
                        break;
                    }
                    Product* arr[n];
                    products.getAllProducts(arr, n);

                    bubbleSortProducts(arr, n);
                    cout << "--- Products Sorted by ID ---\n";
                    for (int i=0; i<n; i++) {
                        arr[i]->display();
                    }
                    cout << "-----------------------------\n";
                    break;
                }
                case 11: {
                    // Sort Stocks by quantity (merge sort descending)
                    int n = stocks.count();
                    if (n == 0) {
                        cout << "No stocks to sort.\n";
                        break;
                    }
                    Stock* arr[n];
                    stocks.getAllStocks(arr, n);

                    mergeSortStocks(arr, 0, n-1);
                    cout << "--- Stocks Sorted by Quantity ---\n";
                    for (int i=0; i<n; i++) {
                        arr[i]->display();
                    }
                    cout << "----------------------------------------\n";
                    break;
                }
                case 12: {
                    // Save all data
                    saveProductsToFile(products, productFile);
                    saveSuppliersToFile(suppliers, supplierFile);
                    saveStocksToFile(stocks, stockFile);
                    cout << "Data saved successfully.\n";
                    break;
                }
                case 13: {
                    // Load all data
                    products = ProductBST();   // clear existing
                    suppliers = SupplierList();
                    stocks = StockList();

                    loadProductsFromFile(products, productFile);
                    loadSuppliersFromFile(suppliers, supplierFile);
                    loadStocksFromFile(stocks, stockFile);

                    cout << "Data loaded successfully.\n";
                    break;
                }
                case 0:
                    running = false;
                    cout << "Exiting program.\n";
                    break;

                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
