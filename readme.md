# Rainfall

Rainfall is a cybersecurity challenge with 14 different levels of binary exploitation.

This is a 42 project, here is [the subject](subject.pdf).

## Rules

This challenge is done in a virtual machine with different users, from `level0` to `level9` and from `bonus0` to `bonus3`. The last user is `end`.

From this point, we need to find a way to read the `.pass` file located in the home directory of the next user. The content of this file is the password for the user, thus enabling us to log in as the next user and completing the challenge of the current one.

Example: Being able to read `/home/level3/.pass` reveals us the password of the user `level3`, thus completing the challenge of the user `level2`.

To achieve this, we need to exploit a binary located in the home directory of the current user.

For a challenge to be valid, we need to explain how to realise it in a walkthrough. This means that bruteforcing the password of a user is forbidden, as it does not gives a proper explanation of how to exploit the given binary. We also need to give a self-developped source code that matches the behavior of the binary exploited.

## Set-up

To run this project at home, you will need to download [this .iso file](https://cdn.intra.42.fr/isos/RainFall.iso) and make it a 64 bit Linux virtual machine.

Once the virtual machine is running, You can connect via SSH to it on the port 4242.

The credentials for the first user are:
```
username: level0
password: level0
```

> ⚠️ This .iso file handles poorly the restart of the computer. So we would advise you to use the "Save state" (or similar depending on your virtual machine software) option to close it. 