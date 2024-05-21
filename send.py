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
        new_file = file
        if "/" in new_file:
            new_file = new_file[new_file.rfind("/") + 1::]
        cmd = "sshpass -p " + password + " scp -P 4243 " + file + " " + user + "@localhost:" + new_file
        print(cmd)
        os.system(cmd)

        cmd = "sshpass -p " + password + " ssh -p 4243 " + user + "@localhost chmod 644 " + new_file
        print(cmd)
        os.system(cmd)
        break
else:
    print("User", user, "not found in data.csv")