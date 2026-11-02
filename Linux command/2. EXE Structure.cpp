#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class Terminal {
private:
    string username;
    string host;
    string user_directory;
    string home_directory;
    bool running;

    map<string, vector<string>> directory;
    map<string, string> files;

    static string normalize(const string& path) {
        vector<string> parts;
        string part;
        stringstream ss(path);

        while (getline(ss, part, '/')) {
            if (part.empty() || part == ".") {
                continue;
            }

            if (part == "..") {
                if (!parts.empty()) {
                    parts.pop_back();
                }
            } else {
                parts.push_back(part);
            }
        }

        string result = "/";
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) {
                result += "/";
            }
            result += parts[i];
        }

        return result;
    }

    static string basename(const string& path) {
        size_t pos = path.find_last_of('/');
        return path.substr(pos + 1);
    }

    static vector<string> split(const string& input) {
        istringstream iss(input);
        vector<string> args;
        string arg;

        while (iss >> arg) {
            args.push_back(arg);
        }

        return args;
    }

public:
    Terminal(
        string username = "user_name",
        string host = "HOST",
        string user_directory = "/home/ubuntu/"
    ) {
        this->username = username;
        this->host = host;
        this->user_directory = normalize(user_directory);
        this->home_directory = this->user_directory;
        this->running = true;

        directory["/"] = {"home"};
        directory["/home"] = {"tester"};

        directory["/home/tester"] = {
            "Downloads", "Documents", "Pictures",
            "Musics", "Videos", "Public"
        };

        directory["/home/tester/Downloads"] = {
            "hi.c", "hi.cpp", "hi.java", "hi.py"
        };

        directory["/home/tester/Documents"] = {};
        directory["/home/tester/Pictures"] = {};
        directory["/home/tester/Musics"] = {};
        directory["/home/tester/Videos"] = {};

        directory["/home/tester/Public"] = {
            "folder1", "folder2", "folder3", "folder4"
        };

        directory["/home/tester/Public/folder1"] = {
            "README.txt"
        };

        directory["/home/tester/Public/folder2"] = {
            "leetcode_1_solution.py",
            "leetcode_50_solution.cpp",
            "leetcode_8_solution.py"
        };

        directory["/home/tester/Public/folder3"] = {
            "flag.txt"
        };

        directory["/home/tester/Public/folder4"] = {};

        files["/home/tester/Downloads/hi.c"] =
            "#include <stdio.h>\n\n"
            "int main(void) {\n"
            "    return 0;\n"
            "}\n";

        files["/home/tester/Downloads/hi.cpp"] =
            "#include <iostream>\n\n"
            "int main() {\n"
            "    return 0;\n"
            "}\n";

        files["/home/tester/Downloads/hi.java"] =
            "public class hi\n"
            "{\n"
            "    public static void main(String[] args) {}\n"
            "}\n";

        files["/home/tester/Downloads/hi.py"] =
            "print(\"Hello, World!\")\n";

        files["/home/tester/Public/folder1/README.txt"] =
            "支援指令有:\n"
            "ls, cd, cat, find, echo, pwd, whoami, uname, exit\n"
            "\n"
            "你需要尋找 flag ，該flag為一文字檔(.txt)\n"
            "使用 find 指令可以快速搜尋對應的路徑，但是你需要指定目錄跟特定參數。\n";

        files["/home/tester/Public/folder2/leetcode_1_solution.py"] =
            "from itertools import combinations as c\n"
            "\n"
            "class Solution:\n"
            "    def twoSum(self, nums: List[int], target: int) -> List[int]:\n"
            "        for a, b in c(range(len(nums)), 2):  # 取得排列組合中的兩數字的索引\n"
            "            if nums[a] + nums[b] == target:\n"
            "                return [a, b]  # 回傳「索引」\n";

        files["/home/tester/Public/folder2/leetcode_50_solution.cpp"] =
            "# include <math.h>  // 為 pow() 函數引入\n"
            "\n"
            "class Solution {\n"
            "    double myPow(double x, int n) {\n"
            "        return pow(x, n);  // x^n\n"
            "    }\n"
            "};\n";

        files["/home/tester/Public/folder2/leetcode_8_solution.py"] =
            "class Solution:\n"
            "    def myAtoi(self, s: str) -> int:\n"
            "        s: str = s.lstrip()\n"
            "        if not s:\n"
            "            return 0\n"
            "        \n"
            "        i: int = 0\n"
            "        sign: int = 1\n"
            "        if s[0] == '+':\n"
            "            i = 1\n"
            "        elif s[0] == '-':\n"
            "            sign: int = -1\n"
            "            i: int = 1\n"
            "        elif not s[0].isdigit():\n"
            "            return 0\n"
            "        result: int = 0\n"
            "        while i < len(s) and s[i].isdigit():\n"
            "            result: int = result * 10 + int(s[i])\n"
            "            i: int = i + 1\n"
            "        \n"
            "        result: int = sign * result\n"
            "        INT_MAX: int = 2**31 - 1\n"
            "        INT_MIN: int = -2**31\n"
            "        \n"
            "        if result > INT_MAX:\n"
            "            return INT_MAX\n"
            "        elif result < INT_MIN:\n"
            "            return INT_MIN\n"
            "        else:\n"
            "            return result\n";

        files["/home/tester/Public/folder3/flag.txt"] =
            "flag考試之前不公開!\n";
    }

    void write(const string& message) {
        cout << message;
        cout.flush();
    }

    string resolve_path(const string& path) const {
        if (path.empty() || path == "~") {
            return home_directory;
        }

        if (path == ".") {
            return user_directory;
        }

        if (path.compare(0, 2, "~/") == 0) {
            return normalize(
                home_directory + "/" + path.substr(2)
            );
        }

        if (path[0] != '/') {
            return normalize(user_directory + "/" + path);
        }

        return normalize(path);
    }

    bool is_directory(const string& path) const {
        return directory.find(path) != directory.end();
    }

    bool is_file(const string& path) const {
        return files.find(path) != files.end();
    }

    void list_directory(const string& path) {
        if (!is_directory(path)) {
            write("No such file or directory\n");
            return;
        }

        const vector<string>& entries = directory.at(path);

        for (size_t i = 0; i < entries.size(); ++i) {
            if (i > 0) {
                cout << "  ";
            }
            cout << entries[i];
        }

        cout << '\n';
    }

    void find_files(
        const string& start_path,
        const string& name,
        vector<string>& results
    ) const {
        if (is_file(start_path)) {
            if (basename(start_path) == name) {
                results.push_back(start_path);
            }
            return;
        }

        if (!is_directory(start_path)) {
            return;
        }

        if (basename(start_path) == name) {
            results.push_back(start_path);
        }

        for (const string& entry : directory.at(start_path)) {
            string child = start_path == "/"
                ? "/" + entry
                : start_path + "/" + entry;

            if (is_file(child)) {
                if (entry == name) {
                    results.push_back(child);
                }
            } else if (is_directory(child)) {
                find_files(child, name, results);
            }
        }
    }

    void command_analysis(const string& input) {
        vector<string> args = split(input);

        if (args.empty()) {
            return;
        }

        string command = args[0];
        vector<string> other(args.begin() + 1, args.end());

        if (command == "exit") {
            running = false;
        }

        else if (command == "pwd") {
            write(user_directory + "\n");
        }

        else if (command == "whoami") {
            write(username + "\n");
        }

        else if (command == "uname") {
            if (other.empty() ||
                find(other.begin(), other.end(), "-a") != other.end()) {
                write(
                    "Linux " + host +
                    " 6.14.0-1018-aws "
                    "#18~24.04.1-Ubuntu SMP Mon Nov 24\n"
                );
            } else if (other[0] == "-s") {
                write("Linux\n");
            } else if (other[0] == "-n") {
                write(host + "\n");
            } else if (other[0] == "-r") {
                write("6.14.0-1018-aws\n");
            } else {
                write("uname: invalid option '" + other[0] + "'\n");
            }
        }

        else if (command == "ls") {
            string path = resolve_path(
                other.empty() ? "." : other[0]
            );

            if (is_directory(path)) {
                list_directory(path);
            } else if (is_file(path)) {
                write(basename(path) + "\n");
            } else {
                write("No such file or directory\n");
            }
        }

        else if (command == "cd") {
            string path = resolve_path(
                other.empty() ? "~" : other[0]
            );

            if (is_directory(path)) {
                user_directory = path;
            } else {
                write("No such file or directory\n");
            }
        }

        else if (command == "cat") {
            if (other.empty()) {
                write("cat: missing file operand\n");
                return;
            }

            for (const string& arg : other) {
                string path = resolve_path(arg);

                if (is_file(path)) {
                    write(files.at(path));
                } else if (is_directory(path)) {
                    write("cat: " + arg + ": Is a directory\n");
                } else {
                    write(
                        "cat: " + arg +
                        ": No such file or directory\n"
                    );
                }
            }
        }

        else if (command == "find") {
            if (find(other.begin(), other.end(), "~") == other.end() ||
                find(other.begin(), other.end(), "-name") == other.end()) {
                write(
                    "Please specify the directory as `~` "
                    "and include the `-name` parameter.\n"
                );
                return;
            }

            vector<string>::const_iterator it =
                find(other.begin(), other.end(), "-name");

            if (it + 1 == other.end()) {
                write("find: missing argument to '-name'\n");
                return;
            }

            string name = *(it + 1);
            string start_path = resolve_path(other[0]);

            if (!is_directory(start_path)) {
                write(
                    "find: '" + other[0] +
                    "': No such file or directory\n"
                );
                return;
            }

            vector<string> results;
            find_files(start_path, name, results);

            for (const string& result : results) {
                string display_path = result;

                if (result == home_directory) {
                    display_path = "~";
                } else if (
                    result.find(home_directory + "/") == 0
                ) {
                    display_path = "~/" +
                        result.substr(home_directory.size() + 1);
                }

                write(display_path + "\n");
            }
        }

        else if (command == "echo") {
            if (other.empty()) {
                write("\n");
                return;
            }

            for (const string& arg : other) {
                if (arg.find('$') != string::npos) {
                    write(
                        "The current terminal does not support "
                        "using `echo $variable`.\n"
                    );
                    return;
                }
            }

            bool no_newline = other[0] == "-n";
            size_t start = no_newline ? 1 : 0;

            for (size_t i = start; i < other.size(); ++i) {
                if (i > start) {
                    cout << " ";
                }
                cout << other[i];
            }

            if (!no_newline) {
                cout << '\n';
            }

            cout.flush();
        }

        else {
            write("bash: " + command + ": command not found\n");
        }
    }

    string prompt() const {
        string current_path;

        if (user_directory == home_directory) {
            current_path = "~";
        } else if (
            user_directory.find(home_directory + "/") == 0
        ) {
            current_path = "~/" +
                user_directory.substr(home_directory.size() + 1);
        } else {
            current_path = user_directory;
        }

        string symbol = username == "root" ? "#" : "$";

        return username + "@" + host + ":" +
            current_path + symbol + " ";
    }

    bool is_running() const {
        return running;
    }
};

int main() {
    SetConsoleCP(CP_UTF8);  // 將主控台輸入編碼設定為 UTF-8
    SetConsoleOutputCP(CP_UTF8);  // 將主控台輸出編碼設定為 UTF-8
    
    Terminal terminal(
        "tester",
        "10.255.255.255",
        "/home/tester/"
    );

    string line;

    while (terminal.is_running()) {
        cout << terminal.prompt();
        cout.flush();

        if (!getline(cin, line)) {
            break;
        }

        terminal.command_analysis(line);
    }

    return 0;
}
