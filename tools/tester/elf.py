from input_stream import InputStream
from dataclasses import dataclass

"""
1. Fully parse file header
2. Fully parse section headers into list (including section data)
3. Fill in section header names using shstrtab
4. Fully parse symtab into list of symbols (don't read sym data yet)
5. Fill in symbol names using strtab
6. Fixup symbol sizes (0 == next sym ofs - this sym ofs)
7. Read symbol data
"""


@dataclass
class ELFSection():
    name: str
    data: bytearray

    sh_name: int
    sh_type: int
    sh_flags: int
    sh_addr: int
    sh_offset: int
    sh_size: int
    sh_link: int
    sh_info: int
    sh_addralign: int
    sh_entsize: int

    def __init__(self, strm: InputStream):
        """Parse ELF section header from stream
        """
        assert not strm.eof()

        # Must be set manually later,
        # after shstrtab is read
        self.name = "NoName"

        self.sh_name = strm.get_u32()
        self.sh_type = strm.get_u32()
        self.sh_flags = strm.get_u32()
        self.sh_addr = strm.get_u32()
        self.sh_offset = strm.get_u32()
        self.sh_size = strm.get_u32()
        self.sh_link = strm.get_u32()
        self.sh_info = strm.get_u32()
        self.sh_addralign = strm.get_u32()
        self.sh_entsize = strm.get_u32()

        # Read section data
        if self.sh_size > 0:
            strm.seek(self.sh_offset, InputStream.SEEK_BEGIN)
            self.data = strm.read(self.sh_size)


@dataclass
class ELFHeader():
    ei_mag: str
    ei_class: int
    ei_data: int
    ei_version: int
    ei_osabi: int
    ei_abiversion: int
    e_type: int
    e_machine: int
    e_version: int
    e_entry: int
    e_phoff: int
    e_shoff: int
    e_flags: int
    e_ehsize: int
    e_phentsize: int
    e_phnum: int
    e_shentsize: int
    e_shnum: int
    e_shstrndx: int

    E_CLASS_32 = 1
    E_DATA_BE = 2
    E_MACHINE_PPC = 0x14

    def __init__(self, strm: InputStream):
        """Parse ELF file header from stream
        """
        assert not strm.eof()

        self.ei_mag = strm.get_string_sz(4)
        self.ei_class = strm.get_u8()
        self.ei_data = strm.get_u8()
        self.ei_version = strm.get_u8()
        self.ei_osabi = strm.get_u8()
        self.ei_abiversion = strm.get_u8()

        # Skip padding
        strm.read(7)

        self.e_type = strm.get_u16()
        self.e_machine = strm.get_u16()
        self.e_version = strm.get_u32()
        self.e_entry = strm.get_u32()
        self.e_phoff = strm.get_u32()
        self.e_shoff = strm.get_u32()
        self.e_flags = strm.get_u32()
        self.e_ehsize = strm.get_u16()
        self.e_phentsize = strm.get_u16()
        self.e_phnum = strm.get_u16()
        self.e_shentsize = strm.get_u16()
        self.e_shnum = strm.get_u16()
        self.e_shstrndx = strm.get_u16()

    def is_valid(self):
        """Confirm that the ELF header is of the type
        expected by this tool (32-bit, PowerPC, Big Endian)
        """
        if self.ei_mag != "\x7FELF":
            return False

        if self.ei_class != self.E_CLASS_32:
            return False

        if self.ei_data != self.E_DATA_BE:
            return False

        if self.e_machine != self.E_MACHINE_PPC:
            return False

        return self.e_shnum > 0


@dataclass
class ELFFile():
    header: ELFHeader
    sections: list[ELFSection]

    def __init__(self, strm: InputStream):
        """Parse ELF file
        """
        assert not strm.eof()

        # File header
        self.header = ELFHeader(strm)
        assert self.header.is_valid(), "Invalid ELF header"

        # Section headers
        self.sections = list()
        for i in range(self.header.e_shnum):
            # Seek to section
            strm.seek(self.header.e_shoff +
                      (i * self.header.e_shentsize), InputStream.SEEK_BEGIN)
            self.sections.append(ELFSection(strm))

        # Fixup section header names using shstrtab
        shstrtab = InputStream(
            self.sections[self.header.e_shstrndx].data, InputStream.ENDIAN_BIG)
        for sect in self.sections:
            # Seek to name data
            shstrtab.seek(sect.sh_name, InputStream.SEEK_BEGIN)
            # Read section name
            sect.name = shstrtab.get_string()


# class ELFFile():
#     def __init__(self, strm: InputStream):
#         """Parse ELF file
#         """
#         self.__sh_list = list()
#         self.__st_dict = dict()
#         self.__shstrtab = None
#         self.__strtab = None
#         self.__shoff = -1
#         self.__shentsize = -1
#         self.__shnum = -1
#         self.__shstrndx = -1

#         assert not strm.eof()

#         # Parse header + extract section names
#         self.__parse_file_header(strm)
#         self.__extract_shstrtab(strm)

#         # Parse sections
#         self.__parse_section_headers(strm)

#         # Extract string table + parse symbols
#         self.__extract_strtab(strm)
#         self.__parse_symbols(strm)

#     def __parse_file_header(self, strm: InputStream):
#         """Parse required info from the ELF header
#         """
#         assert not strm.eof()

#         # Magic
#         assert strm.get_string_sz(
#             4) == "\x7FELF", "Object file is not an ELF file"
#         # Class
#         assert strm.get_u8() == 1, "Object file is not a 32-bit executable"
#         # Endianness
#         assert strm.get_u8() == 2, "Object file is not a big-endian executable"
#         # Machine
#         strm.seek(0x12, strm.SEEK_BEGIN)
#         assert strm.get_u16() == 0x14, "Object file does not use 32-bit PowerPC arch"

#         # Section header table offset
#         strm.seek(0x20, strm.SEEK_BEGIN)
#         self.__shoff = strm.get_u32()
#         # Section header table entry size
#         strm.seek(0x2E, strm.SEEK_BEGIN)
#         self.__shentsize = strm.get_u16()
#         # Section header count
#         strm.seek(0x30, strm.SEEK_BEGIN)
#         self.__shnum = strm.get_u16()
#         # Section ID of .shstrtab
#         self.__shstrndx = strm.get_u16()

#     def __extract_shstrtab(self, strm: InputStream):
#         """Find and extract the shstrtab section (section header name string table)
#         """
#         assert not strm.eof()

#         # Seek to shstrtab section header
#         strm.seek(self.__shoff + (self.__shstrndx *
#                   self.__shentsize), strm.SEEK_BEGIN)

#         # Get shstrtab data offset/size
#         strm.seek(0x10, strm.SEEK_CURRENT)
#         shstrtab_off = strm.get_u32()
#         shstrtab_sz = strm.get_u32()

#         # Extract section
#         strm.seek(shstrtab_off, strm.SEEK_BEGIN)
#         self.__shstrtab = strm.read(shstrtab_sz)

#     def __extract_strtab(self, strm: InputStream):
#         """Find and extract the strtab section (symbol name string table)
#         """
#         assert not strm.eof()

#         # Seek to strtab section header
#         strtab_idx = self.__get_section_idx(".strtab")
#         assert strtab_idx, ".strtab missing?"
#         strm.seek(self.__shoff + (strtab_idx *
#                   self.__shentsize), strm.SEEK_BEGIN)

#         # Get strtab data offset/size
#         strm.seek(0x10, strm.SEEK_CURRENT)
#         strtab_ofs = strm.get_u32()
#         strtab_sz = strm.get_u32()

#         # Extract strtab data
#         strm.seek(strtab_ofs, strm.SEEK_BEGIN)
#         self.__strtab = strm.read(strtab_sz)

#     def __parse_section_headers(self, strm: InputStream):
#         """Parse ELF section headers (minimally).
#         We only need a list of the section names, to index once we parse symbols.
#         """
#         assert not strm.eof()

#         for i in range(self.__shnum):
#             # Seek to section's header
#             strm.seek(self.__shoff + (i *
#                                       self.__shentsize), strm.SEEK_BEGIN)
#             # Get name of section
#             name_off = strm.get_u32()
#             self.__sh_list.append(self.__get_sh_name(name_off))

#     def __parse_symbols(self, strm: InputStream):
#         """Parse symtab section
#         """
#         assert not strm.eof()

#         # Find symtab section header
#         symtab_shidx = self.__get_section_idx(".symtab")
#         assert symtab_shidx != -1, ".symtab missing?"
#         strm.seek(self.__shoff + (symtab_shidx *
#                   self.__shentsize), strm.SEEK_BEGIN)

#         # Get symtab data offset/size
#         strm.seek(0x10, strm.SEEK_CURRENT)
#         symtab_off = strm.get_u32()
#         symtab_sz = strm.get_u32()
#         strm.seek(0xC, strm.SEEK_CURRENT)
#         symtab_entsz = strm.get_u32()

#         # Number of entries in symbol table
#         symtab_count = symtab_sz // symtab_entsz

#         # Parse symtab data
#         for i in range(symtab_count):
#             # Seek to symtab entry
#             strm.seek(symtab_off + (i * symtab_entsz), strm.SEEK_BEGIN)

#             # Symbol name
#             st_name = strm.get_u32()
#             st_name = self.__get_sym_name(st_name)

#             # Symbol value (or addr)
#             st_value = strm.get_u32()
#             # Symbol size
#             st_size = strm.get_u32()
#             # Symbol type/bind
#             st_info = strm.get_u8()
#             # Symbol visibility
#             st_other = strm.get_u8()
#             # Symbol parent section index
#             st_shndx = strm.get_u16()

#             # No name symbol
#             if st_name == "":
#                 st_name = self.__sh_list[st_shndx]

#             # Read symbol data (if it is not in a special section, or is UNDEF)
#             if st_shndx > 0 and st_shndx < self.__shnum:
#                 # Extract symbol data/code
#                 strm.seek(self.__shoff + (st_shndx *
#                           self.__shentsize), strm.SEEK_BEGIN)
#                 strm.seek(0x10, strm.SEEK_CURRENT)
#                 # Offset of parent section
#                 sect_off = strm.get_u32()
#                 # Offset of symbol data
#                 sym_off = sect_off + st_value
#                 strm.seek(sym_off, strm.SEEK_BEGIN)
#                 sym_data = strm.read(st_size)

#                 # Add symbol to dict
#                 sect_name = self.__sh_list[st_shndx]
#                 if not sect_name in self.__st_dict:
#                     self.__st_dict[sect_name] = list()

#                 self.__st_dict[sect_name].append(
#                     ELFSymbol(st_name, st_value, st_size, st_info, st_other, self.__sh_list[st_shndx], sym_data))

#     def __get_section_idx(self, name: str):
#         """Get index of section by name
#         """
#         for i, sect in enumerate(self.__sh_list):
#             if sect == name:
#                 return i
#         return -1

#     def __get_sh_name(self, off: int):
#         """Get section name, given an offset into shstrtab
#         """
#         name = ""
#         c = self.__shstrtab[off]
#         off += 1
#         while c != 0x00:
#             name += chr(c)
#             c = self.__shstrtab[off]
#             off += 1
#         return name

#     def __get_sym_name(self, off: int):
#         """Get symbol name, given an offset into strtab
#         """
#         name = ""
#         c = self.__strtab[off]
#         off += 1
#         while c != 0x00:
#             name += chr(c)
#             c = self.__strtab[off]
#             off += 1
#         return name

# class ELFSymbol():
#     def __init__(self, st_name: str, st_value: int, st_size: int, st_info: int,
#                  st_other: int, section: str, data: bytes):
#         self.__name = st_name
#         self.__value = st_value
#         self.__size = st_size
#         self.__info = st_info
#         self.__other = st_other
#         self.__section = section
#         self.__data = data
