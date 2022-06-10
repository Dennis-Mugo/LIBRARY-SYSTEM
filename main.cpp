#include<iostream>
#include<fstream>
#include<cctype>
#include<cstring>
#include <conio.h>
#include<unordered_map>
#include<vector>
#include<algorithm>
using namespace std;

string current_user;
void main_menu();
void horizontal_rule(int length) {
    for (int i = 0; i < length; i++) {
        cout << "-";
    }
    cout << "\n";
}

string add_commas_to_number(string num) {
    string result = "";
    int size = num.size();
    for (int i = 0; i < size; i++) {
        result = num[size - i - 1] + result;
        if ((i + 1) % 3 == 0 && (i != size - 1)) {
            result = ',' + result;
        }
    }
    return result;
}

string remove_underscores(string word) {
    string result = "";
    for (auto letter : word) {
        if (letter == '_') {
            result.push_back(' ');
        } else {
            result.push_back(letter);
        }
    }
    return result;
}


string get_secure_password() {
    string password;
    int ch = getch();
    while (ch != 13) {
        password.push_back(ch);
        cout << "*";
        ch = getch();
    }
    cout << "\n";
    return password;
}

unordered_map<string, string> get_user_details(string user = current_user) {
    ifstream fin;
    string username_db, password_db, amount_db;
    unordered_map<string, string> user_details;
    fin.open("student_db.txt");
    while (fin >> username_db >> password_db >> amount_db) {
        if (username_db == user) {
            user_details["name"] = username_db;
            user_details["password"] = password_db;
            user_details["amount"] = amount_db;
            return user_details;
        }
    }
    return user_details;
}


void update_book_quantity(string book_title, int value) {
    ifstream fin;
    ofstream fout;
    fin.open("books.txt");
    unordered_map<string, int> books;
    string book_title_db;
    int quantity_db;
    while (fin >> book_title_db >> quantity_db) {
        books[book_title_db] = quantity_db;
    }
    fin.close();
    books[book_title] += value;
    remove("books.txt");
    fout.open("books.txt");
    for (auto record : books) {
        fout << record.first << " " << record.second << "\n";
    }
    fout.close();
}

void update_borrowed_books(string user_name, string book_title, string status) {
    ifstream fin;
    ofstream fout;
    string file_name = user_name + ".txt";
    if (status == "borrowed") {
        fout.open(file_name, ios::app);
        fout << book_title << " " << status << "\n";
        fout.close();
    } else {
        unordered_map<string, string> books;
        string book_title_db, book_status_db;
        fin.open(file_name);
        while (fin >> book_title_db >> book_status_db) {
            books[book_title_db] = book_status_db;
        }
        fin.close();
        books[book_title_db] = status;
        string filename(file_name);
        remove(filename.c_str());
        fout.open(file_name);
        for (auto record : books) {
            fout << record.first << " " << record.second << "\n";
        }
        fout.close();
    }
}

bool librarian_login(string user_name, string password) {
    ifstream fin;
    string db_user_name, db_password;
    fin.open("librarian_db.txt");
    if (fin.is_open()) {
        fin >> db_user_name >> db_password;
        if (db_user_name != user_name) {
            cout << "Incorrect username! Please try again.\n";
            fin.close();
            horizontal_rule(60);
            return false;
        }
        if (db_password != password) {
            cout << "Incorrect password! Please try again.\n";
            fin.close();
            horizontal_rule(60);
            return false;
        }
        current_user = user_name;
        horizontal_rule(60);
        cout << "Successfully logged in!...\n\n";
        fin.close();
        
    }
    return true;
}

bool student_login(string user_name, string password) {
    ifstream fin;
    string db_user_name, db_password, db_amount;
    fin.open("student_db.txt");
    if (fin.is_open()) {
        while (fin >> db_user_name >> db_password >> db_amount) {
            if (db_user_name == user_name && db_password == password) {
                horizontal_rule(60);
                cout << "Successfully logged in!...\n\n";
                current_user = user_name;
                fin.close();
                return true;
            }
        }
    }
    
    horizontal_rule(60);
    cout << "Invalid credentials. Please try again.\n";
    fin.close();
    return false;
}

void display_librarian_menu() {
    cout << "Select one option below:\n";
    cout << "\t1.Register new student\n";
    cout << "\t2.Approve student's request to borrow a book\n";
    cout << "\t3.Accept returned books\n";
    cout << "\t4.Change password\n";
    cout << "\t5.Logout\n";
}

void display_student_menu() {
    cout << "Select one option below:\n";
    cout << "\t1.View Personal Details/Profile\n";
    cout << "\t2.Request to borrow/rent a book\n";
    cout << "\t3.Return borrowed/rented book\n";
    cout << "\t4.Change password\n";
    cout << "\t5.Logout\n";
}

void change_librarian_password() {
    ifstream fin;
    ofstream fout;
    string new_password, user_name;
    cout << "*****Change My Password*****\n";
    cout << "Enter new password: ";
    new_password = get_secure_password();
    fin.open("librarian_db.txt");
    if (fin.is_open()) {
        fin >> user_name;
    }
    fin.close();
    remove("librarian_db.txt");
    fout.open("librarian_db.txt");
    fout << user_name << " " << new_password;
    fout.close();
    cout << "Password changed successfully!\n";
    horizontal_rule(60);
}

void approve_borrow_requests() {
    ofstream fout;
    ifstream fin;
    horizontal_rule(60);
    cout << "****** Approve student borrow requests ******\n";
    horizontal_rule(60);
    vector<vector<string>> requests;
    string student_name, book_title, status;
    fin.open("book_approvals.txt");
    while (fin >> student_name >> book_title >> status) {
        if (status == "pending") {
            requests.push_back({student_name, book_title, status});
        }
    }
    fin.close();
    if (requests.size() == 0) {
        cout << "There are no requests to borrow any books.\n";
        horizontal_rule(60);
        return;
    }
    for (int i = 0; i < requests.size(); i++) {
        int option;
        student_name = requests[i][0];
        book_title = requests[i][1];
        cout << student_name << " has requested to borrow " << "\"" << book_title << "\".\n";
        cout << "\t1.Approve\n";
        cout << "\t2.Decline\n";
        cout << "Select option to proceed: ";
        cin >> option;
        if (option == 1) {
            requests[i][2] = "approved";
            cout << student_name << "'s request successfully approved!\n";
        } else {
            requests[i][2] = "declined";
        }
        horizontal_rule(60);
    }
    fout.open("book_approvals.txt");
    for (auto record : requests) {
        fout << record[0] << " " << record[1] << " " << record[2] << "\n";
    }
    fout.close();
    for (auto record : requests) {
        if (record[2] == "approved") {
            update_book_quantity(record[1], -1);
            update_borrowed_books(record[0], record[1], "borrowed");
        }
    }
}

void approve_return_requests() {
    ofstream fout;
    ifstream fin;
    horizontal_rule(60);
    cout << "****** Approve student return book requests ******\n";
    horizontal_rule(60);
    vector<vector<string>> requests;
    string student_name, book_title, status;
    fin.open("book_return_approvals.txt");
    while (fin >> student_name >> book_title >> status) {
        if (status == "pending") {
            requests.push_back({student_name, book_title, status});
        }
    }
    fin.close();
    if (requests.size() == 0) {
        cout << "There are no requests to return any books.\n";
        horizontal_rule(60);
        return;
    }
    for (int i = 0; i < requests.size(); i++) {
        int option;
        student_name = requests[i][0];
        book_title = requests[i][1];
        cout << student_name << " has requested to return " << "\"" << book_title << "\".\n";
        cout << "\t1.Approve\n";
        cout << "\t2.Decline\n";
        cout << "Select option to proceed: ";
        cin >> option;
        if (option == 1) {
            requests[i][2] = "approved";
            cout << student_name << "'s request successfully approved!\n";
        } else {
            requests[i][2] = "declined";
        }
        horizontal_rule(60);
    }
    fout.open("book_return_approvals.txt");
    for (auto record : requests) {
        fout << record[0] << " " << record[1] << " " << record[2] << "\n";
    }
    fout.close();
    for (auto record : requests) {
        if (record[2] == "approved") {
            update_book_quantity(record[1], 1);
            update_borrowed_books(record[0], record[1], "returned");
        }
    }
    
}

void request_book() {
    ofstream fout;
    ifstream fin;
    int option;
    string book_title, quantity;
    horizontal_rule(60);
    cout << "****** Borrow a book ******\n";
    horizontal_rule(60);
    vector<vector<string>> books;
    fin.open("books.txt");
    if (fin.is_open()) {
        while (fin >> book_title >> quantity) {
            books.push_back({book_title, quantity});
        }
    }
    fin.close();
    cout << "Select one book below:\n";
    for (int i = 0; i < books.size(); i++) {
        string title = remove_underscores(books[i][0]);
        cout << "\t" << i + 1 << "." << title << "\n";
    }
    cout << "Pick book to proceed: ";
    cin >> option;
    while (option < 1 || option > books.size()) {
        cout << "Invalid option. Please try again: ";
        cin >> option;
    }
    int book_quantity = stoi(books[option - 1][1]);
    string title_borrowed = books[option - 1][0];
    if (book_quantity == 0) {
        horizontal_rule(60);
        cout << "The book is not available at the moment.\n";
        horizontal_rule(60);
        return;
    }
    unordered_map<string, string> user_details = get_user_details();
    if (stoi(user_details["amount"]) < 100) {
        horizontal_rule(60);
        cout << "Sorry, You do not have enough money in your library account to borrow any book.\n";
        cout << "Your account balance is " << user_details["amount"] << " shillings.\n";
        horizontal_rule(60);
        return;
    }
    fout.open("book_approvals.txt", ios::app);
    fout << current_user << " " << title_borrowed << " " << "pending\n";
    fout.close();
    int pressed;
    string approved = "pending";
    horizontal_rule(60);
    cout << "Please wait for the librarian to approve your request.\n";
    cout << "Press 1 to view approval status\n";
    cin >> pressed;
    while (approved == "pending") {
        string user, title, status;
        
        fin.open("book_approvals.txt");
        while (fin >> user >> title >> status) {
            if (user == current_user && title == title_borrowed) {
                approved = status;
            }
        }
        fin.close();
        horizontal_rule(60);
        if (approved == "pending") {
            cout << "Your request is still pending.\n";
            cout << "Please wait for the librarian to approve it.\n";
            cout << "Press 1 to view approval status\n";
            cin >> pressed;
        } else if (approved == "declined") {
            cout << "Sorry, your request to borrow the book has been declined!\n";
            horizontal_rule(60);
        } else {
            cout << "Your request has been approved\n";
            cout << "Enjoy your book!\n";
            horizontal_rule(60);
        }
        
    }   
}

void return_book() {
    horizontal_rule(60);
    cout << "****** Return a Book *****\n";
    horizontal_rule(60);
    ifstream fin;
    ofstream fout;
    string book_title_db, book_status_db;
    int option;
    vector<vector<string>> borrowed_books, books;
    fin.open(current_user + ".txt");
    while (fin >> book_title_db >> book_status_db) {
        if (book_status_db == "borrowed") {
            borrowed_books.push_back({book_title_db, book_status_db});
        }
        
    }
    fin.close();
    if (borrowed_books.size() == 0) {
        cout << "You have no borrowed books to return.\n";
        horizontal_rule(60);
        return;
    }
    cout << "These are the books that you have borrowed:\n";
    horizontal_rule(60);
    for (int i = 0; i < borrowed_books.size(); i++) {
        string title = remove_underscores(borrowed_books[i][0]);
        cout << "\t" << i + 1 << "." << title << "\n";
    }
    horizontal_rule(60);
    cout << "Select the one you would like to return: ";
    cin >> option;
    if (option < 1 || option > borrowed_books.size()) {
        cout << "Invalid option. Please try again\n";
        cin >> option;
    }
    vector<string> book_to_return = borrowed_books[option - 1];
    string title_borrowed = book_to_return[0];
    fout.open("book_return_approvals.txt", ios::app);
    fout << current_user << " " << title_borrowed << " pending\n";
    fout.close();
    int press_to_continue;
    string approval_status = "pending";
    cout << "Please wait for the librarian to approve your request.\n";
    cout << "Press 1 to view approval status.\n";
    cin >> press_to_continue;
    while (approval_status == "pending") {
        string user, title, status;
        fin.open("book_return_approvals.txt");
        while (fin >> user >> title >> status) {
            if (user == current_user && title == title_borrowed) {
                approval_status = status;
            }
        }
        fin.close();
        horizontal_rule(60);
        if (approval_status == "pending") {
            cout << "Your request is still pending.\n";
            cout << "Please wait for the librarian to approve it.\n";
            cout << "Press 1 to view approval status\n";
            cin >> press_to_continue;
        } else if (approval_status == "declined") {
            cout << "Sorry, your request to return the book has been declined!\n";
            horizontal_rule(60);
        } else {
            cout << "Your request has been approved\n";
            cout << "We hope you enjoyed reading the book!\n";
            horizontal_rule(60);
        }
        
    }   

}

void change_student_password() {
    string new_password1, new_password2;
    ifstream fin;
    ofstream fout;
    string user_name_db, password_db;
    string amount_db;
    unordered_map<string, vector<string>> users_data;
    horizontal_rule(60);
    cout << "****** Change My Password ******\n";
    horizontal_rule(60);
    cout << "Enter new password: ";
    new_password1 = get_secure_password();
    cout << "Confirm new password: ";
    new_password2 = get_secure_password();
    while (new_password1 != new_password2) {
        cout << "The passwords do not match. Please try again.\n";
        cout << "Enter new password: ";
        new_password1 = get_secure_password();
        cout << "Confirm new password: ";
        new_password2 = get_secure_password();
    }

    fin.open("student_db.txt");
    if (fin.is_open()) {
        while (fin >> user_name_db >> password_db >> amount_db) {
            users_data[user_name_db] = {password_db, amount_db}; 
        }
    }
    fin.close();
    remove("student_db.txt");
    
    users_data[current_user][0] = new_password1;
    fout.open("student_db.txt");
    if (fout.is_open()) {
    for (auto record : users_data) {
        fout << record.first << " " << record.second[0] << " " << record.second[1] << "\n";
        }
    } 
    
    fout.close();
    horizontal_rule(60);
    cout << "Password changed successfully!\n";
    horizontal_rule(60);

}


void display_user_details() {
    horizontal_rule(60);
    cout << "****** MY PROFILE ******\n";
    horizontal_rule(60);
    unordered_map<string, string> user_details = get_user_details();
    cout << "\tUsername: " << user_details["name"] << "\n";
    cout << "\tPassword: " << user_details["password"] << "\n";
    cout << "\tAccount Balance: " << add_commas_to_number(user_details["amount"]) << " shillings\n";
    horizontal_rule(60);

}

void register_new_student() {
    ifstream fin;
    ofstream fout;
    string user_name, password1, password2;
    string user_name_db, password_db;
    vector<string>user_names;
    int flat_fee, flat_fee_db;
    horizontal_rule(60);
    fin.open("student_db.txt");
    if (fin.is_open()){
        while (fin >> user_name_db >> password_db >> flat_fee_db) {
            user_names.push_back(user_name_db);
        }
    }
    fin.close();
    cout << "******Register a new Student******\n";
    horizontal_rule(60);
    cout << "Enter student's username: ";
    cin >> user_name;
    while (find(user_names.begin(), user_names.end(), user_name) != user_names.end()) {
        cout << "The username already exists. Please try a new username: ";
        cin >> user_name;
    }
    cout << "Enter student's password: ";
    password1 = get_secure_password();
    cout << "Confirm password: ";
    password2 = get_secure_password();
    while (password1 != password2) {
        horizontal_rule(60);
        cout << "The passwords do not match. Please try again\n";
        cout << "Enter student's password: ";
        password1 = get_secure_password();
        cout << "Confirm password: ";
        password2 = get_secure_password();
    }
    cout << "Enter flat fee paid: ";
    cin >> flat_fee;
    fout.open("student_db.txt", ios::app);
    fout << user_name << " " << password1 << " " << flat_fee << "\n";
    fout.close();
    horizontal_rule(60);
    cout << "Student registered successfully!\n";
    horizontal_rule(60);
}

void librarian_activities() {
    display_librarian_menu();
    int librarian_option;
    cout << "Enter option to proceed: ";
    cin >> librarian_option;
    
    while (librarian_option < 1 || librarian_option > 5) {
        display_librarian_menu();
        cout << "Invalid option. Please try again: ";
        cin >> librarian_option;
    }
    switch (librarian_option) {
        case 1:
            register_new_student();
            break;
        case 2:
            approve_borrow_requests();
            break;
        case 3:
            approve_return_requests();
            break;
        case 4:
            change_librarian_password();
            break;
        case 5:
            horizontal_rule(60);
            main_menu();
            break;
        default:
            break;
    }
}



void librarian_user() {
    horizontal_rule(60);
    string user_name, password;
    bool login_successful = false;
    do {
        cout << "Enter librarian username: ";
        cin >> user_name;
        cout << "Enter librarian password: ";
        password = get_secure_password();
        login_successful = librarian_login(user_name, password);
    } while (!login_successful);
    while (true) {
        librarian_activities();
    }
}

void student_activities() {
    display_student_menu();
    int student_option;
    cout << "Enter option to proceed: ";
    cin >> student_option;
    
    while (student_option < 1 || student_option > 5) {
        display_student_menu();
        cout << "Invalid option. Please try again: ";
        cin >> student_option;
    }
    switch (student_option) {
        case 1:
            display_user_details();
            break;
        case 2:
            request_book();
            break;
        case 3:
            return_book();
            break;
        case 4:
            change_student_password();
            break;
        case 5:
            horizontal_rule(60);
            main_menu();
            break;
        default:
            break;
    }
}

void student_user() {
    horizontal_rule(60);
    string user_name, password;
    bool login_successful = false;
    do {
        cout << "Enter student username: ";
        cin >> user_name;
        cout << "Enter student password: ";
        password = get_secure_password();
        login_successful = student_login(user_name, password);
    } while (!login_successful);
    while (true) {
        student_activities();
    }

}

void main_menu() {
    int role;
    cout << "******Welcome to the Library*******\n\n";
    cout << "Select your role to proceed:\n";
    cout << "\t1.Normal Student Login\n";
    cout << "\t2.Librarian Login\n";
    cout << "Enter role to proceed: (1 or 2): ";
    cin >> role;
    while (role != 1 && role != 2) {
        cout << "Invalid option! Please try again (1 or 2):\n";
        cin >> role;
    }
    switch (role) {
        case 1:
            student_user();
            break;
        case 2:
            librarian_user();
            break;
        default:
            break;
    }
}

int main() {
    main_menu();
    return 0;
}