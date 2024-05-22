void	n() {
	system("/bin/cat /home/user/level7/.pass");
}

void	m() {
	puts("Nope!");
}

int main(int argc, char **argv) {
	char	*str;
	void	*(*func)();

	str = malloc(0x40);
	func = malloc(0x4);

	func = m;
	strcpy(str, argv[1]);
	(*func)();
}