# level0

We have an executable named `level0`. Launching it results in a segmentation fault. but launching it with arguments results in the program printing `No !`. So we can assume that this program checks something in the argument.

```bash
level0@RainFall:~$ ./level0 
Segmentation fault (core dumped)
level0@RainFall:~$ ./level0 a
No !
```

With `objdump`, we can check what is the assembly code of the program.


```bash
level0@RainFall:~$ objdump -M intel -D level0 | grep -n '<main>:' -A 52
1292:08048ec0 <main>:
1293- 8048ec0:	55                   	push   ebp
1294- 8048ec1:	89 e5                	mov    ebp,esp
1295- 8048ec3:	83 e4 f0             	and    esp,0xfffffff0
1296- 8048ec6:	83 ec 20             	sub    esp,0x20
1297- 8048ec9:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
1298- 8048ecc:	83 c0 04             	add    eax,0x4
1299- 8048ecf:	8b 00                	mov    eax,DWORD PTR [eax]
1300- 8048ed1:	89 04 24             	mov    DWORD PTR [esp],eax
1301- 8048ed4:	e8 37 08 00 00       	call   8049710 <atoi>
1302- 8048ed9:	3d a7 01 00 00       	cmp    eax,0x1a7
1303- 8048ede:	75 78                	jne    8048f58 <main+0x98>
1304- 8048ee0:	c7 04 24 48 53 0c 08 	mov    DWORD PTR [esp],0x80c5348
1305- 8048ee7:	e8 04 7d 00 00       	call   8050bf0 <__strdup>
1306- 8048eec:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
1307- 8048ef0:	c7 44 24 14 00 00 00 	mov    DWORD PTR [esp+0x14],0x0
1308- 8048ef7:	00 
1309- 8048ef8:	e8 83 b7 00 00       	call   8054680 <__getegid>
1310- 8048efd:	89 44 24 1c          	mov    DWORD PTR [esp+0x1c],eax
1311- 8048f01:	e8 6a b7 00 00       	call   8054670 <__geteuid>
1312- 8048f06:	89 44 24 18          	mov    DWORD PTR [esp+0x18],eax
1313- 8048f0a:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
1314- 8048f0e:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
1315- 8048f12:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
1316- 8048f16:	89 44 24 04          	mov    DWORD PTR [esp+0x4],eax
1317- 8048f1a:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
1318- 8048f1e:	89 04 24             	mov    DWORD PTR [esp],eax
1319- 8048f21:	e8 da b7 00 00       	call   8054700 <__setresgid>
1320- 8048f26:	8b 44 24 18          	mov    eax,DWORD PTR [esp+0x18]
1321- 8048f2a:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
1322- 8048f2e:	8b 44 24 18          	mov    eax,DWORD PTR [esp+0x18]
1323- 8048f32:	89 44 24 04          	mov    DWORD PTR [esp+0x4],eax
1324- 8048f36:	8b 44 24 18          	mov    eax,DWORD PTR [esp+0x18]
1325- 8048f3a:	89 04 24             	mov    DWORD PTR [esp],eax
1326- 8048f3d:	e8 4e b7 00 00       	call   8054690 <__setresuid>
1327- 8048f42:	8d 44 24 10          	lea    eax,[esp+0x10]
1328- 8048f46:	89 44 24 04          	mov    DWORD PTR [esp+0x4],eax
1329- 8048f4a:	c7 04 24 48 53 0c 08 	mov    DWORD PTR [esp],0x80c5348
1330- 8048f51:	e8 ea b6 00 00       	call   8054640 <execv>
1331- 8048f56:	eb 28                	jmp    8048f80 <main+0xc0>
1332- 8048f58:	a1 70 e1 0e 08       	mov    eax,ds:0x80ee170
1333- 8048f5d:	89 c2                	mov    edx,eax
1334- 8048f5f:	b8 50 53 0c 08       	mov    eax,0x80c5350
1335- 8048f64:	89 54 24 0c          	mov    DWORD PTR [esp+0xc],edx
1336- 8048f68:	c7 44 24 08 05 00 00 	mov    DWORD PTR [esp+0x8],0x5
1337- 8048f6f:	00 
1338- 8048f70:	c7 44 24 04 01 00 00 	mov    DWORD PTR [esp+0x4],0x1
1339- 8048f77:	00 
1340- 8048f78:	89 04 24             	mov    DWORD PTR [esp],eax
1341- 8048f7b:	e8 b0 12 00 00       	call   804a230 <_IO_fwrite>
1342- 8048f80:	b8 00 00 00 00       	mov    eax,0x0
1343- 8048f85:	c9                   	leave  
1344- 8048f86:	c3                   	ret
```

With a quick glance, we can see (_line 1301-1303_) that atoi is called and its result is compared to 0x1a7 (423 in decimal). <br />
If it's not equal to 423, it jumps to 0x8048f58 (_line 1332_) and eventually calls `write` (_line 1341_). We can assume that this is the `write` call that writes `No !` to the standard output. <br />
However if its equal to 423, it calls `getegid`, `geteuid`, `setresgid` and `setresuid` before calling `execv`. So it changes the gid and uid and then replaces the program with another one. <br />
Lets try with 423 as an argument:

```sh
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ pwd
/home/user/level0
$ cd ../level1
$ ls -la
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1
$ cat .pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
$ 
```

We are presented with a shell prompt, with the help of basic commands, we were able to figure out that we have changed user to `level1`. With this we can change directory to `level1`'s home and display the content of `.pass`

## .pass: `1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a`