#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define NARGS 2

int
args_ok(int nargs)
{
	return nargs == NARGS;
}

void
show_help()
{
	printf("%s\n", "DESCRIPTION:");
	printf("\t%s\n\n", "Make some changes to wordslist specified for do it more robust");

	printf("%s\n", "USAGE:");
	printf("\t%s\n\n", "wordslist_changer [-h] [wordslist]");

	printf("%s\n", "OPTIONS:");
	printf("\t%s\n", "-h: print help");
	printf("\t%s\n", "wordslist: path to wordslist file");
}

int
main(int argc, char *argv[])
{

	if(! args_ok(argc))
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Usage error! -h for help");

	if(strncmp(argv[1], "-h", strlen("-h")) == 0)
		show_help();

	exit(EXIT_SUCCESS);
}
