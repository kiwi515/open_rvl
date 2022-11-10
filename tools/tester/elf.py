"""Simple ELF implementation for parsing assembled object files
    """

from input_stream import InputStream
from dataclasses import dataclass

"""
1. Validate file header [X]
2. Extract shstrtab [X]
3. Extract strtab [X]
4. Build list of sections (w/ names from shstrtab) [X]

5. Find + parse symbols in symtab (use section list to determine sym section, strtab for name)
"""


@dataclass
class ELFFile():
    def __init__(self, strm: InputStream):
        """Parse ELF file
        """
        self.__sh_list = list()
        self.__shstrtab = None
        self.__strtab = None
        self.__shoff = -1
        self.__shentsize = -1
        self.__shnum = -1
        self.__shstrndx = -1

        assert not strm.eof()

        self.__parse_file_header(strm)
        self.__extract_shstrtab(strm)

        self.__parse_section_headers(strm)
        self.__extract_strtab(strm)
        self.__parse_symbols(strm)

        pass

    def __parse_file_header(self, strm: InputStream):
        """Parse required info from the ELF header
        """
        assert not strm.eof()

        # Magic
        assert strm.get_string_sz(
            4) == "\x7FELF", "Object file is not an ELF file"
        # Class
        assert strm.get_u8() == 1, "Object file is not a 32-bit executable"
        # Endianness
        assert strm.get_u8() == 2, "Object file is not a big-endian executable"
        # Machine
        strm.seek(0x12, strm.SEEK_BEGIN)
        assert strm.get_u16() == 0x14, "Object file does not use 32-bit PowerPC arch"

        # Section header table offset
        strm.seek(0x20, strm.SEEK_BEGIN)
        self.__shoff = strm.get_u32()
        # Section header table entry size
        strm.seek(0x2E, strm.SEEK_BEGIN)
        self.__shentsize = strm.get_u16()
        # Section header count
        strm.seek(0x30, strm.SEEK_BEGIN)
        self.__shnum = strm.get_u16()
        # Section ID of .shstrtab
        self.__shstrndx = strm.get_u16()

    def __extract_shstrtab(self, strm: InputStream):
        """Find and extract the shstrtab section (section header name string table)
        """
        assert not strm.eof()

        # Seek to shstrtab section header
        strm.seek(self.__shoff + (self.__shstrndx *
                  self.__shentsize), strm.SEEK_BEGIN)

        # Get shstrtab data offset/size
        strm.seek(0x10, strm.SEEK_CURRENT)
        shstrtab_off = strm.get_u32()
        shstrtab_sz = strm.get_u32()

        # Extract section
        strm.seek(shstrtab_off, strm.SEEK_BEGIN)
        self.__shstrtab = strm.read(shstrtab_sz)

    def __extract_strtab(self, strm: InputStream):
        """Find and extract the strtab section (symbol name string table)
        """
        assert not strm.eof()

        # Seek to strtab section header
        strtab_idx = self.__get_section_idx(".strtab")
        assert strtab_idx, ".strtab missing?"
        strm.seek(self.__shoff + (strtab_idx *
                  self.__shentsize), strm.SEEK_BEGIN)

        # Get strtab data offset/size
        strm.seek(0x10, strm.SEEK_CURRENT)
        strtab_ofs = strm.get_u32()
        strtab_sz = strm.get_u32()

        # Extract strtab data
        strm.seek(strtab_ofs, strm.SEEK_BEGIN)
        self.__strtab = strm.read(strtab_sz)

    def __parse_section_headers(self, strm: InputStream):
        """Parse ELF section headers (minimally).
        We only need a list of the section names, to index once we parse symbols.
        """
        assert not strm.eof()

        for i in range(self.__shnum):
            # Seek to section's header
            strm.seek(self.__shoff + (i *
                                      self.__shentsize), strm.SEEK_BEGIN)
            # Get name of section
            name_off = strm.get_u32()
            self.__sh_list.append(self.__get_sh_name(name_off))

    def __parse_symbols(self, strm: InputStream):
        """Parse symtab section
        """
        assert not strm.eof()

        # Find symtab section header
        symtab_shidx = self.__get_section_idx(".symtab")
        assert symtab_shidx != -1, ".symtab missing?"
        strm.seek(self.__shoff + (symtab_shidx *
                  self.__shentsize), strm.SEEK_BEGIN)

        # Get symtab data offset/size
        strm.seek(0x10, strm.SEEK_CURRENT)
        symtab_off = strm.get_u32()
        symtab_sz = strm.get_u32()
        strm.seek(0xC, strm.SEEK_CURRENT)
        symtab_entsz = strm.get_u32()

        # Number of entries in symbol table
        symtab_count = symtab_sz // symtab_entsz

        # Parse symtab data
        for i in range(symtab_count):
            # Seek to symtab entry
            strm.seek(symtab_off + (i * symtab_entsz), strm.SEEK_BEGIN)
            #
            pass

    def __get_section_idx(self, name: str):
        """Get index of section by name
        """
        for i, sect in enumerate(self.__sh_list):
            if sect == name:
                return i
        return -1

    def __get_sh_name(self, off: int):
        """Get section name, given an offset into shstrtab
        """
        name = ""
        c = self.__shstrtab[off]
        off += 1
        while c != 0x00:
            name += chr(c)
            c = self.__shstrtab[off]
            off += 1
        return name

    def __get_sym_name(self, off: int):
        """Get section name, given an offset into strtab
        """
        name = ""
        c = self.__strtab[off]
        off += 1
        while c != 0x00:
            name += chr(c)
            c = self.__strtab[off]
            off += 1
        return name
