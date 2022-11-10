"""Generate test case file from a given object file.
    """

from sys import argv
from elf import ELFFile
from input_stream import InputStream

# REMOVE LATER
argv = ["gen_test.py",
        "G:/Windows.old/Desktop/ogws/kiwi515/master/ogws/build/asm/RevoSDK/OS/OSReset.o"]
# REMOVE LATER


def main():
    if len(argv) < 2:
        print("No object file specified.")
        exit(1)

    strm = InputStream.open_file(argv[1], InputStream.ENDIAN_BIG)
    elf = ELFFile(strm)


main()
