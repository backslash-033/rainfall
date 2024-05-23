#include <stdio.h>

char *c;

void	m() {
	printf("%s - %d\n", time(0), c);
}

int main(int param_1,char **argv) {
	long **tab1;
	long *a;
	long **tab2;
	FILE *file;

	tab1 = malloc(8);
	tab1[0] = 1;

	tab1[1] = malloc(8);

	tab2 = malloc(8);
	tab2[0] = 2;

	tab2[1] = malloc(8);

	strcpy(tab1[1], argv[1]);
	strcpy(tab2[1], argv[2]);

	file = fopen("/home/user/level8/.pass","r");
	fgets(c, 0x44, file);
	puts("~~");
	return 0;
}