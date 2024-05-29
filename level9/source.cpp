#include <unistd.h>
#include <string.h>

class N {
public:
	N(int a) {
		this->param = a;
	}

	int operator+(N *this, N *other) {
		return other->param + this->param;
	}

	void	setAnnotation(char *str) {
		size_t	n;
	
		n = strlen(str);
		memcpy(this.s, str, n);
	}

	char s[100]; // Not sure
	int	param;
}

void main(int argc,char **argv)
{
	N *base;
	N *other;

	if (argc < 2) {
		exit(1);
	}
	base = new N(5);

	other = new N(6);

	base.setAnnotation(argv[1]);

	other + base;
	return;
}