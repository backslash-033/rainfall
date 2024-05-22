# level4

This is basically the same as the level3, but now we have to change the value
to 0x01025544 instead of 0x40. Which is equal to 16930116.
This value is too much so we need to split it into two values:
- 0x0102 -> 258
- 0x5544 -> 21828

But now we are facing a problem, `%n` only writes `int` so 4 bytes.
Which makes it impossible to have the good value

```
first write		01020000
second write	00005544
=
final value		00005544
```

Thankfully, `printf` comes with `%hn`, to write half an int to the location, so 2 bytes.
Now it looks like this:

```
first write		0102
second write	    5544
=
final value		01025544
```

So now instead of writing once, we need to write twice with totally different values.

Also, instead of giving us a shell, the program directly `cat`s the file.
Barely anything to do!

```
level4@RainFall:~$ python level4.py | ./level4
���X�P�                                                                                                                                                                                                                                  3086956496

[...]
                                                                                                                                      0x8049810
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

## .pass: 0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a