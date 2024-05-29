# Bonus 3

Basically, what the program does is:
- Reading the `/home/user/end/.pass` file
- Comparing the first `n` bytes with `argv[1]`, where `n` is `atoi(argv[1])`
- If its the same, it gives us control to a shell
- Otherwise, it prints a new line.

Pretty straightforward, but if we give it an empty string, the `n` will be 0,
so it will compare an empty string (the first 0 bytes it read)
with another empty string (`argv[1]`)

```bash
bonus3@RainFall:~$ ./bonus3 ""
$ cd ../end
$ cat .pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```

## .pass: 3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
