"""
Utils: colored output, hexdump
"""

__all__ = ['hexdump', 'CPrinter', 'cprinter']

import re
import sys
import ctypes


class COORD(ctypes.Structure):
    _fields_ = [("X", ctypes.c_short), ("Y", ctypes.c_short)]

class SMALL_RECT(ctypes.Structure):
    _fields_ = [("Left", ctypes.c_short), ("Top", ctypes.c_short), ("Right", ctypes.c_short), ("Bottom", ctypes.c_short)]

class CONSOLE_SCREEN_BUFFER_INFO(ctypes.Structure):
    _fields_ = [
        ("dwSize", COORD),
        ("dwCursorPosition", COORD),
        ("wAttributes", ctypes.c_ushort),
        ("srWindow", SMALL_RECT),
        ("dwMaximumWindowSize", COORD)]


class CPrinter(object):
    linux_colors = {'red': 91, 'green': 92, 'blue': 94, 'yellow': 93, 'cyan': 96, 'white': 97}
    win_colors = {'red': 0x04, 'green': 0x02, 'blue': 0x01, 'yellow': 0x0E, 'cyan': 0x0B, 'white': 0x0F}
    tag_re = re.compile(r'\[([a-z]+)\]((?:.|\n)*?)\[/([a-z]+)\]', flags=re.M)

    def __init__(self):
        if sys.platform != 'win32':
            self.handle = None
        else:
            self.handle = ctypes.windll.kernel32.GetStdHandle(-11)
            self.csbi = CONSOLE_SCREEN_BUFFER_INFO()
            ctypes.windll.kernel32.GetConsoleScreenBufferInfo(self.handle, ctypes.byref(self.csbi))

    def cprint_text(self, color, text):
        if self.handle:
            ctypes.windll.kernel32.SetConsoleTextAttribute(self.handle, self.win_colors[color] if color in self.win_colors else 0xCF)
            sys.stdout.write(text)
            ctypes.windll.kernel32.SetConsoleTextAttribute(self.handle, self.csbi.wAttributes)
        else:
            sys.stdout.write('\033[%dm' % self.linux_colors[color] if color in self.linux_colors else 0)
            sys.stdout.write(text)
            sys.stdout.write('\033[0m')

    def cprint(self, format_str, *args, **kwargs):
        formatted_str = format_str.format(*args, **kwargs)
        pos = 0
        while pos < len(formatted_str):
            m = self.tag_re.search(formatted_str, pos)
            if not m:
                sys.stdout.write(formatted_str[pos:])
                break
            assert m.group(1) == m.group(3), 'Wrong color tag syntax'
            start, end = m.span()
            if start > pos:
                sys.stdout.write(formatted_str[pos:start])
            pos = end
            self.cprint_text(m.group(1), m.group(2))


cprinter = CPrinter().cprint


# character translation table to show only printable ascii chars
visible_ascii = ''.join(chr(x) if len(`chr(x)`) == 3 or x == ord('\\') else '.' for x in range(256))

def hexdump(buf, length=-1, offset=0, print_offset=0, cols=16, ident=0):
    length = len(buf) - offset if length == -1 else length
    end_index = offset + length
    spaces = ' ' * ident
    if 0 <= offset < end_index <= len(buf):
        buf = memoryview(buf)
        print_delta = print_offset - offset
        for ofs in xrange(offset, end_index, cols):
            data = buf[ofs:min(ofs + cols, end_index)]
            print spaces + '{:08X}: {} {}{}'.format(print_delta + ofs,  # displayed offset
                ' '.join('{:02X}'.format(ord(x)) for x in data),        # hex bytes
                '   ' * (cols - len(data)),                             # padding if incomplete line of bytes
                data.tobytes().translate(visible_ascii))                # printable ascii chars
