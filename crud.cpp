#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json11.hpp"

using json11::Json;
using namespace std;

const string filename = "users.json";

// Hàm đọc dữ liệu từ file JSON
vector<Json> loadData() {
    ifstream inFile(filename);
    if (!inFile) {
        // Nếu file không tồn tại, trả về vector rỗng
        return {};
    }
    string data((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    auto json = Json::parse(data, "");
    if (json.is_array()) {
        vector<Json> users = json.array_items();
        return users;
    }
    return {};
}

// Hàm ghi dữ liệu vào file JSON
void saveData(const vector<Json>& users) {
    Json::array arr;
    for (const auto& user : users) {
        arr.push_back(user);
    }
    Json json(arr);
    ofstream outFile(filename);
    outFile << json.dump(4) << endl; // in đẹp 4 spaces
}

// Hàm tạo mới user
void createUser() {
    vector<Json> users = loadData();
    int id;
    string name, email;

    cout << "Nhập ID người dùng: ";
    cin >> id;
    // Kiểm tra trùng ID
    for (const auto& user : users) {
        if (user["id"].int_value() == id) {
            cout << "ID đã tồn tại. Thoát.\n";
            return;
        }
    }
    cin.ignore(); // bỏ qua dòng mới còn lại
    cout << "Nhập tên: ";
    getline(cin, name);
    cout << "Nhập email: ";
    getline(cin, email);

    Json newUser = Json::object{
        {"id", id},
        {"name", name},
        {"email", email}
    };
    users.push_back(newUser);
    saveData(users);
    cout << "Người dùng đã tạo.\n";
}

// Hàm đọc danh sách user
void readUsers() {
    vector<Json> users = loadData();
    if (users.empty()) {
        cout << "Chưa có dữ liệu người dùng.\n";
        return;
    }
    cout << "Danh sách người dùng:\n";
    for (const auto& user : users) {
        cout << "ID: " << user["id"].int_value()
             << ", Name: " << user["name"].string_value()
             << ", Email: " << user["email"].string_value() << "\n";
    }
}

// Hàm cập nhật user
void updateUser() {
    vector<Json> users = loadData();
    int id;
    cout << "Nhập ID người dùng cần cập nhật: ";
    cin >> id;
    bool found = false;
    for (auto& user : users) {
        if (user["id"].int_value() == id) {
            string name, email;
            cin.ignore();
            cout << "Nhập tên mới: ";
            getline(cin, name);
            cout << "Nhập email mới: ";
            getline(cin, email);
            user["name"] = name;
            user["email"] = email;
            found = true;
            break;
        }
    }
    if (found) {
        saveData(users);
        cout << "Cập nhật thành công.\n";
    } else {
        cout << "Không tìm thấy người dùng với ID này.\n";
    }
}

// Hàm xóa user
void deleteUser() {
    vector<Json> users = loadData();
    int id;
    cout << "Nhập ID người dùng cần xóa: ";
    cin >> id;
    auto it = users.begin();
    bool found = false;
    for (; it != users.end(); ++it) {
        if ((*it)["id"].int_value() == id) {
            users.erase(it);
            found = true;
            break;
        }
    }
    if (found) {
        saveData(users);
        cout << "Xóa thành công.\n";
    } else {
        cout << "Không tìm thấy người dùng với ID này.\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n=== CRUD ===\n";
        cout << "1. CREATE\n";
        cout << "2. READ\n";
        cout << "3. UPDATE\n";
        cout << "4. DELETE\n";
        cout << "0. EXIT\n";
        cout << "CHOOSE: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 0:
                cout << "END.\n";
                break;
            default:
                cout << "WRONG.\n";
        }
    } while (choice != 0);

    return 0;
}