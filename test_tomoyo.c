#include <unistd.h>

int main(int argc, char **argv)
{
	chdir("Test");
	return execv(argv[1], argv+1);
}

