from sys import stdin as si, stdout as so


def main() -> int:
    so.write("Say 'Hello world!', you can get the flag!\n")

    for line in si:
        data: str = line.strip()

        if data == "Hello world!":
            so.write("[flag]\n")
        else:
            so.write("Don't you wanna get the flag?\n")

        break

    return 0


exit(main())
