#include <stdbool.h>
#include <stdio.h>

char	*service;
char	*auth;

void main() {
  char auth_char_cmp;
  char *str_in;
  int j;
  uint after_auth_length;
  bool bool_1;
  bool bool_2;
  bool bool_3;
  bool bool_4;
  bool bool_5;
  char zero;
  char buff[5];
  char after_auth[2];
  char after_service[125];
  char *buff_cpy;
  char *compared_str;
  
  zero = 0;
  do {
	// Getting the imput
	printf("%p, %p \n", auth, service);
	str_in = fgets(buff, 128, stdin);
	bool_3 = false;
	bool_4 = str_in == 0x0;
	if (bool_4) {
	  return 0;
	}

	// AUTH

	// (Wierdly) comparing buffer with "auth "
	j = 5;
	buff_cpy = buff;
	compared_str = "auth ";
	do {
	  if (j == 0) break;
	  j = j + -1;
	  bool_3 = *buff_cpy < *compared_str;
	  bool_4 = *buff_cpy == *compared_str;
	  buff_cpy = buff_cpy + zero * -2 + 1;
	  compared_str = compared_str + zero * -2 + 1;
	} while (bool_4);

	// If buffer is "auth ", mallocs for auth and
	//	copies the whats in after_auth into auth
	bool_1 = 0;
	bool_2 = (!bool_3 && !bool_4) == bool_3;
	if (bool_2) {
	  auth = malloc(4);
	  *auth = 0;
	  after_auth_length = 0xffffffff;
	  str_in = after_auth;

	  do { // FOR THIS LOOP: str_in is the stack array of 2 char
		if (after_auth_length == 0) break;
		after_auth_length = after_auth_length - 1;
		auth_char_cmp = *str_in;
		str_in = str_in + zero * -2 + 1;
	  } while (auth_char_cmp != '\0');

	  after_auth_length = ~after_auth_length - 1;
	  bool_1 = after_auth_length < 30;
	  bool_2 = after_auth_length == 30;

	  if (after_auth_length < 31) {
		strcpy(auth, after_auth);
	  }
	}

	// RESET

	// (Wierdly) comparing buffer with "reset"
	j = 5;
	buff_cpy = buff;
	compared_str = "reset";
	do {
	  if (j == 0) break;
	  j = j + -1;
	  bool_1 = *buff_cpy < *compared_str;
	  bool_2 = *buff_cpy == *compared_str;
	  buff_cpy = buff_cpy + zero * -2 + 1;
	  compared_str = compared_str + zero * -2 + 1;
	} while (bool_2);

	// If the buffer is "reset", free auth
	bool_5 = 0;
	bool_1 = (!bool_1 && !bool_2) == bool_1;
	if (bool_1) {
	  free(auth);
	}

	// SERVICE

	// (Wierdly) comparing buffer with "service"
	j = 6;
	buff_cpy = buff;
	compared_str = "service";
	do {
	  if (j == 0) break;
	  j = j + -1;
	  bool_5 = *buff_cpy < *compared_str;
	  bool_1 = *buff_cpy == *compared_str;
	  buff_cpy = buff_cpy + zero * -2 + 1;
	  compared_str = compared_str + zero * -2 + 1;
	} while (bool_1);

	// If buffer is "service", strdup after_service in service
	bool_2 = 0;
	bool_1 = (!bool_5 && !bool_1) == bool_5;
	if (bool_1) {
	  bool_2 = 0xfffffff8 < buff;
	  bool_1 = after_service == 0;
	  service = strdup(after_service);
	}

	// LOGIN

	// (Wierdly) comparing buffer with "login"
	j = 5;
	buff_cpy = buff;
	compared_str = "login";
	do {
	  if (j == 0) break;
	  j = j + -1;
	  bool_2 = *buff_cpy < *compared_str;
	  bool_1 = *buff_cpy == *compared_str;
	  buff_cpy = buff_cpy + zero * -2 + 1;
	  compared_str = compared_str + zero * -2 + 1;
	} while (bool_1);

	// If the buffer is "login"
	//	If there is nothing in auth[32], print "Password:"
	//	else, we have a shell
	if ((!bool_2 && !bool_1) == bool_2) {
	  if (auth[0x20 /* 32 */] == 0) {
		fwrite("Password:\n", 1, 10, stdout);
	  }
	  else {
		system("/bin/sh");
	  }
	}
  } while(true);
}