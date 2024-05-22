#include <stdio.h>

int m;

void	o() {
	system("/bin/sh");
	_exit(1);
}

void	n() {
	char buff[0x200];

	fgets(buff, 0x200, stdin);
	printf(buff);
	exit(1);
}

int main() {
	n();
}