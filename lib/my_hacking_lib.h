#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>

#define MACLEN 17

typedef struct WordsChangedType WordsChangedType;

void mk_mac(char *mac, int mac_len);
