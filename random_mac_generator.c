#include <stdlib.h>
#include <stdio.h>

#define MACLEN 17

char
random_hexchar()
{
    return 'a';
}

void
mk_mac(char *mac)
{
    int i = 0;
    while(i < MACLEN){
        mac[i++] = random_hexchar();
        mac[i++] = random_hexchar();
        if(i < MACLEN - 1)
            mac[i++] = ':';
    }
}

int
main(int argc, char *argv[])
{
    char mac[MACLEN];

    mk_mac(mac);

    //For Debugging:
    for(int i = 0; i < MACLEN; i++){
        printf("%c", mac[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
