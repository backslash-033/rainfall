#include <stdio.h>

int	m;

void	v() {
	char	buff[512];

	fgets(buff, 512, stdin);
	printf(buff);

	if (m == 64) {
		printf("Wait what!?");
		system("/bin/sh");
	}
}

void	main() {
	v();
}