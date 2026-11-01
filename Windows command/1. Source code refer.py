from sys import stdin as si, stdout as so

ROOT = r"C:\Users\user"

directories = {
    r"c:\users\user": ["folder1", "folder2", "folder3"],
    r"c:\users\user\folder1": ["README.txt"],
    r"c:\users\user\folder2": [
        "Documents", "Downloads", "notes.txt"
    ],
    r"c:\users\user\folder2\documents": [],
    r"c:\users\user\folder2\downloads": [],
    r"c:\users\user\folder3": ["flag.txt"],
}

files = {
    r"c:\users\user\folder1\readme.txt":
        "Supported commands:\n"
        "dir, cd, type\n\n"
        "dir       : List files and directories\n"
        "dir /a    : Include hidden files\n"
        "dir /s    : List all subdirectories\n"
        "cd        : Display current directory\n"
        "cd path   : Change directory\n"
        "type file : Display file contents\n\n"
        "Hint: The flag is hidden somewhere.\n",

    r"c:\users\user\folder2\notes.txt":
        "Windows CMD Practice\n"
        "Try exploring the directories.\n",

    r"c:\users\user\folder3\flag.txt": "[flag]\n"
}

hidden_files = {
    r"c:\users\user\folder3\flag.txt"
}

def normalize(path: str) -> str:
    path = path.replace("/", "\\")
    parts = []

    for part in path.split("\\"):
        if not part or part == ".":
            continue

        if part == "..":
            if parts:
                parts.pop()
        else:
            parts.append(part)

    return "\\".join(parts)

def resolve_path(path: str, current: str) -> str:
    if not path:
        return current

    path = path.replace("/", "\\")

    if len(path) >= 2 and path[1] == ":":
        return normalize(path)

    if path.startswith("\\"):
        return normalize("C:" + path)

    return normalize(current + "\\" + path)

def is_directory(path: str) -> bool:
    return path.lower() in directories

def is_file(path: str) -> bool:
    return path.lower() in files

def list_directory(path: str, show_hidden: bool):
    key = path.lower()

    if key not in directories:
        so.write("The system cannot find the path specified.\n")
        return

    so.write(f"\n Directory of {path}\n\n")

    entries = directories[key]

    for name in entries:
        child = (path + "\\" + name).lower()

        if child in hidden_files and not show_hidden:
            continue

        if child in directories:
            so.write(f"<DIR>          {name}\n")
        else:
            so.write(f"               {name}\n")

    so.write("\n")

def recursive_dir(path: str, show_hidden: bool):
    list_directory(path, show_hidden)

    for name in directories.get(path.lower(), []):
        child = path + "\\" + name

        if is_directory(child):
            recursive_dir(child, show_hidden)

def command_analysis(command: str, current: str) -> str:
    args = command.split()

    if not args:
        return current

    cmd = args[0].lower()
    other = args[1:]

    if cmd == "exit":
        return "EXIT"

    elif cmd == "cd":
        if not other:
            so.write(current + "\n")
            return current

        path = resolve_path(other[0], current)

        if is_directory(path):
            return path
        else:
            so.write("The system cannot find the path specified.\n")
            return current

    elif cmd == "dir":
        show_hidden = any(x.lower() == "/a" for x in other)
        recursive = any(x.lower() == "/s" for x in other)

        paths = [
            x for x in other
            if x.lower() not in ("/a", "/s")
        ]

        target = resolve_path(paths[0], current) if paths else current

        if recursive:
            if is_directory(target):
                recursive_dir(target, show_hidden)
            else:
                so.write("The system cannot find the path specified.\n")
        else:
            list_directory(target, show_hidden)

    elif cmd == "type":
        if not other:
            so.write("The syntax of the command is incorrect.\n")
            return current

        path = resolve_path(other[0], current)
        key = path.lower()

        if key in files:
            so.write(files[key])
        elif is_directory(path):
            so.write("Access is denied.\n")
        else:
            so.write("The system cannot find the file specified.\n")

    else:
        so.write(f"'{args[0]}' is not recognized as an internal or external command.\n")

    return current

def main() -> int:
    current = ROOT

    while True:
        so.write(current + ">")
        so.flush()

        line = si.readline()

        if not line:
            break

        current = command_analysis(line.strip(), current)

        if current == "EXIT":
            break

    return 0

exit(main())
