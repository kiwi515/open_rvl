"""Fixes GAS assembly to work with the tester.

1. Prevent function-local labels from being emitted
- lbl_* -> .L_*

2. Correct SDA-macro relocations
_SDA_BASE_ -> @sda21
_SDA2_BASE_ -> @sda21

3. Correct r13 relocations
stw rX, Y@sda21(r13) -> stw rX, Y@sda21(0)
lwz rX, Y@sda21(r13) -> lwz rX, Y@sda21(0)
addi rX, r13, Y@sda21 -> li rX, Y@sda21

These changes are only required because the objects are compared before linking,
so these instruction patterns do actually generate different code at that stage.
"""

from sys import argv
from re import match

# Section declaration
SECTION_REGEX = r"^\s*.section\s+(?P<Name>.[a-zA-Z0-9_$]+)"
# Label declaration
LABEL_REGEX = r"^\s*(?P<Name>lbl_[0-9A-F]{8})\s*:"


def main():
    if len(argv) < 2:
        print("Invalid arguments.")
        print("Valid usage:")
        print("    fix_asm.py myFile.s")

    # Read input file
    with open(argv[1], "r") as f:
        asm = f.readlines()

    # First pass:
    # Find all function-local labels (.text)
    labels = []
    curr_sect = None
    for line in asm:
        # Search line for patterns
        sect_match = match(SECTION_REGEX, line)
        lbl_match = match(LABEL_REGEX, line)
        # Are we looking at a section declaration?
        if sect_match != None:
            curr_sect = sect_match.group("Name")
        # Are we looking at a function-local label declaration?
        elif lbl_match != None and curr_sect == ".text":
            labels.append(lbl_match.group("Name"))

    # Second pass:
    # Correct relocations, and rename local labels
    for i, line in enumerate(asm):
        if "lbl_" in line:
            for label in labels:
                # Convert to new label type
                if label in line:
                    line = line.replace(label, label.replace("lbl_", ".L_"))
                    break

        # SDA macro reloc
        line = line.replace("-_SDA_BASE_", "@sda21")
        line = line.replace("-_SDA2_BASE_", "@sda21")

        # r13 reloc
        line = line.replace("@sda21(r13)", "@sda21(0)")

        # !!! Assumes doldisasm format !!!
        if line.startswith("/*"):
            insn = line[37:].split()

            # GAS addi -> li
            if insn[0] == "addi" and "@sda21" in line:
                assert len(insn) == 4
                line = line[:37] + f"li {insn[1]} {insn[3]}\n"

        asm[i] = line

    with open("temp.s", "w+") as f:
        f.writelines(asm)


main()
