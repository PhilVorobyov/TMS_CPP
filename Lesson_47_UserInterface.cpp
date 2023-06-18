#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <vector>

using namespace std;

struct Action {
    string description;
};

void clearConsole();
void displayMainMenu();
void displayProfileMenu();
void displayHistory(const vector<Action>& history);
bool exitApp();
void showSystemHistory(vector<Action>& history);
string enterProfile(string username, vector<Action>& history);
void displayName(string username);

int main() {
    string username;
    bool hasUsername = false;
    int mainChoice;
    vector<Action> history;

    while (true) {
        clearConsole();
        displayMainMenu();
        cout << "Choose an option (enter the corresponding number): ";
        cin >> mainChoice;

        if (mainChoice == 0) {
            if (exitApp()) {
                return 0;
            }
        }
        else if (mainChoice == 3) {
            showSystemHistory(history);
        }
        else if (mainChoice == 1) {
            username = enterProfile(username, history);
        }
        else if (mainChoice == 2) {
            displayName(username);
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Press Enter to continue." << endl;
            cin.get();
        }
    }

    return 0;
}

bool exitApp() {
    char confirm;
    cout << "Are you sure you want to exit? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        return true;
    }
    else {
        return false;
    }
}

void showSystemHistory(vector<Action>& history) {
    clearConsole();
    cout << "This is a system message." << endl;
    displayHistory(history);
    Action systemMessage;
    systemMessage.description = "System message viewed";
    history.push_back(systemMessage);
}

string enterProfile(string username, vector<Action>& history) {
    int profileChoice;
    while (true) {
        clearConsole();
        displayProfileMenu();
        cout << "Choose an option (enter the corresponding number): ";
        cin >> profileChoice;

        if (profileChoice == 0) {
            break;
        }
        else if (profileChoice == 1) {
            cout << "Enter username: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, username);
            Action usernameUpdate;
            usernameUpdate.description = "Username updated: " + username;
            history.push_back(usernameUpdate);
        }
        else if (profileChoice == 2) {
            if (!username.empty()) {
                cout << "Username: " << username << endl;
            }
            else {
                cout << "You have not entered a username yet." << endl;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Press Enter to return to the profile menu." << endl;
            cin.get();
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Press Enter to continue." << endl;
            cin.get();
        }
    }
    return username;
}

void displayName(string username) {
    if (!username.empty()) {
        cout << "Username: " << username << endl;
    }
    else {
        cout << "You have not entered a username yet." << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Press Enter to return to the main menu." << endl;
    cin.get();
}

void clearConsole() {
    system("cls");
}

void displayMainMenu() {
    cout << "Main Menu:" << endl;
    cout << "1. User Profile" << endl;
    cout << "2. Display Username" << endl;
    cout << "3. System Message" << endl;
    cout << "0. Exit" << endl;
}

void displayProfileMenu() {
    cout << "Profile Menu:" << endl;
    cout << "1. Enter Username" << endl;
    cout << "2. Display Username" << endl;
    cout << "0. Back" << endl;
}

void displayHistory(const vector<Action>& history) {
    clearConsole();
    cout << "Action History:" << endl;

    for (const Action& action : history) {
        cout << action.description << endl;
    }

    cout << "Press Enter to exit." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
