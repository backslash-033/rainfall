#include <string.h>
#include <stdlib.h>

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