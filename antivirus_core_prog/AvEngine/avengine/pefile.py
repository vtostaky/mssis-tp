"""
Works with MZ PE files
"""

__all__ = ['PeFileParser']

import struct


class PeFileParser(object):
    """
        PeFileParser gets data and tries to parse
        them as PE file.
    """
    def __init__(self):
        self.clean()

    def clean(self):
        self.valid = False
        self.data = None
        self.size = 0
        self.pe_offset = 0
        self.number_of_sections = 0
        self.entry_rva = 0
        self.sections = []

    def parse(self, data, size):
        """
            Parses file header.
            Return:
                * True if file is valid PE file,
                * False otherwise
        """
        self.clean()
        self.data = data
        self.size = size
        if size <= 0x40:
            return False

        # check MZ signature
        if data[:2] != 'MZ':
            return False

        try:
            # get file offset to PE header
            self.pe_offset, = e_lfanew, = struct.unpack_from('<L', data, 0x3C)
            if e_lfanew + 0x2C >= size:
                return False

            # check PE\0\0 signature
            if data[e_lfanew : e_lfanew + 4] != 'PE\0\0':
                return False

            # read FileHeader & standard fields of OptionalHeader
            machine, \
            self.number_of_sections, \
            _, _, _, \
            size_of_opt_header, \
            characteristics, \
            magic, \
            _, _, _, _, _, \
            self.entry_rva = struct.unpack_from('<HHLLLHHHBBLLLL', data, e_lfanew + 4)

            if self.number_of_sections == 0:
                return False

            # read sections
            sections_offset = e_lfanew + size_of_opt_header + 0x18
            if sections_offset + 0x28 * self.number_of_sections > size:
                return False

            for ofs in xrange(sections_offset, sections_offset + 0x28 * self.number_of_sections, 0x28):
                section = {}
                section['Name'], \
                section['VirtualSize'], \
                section['VirtualAddress'], \
                section['SizeOfRawData'], \
                section['PointerToRawData'] = struct.unpack_from("<8sLLLL", data, ofs)
                self.sections.append(section)
        except struct.error:
            return False
        self.valid = True
        return self.valid

    def get_datasect_rva(self):
        """
            Return RVA offset to the first section 
             with data on disk (i.e SizeOfRawData != 0)
             [we call it DATA section]
            If there's no such section, return None
        """
        if self.valid:
            for sec in self.sections:
                if sec['SizeOfRawData']:
                    return sec['VirtualAddress']

    def get_pe_header_offset(self):
        """
            Get RVA offset to PE Header
             [we call it HEADER section]
        """
        if self.valid:
            return self.pe_offset

    def get_ep_rva(self):
        """
            Get RVA offset to the entry point
             [we call it EASY section]
        """
        if self.valid:
            return self.entry_rva

    def rva_to_offset(self, rva):
        """
            Convert RVA offset to File offset
        """
        if self.valid and rva is not None:
            for sec in self.sections:
                if sec['VirtualAddress'] <= rva < sec['VirtualAddress'] + sec['SizeOfRawData']:
                    return rva - sec['VirtualAddress'] + sec['PointerToRawData']

            # check if it's inside the header
            if rva < self.sections[0]['VirtualAddress']:
                return rva
