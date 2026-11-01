from sys import stdin as si, stdout as so
from typing import Dict, List, Optional
import posixpath
import shlex


class Terminal:
    def __init__(
            self,
            username: str = "user_name",
            host: str = "HOST",
            user_directory: str = "/home/ubuntu/"
    ) -> None:
        self.username: str = username
        self.host: str = host
        self.user_directory: str = posixpath.normpath(user_directory)
        self.home_directory: str = self.user_directory
        self.running: bool = True

        self.directory: Dict[str, List[str]] = {
            "/": ["home"],
            "/home": ["tester"],
            "/home/tester": [
                "Downloads", "Documents", "Pictures",
                "Musics", "Videos", "Public"
            ],
            "/home/tester/Downloads": [
                "hi.c", "hi.cpp", "hi.java", "hi.py"
            ],
            "/home/tester/Documents": [],
            "/home/tester/Pictures": [],
            "/home/tester/Musics": [],
            "/home/tester/Videos": [],
            "/home/tester/Public": [
                "folder1", "folder2", "folder3", "folder4"
            ],
            "/home/tester/Public/folder1": [
                "README.txt"
            ],
            "/home/tester/Public/folder2": [
                "leetcode_1_solution.py",
                "leetcode_50_solution.cpp",
                "leetcode_8_solution.py"
            ],
            "/home/tester/Public/folder3": [
                "flag.txt"
            ],
            "/home/tester/Public/folder4": [],
        }

        self.files: Dict[str, str] = {
            "/home/tester/Downloads/hi.c": (
                '#include <stdio.h>\n\n'
                'int main(void) {\n'
                '    return 0;\n'
                '}\n'
            ),
            "/home/tester/Downloads/hi.cpp": (
                '#include <iostream>\n\n'
                'int main() {\n'
                '    return 0;\n'
                '}\n'
            ),
            "/home/tester/Downloads/hi.java": (
                'public class hi\n'
                '{\n'
                '    public static void main(String[] args) {}\n'
                '}\n'
            ),
            "/home/tester/Downloads/hi.py": (
                'print("Hello, World!")\n'
            ),

            "/home/tester/Public/folder1/README.txt": (
                "支援指令有:\n"
                "ls, cd, cat, find, echo, pwd, whoami, uname, exit\n"
                "\n"
                "你需要尋找 flag ，該flag為一文字檔(.txt)\n"
                "使用 find 指令可以快速搜尋對應的路徑，但是你需要指定目錄跟特定參數。\n"
            ),

            "/home/tester/Public/folder2/leetcode_1_solution.py": (
                "from itertools import combinations as c\n"
                "\n"
                "class Solution:\n"
                "    def twoSum(self, nums: List[int], target: int) -> List[int]:\n"
                "        for a, b in c(range(len(nums)), 2):  # 取得排列組合中的兩數字的索引\n"
                "            if nums[a] + nums[b] == target:\n"
                "                return [a, b]  # 回傳「索引」\n"
            ),

            "/home/tester/Public/folder2/leetcode_50_solution.cpp": (
                "# include <math.h>  // 為 pow() 函數引入\n"
                "\n"
                "class Solution {\n"
                "    double myPow(double x, int n) {\n"
                "        return pow(x, n);  // x^n\n"
                "    }\n"
                "};\n"
            ),

            "/home/tester/Public/folder2/leetcode_8_solution.py": (
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
                "            return result\n"
            ),

            "/home/tester/Public/folder3/flag.txt": (
                "[flag]\n"
            ),
        }

    @staticmethod  # 該修飾器聲明該 Method 為不使用 self 的靜態 Method
    def write(message: str) -> None:
        so.write(message)
        so.flush()

    def resolve_path(self, path: Optional[str]) -> str:
        if path is None or path == "~":
            return self.home_directory

        if path == ".":
            return self.user_directory

        if path.startswith("~/"):
            path = posixpath.join(
                self.home_directory, path[2:]
            )
        elif not path.startswith("/"):
            path = posixpath.join(
                self.user_directory, path
            )

        return posixpath.normpath(path)

    def is_directory(self, path: str) -> bool:
        return path in self.directory

    def is_file(self, path: str) -> bool:
        return path in self.files

    def is_valid_path(self, path: str) -> bool:
        return self.is_directory(path) or self.is_file(path)

    def list_directory(self, path: str) -> None:
        if not self.is_directory(path):
            self.write("No such file or directory\n")
            return

        self.write("  ".join(self.directory[path]) + "\n")

    def find_files(
            self, start_path: str, name: str
    ) -> List[str]:
        results: List[str] = []

        if self.is_file(start_path):
            if posixpath.basename(start_path) == name:
                results.append(start_path)
            return results

        if not self.is_directory(start_path):
            return results

        if posixpath.basename(start_path) == name:
            results.append(start_path)

        for entry in self.directory[start_path]:
            child = posixpath.join(start_path, entry)

            if self.is_file(child):
                if entry == name:
                    results.append(child)
            elif self.is_directory(child):
                results.extend(
                    self.find_files(child, name)
                )

        return results

    def command_analysis(self, commands: str) -> None:
        commands = commands.strip()

        if not commands:
            return

        try:
            args = shlex.split(commands)
        except ValueError:
            self.write("bash: syntax error: invalid quoting\n")
            return

        if not args:
            return

        command = args[0]
        other = args[1:]

        if command == "exit":  # 退出當前 shell，這裡用以終止程式運作
            self.running = False

        elif command == "pwd":  # 工作目錄
            self.write(self.user_directory + "\n")

        elif command == "whoami":  # 使用者名稱
            self.write(self.username + "\n")

        elif command == "uname":  # 顯示作業系統與核心（Kernel）相關資訊的指令，這裡只是模擬用途
            if not other or "-a" in other:
                self.write(
                    f"Linux {self.host} 6.14.0-1018-aws "
                    f"#18~24.04.1-Ubuntu SMP Mon Nov 24\n"
                )
            elif "-s" in other:
                self.write("Linux\n")
            elif "-n" in other:
                self.write(self.host + "\n")
            elif "-r" in other:
                self.write("6.14.0-1018-aws\n")
            else:
                self.write(
                    f"uname: invalid option '{other[0]}'\n"
                )

        elif command == "ls":  # 羅列當下目錄下的所有檔案跟目錄
            path_arg = other[0] if other else "."
            path = self.resolve_path(path_arg)

            if self.is_directory(path):
                self.list_directory(path)
            elif self.is_file(path):
                self.write(posixpath.basename(path) + "\n")
            else:
                self.write("No such file or directory\n")

        elif command == "cd":
            path_arg = other[0] if other else "~"
            path = self.resolve_path(path_arg)

            if self.is_directory(path):
                self.user_directory = path
            else:
                self.write("No such file or directory\n")

        elif command == "cat":
            if not other:
                self.write("cat: missing file operand\n")
                return

            for path_arg in other:
                path = self.resolve_path(path_arg)

                if self.is_file(path):
                    self.write(self.files[path])
                elif self.is_directory(path):
                    self.write(
                        f"cat: {path_arg}: Is a directory\n"
                    )
                else:
                    self.write(
                        f"cat: {path_arg}: No such file or directory\n"
                    )

        elif command == "find":
            if "~" not in other or "-name" not in other:  # Directory 僅支援 /home ; 參數僅支援 -name
                self.write(
                    "Please specify the directory as `~` "
                    "and include the `-name` parameter.\n"
                )
                return

            try:
                name_index = other.index("-name")
                name = other[name_index + 1]
            except IndexError:
                self.write(
                    "find: missing argument to '-name'\n"
                )
                return

            start_path = self.resolve_path(other[0])

            if not self.is_directory(start_path):
                self.write(
                    f"find: '{other[0]}': No such file or directory\n"
                )
                return

            for result in self.find_files(start_path, name):
                if result == self.home_directory:
                    display_path = "~"
                elif result.startswith(self.home_directory + "/"):
                    display_path = "~/" + posixpath.relpath(
                        result, self.home_directory
                    )
                else:
                    display_path = result

                self.write(display_path + "\n")

        elif command == "echo":
            if not other:  # 無 -n 參數值接近出
                self.write("\n")
                return

            if any("$" in arg for arg in other):
                self.write(
                    "The current terminal does not support "
                    "using `echo $variable`.\n"
                )
                return

            no_newline = other[0] == "-n"
            output_args = other[1:] if no_newline else other
            output = " ".join(output_args)

            self.write(
                output if no_newline else output + "\n"
            )

        else:
            self.write(
                f"bash: {command}: command not found\n"
            )

    def prompt(self) -> str:
        if self.user_directory == self.home_directory:
            current_path = "~"
        elif self.user_directory.startswith(
                self.home_directory + "/"
        ):
            current_path = "~/" + posixpath.relpath(
                self.user_directory, self.home_directory
            )
        else:
            current_path = self.user_directory

        symbol = "#" if self.username == "root" else "$"

        return (
            f"{self.username}@{self.host}:"
            f"{current_path}{symbol} "
        )


def main() -> int:
    terminal = Terminal(
        username="tester",
        host="10.255.255.255",
        user_directory="/home/tester/"
    )  # 設定用戶命稱、主機、初始目錄

    while terminal.running:
        so.write(terminal.prompt())  # Prompt 印出
        so.flush()

        line = si.readline()

        if not line:  # 針對沒有輸入的情況做處理
            break

        terminal.command_analysis(line)  # 對輸入的文字做處理

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
