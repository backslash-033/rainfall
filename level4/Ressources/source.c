#include <stdio.h>

int m;

void	p(char *buff) {
	printf(buff);
}

void	n() {
	char buff[0x200];

	fgets(buff, 0x200, stdin);
	p(buff);
	if (m == 0x01025544) {
		system("/bin/cat /home/user/level5/.pass");
	}
}

int main() {
	n();
}