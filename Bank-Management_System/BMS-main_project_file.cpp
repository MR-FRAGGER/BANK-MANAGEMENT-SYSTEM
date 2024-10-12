#include <iostream>                                 // Include input/output stream header
#include <fstream>                                  // Include file stream header for file operations
#include <cctype>                                   // Include header for character handling functions

#include <iomanip>                                  // Include header for formatting output
/*The #include <iomanip> directive is used to include the Input/Output Manipulation header 
file in C++. This header file provides functionality for formatting and manipulating input 
and output in a C++ program. It introduces the std::setw, std::setprecision, std::setfill, 
and other formatting manipulators.*/
//                                                  //
#include <cstring>                                  // Include C-style string header for string functions
#include <windows.h>                                // Include Windows-specific header for system commands

using namespace std;                                // Use standard namespace cout<< instead of std::cout<<

const char MANAGER_USERNAME[] = "manager";          // Define manager's username
const char MANAGER_PASSWORD[] = "password";         // Define manager's password
bool manager_login();                               // Function declaration for manager login

//_____________________________________________________________________________________________________________________________________________________________________________________
// CLASS ACCOUNT
//_____________________________________________________________________________________________________________________________________________________________________________________

class account {
    int acno;                                       // Account number
    char name[50];                                  // Account holder's name
    int deposit;                                    // Account balance
    char type;                                      // Account type (Savings or Current)

public:
    void create_account();                          // Function to create a new account
    void show_account() const;                      // Function to display account details
    void modify();                                  // Function to modify account details
    void dep(int);                                  // Function to handle deposit transactions
    void draw(int);                                 // Function to handle withdrawal transactions
    void report() const;                            // Function to display account details in report format
    int retacno() const;                            // Function to return the account number
    int retdeposit() const;                         // Function to return the account balance
    char rettype() const;                           // Function to return the account type
};

//____________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION DECLARATION
//____________________________________________________________________________________________________________________________________________________________________________________

void write_account();                               // Function to write account details to file
void display_all();                                 // Function to display details of all accounts
void delete_account(int);                           // Function to delete an account
void modify_account(int);                           // Function to modify account details
void manager_menu();                                // Function for manager's main menu

void deposit_withdraw(int, int);                    // Function for deposit or withdrawal transactions
void display_sp(int);                               // Function to display specific account details
void customer_menu();                               // Function for customer's main menu
void intro();                                       // Function to display program introduction

//_____________________________________________________________________________________________________________________________________________________________________________________
// MAIN
//_____________________________________________________________________________________________________________________________________________________________________________________

int main() {
    char ch;  // User's choice
    intro();  // Display program introduction

    do {
        system("cls");  // Clear console screen

        cout << "\n\n\n\t\033[1;91m=== MAIN MENU ===\033[0m";         // Display main menu header in Bold Red text
        cout << "\n\n\t\033[1;37m1. LOGIN AS MANAGER\033[0m";         // Display manager login option in Bold Light Gray text
        cout << "\n\n\t\033[1;37m2. LOGIN AS CUSTOMER\033[0m";        // Display customer login option in Bold Light Gray text
        cout << "\n\n\t\033[1;37m3. EXIT\033[0m";                     // Display exit option in Bold Light Gray text
        cout << "\n\n\t\033[1;37mSelect Your Option (1-3): \033[0m";  // Prompt user for choice in Bold Light Gray text
        cin >> ch;

        system("cls");  // Clear console screen
        switch (ch) {
            case '1':
                // Check manager credentials
                if (manager_login()) {
                    manager_menu();  // Display manager menu
                } else {
                    cout << "\n\n\tInvalid manager credentials. Access denied.";
                    cin.ignore();
                    cin.get();
                }
                break;
            case '2':
                customer_menu();  // Display customer menu
                break;
            case '3':
                cout << "\n\n\tThanks for using the Fragger's bank management system. We wish to see you again";  // Display exit message
                break;
            default:
                cout << "\a";  // Beep for invalid input
        }

        cin.ignore();  // Ignore newline character from previous input
        cin.get();  // Wait for user to press Enter
    } while (ch != '3');  // Repeat until user chooses to exit

    return 0;  // Return 0 to indicate successful program execution
}

//_____________________________________________________________________________________________________________________________________________________________________________________
// MANAGER'S MAIN MENU
//_____________________________________________________________________________________________________________________________________________________________________________________

void manager_menu() {
    char ch;  // User's choice
    int num;  // Account number

    do {
        system("cls");  // Clear console screen

        cout << "\n\n\n\t\033[1;91mMANAGER MENU\033[0m";              // Display manager menu header in Bold Red text
        cout << "\n\n\t\033[1;37m1. NEW ACCOUNT\033[0m";              // Display option to create a new account in Bold Light Gray text
        cout << "\n\n\t\033[1;37m2. ALL ACCOUNT HOLDER LIST\033[0m";  // Display option to display all accounts in Bold Light Gray text
        cout << "\n\n\t\033[1;37m3. CLOSE AN ACCOUNT\033[0m";         // Display option to close an account in Bold Light Gray text
        cout << "\n\n\t\033[1;37m4. MODIFY AN ACCOUNT\033[0m";        // Display option to modify an account in Bold Light Gray text
        cout << "\n\n\t\033[1;37m5. RETURN TO MAIN MENU\033[0m";      // Display option to return to main menu in Bold Light Gray text
        cout << "\n\n\t\033[1;37m6. EXIT\033[0m";                     // Display exit option in Bold Light Gray text
        cout << "\n\n\t\033[1;37mSelect Your Option (1-6): \033[0m";  // Prompt user for choice in Bold Light Gray text
        cin >> ch;

        system("cls");  // Clear console screen
        switch (ch) {
            case '1':
                write_account();  // Create a new account
                break;
            case '2':
                display_all();  // Display details of all accounts
                break;
            case '3':
                cout << "\n\n\tEnter The account No. : ";
                cin >> num;
                delete_account(num);// Delete an account with the specified account number
                break;
            case '4':
                cout << "\n\n\tEnter The account No. : ";
                cin >> num;
                modify_account(num);  // Modify details of an account with the specified account number
                break;
            case '5':
                return;  // Return to the main menu
            case '6':
                cout << "\n\n\tThanks for using the Fragger's bank management system";  // Display exit message
                break;
            default:
                cout << "\a";  // Beep for invalid input
        }

        cin.ignore();  // Ignore newline character from previous input
        cin.get();  // Wait for user to press Enter
    } while (ch != '6');  // Repeat until user chooses to exit
}

//_____________________________________________________________________________________________________________________________________________________________________________________
// CUSTOMER'S MAIN MENU
//_____________________________________________________________________________________________________________________________________________________________________________________

void customer_menu() {
    char ch;  // User's choice
    int num;  // Account number

    do {
        system("cls");  // Clear console screen

        cout << "\n\n\n\t\033[1;91mCUSTOMER MENU\033[0m";             // Display customer menu header in Bold Red text
        cout << "\n\n\t\033[1;37m1. DEPOSIT AMOUNT\033[0m";           // Display option to deposit amount in Bold Light Gray text
        cout << "\n\n\t\033[1;37m2. WITHDRAW AMOUNT\033[0m";          // Display option to withdraw amount in Bold Light Gray text
        cout << "\n\n\t\033[1;37m3. BALANCE ENQUIRY\033[0m";          // Display option to check balance in Bold Light Gray text
        cout << "\n\n\t\033[1;37m4. RETURN TO MAIN MENU\033[0m";      // Display option to return to main menu in Bold Light Gray text
        cout << "\n\n\t\033[1;37m5. EXIT\033[0m";                     // Display exit option in Bold Light Gray text
        cout << "\n\n\t\033[1;37mSelect Your Option (1-5): \033[0m";  // Prompt user for choice in Bold Light Gray text
        cin >> ch;

        system("cls");  // Clear console screen
        switch (ch) {
            case '1':
                cout << "\n\n\tEnter The account No. : ";
                cin >> num;
                deposit_withdraw(num, 1);  // Deposit amount for the specified account
                break;
            case '2':
                cout << "\n\n\tEnter The account No. : ";
                cin >> num;
                deposit_withdraw(num, 2);  // Withdraw amount for the specified account
                break;
            case '3':
                cout << "\n\n\tEnter The account No. : ";
                cin >> num;
                display_sp(num);  // Display specific account details
                break;
            case '4':
                return;  // Return to the main menu
            case '5':
                cout << "\n\n\tThanks for using the Fragger's bank management system";  // Display exit message
                break;
            default:
                cout << "\a";  // Beep for invalid input
        }

        cin.ignore();  // Ignore newline character from previous input
        cin.get();  // Wait for user to press Enter
    } while (ch != '5');  // Repeat until user chooses to exit
}

//_____________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION DEFINATION FOR 'ACCOUNT'S' MEMBER FUNCTIONS
//_____________________________________________________________________________________________________________________________________________________________________________________

void account::create_account() {
    cout << "\nEnter The account No. (7 digits): ";
    cin >> acno;  // Prompt user for account number and store it in acno

    // Validate the account number length
    while (acno < 1000000 || acno >= 10000000) {
        cout << "\nAccount number must be 7 digits. Please enter again: ";
        cin >> acno;
    }

    cout << "\n\nEnter The Name of The account Holder : ";
    cin.ignore();
    cin.getline(name, 50);  // Prompt user for account holder's name and store it in name

    cout << "\nEnter Type of The account (C/S) : ";
    cin >> type;  // Prompt user for account type (C/S) and store it in type
    type = toupper(type);  // Convert type to uppercase

    cout << "\nEnter The Initial amount (>=500 for Saving and >=1000 for current): ";
    cin >> deposit;  // Prompt user for initial deposit and store it in deposit
    cout << "\n\n\nAccount Created..";  // Display account creation message
}

void account::show_account() const {
    cout << "\nAccount No. : " << acno;  // Display account number
    cout << "\nAccount Holder Name : " << name;  // Display account holder's name
    cout << "\nType of Account : " << type;  // Display account type
    cout << "\nBalance amount : " << deposit;  // Display account balance
}

void account::modify() {
    cout << "\nAccount No. : " << acno;  // Display account number
    cout << "\nModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name, 50);  // Prompt user for modified account holder's name and store it in name

    cout << "\nModify Type of Account : ";
    cin >> type;  // Prompt user for modified account type (C/S) and store it in type
    type = toupper(type);  // Convert type to uppercase

    cout << "\nModify Balance amount : ";
    cin >> deposit;  // Prompt user for modified balance and store it in deposit
}

void account::dep(int x) {
    deposit += x;  // Increase account balance by the deposited amount
}

void account::draw(int x) {
    deposit -= x;  // Decrease account balance by the withdrawn amount
}

void account::report() const {
    cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;  // Display account details in report format
}

int account::retacno() const {
    return acno;  // Return account number
}

int account::retdeposit() const {
    return deposit;  // Return account balance
}

char account::rettype() const {
    return type;  // Return account type
}

//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION DEFINATION FOR 'MANAGER'S' SECTION
//________________________________________________________________________________________________________________________________________________________________________________________

void write_account() {
    account ac;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);  // Open binary file for writing in append mode
    ac.create_account();  // Create a new account
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));  // Write account details to file
    outFile.close();  // Close the file
}

void display_all() {
    account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);  // Open binary file for reading
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }

    int userNumber = 1;  // Initialize user number

    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
        cout << "==============================================================================\n";
        cout << "\n\t\t\tUSER-NO " << userNumber << "\n";  // Display User Number
        cout << "==============================================================================\n";

        ac.show_account();  // Display A/c no. and NAME

        // Display Type and Balance under their respective columns
        cout << setw(10) << ac.rettype() << setw(9) << ac.retdeposit();

        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl << endl << endl;

        userNumber++;  // Increment user number for the next user
    }

    inFile.close();  // Close the file
}

void delete_account(int n) {
    account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);  // Open binary file for reading
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    outFile.open("Temp.dat", ios::binary);  // Open temporary binary file for writing
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
        if (ac.retacno() != n) {
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));  // Write account details to temporary file
        }
    }
    inFile.close();  // Close the file
    outFile.close();  // Close the temporary file
    remove("account.dat");  // Remove the original file
    rename("Temp.dat", "account.dat");  // Rename the temporary file to the original file
    cout << "\n\n\tRecord Deleted ..";  // Display deletion message
}

void modify_account(int n) {
    bool found = false;
    account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);  // Open binary file for reading and writing
    if (!File) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    
    while (File.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
        if (ac.retacno() == n) {
            ac.show_account();
            cout << "\n\nEnter The New Details of account" << endl;
            ac.modify();
            int pos = (-1) * static_cast<int>(sizeof(account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\n\t Record Updated";
            found = true;
            break;  // Exit the loop once the record is found and updated
        }
    }
    
    File.close();  // Close the file
    
    if (found == false) {
        cout << "\n\n Record Not Found ";
    }
}

//_____________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION DEFINITION FOR 'CUSTOMER'S' SECTION
//_____________________________________________________________________________________________________________________________________________________________________________________

void deposit_withdraw(int n, int option) {
    int amt;
    bool found = false;
    account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);  // Open binary file for reading and writing
    if (!File) {
        cout << "File could not be open !! Press any Key...";
        return;
    }

    // Iterate through the file to find the account with the specified account number
    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char *>(&ac), sizeof(account));
        if (ac.retacno() == n) {
            ac.show_account();

            // Option 1: Deposit Amount
            if (option == 1) {
                cout << "\n\n\tTO DEPOSIT AMOUNT ";
                cout << "\n\nEnter The amount to be deposited: ";
                cin >> amt;
                ac.dep(amt);  // Increase account balance by the deposited amount
            }

            // Option 2: Withdraw Amount
            if (option == 2) {
                cout << "\n\n\tTO WITHDRAW AMOUNT ";
                cout << "\n\nEnter The amount to be withdrawn: ";
                cin >> amt;
                int bal = ac.retdeposit() - amt;

                // Check if withdrawal leads to insufficient balance for the account type
                if ((bal < 500 && ac.rettype() == 'S') || (bal < 1000 && ac.rettype() == 'C'))
                    cout << "Insufficient balance";
                else
                    ac.draw(amt);  // Decrease account balance by the withdrawn amount
            }

            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }

    File.close();  // Close the file

    if (found == false)
        cout << "\n\n Record Not Found ";
}

void display_sp(int n) {
    account ac;
    bool flag = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);  // Open binary file for reading
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\nBALANCE DETAILS\n";

    // Iterate through the file to find the account with the specified account number
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
        if (ac.retacno() == n) {
            ac.show_account();
            flag = true;
        }
    }

    inFile.close();  // Close the file

    if (flag == false)
        cout << "\n\nAccount number does not exist";
}
//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION TO CHECK MANAGER CREDENTIAL
//________________________________________________________________________________________________________________________________________________________________________________________

bool manager_login() {
    char username[50];
    char password[50];

    cout << "\n\n\tEnter Manager Username: ";
    cin >> username;

    cout << "\n\n\tEnter Manager Password: ";
    cin >> password;

    return (strcmp(username, MANAGER_USERNAME) == 0 && strcmp(password, MANAGER_PASSWORD) == 0);
}

//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION OF INTRODUCTION 
//________________________________________________________________________________________________________________________________________________________________________________________

void intro() {
    cout << "\n\n\n\t\033[1;33m+--------------------------------------------------------------------------+\033[0m";
    cout << "\n\n\t\033[1;33m|           WELCOME TO THE MR-FRAGGER'S BANK MANAGEMENT SYSTEM             |\033[0m";
    cout << "\n\n\t\033[1;33m+--------------------------------------------------------------------------+\033[0m";

    cin.get();
}

