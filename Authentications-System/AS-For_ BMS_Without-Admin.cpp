#include <iostream>
/*   This includes the Input/Output stream header file. It provides functionality for basic 
input and output operations in C++, including the cin and cout objects*/

#include <fstream>
/*   This includes the File stream header file. It provides functionality for reading from 
and writing to files in C++. It introduces classes like ifstream (for reading from files) 
and ofstream (for writing to files).*/

#include <string>
/*   This includes the String handling header file. It provides functionality for 
manipulating strings in C++*/

#include <cctype>
/*   This includes the Character handling functions header file. It provides functions
 for character classification (e.g., isalpha, isdigit) and case conversion 
 (e.g., tolower, toupper)*/

#ifdef _WIN32
//_________________________________________________________________________________________________________
//                                                                                                         |
#include <windows.h>//                                                                                     |
/*   If the code is being compiled on a Windows system, this includes the Windows-specific                 |
functions header file. It provides access to various functions related to Windows programming.*///         |
//                                                                                                         |                                                          
#include <conio.h>//                                                                                       |
/*   This includes the Console input/output functions header file, which contains functions for            |
console-based input and output. Note that conio.h is not part of the C++ standard and is                   |
specific to certain compilers and environments.*///                                                        |
//                                                                                                         |
#else//                                                                                                    *          + --------------------------------------------------------------------------------------------------------------------------------- +
//--------------------------------------------------------------------------------------------------------------->>   |         These are conditional compilation directives. #ifdef _WIN32 checks if the code is being compiled on a Windows system.     |
//                                                                                                         ^          |   If true, the subsequent code block (up to #else or #endif) is included. If false, the code block after #else (if present) is    |
//                                                                                                         |          |   included. The #endif marks the end of the conditional compilation block                                                         |
//                                                                                                         |          + --------------------------------------------------------------------------------------------------------------------------------- +
#include <termios.h>    // POSIX terminal I/O____________________________________________________          |
/*   If the code is not being compiled on Windows (as determined by the absence of _WIN32),      |         |
these headers are included. They provide functionality for POSIX terminal I/O, which is          |         |
relevant for Unix-like systems.*///                                                              |         |
//                                                                                               |         |
#include <unistd.h>     // POSIX standard function definitions                                   |         |
/*   same as #include <terminod.h> *///__________________________________________________________^         |
//                                                                                                         |
#endif//                                                                                                   |
//_________________________________________________________________________________________________________|

#include <limits>       // Limits of integral types
/*   This includes the Limits of integral types header file. It provides information about the
limits of various integral types, such as the maximum and minimum representable values.*/

#include <cstdlib>      // C standard library functions
/*   This includes the C Standard Library header file. It provides general-purpose functions
 in C++, such as memory allocation and conversion functions*/

using namespace std;

//________________________________________________________________________________________________________________________________________________________________________________________
// USER CLASS.                                                                                                                                                                           |
//________________________________________________________________________________________________________________________________________________________________________________________

class User {
public:
    string username;    // User's username
    string password;    // User's password
    string secretInfo;  // User's secret information
    bool blocked;       // Flag to indicate if the user is blocked

    // Default constructor, initializes 'blocked' to false
    User() : blocked(false) {}

    // Parameterized constructor for creating a User with provided values
    User(string uname, string pwd, string info, bool isBlocked = false)
        : username(uname), password(pwd), secretInfo(info), blocked(isBlocked) {}

    // Member functions for user actions
    void userResetPassword();
    void userRetrieveSecretInfo();
    void showAccountInformation();
};

//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION DECLARATION.
//________________________________________________________________________________________________________________________________________________________________________________________

bool isUsernameTaken(const string& username);
void registerUser();
bool loginUser();

#ifdef _WIN32
//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION TO GET PASSWORD INPUT ON WINDOWS PLATFORM.
//________________________________________________________________________________________________________________________________________________________________________________________

string getPasswordInput() {
    string password;
    char ch;

    // Loop until Enter key is pressed
    while ((ch = _getch()) != 13) {
        if (ch == 8) {
            // Handle backspace to erase the last character
            if (!password.empty()) {
                cout << "\b \b";
                password.erase(password.size() - 1);
            }
        } else {
            cout << '*';
            password += ch;
        }
    }

    cout << endl;
    return password;
}
#else
//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION TO GET PASSWORD INPUT ON NON-WINDOWS PLATFORM.
//________________________________________________________________________________________________________________________________________________________________________________________
string getPasswordInput() {
    string password;
    char ch;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    /*tcgetattr: This function is used to get the terminal attributes. It takes two arguments: 
the file descriptor representing the terminal (in this case, STDIN_FILENO for standard input) 
and a pointer to a termios structure where the attributes will be stored.STDIN_FILENO: This 
constant represents the file descriptor for standard input. It is typically defined as 0.
&oldt: The address of the oldt structure is passed to tcgetattr, allowing the function to 
store the current terminal attributes in this structure.So, after this line executes, the 
oldt structure contains the original terminal attributes, preserving the state of the terminal 
before any modifications are made. Later in the code, when echoing is turned off and other changes 
are made to the terminal settings, this saved state (oldt) will be used to restore the terminal to 
its original state after password input is obtained.The line newt = oldt; creates a copy of the original 
terminal settings in the newt structure. This step is important because it allows modifications to be made 
to the newt structure without affecting the original settings stored in oldt. It ensures that the program 
can later revert to the initial state of the terminal by using the information stored in oldt.*/
    newt.c_lflag &= ~(ECHO); // Turn off echoing
	/*newt.c_lflag represents the local mode flags for the terminal.
ECHO is a flag constant that, when set, enables echoing.
The bitwise NOT (~) operator is used to create a bitmask with all bits inverted.
The bitwise AND (&) operator is then applied to the existing c_lflag value, effectively 
turning off the ECHO flag while leaving other flags unchanged.*/

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);/* is telling the system to set the terminal attributes for 
standard input (associated with file descriptor 0) to the values specified in the termios structure 
newt, and the changes should take effect immediately (TCSANOW).*/

    // Loop until Enter key is pressed
    while ((ch = getchar()) != '\n') {
        if (ch == 8 && !password.empty()) {
            // Handle backspace to erase the last character
            cout << "\b \b";
            password.pop_back();
        } else if (ch != 8) {
            cout << '*';
            password += ch;
        }
    }

    cout << endl;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings

    return password;
}
#endif

//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION TO CHECK IF THE USER NAME IS ALREADY TAKEN.
//________________________________________________________________________________________________________________________________________________________________________________________

bool isUsernameTaken(const string& username) {
    ifstream file("users.txt");
    string line;

    // Read each line from the file
    while (getline(file, line)) {
        size_t pos = line.find(':');
        string storedUsername = line.substr(0, pos);
        // Check if the username matches the stored username
        if (storedUsername == username) {
            return true;
        }
    }

    return false;
}

//________________________________________________________________________________________________________________________________________________________________________________________
// MAIN FUNCTION OF USER'S REGISTRATION.
//________________________________________________________________________________________________________________________________________________________________________________________

void registerUser() {
    // Declare variables for user registration
    string username, password, confirmPassword, secretInfo;

    // Display a decorative header
    cout << "\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n " << endl;

    // Flag to check if the entered username is valid
    bool isValidUsername = false;

    // Loop until a valid username is entered
    do {
        // Prompt the user to enter a username
        cout << "1. Enter your desired username (more than 5 characters, must include at least one digit, and must include '@'): ";
        cin >> username;

        // Flags to check username requirements
        bool hasDigit = false;
        bool hasAtSymbol = false;

        // Check each character in the username
        for (size_t i = 0; i < username.length(); ++i) {
            if (isdigit(username[i])) {
                hasDigit = true;
            } else if (username[i] == '@') {
                hasAtSymbol = true;
            }
        }

        // Check username length and requirements
        if (username.length() <= 5 || !hasDigit || !hasAtSymbol || isUsernameTaken(username)) {
            if (username.length() <= 5) {
                cout << "Username must be more than 5 characters.\n";
            }
            if (!hasDigit) {
                cout << "Username must include at least one digit.\n";
            }
            if (!hasAtSymbol) {
                cout << "Username must include '@'.\n";
            }
            if (isUsernameTaken(username)) {
                cout << "Username already exists. Please choose another one.\n";
            }
        } else {
            // Set flag to true if username is valid
            isValidUsername = true;
        }
    } while (!isValidUsername);

    // Prompt the user to enter a password with specific requirements
    cout << "Enter your password (more than 5 characters, with at least one uppercase letter, and at least one digit or symbol): ";

    // Flags to check password requirements
    bool hasUppercase = false;
    bool hasDigitOrSymbol = false;

    // Loop until a valid password is entered
    do {
        // Get password input using the platform-specific function
        password = getPasswordInput();

        // Check each character in the password
        for (size_t i = 0; i < password.length(); ++i) {
            char c = password[i];
            if (isupper(c)) {
                hasUppercase = true;
            } else if (isdigit(c) || ispunct(c)) {
                hasDigitOrSymbol = true;
            }
        }

        // Check password length and requirements
        if (password.length() <= 5 || !hasUppercase || !hasDigitOrSymbol) {
            cout << "Password does not meet the requirements. Please re-enter: ";
        } else {
            // Break the loop if the password is valid
            break;
        }
    } while (true);

    // Confirm the entered password
    do {
        cout << "Confirm your password:";
        confirmPassword = getPasswordInput();

        // Check if the confirmation matches the original password
        if (password != confirmPassword) {
            cout << "Password confirmation failed. Please re-enter the registration details.\n";
        }
    } while (password != confirmPassword);

    // Prompt the user to enter secret information
    cout << "Enter your secret information: ";
    cin.ignore(); // Ignore newline character from previous input
    getline(cin, secretInfo);

    // Create a new User object with entered information
    User newUser(username, password, secretInfo);

    // Open the "users.txt" file in append mode
    ofstream file("users.txt", ios::app);
    
    // Write user information to the file
    file << newUser.username << ":" << newUser.password << ":" << newUser.secretInfo << "\n";

    // Close the file
    file.close();

    // Display a success message
    cout << "Registration successful!\n";
}

//________________________________________________________________________________________________________________________________________________________________________________________
// METHOD TO RETRIEVE AND DISPLAY THE SECRET INFORMATION FOR A USER.
//________________________________________________________________________________________________________________________________________________________________________________________

void User::userRetrieveSecretInfo() {
    cout << "The secret information for user '" << username << "' is: " << secretInfo << endl;
}


//________________________________________________________________________________________________________________________________________________________________________________________
// METHOD TO DISPLAY THE ACCOUNT'S INFORMATION FOR A USER.
//________________________________________________________________________________________________________________________________________________________________________________________

void User::showAccountInformation() {
    cout << "Account Information for User  '' " << username << " ''." << "\n password = " << password << "\n secret-information = " << secretInfo << endl;
}

//________________________________________________________________________________________________________________________________________________________________________________________
// FUNCTION TO AUTHENTICATE & LOG-IN A USER.
//________________________________________________________________________________________________________________________________________________________________________________________

bool loginUser() {
    string username, password, secretInfo;

    // Prompt the user to enter their username
    cout << "Enter your username: ";
    cin >> username;

    // Open the user file for reading
    ifstream file("users.txt");
    string line;

    // Iterate through each line in the user file
    while (getline(file, line)) {
        size_t pos = line.find(':');
        string storedUsername = line.substr(0, pos);

        // Check if the entered username matches a stored username
        if (storedUsername == username) {
            User currentUser;
            currentUser.username = storedUsername;

            // Find the position of the password in the line
            size_t infoPos = line.find(':', pos + 1);
            // Extract password and secret information
            currentUser.password = line.substr(pos + 1, infoPos - pos - 1);
            currentUser.secretInfo = line.substr(infoPos + 1);
            currentUser.blocked = (line.find(":blocked") != string::npos);

            // Check if the account is blocked
            if (currentUser.blocked) {
                cout << "Your account is currently blocked due to security reasons. Kindly contact the authorities.\n";
                return false;
            }

            int loginAttempts = 3;
            do {
                // Prompt the user to enter the password
                cout << "Enter your password: ";
                password = getPasswordInput();

                // Check if the entered password is correct
                if (currentUser.password == password) {
                    cout << "Welcome, " << currentUser.username << "! Login successful.\n";
                    return true;
                } else {
                    // Display remaining attempts and handle account lockout
                    cout << "Incorrect password. " << loginAttempts << " attempts remaining.\n";
                    loginAttempts--;

                    if (loginAttempts == 0) {
                        cout << "Too many incorrect password attempts. Please verify your identity.\n";
                        int secretAttempts = 3;
                        while (secretAttempts > 0) {
                            // Prompt the user to enter secret information for verification
                            cout << "Enter your secret information: ";
                            cin.ignore();
                            getline(cin, secretInfo);

                            // Check if the entered secret information is correct
                            if (currentUser.secretInfo == secretInfo) {
                                cout << "Secret information verification successful! Your password is: " << currentUser.password << endl;
                                return true;
                            } else {
                                // Display remaining attempts and handle account blockage
                                cout << "Incorrect secret information. " << secretAttempts << " attempts remaining.\n";
                                secretAttempts--;

                                if (secretAttempts == 0) {
                                    currentUser.blocked = true;

                                    // Update user file with blocked status
                                    ofstream outFile("temp.txt");
                                    while (getline(file, line)) {
                                        if (line.substr(0, pos) == username) {
                                            outFile << username << ":" << currentUser.password << ":" << currentUser.secretInfo << ":blocked\n";
                                        } else {
                                            outFile << line << "\n";
                                        }
                                    }
                                    file.close();
                                    outFile.close();
                                    remove("users.txt");
                                    rename("temp.txt", "users.txt");

                                    // Display a message about account blockage
                                    cout << "Secret information verification failed. Your account is now permanently blocked. Please contact the authorities.\n";
                                    return false;
                                }
                            }
                        }
                    }
                }
            } while (loginAttempts > 0);

            return false;
        }
    }

    // Display an error message if the username is not found
    cout << "Username not found. Please re-enter the login details.\n";
    return false;
}

//________________________________________________________________________________________________________________________________________________________________________________________
// MAIN.
//________________________________________________________________________________________________________________________________________________________________________________________

int main() {
    int choice;

    // Loop for displaying the main menu until the user chooses to exit
    do {
        // Display the main menu for user interaction
        cout << "\n\t\t  ====== AUTHENTICATION-SYSTEM ======\n" << endl;
        cout << "\n\t\t1. REGISTRATION\n";
        cout << "\t\t2. LOGIN\n";
        cout << "\t\t3. EXIT\n";
        cout << "\n\t\t KINDLY ENTER YOUR DESIRED CHOICE  " << endl;

        // Get user's choice for menu selection
        cin >> choice;

        // Clear the console screen for a clean display
        system("cls");

        // Switch statement to perform actions based on the user's choice
        switch (choice) {
            // Case 1: User chose registration
            case 1:
                // Call the registration function
                registerUser();
                break;
            // Case 2: User chose login
            case 2:
                // Check if login is successful
                if (loginUser()) {
                    cout << "Login successful!\n";
                } else {
                    cout << "Login failed.\n";
                }
                break;
            // Case 3: User chose to exit
            case 3:
                cout << "Exiting program.\n";
                break;
            // Default case: Invalid choice
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);  // Repeat the loop until the user chooses to exit

    return 0;  // Exit the program with a status code of 0
}

