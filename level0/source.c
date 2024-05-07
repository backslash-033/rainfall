#include <unistd.h>

int main(int argc, char **argv) {
	int uid;
	int gid;
	char *str;

	if (atoi(argv[1]) != 423) {
		write(2, "No !\n", 5);
	} else {
		str = strdup("/bin/sh");
		gid = getegid();
		uid = geteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);
		execv("/bin/sh", str);
	}
}