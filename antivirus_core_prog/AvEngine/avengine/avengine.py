#!/usr/bin/python
"""
Simple Antivirus Engine
"""

import os
import sys
import glob
import json
import mmap
import time

from argparse import ArgumentParser

import pefile
from utils import cprinter, hexdump

import hashlib

class AvEngine(object):
    RESULT_CLEAN = 0
    RESULT_INFECTED = 1
    RESULT_ERROR = 2

    def __init__(self, virusbase_dir):
        self.virusbase_dir = virusbase_dir  # directory, where virus base is located
        self.virus_records = []             # a list of virus records, should be filled somehow
        self.cur_map = None                 # current scanning file is loaded as memory mapping
        self.pe = pefile.PeFileParser()     # helper object to work with PE files

    # next two methods for `with` statement
    def __enter__(self):
        self.load_virusbases()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type:
            from traceback import format_exception
            cprinter('\n[red]Exception in AvEngine:[/red]\n')
            sys.stdout.write('\n'.join(format_exception(exc_type, exc_val, exc_tb)))
        if self.cur_map:
            self.cur_map.close()
        return True

    def load_virusbases(self):
        # load virus base
        # as an example, we implemented loading virus bases, stored in json format 
        for virbase in glob.iglob(os.path.join(self.virusbase_dir, '*.vdb')):
            with open(virbase, 'rb') as f:
                self.virus_records.extend(json.load(f))
        cprinter('[white]loaded {} records[/white]\n', len(self.virus_records))

    def scanmem(self):
        ####
        pass

    def scanfile(self, fname):
        # main function for scanning a file
        result = self.RESULT_CLEAN
        virus_name = ''
        cprinter('[white]{}[/white] - ', fname)
        try:
            self.cur_map = None
            file_size = os.path.getsize(fname)
            if file_size:
                with open(fname, 'rb') as f:
                    # map file to memory
                    self.cur_map = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
                    # load the new file to the PE object, parse it and check if it is valid PE file or not
                    valid = self.pe.parse(self.cur_map, file_size)
                    
                    # add scanning logic here (day 3)

                    # ============== start ==============
                    # this is only an example. just for information, delete this block
                    if valid:
                        cprinter('[green]Valid PE file[/green]\n')
                        cprinter('[yellow]  * Header (fo): {:08X}[/yellow]\n', self.pe.get_pe_header_offset())
                        data_rva = self.pe.get_datasect_rva()
                        cprinter('[yellow]  *  Data (rva): {:08X}[/yellow]  [cyan]Data bytes:[/cyan]\n', data_rva)
                        data_offset = self.pe.rva_to_offset(data_rva)
                        hexdump(self.pe.data[data_offset:data_offset+64], ident=5)
                        ep_rva = self.pe.get_ep_rva()
                        cprinter('[yellow]  *  Easy (rva): {:08X}[/yellow]  [cyan]Data bytes:[/cyan]\n', ep_rva)
                        ep_offset = self.pe.rva_to_offset(ep_rva)
                        hexdump(self.pe.data[ep_offset:ep_offset+4096], ident=5)
                        for v in self.virus_records:
                            offset = v["Offset"]
                            if v["Type"] == "EASY":
                                offset += ep_offset
                            elif v["Type"] == "DATA":
                                offset += data_offset
                            else:
                                continue

                            if v["ChkByte"] != ord(self.pe.data[offset]):
                                continue
                            if v["Hash"] == hashlib.md5(self.pe.data[offset:offset+v["Length"]]).hexdigest():
                                print "Virus detected: "+v["VirusName"]

                    else:
                        cprinter('[cyan]Not a PE file[/cyan]\n')
                    return
                    # =============== end ===============
        except:
            if self.cur_map:
                self.cur_map.close()
            result = self.RESULT_ERROR
        
        # print scanning result
        if result == self.RESULT_ERROR:
            cprinter('[red]Error[/red]\n')
        elif result == self.RESULT_CLEAN:
            cprinter('[green]Clean[/green]\n')
        elif result == self.RESULT_INFECTED:
            cprinter('[red]Infected[/green] - [cyan]{}[/cyan]\n', virus_name)

    def scanfiles(self, directory):
        # scan files inside the directory
        for root, dirs, files in os.walk(directory):
            for fname in files:
                self.scanfile(os.path.join(root, fname))


def main():
    start_time = time.clock()
    
    parser = ArgumentParser(description=__doc__, prog=os.path.basename(__file__))
    parser.add_argument('-m', '--memoryscan', action='store_true', help='Force memory scanning')
    parser.add_argument('-v', '--virbasedir', default='.', help='Directory where virus bases located')
    parser.add_argument('dirs', nargs='*', help='Directories to scan')
    args = parser.parse_args()

    with AvEngine(args.virbasedir) as engine:
        if args.memoryscan:
            cprinter('[yellow]Scanning memory[/yellow]\n')
            engine.scanmem()

        cprinter('[yellow]Scanning files[/yellow]\n')
        if not args.dirs:
            args.dirs.append('.')
        for directory in args.dirs:
            engine.scanfiles(directory)
    
    cprinter('[white]Scanning done in {:.1f} seconds[/white]\n', time.clock() - start_time)
    return 0


if __name__ == '__main__':
    sys.exit(main())
