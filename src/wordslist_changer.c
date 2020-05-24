#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include "../lib/my_hacking_lib.h"

#define MINARGS 2

int
args_ok(int nargs)
{
	return nargs == MINARGS;
}

int
file_ok(char *file)
{
	return access(file, R_OK) == 0;
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

void
change_wordslist(char *wordslist_file)
{
	WordsChangedType *words_changed_list;
	
	if(! file_ok(wordslist_file))
		errx(EXIT_FAILURE, "%s\n", "[ERROR] wordslist file is not correct!");


	//read file and change words one by one and print it by stdout
}

int
main(int argc, char *argv[])
{

	if(! args_ok(argc))
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Usage error! -h for help");

	if(strncmp(argv[1], "-h", strlen("-h")) == 0){
		show_help();
		exit(EXIT_SUCCESS);
	}
	if(strncmp(argv[1], "-c", strlen("-c")) == 0)
		printf("%s\n", "Combination option");
	else
		change_wordslist(argv[1]);

	exit(EXIT_SUCCESS);
}
