import csv
from sys import argv
import os

file = ".gdbinit"
user = "level0"

if len(argv) >= 2:
    user = argv[1]
else:
    print("Please put user name as first argument")
    exit(2)

if len(argv) >= 3:
    file = argv[2]

data = csv.reader(open("data.csv"))

next(data)

for username, password in data:
    if username == user:
        cmd = "sshpass -p " + password + " ssh -p 4243 " + user + "@localhost chmod 755 ."
        print(cmd)
        os.system(cmd)

        cmd = "sshpass -p " + password + " scp -P 4243 " + file + " " + user + "@localhost:" + file
        print(cmd)
        os.system(cmd)

        cmd = "sshpass -p " + password + " ssh -p 4243 " + user + "@localhost chmod 644 " + file
        print(cmd)
        os.system(cmd)
        break
else:
    print("User", user, "not found in data.csv")