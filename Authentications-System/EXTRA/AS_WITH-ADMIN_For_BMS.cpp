#include <iostream>
#include <fstream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <limits>
#include <cstdlib>

using namespace std;

class User {
public:
    string username;
    string password;
    string secretInfo;
    bool blocked;

    User() : blocked(false) {}
    User(string uname, string pwd, string info, bool isBlocked = false) : username(uname), password(pwd), secretInfo(info), blocked(isBlocked) {}

    void adminReset();
    void adminResetPassword();
    void adminRetrieveSecretInfo();
    void adminShowAccountInformation();
};

// Function declarations
bool isUsernameTaken(const string& username);
void registerUser();
bool loginUser();

#ifdef _WIN32
string getPasswordInput() {
    string password;
    char ch;

    while ((ch = _getch()) != 13) {
        if (ch == 8) {
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
string getPasswordInput() {
    string password;
    char ch;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n') {
        if (ch == 8 && !password.empty()) {
            cout << "\b \b";
            password.pop_back();
        } else if (ch != 8) {
            cout << '*';
            password += ch;
        }
    }

    cout << endl;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return password;
}
#endif

bool isUsernameTaken(const string& username) {
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        size_t pos = line.find(':');
        string storedUsername = line.substr(0, pos);
        if (storedUsername == username) {
            return true;
        }
    }

    return false;
}

void registerUser() {
    string username, password, confirmPassword, secretInfo;

    cout << "\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n " << endl;
    cout << "1. Enter your desired username: ";
    cin >> username;

    while (isUsernameTaken(username)) {
        cout << "\n|||Username already taken. Please choose another one.|||\n\n";
        Beep(100, 900);

        cout << "1. Try another username: ";
        cin >> username;
    }

    cout << "Enter your password: ";
    password = getPasswordInput();

    do {
        cout << "Confirm your password:";
        confirmPassword = getPasswordInput();

        if (password != confirmPassword) {
            cout << "Password confirmation failed. Please re-enter the registration details.\n";
        }
    } while (password != confirmPassword);

    cout << "Enter your secret information: ";
    cin.ignore();
    getline(cin, secretInfo);

    User newUser(username, password, secretInfo);

    ofstream file("users.txt", ios::app);
    file << newUser.username << ":" << newUser.password << ":" << newUser.secretInfo << "\n";
    file.close();

    cout << "Registration successful!\n";
}

void User::adminReset() {
    if (blocked) {
        cout << "This user's account is blocked.\n";
        return;
    }

    string newUsername, newPassword, newSecretInfo;

    cout << " Username will be the previous one : " << username <<endl;
    getline(cin, newUsername);

    if (!newUsername.empty() && isUsernameTaken(newUsername)) {
        cout << "Username already taken. Operation canceled."<<endl;
        return;
    }

    string confirmPassword;
    do {
        cout << "Enter new password: ";
        newPassword = getPasswordInput();

        cout << "Confirm new password: ";
        confirmPassword = getPasswordInput();

        if (newPassword != confirmPassword) {
            cout << "Password confirmation failed. Please re-enter the new details.\n";
        }
    } while (newPassword != confirmPassword);

    cout << "Enter new secret information: ";
    getline(cin, newSecretInfo);

    if (!newUsername.empty()) {
        username = newUsername;
    }
    password = newPassword;
    secretInfo = newSecretInfo;

    ofstream outFile("temp.txt");
    ifstream inFile("users.txt");
    string line;

    while (getline(inFile, line)) {
        if (line.substr(0, line.find(':')) == username) {
            outFile << username << ":" << password << ":" << secretInfo << "\n";
        } else {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");

    cout << "Account reset successful!\n";
}

void User::adminResetPassword() {
    if (blocked) {
        cout << "This user's account is blocked.\n";
        return;
    }

    string newPassword, confirmPassword;

    string newUsername;
    cout << "Enter new username (press Enter to keep the current username '" << username << "'): ";
    getline(cin, newUsername);

    if (!newUsername.empty() && isUsernameTaken(newUsername)) {
        cout << "Username already taken. Operation canceled.\n";
        return;
    }

    do {
        cout << "Enter new password: ";
        newPassword = getPasswordInput();

        cout << "Confirm new password: ";
        confirmPassword = getPasswordInput();

        if (newPassword != confirmPassword) {
                        cout << "Password confirmation failed. Please re-enter the new details.\n";
        }
    } while (newPassword != confirmPassword);

    if (!newUsername.empty()) {
        username = newUsername;
    }
    password = newPassword;

    ofstream outFile("temp.txt");
    ifstream inFile("users.txt");
    string line;

    while (getline(inFile, line)) {
        if (line.substr(0, line.find(':')) == username) {
            outFile << username << ":" << password << ":" << secretInfo << "\n";
        } else {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");

    cout << "Password reset successful!\n";
}

void User::adminRetrieveSecretInfo() {
    cout << "The secret information for user '" << username << "' is: " << secretInfo << endl;
}

void User::adminShowAccountInformation() {
    cout << "Account Information for User  '' " << username << " ''." << "\n password = " << password << "\n secret-information = " << secretInfo << endl;
}

bool loginUser() {
    string username, password, secretInfo;

    cout << "Enter your username: ";
    cin >> username;

    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        size_t pos = line.find(':');
        string storedUsername = line.substr(0, pos);

        if (storedUsername == username) {
            User currentUser;
            currentUser.username = storedUsername;
            size_t infoPos = line.find(':', pos + 1);
            currentUser.password = line.substr(pos + 1, infoPos - pos - 1);
            currentUser.secretInfo = line.substr(infoPos + 1);
            currentUser.blocked = (line.find(":blocked") != string::npos);

            if (currentUser.blocked) {
                cout << "Your account is currently blocked due to security reasons. Kindly contact the authorities.\n";
                return false;
            }

            int loginAttempts = 3;
            do {
                cout << "Enter your password: ";
                password = getPasswordInput();

                if (currentUser.password == password) {
                    cout << "Welcome, " << currentUser.username << "! Login successful.\n";
                    return true;
                } else {
                    cout << "Incorrect password. " << loginAttempts << " attempts remaining.\n";
                    loginAttempts--;

                    if (loginAttempts == 0) {
                        cout << "Too many incorrect password attempts. Please verify your identity.\n";
                        int secretAttempts = 3;
                        while (secretAttempts > 0) {
                            cout << "Enter your secret information: ";
                            cin.ignore();
                            getline(cin, secretInfo);

                            if (currentUser.secretInfo == secretInfo) {
                                cout << "Secret information verification successful! Your password is: " << currentUser.password << endl;
                                return true;
                            } else {
                                cout << "Incorrect secret information. " << secretAttempts << " attempts remaining.\n";
                                secretAttempts--;

                                if (secretAttempts == 0) {
                                    currentUser.blocked = true;
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

    cout << "Username not found. Please re-enter the login details.\n";
    return false;
}

int main() {
    int choice;

    do {
        cout << "\n  ====== LOGIN FIRST ======\n" << endl;
        cout << "\n\t\t1. REGISTRATION\n";
        cout << "\t\t2. LOGIN\n";
        cout << "\t\t3. ADMIN\n";
        cout << "\t\t4. EXIT\n";
        cout << "\n\t\t KINDLY ENTER YOUR DESIRED CHOICE  " << endl;
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    cout << "Login successful!\n";
                } else {
                    cout << "'.\n";
                }
                break;
            case 3:
            {
                string adminPassword;
                cout << "Enter admin password: ";
                adminPassword = getPasswordInput();

                if (adminPassword == "admin123") {
                    cout << "Admin access granted!\n";

                    int adminOption;
                    do {
                        cout << "\n===== ADMIN MENU =====\n";
                        cout << "1. Show Account Information\n";
                        cout << "2. Admin Reset Menu\n";
                        cout << "3. Go Back\n";
                        cout << "Enter your choice: ";
                        cin >> adminOption;

                        switch (adminOption) {
                            case 1:
                            {
                                string usernameToShow;
                                cout << "Enter the username to show account information: ";
                                cin >> usernameToShow;

                                ifstream file("users.txt");
                                string line;

                                while (getline(file, line)) {
                                    size_t pos = line.find(':');
                                    string storedUsername = line.substr(0, pos);

                                    if (storedUsername == usernameToShow) {
                                        User currentUser;
                                        currentUser.username = storedUsername;
                                        size_t infoPos = line.find(':', pos + 1);
                                        currentUser.password = line.substr(pos + 1, infoPos - pos - 1);
                                        currentUser.secretInfo = line.substr(infoPos + 1);
                                        currentUser.blocked = (line.find(":blocked") != string::npos);

                                        if (!currentUser.blocked) {
                                            cout << "Account's Login Information of User  '' " << currentUser.username << " ''." << "\n password = " << currentUser.password << "\n secret-information = " << currentUser.secretInfo << endl;
                                        } else {
                                            cout << "This user's account is blocked.\n";
                                        }

                                        break;
                                    }
                                }

                                file.close();
                            }
                            break;
                            case 2:
                            {
                                User userToReset;
                                string usernameToReset;
                                cout << "Enter the username to reset account: ";
                                cin >> usernameToReset;

                                ifstream file("users.txt");
                                string line;

                                bool userFound = false;
                                while (getline(file, line)) {
                                    size_t pos = line.find(':');
                                    string storedUsername = line.substr(0, pos);

                                    if (storedUsername == usernameToReset) {
                                        userToReset.username = storedUsername;
                                        size_t infoPos = line.find(':', pos + 1);
                                        userToReset.password = line.substr(pos + 1, infoPos - pos - 1);
                                        userToReset.secretInfo = line.substr(infoPos + 1);
                                        userToReset.blocked = (line.find(":blocked") != string::npos
                                        );

                                        // Call the adminResetUserAccount function to reset the user account
                                        userToReset.adminReset();
                                        userFound = true;
                                        break;
                                    }
                                }

                                file.close();

                                if (!userFound) {
                                    cout << "Username not found. Operation canceled.\n";
                                }
                            }
                            break;
                            case 3:
                                cout << "Going back to the main menu." << endl;
                                break;
                            default:
                                cout << "Invalid option. Please try again.\n";
                        }
                    } while (adminOption != 3);
                } else {
                    cout << "Admin access denied!\n";
                }
            }
            break;
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}


