#include "my_hacking_lib.h"

struct WordsChangedType{
	char *word;
	WordsChangedType *next;
};


//PRIVATE FUNCTIONS

char
random_hexchar(int *seed)
{
    int random_int, range;
    range = 'F' - '0' + 1;

    //Initialize seed for pseudo-random int generator
    srand(*seed);

    random_int = rand();
    *seed = random_int;

    random_int = (random_int % range) + '0';

    if(random_int > '9' && random_int < 'A')
        random_int+= ('A' - '9' - 1);
    if(random_int > 'F')
        random_int = 'F';

    return random_int;
}

//PUBLIC FUNCTIONS

void
mk_mac(char *mac, int mac_len)
{
    int i, seed;

    if(mac_len < MACLEN)
        errx(EXIT_FAILURE, "%s\n", "[ERROR] MAC length has to be over than 17");

    seed = getpid();
    i = 0;
    while(i < MACLEN){
        mac[i++] = random_hexchar(&seed);
        mac[i++] = random_hexchar(&seed);
        if(i < MACLEN - 1)
            mac[i++] = ':';
    }
}
