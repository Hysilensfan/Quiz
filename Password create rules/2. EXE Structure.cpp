#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cctype>

using namespace std;

void print_flag(const string& flag) {
    for (const auto& character : flag) {
        cout << character << flush;

        this_thread::sleep_for(
            chrono::milliseconds(100)
        );
    }

    cout << endl;
}

int main() {
    string data, flag = "flag考試完之前不公開!";

    cout << "Set a \033[31;40m valid password\033[0m , "
         << "you can get the flag!" << endl;

    cout << "There's the rule for the password create:\n"
         << "1. The first character must a low case.\n"
         << "2. Must have a low case and a up case.\n"
         << "3. Must have a special character(Example: !@#$%^&*).\n\n";

    getline(cin, data);

    bool first_is_lower = false,
         has_special = false,
         has_upper = false,
         has_lower = false;

    // Rule 1: 第一個字元必須是小寫
    if (!data.empty()) {
        first_is_lower = islower(
            static_cast<unsigned char>(data[0])
        );
    }

    // Rule 2 & 3: 必須同時包含大小寫及指定特殊字元
    for (unsigned char x : data) {

        if (isupper(x)) {
            has_upper = true;
        }

        if (islower(x)) {
            has_lower = true;
        }

        if (string("!@#$%^&*").find(x) != string::npos) {
            has_special = true;
        }
    }

    if (first_is_lower && has_lower && has_upper && has_special) {
        print_flag(flag);
    }
    else {
        cout << "Don't you wanna get the flag?" << endl;
    }

    this_thread::sleep_for(chrono::milliseconds(1000 * 10));
    return 0;
}
