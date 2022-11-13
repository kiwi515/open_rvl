"""tester.py
Create or run test cases:
    tester.py create {known good obj file} {test case path (optional)}
    tester.py run {test case file (json)}
"""
from sys import argv
from os import remove
from json import loads, dumps
from subprocess import run, PIPE

from src.elf import ELFFile, ELFSection
from src.stream import InputStream
from src.hasher import Hasher

# Can be overridden per test case
CC = "tools\\mwcceppc.exe"
CFLAGS = "-msgstyle gcc -lang c -enum int -inline auto -ipa file -volatileasm -Cpp_exceptions off -RTTI off -proc gekko -fp hard -I- -Iinclude -ir include -nodefaults"
OPT = "-O4,p"


def make_test(obj_file: str) -> str:
    """Generate test case file from a given object file
    """
    # Parse object file
    try:
        strm = InputStream.open_file(obj_file, InputStream.ENDIAN_BIG)
        elf = ELFFile(strm)
    except FileNotFoundError:
        print(f"[FATAL] Object file could not be opened: {obj_file}")
        return

    hasher = Hasher()
    test = {}
    # Hash code/data and output to JSON
    for sect in elf.all_sections():
        if not sect.is_code() and not sect.is_data():
            continue

        test[sect.name] = list()
        for sym in sect.all_symbols():
            digest = hasher.hash_symbol(sym)
            if digest != None:
                test[sect.name].append([sym.name, digest.hex()])

    return dumps(test, indent=4)


def run_test(test_file: str) -> bool:
    """Run test case file
    Only returns True if ALL cases pass
    """
    global CC, CFLAGS, OPT

    # Open test case file
    try:
        with open(test_file, "rb") as f:
            test_json = loads(f.read())
    except FileNotFoundError:
        print(f"[FATAL] Test case could not be opened: {test_file}")
        return
    except UnicodeDecodeError:
        print(f"[FATAL] JSON data could not be decoded. {test_file}")
        return

    # TU-specific compiler/flags overrides
    if "CC" in test_json:
        CC = test_json["CC"]
    if "CFLAGS" in test_json:
        CFLAGS = test_json["CFLAGS"]
    if "OPT" in test_json:
        OPT = test_json["OPT"]

    # Compile source file
    src_file = test_file.replace(".json", "")
    src_file = src_file.replace("tests", "src")
    cmd = f"{CC} {CFLAGS} {OPT} -c -o temp.o {src_file}"

    result = run(cmd, shell=True, stdout=PIPE,
                 stderr=PIPE, universal_newlines=True)
    if result.returncode != 0:
        print("[FATAL] Compiler threw an error:")
        print(result.returncode, result.stdout, result.stderr)
        return

    # Hash stuff in object file
    try:
        strm = InputStream.open_file("temp.o", InputStream.ENDIAN_BIG)
        elf = ELFFile(strm)
    except FileNotFoundError:
        print("[FATAL] Temporary object file could not be opened")

    # Validate section-by-section
    print(f"Unit test: {test_file}")
    any_fail = False
    for name in ELFSection.ALL_SECTIONS:
        # Test cases for this section
        cases = test_json.get(name, [])
        # Compiled data for this section
        my_sect = elf.section(name)

        # Compare hashes
        for case in cases:
            if my_sect == None:
                print(f"    [WARN] Missing section {name}")
                break

            # Expected
            test_name = case[0]
            test_hash = case[1]

            # Find target symbol in ELF
            my_sym = my_sect.symbol(test_name)
            if my_sym == None:
                # Symbol is missing from user code
                print(f"    [FAIL] {test_name} (Missing symbol)")
                any_fail = True
            else:
                # Compare hash
                hasher = Hasher()
                my_hash = hasher.hash_symbol(my_sym)
                if my_hash != None and my_hash.hex() == test_hash:
                    print(f"    [OK] {test_name}")
                else:
                    print(f"    [FAIL] {test_name} (Hash mismatch)")
                    any_fail = True

    # Delete temp object file
    try:
        remove("temp.o")
    # It is okay if the file is already gone
    except Exception:
        pass

    # The program must know if any case failed,
    # so it can return the proper error code
    return any_fail


def show_usage():
    print("Valid usage:")
    print(
        "    tester.py create {known good obj file} {test case path (optional)}")
    print("    tester.py run {test case file (json)}")


def main():
    if len(argv) < 3:
        print("[FATAL] Not enough arguments specified.")
        show_usage()
        return

    # Create test case
    if argv[1].casefold() == "create":
        test_data = make_test(argv[2])

        # Optionally write data to file
        if len(argv) >= 4:
            with open(argv[3], "w+") as f:
                f.write(test_data)

        # Print data to console
        print(test_data)

    # Run test case
    elif argv[1].casefold() == "run":
        # Return 1 if any test fails (for automation purposes)
        if run_test(argv[2]):
            exit(1)


main()
