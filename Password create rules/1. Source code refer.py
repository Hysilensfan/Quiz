from sys import stdin as si, stdout as so


def main() -> int:
    so.write("Set a \033[31;40m valid password\033[0m , you can get the flag!\n")

    so.write(
        "There's the rule for the password create:\n"
        "1. The first character must a low case.\n"
        "2. Must have a low case and a up case.\n"
        "3. Must have a special character(Example: !@#$%^&*).\n\n"
    )

    for line in si:
        data: str = line.strip()

        if (
            data
            and data[0].islower()  # Rule 1: 第一個字元必須是小寫
            and any(char.islower() for char in data)  # Rule 2: 必須包含大小寫
            and any(char.isupper() for char in data)
            and any(char in "!@#$%^&*" for char in data)  # Rule 3: 必須包含指定特殊字元
        ):
            so.write("[flag]\n")
        else:
            so.write("Don't you wanna get the flag?\n")

        break

    return 0


exit(main())
