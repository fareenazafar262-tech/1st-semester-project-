
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
using namespace std;

// Colors
string green = "\033[32m";
string red = "\033[31m";
string white = "\033[37m";
string cyan = "\033[36m";
string yellow = "\033[33m";
string blue = "\033[34m";
string purple = "\033[35m";
string reset = "\033[0m";

// Mobile Phone Arrays (Max 100)
string phoneBrands[100];
string phoneModels[100];
string phonePrices[100];
string phoneQuantities[100];
string phoneSpecs[100];
int phoneCount = 0;

// Sales Arrays
string saleCustomers[100];
string salePhones[100];
string saleQuantities[100];
string salePrices[100];
int saleCount = 0;

// Global variables for users
string usernames[100];
string passwords[100];
string roles[100];
int userCount = 0;
string currentUser = "";
string currentRole = "";

// ==================== FUNCTION DECLARATIONS (BEFORE main) ====================
void showBanner();
void myPause();
void clearScreen();
void showMainMenu();
void showSignInScreen();
bool checkUsername(string username);
bool checkPassword(string password);
void registerUser(string roleType);
void showDashboard(string username, string role);
void showSignUpMenu();
bool signIn(string &currentUser, string &currentRole);
void ownerMenu();
void addPhone();
void viewPhones();
void updatePhone();
void deletePhone();
void viewCustomers();
void viewSales();
void customerMenu(string username);
void viewAvailablePhones();
void buyPhone(string customerName);
bool isValidNumber(const string& str);
int safeStringToInt(const string& str);
string getNumInput(string prompt);
void loadUsersFromFile();
void saveUsersToFile();
void loadPhonesFromFile();
void savePhonesToFile();
void loadSalesFromFile();
void saveSalesToFile();

// ==================== MAIN FUNCTION ====================
int main() {
    string choice;
    
    // Load data from files
    loadUsersFromFile();
    loadPhonesFromFile();
    loadSalesFromFile();
    
    // Show banner only once
    showBanner();
    myPause();
    clearScreen();
    
    // Main program loop
    while(true) {
        showMainMenu();
        cout << cyan << "Choose: " << reset;
        cin >> choice;
        
        if(choice == "2") {
            // SIGN IN
            if(signIn(currentUser, currentRole)) {
                // Successful login
                showDashboard(currentUser, currentRole);
                
                // Show appropriate menu
                if(currentRole == "Owner") {
                    ownerMenu();
                } else {
                    customerMenu(currentUser);
                }
                
                // Save data when returning from menu
                savePhonesToFile();
                saveSalesToFile();
                myPause();
                clearScreen();
            } else {
                // Failed login
                cout << red << "\n Login failed! Invalid username or password.\n" << reset;
                myPause();
                clearScreen();
            }
        }
        else if(choice == "1") {
            // SIGN UP
            string signUpChoice;
            showSignUpMenu();
            cout << cyan << "Choose: " << reset;
            cin >> signUpChoice;
            
            if(signUpChoice == "1") {
                // Only allow ONE owner (prevent multiple owners)
                bool ownerExists = false;
                for(int i = 0; i < userCount; i++) {
                    if(roles[i] == "Owner") {
                        ownerExists = true;
                        break;
                    }
                }
                
                if(ownerExists) {
                    cout << red << "\nOwner already exists! Only one owner allowed.\n" << reset;
                } else {
                    registerUser("Owner");
                }
                myPause();
                clearScreen();
            }
            else if(signUpChoice == "2") {
                registerUser("Customer");
                myPause();
                clearScreen();
            }
            else if(signUpChoice == "3") {
                clearScreen();
                continue;
            }
            else {
                cout << red << "Invalid choice!\n" << reset;
                myPause();
                clearScreen();
            }
        }
        else if(choice == "3") {
            cout << green << "\nThank you for using Mobile Shop System!\n" << reset;
            // Save all data before exiting
            saveUsersToFile();
            savePhonesToFile();
            saveSalesToFile();
            break;
        }
        else {
            cout << red << "Invalid choice!\n" << reset;
            myPause();
            clearScreen();
        }
    }
    
    return 0;
}

// ==================== FILE HANDLING FUNCTIONS ====================

void loadUsersFromFile() {
    ifstream file("users.txt");
    if(!file.is_open()) return;
    
    userCount = 0;
    while(file >> usernames[userCount] >> passwords[userCount] >> roles[userCount]) {
        userCount++;
        if(userCount >= 100) break;
    }
    file.close();
}

void saveUsersToFile() {
    ofstream file("users.txt");
    if(!file.is_open()) {
        cout << red << "Error saving users to file!\n" << reset;
        return;
    }
    
    for(int i = 0; i < userCount; i++) {
        file << usernames[i] << " " << passwords[i] << " " << roles[i] << endl;
    }
    file.close();
}

void loadPhonesFromFile() {
    ifstream file("phones.txt");
    if(!file.is_open()) return;
    
    phoneCount = 0;
    while(getline(file, phoneBrands[phoneCount]) && 
          getline(file, phoneModels[phoneCount]) &&
          getline(file, phonePrices[phoneCount]) &&
          getline(file, phoneQuantities[phoneCount]) &&
          getline(file, phoneSpecs[phoneCount])) {
        phoneCount++;
        if(phoneCount >= 100) break;
    }
    file.close();
}

void savePhonesToFile() {
    ofstream file("phones.txt");
    if(!file.is_open()) {
        cout << red << "Error saving phones to file!\n" << reset;
        return;
    }
    
    for(int i = 0; i < phoneCount; i++) {
        file << phoneBrands[i] << endl
             << phoneModels[i] << endl
             << phonePrices[i] << endl
             << phoneQuantities[i] << endl
             << phoneSpecs[i] << endl;
    }
    file.close();
}

void loadSalesFromFile() {
    ifstream file("sales.txt");
    if(!file.is_open()) return;
    
    saleCount = 0;
    while(getline(file, saleCustomers[saleCount]) && 
          getline(file, salePhones[saleCount]) &&
          getline(file, saleQuantities[saleCount]) &&
          getline(file, salePrices[saleCount])) {
        saleCount++;
        if(saleCount >= 100) break;
    }
    file.close();
}

void saveSalesToFile() {
    ofstream file("sales.txt");
    if(!file.is_open()) {
        cout << red << "Error saving sales to file!\n" << reset;
        return;
    }
    
    for(int i = 0; i < saleCount; i++) {
        file << saleCustomers[i] << endl
             << salePhones[i] << endl
             << saleQuantities[i] << endl
             << salePrices[i] << endl;
    }
    file.close();
}

// ==================== HELPER FUNCTIONS ====================

bool isValidNumber(const string& str) {  
    if(str.empty()) return false;
    for(char c : str) {
        if(!isdigit(c)) return false;
    }
    return true;
}

int safeStringToInt(const string& str) {
    if(isValidNumber(str)) {
        return stoi(str);
    }
    return -1;
}

string getNumInput(string prompt) {
    string input;
    while(true) {
        cout << prompt;
        cin >> input;
        if(isValidNumber(input)) return input;
        cout << red << "Invalid! Enter numbers only.\n" << reset;
    }
}
// ======================Functions for sign in and sign up================== //
bool checkUsername(string username) {
    for(int i = 0; i < userCount; i++) {
        if(usernames[i] == username) {
            return true;
        }
    }
    return false;
}

bool checkPassword(string password) {
    if(password.length() < 6) {
        return false;
    }
    
    bool capital = false;
    bool number = false;
    
    for(int i = 0; i < password.length(); i++) {
        if(password[i] >= 'A' && password[i] <= 'Z') {
            capital = true;
        }
        if(password[i] >= '0' && password[i] <= '9') {
            number = true;
        }
    }
    
    return (capital && number);
}

// ==================== CUSTOMER MENU & FUNCTIONS ====================

void customerMenu(string username) {
    string choice;
    
    while(true) {
        cout << blue << "\n+========================================+\n" << reset;
        cout << blue << "|" << cyan << "          CUSTOMER MENU              " << blue << "|\n" << reset;
        cout << blue << "+========================================+\n" << reset;
        cout << blue << "|" << green << " 1. View Available Phones           " << blue << "|\n" << reset;
        cout << blue << "|" << green << " 2. Buy Phone                       " << blue << "|\n" << reset;
        cout << blue << "|" << red << " 3. Logout                          " << blue << "|\n" << reset;
        cout << blue << "+========================================+\n" << reset;
        
        cout << cyan << "\nChoose: " << reset;
        cin >> choice;
        
        if(choice == "1") viewAvailablePhones();
        else if(choice == "2") buyPhone(username);
        else if(choice == "3") { 
            cout << green << "\nLogging out...\n" << reset; 
            savePhonesToFile();
            saveSalesToFile();
            break; 
        }
        else cout << red << "Invalid choice!\n" << reset;
        
        myPause();
        clearScreen();
    }
}

void viewAvailablePhones() {
    cout << cyan << "\n=== AVAILABLE PHONES ===\n" << reset;
    
    if(phoneCount == 0) {
        cout << red << "No phones available.\n" << reset;
        return;
    }
    
    bool available = false;
    for(int i = 0; i < phoneCount; i++) {
        int qty = safeStringToInt(phoneQuantities[i]);
        if(qty > 0) {
            available = true;
            cout << i+1 << ". " << phoneBrands[i] << " " << phoneModels[i] 
                 << " - Rs" << phonePrices[i] << " (Stock: " << qty << ")\n";
            cout << "   Specs: " << phoneSpecs[i] << "\n\n";
        }
    }
    
    if(!available) {
        cout << red << "No phones in stock.\n" << reset;
    }
}
//============================Functions for customer to buy and view order summary========================//


void buyPhone(string customerName) {
    viewAvailablePhones();
    if(phoneCount == 0) return;
    
    string choice = getNumInput("\nEnter phone number to buy: ");
    int i = safeStringToInt(choice) - 1;
    
    if(i < 0 || i >= phoneCount) {
        cout << red << "Invalid selection!\n" << reset;
        return;
    }
    
    int stock = safeStringToInt(phoneQuantities[i]);
    if(stock <= 0) {
        cout << red << "This phone is out of stock!\n" << reset;
        return;
    }
    
    string qtyStr = getNumInput("Enter quantity: ");
    int qty = safeStringToInt(qtyStr);
    
    if(qty <= 0) {
        cout << red << "Invalid quantity!\n" << reset;
        return;
    }
    
    if(qty > stock) {
        cout << red << "Only " << stock << " units available!\n" << reset;
        return;
    }
    
    // Calculate total price
    double price = stod(phonePrices[i]);
    double total = price * qty;
    
    cout << cyan << "\n=== ORDER SUMMARY ===\n" << reset;
    cout << "Phone: " << phoneBrands[i] << " " << phoneModels[i] << "\n";
    cout << "Quantity: " << qty << "\n";
    cout << "Price per unit: Rs" << price << "\n";
    cout << "Total: Rs" << total << "\n";
    
    string confirm;
    cout << "\nConfirm purchase? (yes/no): ";
    cin >> confirm;
    
    if(confirm == "yes" || confirm == "y") {
        // Update stock
        int newStock = stock - qty;
        phoneQuantities[i] = to_string(newStock);
        
        // Record sale
        if(saleCount < 100) {
            saleCustomers[saleCount] = customerName;
            salePhones[saleCount] = phoneBrands[i] + " " + phoneModels[i];
            saleQuantities[saleCount] = to_string(qty);
            salePrices[saleCount] = to_string(total);
            saleCount++;
        }
        
        cout << green << "\nPurchase successful! Thank you for shopping.\n" << reset;
    } else {
        cout << yellow << "\nPurchase cancelled.\n" << reset;
    }
}

// ==================== OWNER MENU & FUNCTIONS ====================

void ownerMenu() {
    string choice;
    
    while(true) {
        cout << yellow << "\n+========================================+\n" << reset;
        cout << yellow << "|" << cyan << "           OWNER MENU                " << yellow << "|\n" << reset;
        cout << yellow << "+========================================+\n" << reset;
        cout << yellow << "|" << green << " 1. Add Mobile Phone                " << yellow << "|\n" << reset;
        cout << yellow << "|" << green << " 2. View All Phones                 " << yellow << "|\n" << reset;
        cout << yellow << "|" << green << " 3. Update Phone                    " << yellow << "|\n" << reset;
        cout << yellow << "|" << green << " 4. Delete Phone                    " << yellow << "|\n" << reset;
        cout << yellow << "|" << green << " 5. View Customers                  " << yellow << "|\n" << reset;
        cout << yellow << "|" << green << " 6. View Sales                      " << yellow << "|\n" << reset;
        cout << yellow << "|" << red << " 7. Logout                          " << yellow << "|\n" << reset;
        cout << yellow << "+========================================+\n" << reset;
        
        cout << cyan << "\nChoose: " << reset;
        cin >> choice;
        
        if(choice == "1") addPhone();
        else if(choice == "2") viewPhones();
        else if(choice == "3") updatePhone();
        else if(choice == "4") deletePhone();
        else if(choice == "5") viewCustomers();
        else if(choice == "6") viewSales();
        else if(choice == "7") { 
            cout << green << "\nLogging out...\n" << reset; 
            savePhonesToFile();
            saveSalesToFile();
            break; 
        }
        else cout << red << "Invalid choice!\n" << reset;
        
        myPause();
        clearScreen();
    }
}

void addPhone() {
    if(phoneCount >= 100) {
        cout << red << "Phone storage full!\n" << reset;
        return;
    }
    
    cout << cyan << "\n=== ADD NEW PHONE ===\n" << reset;
    
    cin.ignore();
    cout << "Brand: ";
    getline(cin, phoneBrands[phoneCount]);
    
    cout << "Model: ";
    getline(cin, phoneModels[phoneCount]);
    
    cout << "Price: Rs";
    cin >> phonePrices[phoneCount];
    
    phoneQuantities[phoneCount] = getNumInput("Quantity: ");
    
    cin.ignore();
    cout << "Specifications: ";
    getline(cin, phoneSpecs[phoneCount]);
    
    phoneCount++;
    cout << green << "\nPhone added successfully!\n" << reset;
    savePhonesToFile();
}

void viewPhones() {
    cout << cyan << "\n=== ALL PHONES ===\n" << reset;
    
    if(phoneCount == 0) {
        cout << red << "No phones in inventory.\n" << reset;
        return;
    }
    
    for(int i = 0; i < phoneCount; i++) {
        cout << i+1 << ". " << phoneBrands[i] << " " << phoneModels[i] 
             << " - Rs" << phonePrices[i] << " (Qty: " << phoneQuantities[i] << ")\n";
        cout << "   Specs: " << phoneSpecs[i] << "\n\n";
    }
}

void updatePhone() {
    viewPhones();
    if(phoneCount == 0) return;
    
    string choice = getNumInput("\nEnter phone number to update: ");
    int idx = safeStringToInt(choice) - 1;
    
    if(idx < 0 || idx >= phoneCount) {
        cout << red << "Invalid selection!\n" << reset;
        return;
    }
    
    cout << cyan << "\n=== UPDATE PHONE ===\n" << reset;
    cin.ignore();
    cout << "New Brand (" << phoneBrands[idx] << "): ";
    getline(cin, phoneBrands[idx]);
    
    cout << "New Model (" << phoneModels[idx] << "): ";
    getline(cin, phoneModels[idx]);
    
    cout << "New Price (Rs." << phonePrices[idx] << "): Rs.";
    cin >> phonePrices[idx];
    
    phoneQuantities[idx] = getNumInput("New Quantity (" + phoneQuantities[idx] + "): ");
    
    cin.ignore();
    cout << "New Specifications (" << phoneSpecs[idx] << "): ";
    getline(cin, phoneSpecs[idx]);
    
    cout << green << "\nPhone updated successfully!\n" << reset;
    savePhonesToFile();
}

void deletePhone() {
    viewPhones();
    if(phoneCount == 0) return;
    
    string choice = getNumInput("\nEnter phone number to delete: ");
    int idx = safeStringToInt(choice) - 1;
    
    if(idx < 0 || idx >= phoneCount) {
        cout << red << "Invalid selection!\n" << reset;
        return;
    }
    
    // Shift all phones after deleted one
    for(int i = idx; i < phoneCount-1; i++) {
        phoneBrands[i] = phoneBrands[i+1];
        phoneModels[i] = phoneModels[i+1];
        phonePrices[i] = phonePrices[i+1];
        phoneQuantities[i] = phoneQuantities[i+1];
        phoneSpecs[i] = phoneSpecs[i+1];
    }
    
    phoneCount--;
    cout << green << "\nPhone deleted successfully!\n" << reset;
    savePhonesToFile();
}

void viewCustomers() {
    cout << cyan << "\n=== ALL CUSTOMERS ===\n" << reset;
    
    int customerCount = 0;
    for(int i = 0; i < userCount; i++) {
        if(roles[i] == "Customer") {
            customerCount++;
            cout << customerCount << ". " << usernames[i] << "\n";
        }
    }
    
    if(customerCount == 0) {
        cout << red << "No customers registered.\n" << reset;
    } else {
        cout << "\nTotal Customers: " << customerCount << "\n";
    }
}

void viewSales() {
    cout << cyan << "\n=== SALES REPORT ===\n" << reset;
    
    if(saleCount == 0) {
        cout << red << "No sales recorded.\n" << reset;
        return;
    }
    
    double totalRevenue = 0;
    for(int i = 0; i < saleCount; i++) {
        cout << i+1 << ". Customer: " << saleCustomers[i] 
             << " | Phone: " << salePhones[i]
             << " | Qty: " << saleQuantities[i]
             << " | Price: Rs" << salePrices[i] << "\n";
        
        double price = stod(salePrices[i]);
        int qty = safeStringToInt(saleQuantities[i]);
        totalRevenue += (price * qty);
    }
    
    cout << "\n" << green << "Total Revenue: Rs" << totalRevenue << reset << "\n";
}

// ==================== SIGN IN/SIGN UP FUNCTIONS ====================

void showBanner() {
   cout << white << "            ______    _____  ______         _____  " << cyan << "              ____          ______    _____           ____          ______         " << endl;
    cout << white << "|\\    / |  |      |  |     |    |    |     |      " << cyan << "   |\\    /|   /    \\  |\\  |  |    |   |       |\\    /| |       |\\  |   |      " << endl;
    cout << white << "| \\  /  |  |      |  |_____|    |    |     |____ " << cyan << "    | \\  / |   |----|  | \\ |  |----|   |   ___ | \\  / | |___    | \\ |   |       " << endl;
    cout << white << "|  \\/   |  |      |  |     |    |    |     |     " << cyan << "    |  \\/  |   |    |  |  \\|  |    |   |     | |  \\/  | |       |  \\|   |        " << endl;
    cout << white << "|       |  |______|  |_____| ___|__  |____ |_____  " << cyan << "  |      |   |    |  |   |  |    |   |_____| |      | |___    |   |   |             " << green << endl;
}

void myPause() {
    cout << purple << "\nPress Enter to continue..." << reset;
    cin.ignore();
    cin.get();
}

void clearScreen() {
    for(int i = 0; i < 50; i++) {
        cout << endl;
    }
}

void showMainMenu() {
    cout << cyan << "\n===== MOBILE SHOP SYSTEM =====\n" << reset << endl;
    cout << red << "FIRSTLY you will have to sign up then sign in\n" << reset << endl;
    cout << "1. Sign Up\n";
    cout << "2. Sign In\n";
    cout << "3. Exit\n";
}

void showSignInScreen() {
    cout << blue << "\n+--------------------------------+\n" << reset;
    cout << blue << "|" << cyan << "          SIGN IN PAGE          " << blue << "|\n" << reset;
    cout << blue << "|--------------------------------|\n" << reset;
}

void showSignUpMenu() {
    cout << cyan << "\n===== SIGN UP =====\n" << reset;
    cout << "1. Register as Owner\n";
    cout << "2. Register as Customer\n";
    cout << "3. Back to Main Menu\n";
}

void registerUser(string roleType) {
    string username, password;
    
    cout << cyan << "\n=== " << roleType << " Registration ===\n" << reset;
    
    // Get username
    cout << blue << "Username: " << reset;
    cin >> username;
    
    // Check if username exists
    if(checkUsername(username)) {
        cout << red << "\n? Username already taken!\n" << reset;
        return;
    }
    
    // Get password
    cout << blue << "Password must have 6+ chars, 1 capital, 1 number\n" << reset;
    cout << blue << "Password: " << reset;
    cin >> password;
    
    // Validate password
    if(!checkPassword(password)) {
        cout << red << "\n Weak password! Try again.\n" << reset;
        return;
    }
    
    // Store user data
    usernames[userCount] = username;
    passwords[userCount] = password;
    roles[userCount] = roleType;
    userCount++;
    
    // Save to file
    saveUsersToFile();
    
    // Success message
    cout << green << "\n Registration successful!\n" << reset;
}

bool signIn(string &currentUser, string &currentRole) {
    string username, password;
    
    // Show sign in screen
    showSignInScreen();
    
    // Get username
    cout << blue << "|" << cyan << " Username: " << reset;
    cin >> username;
    
    // Get password
    cout << blue << "|" << cyan << " Password: " << reset;
    cin >> password;
    cout << blue << "+--------------------------------+\n" << reset;
    
    // Check credentials
    for(int i = 0; i < userCount; i++) {
        if(usernames[i] == username && passwords[i] == password) {
            currentUser = username;
            currentRole = roles[i];
            return true; // Login successful
        }
    }
    
    return false; // Login failed
}

void showDashboard(string username, string role) {
    clearScreen();
    
    if(role == "Owner") {
        cout << yellow << "\n+--------------------------------------+\n" << reset;
        cout << yellow << "|" << cyan << "        SHOP OWNER DASHBOARD       " << yellow << "|\n" << reset;
        cout << yellow << "|--------------------------------------|\n" << reset;
        cout << yellow << "|" << green << " Welcome, " << username << "!" << yellow << "                 |\n" << reset;
        cout << yellow << "+--------------------------------------+\n" << reset;
    } else {
        cout << blue << "\n+--------------------------------------+\n" << reset;
        cout << blue << "|" << cyan << "        CUSTOMER DASHBOARD          " << blue << "|\n" << reset;
        cout << blue << "|--------------------------------------|\n" << reset;
        cout << blue << "|" << green << " Welcome, " << username << "!" << blue << "                 |\n" << reset;
        cout << blue << "+--------------------------------------+\n" << reset;
    }
}
