int	language;

int	greetuser(char *name) {
	char	*hello;
	char	buff[13];

	if (language == 1) {
		hello = "Hyvää päivää ";
		for (int i = 0; i < strlen(hello); i++) {
			buff[i] = hello[i];
		}
	}
	else if (language == 2) {
		hello = "Goedemiddag! ";
		for (int i = 0; i < strlen(hello); i++) {
			buff[i] = hello[i];
		}
	}
	else if (language == 0) {
		hello = "Hello ";
		for (int i = 0; i < strlen(hello); i++) {
			buff[i] = hello[i];
		}
	}
	strcat(buff, name);
	return puts(buff);
}

int main(int argc, char **argv) {
	int ret_value;
	int i;
	char *tmp_buff1;
	char *tmp_buff2;
	char *name;
	char zero;
	char first_arg[40];
	char second_arg[36];
	char *LANG;
	
	zero = 0;
	if (argc == 3) {
		tmp_buff2 = first_arg;
		for (i = 0x13; i != 0; i = i + -1) {
			*tmp_buff2 = 0;
			tmp_buff2 = tmp_buff2 + 4;
		}
		strncpy(first_arg, argv[1], 40);
		strncpy(second_arg, argv[2], 32);
		LANG = getenv("LANG");
		if (LANG != NULL) {
			i = memcmp(LANG, "fi", 2);
			if (i == 0) {
				language = 1;
			}
			else {
				i = memcmp(LANG, "nl", 2);
				if (i == 0) {
					language = 2;
				}
			}
		}
		tmp_buff1 = first_arg;
		for (i = 0x13; i != 0; i = i + -1) {
			name = name + zero * -8 + 4;
			tmp_buff1 = tmp_buff1 + zero * -8 + 4;
			*name = *tmp_buff1;

		}
		ret_value = greetuser(name);
	}
	else {
		ret_value = 1;
	}
	return ret_value;
}