#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

class Product {
protected:
    int Id;
    string Name;
    string Brand;
    double Price;
    int Quantity;

public:
    Product(int id, const string& name, const string& brand, double price, int quantity)
        : Id(id), Name(name), Brand(brand), Price(price), Quantity(quantity) {
        //ensuring correct input
        if (Name.empty()) {
            throw invalid_argument("Name cannot be empty!");
        }
        if (Brand.empty()) {
            throw invalid_argument("Brand cannot be empty!");
        }
        if (Price <= 0) {
            throw invalid_argument("Invalid price for a product!");
        }
        if (Quantity < 0) {
            throw invalid_argument("Quantity cannot be negative!");
        }
    }

    virtual ~Product() {}

    virtual int getWarranty() const {
        return 1;
    }

     double getTotalPrice() const {
        return Price * Quantity;
    }

    string getName() const {
        return Name;
    }
    int getId()const {
        return Id;
    }
    double getPrice() {
        return Price;
    }
    
};

class Component : public Product {
protected:
    int clockSpeed;
    int cores;
    int memorySize;

public:
    Component(int id, const string& name, const string& brand, double price, int quantity,
        int clockSpeed, int cores, int memorySize)
        : Product(id, name, brand, price, quantity),
        clockSpeed(clockSpeed), cores(cores), memorySize(memorySize) {}

    int getWarranty() const override {
        int warranty = Product::getWarranty();
        if (cores > 4) {
            warranty++;
        }
        if (clockSpeed > 3) {
            warranty++;
        }
        if (memorySize >= 16) {
            warranty++;
        }
        return warranty;
    }
};

class Peripheral : public Product {
protected:
    string connectionType;
    int dpi;
    double screenSize;

public:
    Peripheral(int id, const string& name, const string& brand, double price, int quantity,
        const string& connectionType, int dpi, double screenSize)
        : Product(id, name, brand, price, quantity),
        connectionType(connectionType), dpi(dpi), screenSize(screenSize) {}

    int getWarranty() const override {

        int warranty = Product::getWarranty();
        if (screenSize > 24) {
            warranty++;
        }
        if (dpi > 1000) {
            warranty++;
        }
        if (connectionType == "Wireless") {
            warranty++;
        }
        return warranty;
    }
};


class BoyerMoore {
private:
    string pattern;
    int badChar[256];

    void preprocessBadChar() {
        fill(begin(badChar), end(badChar), -1);
        for (int i = 0; i < pattern.size(); ++i) {
            badChar[(unsigned char)pattern[i]] = i;
        }
    }

public:
    BoyerMoore(const string& pat) : pattern(pat) {
        preprocessBadChar();
    }

    int search(const string& text) {
        int m = pattern.size();
        int n = text.size();
        int shift = 0;

        while (shift <= n - m) {
            int j = m - 1;

            while (j >= 0 && pattern[j] == text[shift + j]) {
                j--;
            }

            if (j < 0) {
                return shift;
            }
            else {
                shift += max(1, j - badChar[(unsigned char)text[shift + j]]);
            }
        }

        return -1;
    }
};

void insertionSort(vector<Product*>& a, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        Product* tmp = a[i];
        int j = i;
        while (j > low && tmp->getTotalPrice() < a[j - 1]->getTotalPrice()) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = tmp;
    }
}

void quickSort(vector<Product*>& a, int low, int high) {
    if (low + 10 > high) {
        insertionSort(a, low, high);
    }
    else {
        int mid = (low + high) / 2;
        if (a[mid]->getTotalPrice() < a[low]->getTotalPrice()) 
            swap(a[low], a[mid]);
        if (a[high]->getTotalPrice() < a[low]->getTotalPrice())
            swap(a[low], a[high]);
        if (a[high]->getTotalPrice() < a[mid]->getTotalPrice()) 
            swap(a[mid], a[high]);

        swap(a[mid], a[high - 1]);
        Product* pivot = a[high - 1];

        int i = low, j = high - 1;
        while (true) {
            while (a[++i]->getTotalPrice() < pivot->getTotalPrice()) {}
            while (pivot->getTotalPrice() < a[--j]->getTotalPrice()) {}
            if (i < j) swap(a[i], a[j]);
            else break;
        }

        swap(a[i], a[high - 1]);

        quickSort(a, low, i - 1);
        quickSort(a, i + 1, high);
    }
}

void quickSort(vector<Product*>& products) {
    if (!products.empty())
        quickSort(products, 0, products.size() - 1);
}

int main() {
    vector<Product*> products;
    fstream myFile("products.txt");

    if (!myFile.is_open()) {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    string type;
    while (myFile >> type) {//keep reading the file
        if (type == "Component") {
            int id, quantity, clockSpeed, cores, memorySize;
            string name, brand;
            double price;
            myFile >> id >> name >> brand >> price >> quantity >> clockSpeed >> cores >> memorySize;//getting the input from the file
            replace(name.begin(), name.end(), '_', ' ');//ensuring the correct name is processed, for example Ryzen 5 will be read as two strings, so using this it will read it as one
            replace(brand.begin(), brand.end(), '_', ' ');//same process here
            products.push_back(new Component(id, name, brand, price, quantity, clockSpeed, cores, memorySize));//updating the collection
        }
        else if (type == "Peripheral") {
            int id, quantity, dpi;
            string name, brand, connectionType;
            double price, screenSize;
            myFile >> id >> name >> brand >> price >> quantity >> connectionType >> dpi >> screenSize;
            replace(name.begin(), name.end(), '_', ' ');
            replace(brand.begin(), brand.end(), '_', ' ');
            products.push_back(new Peripheral(id, name, brand, price, quantity, connectionType, dpi, screenSize));
        }
        

    }

    myFile.close();

   

    string input;
    cout << "Search for a product: ";
    
    getline(cin, input);

    if (!input.empty()) {
        BoyerMoore bm(input);

        cout << "\nProducts found \"" << input << "\":\n"; 
        bool found = false;

        for (Product* p : products) {
            if (bm.search(p->getName()) != -1) {//check if BM algorithm gets a match
                cout << "- " << left << setw(20) << p->getName()
                    << " | Price: $" << fixed << setprecision(2) << p->getPrice()
                    <<"  | ID: "<< p->getId()
                    << endl;
                found = true;
            }
        }

        if (!found) {//if no products were found a message is displayed and the whole collection of products in the store is displayed
            cout << "No matching products found.\n";
        }
    }
    else {
        cout << "\nNo keyword entered. Skipping search.\n";
    }



    quickSort(products);
    cout << "\nAll products in the store:\n";
    for (Product* p : products) {
        cout << left << setw(20) << p->getName()
            
            << " | Total: $" << fixed << setprecision(2) << p->getTotalPrice()
            << " | Warranty: " << p->getWarranty() << " yrs\n";
    }

    for (Product* p : products) {
        delete p;
    }

    return 0;
}


