#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

// ----------------------------------------------------
// Class to represent a single item in the vending machine
// ----------------------------------------------------
class Item {
private:
    int code;            // Unique item selection code (e.g. 101, 201)
    string name;         // Name of the product
    string category;     // Category (Hot Drinks, Snacks, etc.)
    double price;        // Price in pounds (£)
    int stock;           // Number of items available

public:
    // Constructor to initialise item details
    Item(int c, const string& n, const string& cat, double p, int s)
        : code(c), name(n), category(cat), price(p), stock(s) {}

    // Getter functions (read-only access)
    int getCode() const { return code; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Check if item is available
    bool isInStock() const {
        return stock > 0;
    }

    // Reduce stock by one when an item is dispensed
    void dispenseOne() {
        if (stock > 0) {
            stock--;
        }
    }
};

// ----------------------------------------------------
// VendingMachine class – controls the whole system
// ----------------------------------------------------
class VendingMachine {
private:
    vector<Item> items;      // Stores all vending machine items
    double insertedMoney;    // Stores user's current balance (£)

    // Safely read an integer input from the user
    int getIntInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                return value;
            } else {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // Safely read a double input from the user
    double getDoubleInput(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                return value;
            } else {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

public:
    // Constructor – initialises items and starting balance
    VendingMachine() : insertedMoney(0.0) {
        // code, name, category, price (£), stock
        items.emplace_back(101, "Coffee",        "Hot Drinks",  1.50, 5);
        items.emplace_back(102, "Tea",           "Hot Drinks",  1.20, 5);
        items.emplace_back(201, "Cola",          "Cold Drinks", 1.00, 5);
        items.emplace_back(202, "Orange Juice",  "Cold Drinks", 1.30, 5);
        items.emplace_back(301, "Chocolate Bar", "Chocolate",   0.90, 5);
        items.emplace_back(302, "Biscuits",      "Snack",       0.80, 5);
        items.emplace_back(303, "Crisps",        "Snack",       0.70, 5);
    }

    // Display vending machine menu
    void displayMenu() const {
        cout << "\n================ VENDING MACHINE MENU ================\n";
        cout << fixed << setprecision(2);
        cout << "Current balance: £" << insertedMoney << "\n\n";

        cout << "Code  Category       Item                 Price (£) Stock\n";
        cout << "--------------------------------------------------------\n";
        for (const auto& item : items) {
            cout << item.getCode() << "   "
                 << setw(13) << left << item.getCategory().substr(0, 13) << " "
                 << setw(18) << left << item.getName().substr(0, 18) << " "
                 << "£" << setw(5) << right << item.getPrice() << "   "
                 << item.getStock() << "\n";
        }
        cout << "--------------------------------------------------------\n";
        cout << "Enter 0 to finish and get your change (£).\n";
    }

    // Find an item by its code
    Item* findItemByCode(int code) {
        for (auto& item : items) {
            if (item.getCode() == code) {
                return &item;
            }
        }
        return nullptr;
    }

    // Allow user to insert money (£)
    void insertMoney() {
        cout << fixed << setprecision(2);
        double amount = getDoubleInput("Insert money (£). Enter 0 to stop: £");

        while (amount < 0) {
            cout << "Amount cannot be negative.\n";
            amount = getDoubleInput("Insert money again (£): £");
        }

        while (amount > 0.0) {
            insertedMoney += amount;
            cout << "You inserted £" << amount
                 << ". Total balance: £" << insertedMoney << "\n";

            amount = getDoubleInput("Insert more money or 0 to stop (£): £");
            while (amount < 0) {
                cout << "Amount cannot be negative.\n";
                amount = getDoubleInput("Insert money again (£): £");
            }
        }
    }

    // Show items affordable with remaining balance
    void showAffordableItems() const {
        cout << "\n--- Items you can afford with your balance (£"
             << fixed << setprecision(2) << insertedMoney << ") ---\n";

        bool any = false;
        for (const auto& item : items) {
            if (item.isInStock() && item.getPrice() <= insertedMoney) {
                any = true;
                cout << "Code " << item.getCode()
                     << " - " << item.getName()
                     << " (£" << item.getPrice()
                     << ") | Stock: " << item.getStock() << "\n";
            }
        }

        if (!any) {
            cout << "No items available within your balance.\n";
        }
        cout << "------------------------------------------------------\n";
    }

    // Suggest another product (basic intelligence)
    void suggestPurchase(const Item& lastItem) {
        cout << "\n--- Purchase Suggestion ---\n";

        if (lastItem.getCategory() == "Hot Drinks") {
            for (auto& item : items) {
                if (item.getName() == "Biscuits" && item.isInStock()) {
                    cout << "You might also like: "
                         << item.getName() << " (£"
                         << item.getPrice() << ")\n";
                    return;
                }
            }
        } else {
            for (auto& item : items) {
                if (item.getName() == "Coffee" && item.isInStock()) {
                    cout << "You might also like: "
                         << item.getName() << " (£"
                         << item.getPrice() << ")\n";
                    return;
                }
            }
        }

        cout << "No suggestions available.\n";
    }

    // Purchasing loop
    void purchaseItems() {
        while (true) {
            displayMenu();
            int code = getIntInput("Enter item code (0 to finish): ");

            if (code == 0) break;

            Item* selected = findItemByCode(code);
            if (!selected) {
                cout << "Invalid code.\n";
                continue;
            }

            if (!selected->isInStock()) {
                cout << selected->getName() << " is OUT OF STOCK.\n";
                continue;
            }

            if (insertedMoney < selected->getPrice()) {
                cout << "\nMoney is not sufficient to buy product, please insert more\n";
                cout << "Item price: £" << selected->getPrice()
                     << ", Your balance: £" << insertedMoney << "\n";
                continue;
            }

            // Complete purchase
            insertedMoney -= selected->getPrice();
            selected->dispenseOne();

            cout << "\nDispensing: " << selected->getName() << "\n";
            cout << "Remaining balance: £" << insertedMoney << "\n";

            suggestPurchase(*selected);

            char again;
            cout << "Buy another item? (y/n): ";
            cin >> again;
            if (again != 'y' && again != 'Y') break;
        }
    }

    // Return change
    void returnChange() {
        cout << "\n================ TRANSACTION COMPLETE ================\n";
        cout << "Your change: £" << fixed << setprecision(2)
             << insertedMoney << "\n";
        cout << "Thank you for using the vending machine!\n";
        insertedMoney = 0.0;
    }

    // Run full program
    void run() {
        cout << "Welcome to the C++ Vending Machine (£ System)\n\n";
        insertMoney();
        purchaseItems();
        returnChange();
    }
};

// ----------------------------------------------------
// Main function – program entry point
// ----------------------------------------------------
int main() {
    VendingMachine vm;
    vm.run();
    return 0;
}
