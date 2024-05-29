void p() {
	int		return_addr;
	char	buffer[50];

	fflush();
	gets(buffer);

	return_addr = __builtin_return_address(0); // Return address of the current function

	if ((return_addr & 0xb0000000) == 0xb0000000) {
		printf("(%p)", return_addr);
		exit(1);
	}

	puts(buffer);
	strdup(buffer);
}

void main() {
	p();
}