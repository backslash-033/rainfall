#include <stdio.h>

int main(int argc, char **argv) {
	int ret_value;
	int cmp;
	char *tmp_buff1;
	char zero;
	char buff_1 [64];
	char buff_2 [66];
	FILE *pass_file;

	zero = 0;
	pass_file = fopen("/home/user/end/.pass", "r");
	tmp_buff1 = buff_1;
	for (cmp = 0x21; cmp != 0; cmp = cmp + -1) {
		*tmp_buff1 = 0;
		tmp_buff1 = tmp_buff1 + zero * -2 + 1;
	}

	if ((pass_file == NULL) || (argc != 2)) {
		ret_value = -1;
	}

	else {
		fread(buff_1, 1, 0x42, pass_file);
		cmp = atoi(argv[1]);
		buff_1[cmp] = '\0';
		fread(buff_2, 1, 0x41, pass_file);
		fclose(pass_file);
		cmp = strcmp(buff_1, argv[1]);
		if (cmp == 0) {
			execl("/bin/sh", "sh", 0);
		}
		else {
			puts(buff_2);
		}
		ret_value = 0;
	}
	return ret_value;
}