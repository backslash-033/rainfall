
void p(char *dest, char *to_print) {
	char	*first_nl;
	char	read_buffer[4104];

	puts(to_print);
	read(0,read_buffer, 0x1000);
	first_nl = strchr(read_buffer, 10);
	*first_nl = '\0';
	strncpy(dest, read_buffer, 0x14);
	return;
}

void pp(char *buff) {
	int		len;
	int		zero;
	char	*tmp_buff;
	char	tmp_buff_chr;
	char	s1[20];
	char	s2[20];

	zero = 0;
	p(s1, " - ");
	p(s2, " - ");
	strcpy(buff, s1);
	len = 0xffffffff;
	tmp_buff = buff;
	do {
		if (len == 0) break;
		len = len - 1;
		tmp_buff_chr = *tmp_buff;
		tmp_buff = tmp_buff + zero * -2 + 1;
	} while (tmp_buff_chr != '\0');

	*(buff + (~len - 1)) = 0x20; // Basically last char of buff is a space
	strcat(buff, s2);
	return;
}

int main(void) {
	char buff[54];

	pp(buff);
	puts(buff);
	return 0;
}