#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

class WindowsTerminal {
private:
    string current_directory = "C:\\Users\\user";

    map<string, vector<string>> directory;
    map<string, string> files;
    map<string, string> display_paths;
    set<string> hidden_files;

    static string lower(string text) {
        transform(text.begin(), text.end(), text.begin(),
            [](unsigned char c) {
                return static_cast<char>(tolower(c));
            });
        return text;
    }

    static string normalize(string path) {
        replace(path.begin(), path.end(), '/', '\\');

        string drive = "C:";
        vector<string> parts;

        if (path.size() >= 2 && path[1] == ':') {
            drive = path.substr(0, 2);
            path = path.substr(2);
        }

        string part;
        stringstream ss(path);

        while (getline(ss, part, '\\')) {
            if (part.empty() || part == ".") {
                continue;
            }

            if (part == "..") {
                if (!parts.empty()) {
                    parts.pop_back();
                }
            }
            else {
                parts.push_back(part);
            }
        }

        string result = drive + "\\";

        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += "\\";
            result += parts[i];
        }

        return result;
    }

    string resolve_path(const string& path) const {
        if (path.empty()) {
            return current_directory;
        }

        if (path.size() >= 2 && path[1] == ':') {
            return normalize(path);
        }

        if (path[0] == '\\' || path[0] == '/') {
            return normalize("C:" + path);
        }

        return normalize(current_directory + "\\" + path);
    }

    bool is_directory(const string& path) const {
        return directory.find(lower(path)) != directory.end();
    }

    bool is_file(const string& path) const {
        return files.find(lower(path)) != files.end();
    }

    void list_directory(const string& path, bool show_hidden) {
        string key = lower(path);

        if (!is_directory(path)) {
            cout << "The system cannot find the path specified.\n";
            return;
        }

        cout << "\n Directory of " << display_paths.at(key)
             << "\n\n";

        for (const string& name : directory.at(key)) {
            string child = lower(path + "\\" + name);

            if (hidden_files.count(child) && !show_hidden) {
                continue;
            }

            if (is_directory(child)) {
                cout << "<DIR>          " << name << "\n";
            }
            else {
                cout << "               " << name << "\n";
            }
        }

        cout << "\n";
    }

    void recursive_dir(const string& path, bool show_hidden) {
        list_directory(path, show_hidden);

        for (const string& name : directory.at(lower(path))) {
            string child = path + "\\" + name;

            if (is_directory(child)) {
                recursive_dir(child, show_hidden);
            }
        }
    }

    void initialize() {
        directory["c:\\users\\user"] = {
            "folder1", "folder2", "folder3"
        };

        directory["c:\\users\\user\\folder1"] = {
            "README.txt"
        };

        directory["c:\\users\\user\\folder2"] = {
            "Documents", "Downloads", "notes.txt"
        };

        directory["c:\\users\\user\\folder2\\documents"] = {};
        directory["c:\\users\\user\\folder2\\downloads"] = {};

        directory["c:\\users\\user\\folder3"] = {
            "flag.txt"
        };

        for (const auto& entry : directory) {
            display_paths[entry.first] = entry.first;
        }

        display_paths["c:\\users\\user"] = "C:\\Users\\user";
        display_paths["c:\\users\\user\\folder1"] =
            "C:\\Users\\user\\folder1";
        display_paths["c:\\users\\user\\folder2"] =
            "C:\\Users\\user\\folder2";
        display_paths["c:\\users\\user\\folder2\\documents"] =
            "C:\\Users\\user\\folder2\\Documents";
        display_paths["c:\\users\\user\\folder2\\downloads"] =
            "C:\\Users\\user\\folder2\\Downloads";
        display_paths["c:\\users\\user\\folder3"] =
            "C:\\Users\\user\\folder3";

        files["c:\\users\\user\\folder1\\readme.txt"] =
            "Supported commands:\n"
            "dir, cd, type\n\n"
            "dir       : List files and directories\n"
            "dir /a    : Include hidden files\n"
            "dir /s    : List all subdirectories\n"
            "cd        : Display current directory\n"
            "cd path   : Change directory\n"
            "type file : Display file contents\n\n"
            "Hint: The flag is hidden somewhere.\n";

        files["c:\\users\\user\\folder2\\notes.txt"] =
            "Windows CMD Practice\n"
            "Try exploring the directories.\n";

        files["c:\\users\\user\\folder3\\flag.txt"] =
            "Hysilensfab{Wlnd0w3_CMD:}\n";

        hidden_files.insert(
            "c:\\users\\user\\folder3\\flag.txt"
        );
    }

public:
    WindowsTerminal() {
        initialize();
    }

    void command_analysis(const string& input) {
        istringstream iss(input);
        vector<string> args;
        string arg;

        while (iss >> arg) {
            args.push_back(arg);
        }

        if (args.empty()) return;

        string command = lower(args[0]);

        if (command == "exit") {
            exit(0);
        }

        else if (command == "cd") {
            if (args.size() == 1) {
                cout << current_directory << "\n";
                return;
            }

            string path = resolve_path(args[1]);

            if (is_directory(path)) {
                current_directory = display_paths.at(lower(path));
            }
            else {
                cout << "The system cannot find the path specified.\n";
            }
        }

        else if (command == "dir") {
            bool show_hidden = false;
            bool recursive = false;
            string target;

            for (size_t i = 1; i < args.size(); ++i) {
                string option = lower(args[i]);

                if (option == "/a") {
                    show_hidden = true;
                }
                else if (option == "/s") {
                    recursive = true;
                }
                else {
                    target = args[i];
                }
            }

            string path = target.empty()
                ? current_directory
                : resolve_path(target);

            if (!is_directory(path)) {
                cout << "The system cannot find the path specified.\n";
                return;
            }

            path = display_paths.at(lower(path));

            if (recursive) {
                recursive_dir(path, show_hidden);
            }
            else {
                list_directory(path, show_hidden);
            }
        }

        else if (command == "type") {
            if (args.size() < 2) {
                cout << "The syntax of the command is incorrect.\n";
                return;
            }

            string path = lower(resolve_path(args[1]));

            if (is_file(path)) {
                cout << files.at(path);
            }
            else if (is_directory(path)) {
                cout << "Access is denied.\n";
            }
            else {
                cout << "The system cannot find the file specified.\n";
            }
        }

        else {
            cout << "'" << args[0]
                 << "' is not recognized as an internal or external command.\n";
        }
    }

    void run() {
        string input;

        while (true) {
            cout << current_directory << ">";
            cout.flush();

            if (!getline(cin, input)) {
                break;
            }

            command_analysis(input);
        }
    }
};

int main() {
    WindowsTerminal terminal;
    terminal.run();

    return 0;
}
