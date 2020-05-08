#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <regex.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <string.h>
#include <ctype.h>

#define NARGS 3
#define MAXIFACELEN 15
#define MACLEN 17

int
char_hexa(char c)
{
	int pos = c;

	return (pos >= '0' && pos <= '9') || (pos >= 'A' && pos <= 'F') ||
												(pos >= 'a' && pos <= 'f');
}

int
iface_isok(char *iface)
{
	struct ifaddrs *ifaddr, *ifa;

	if(getifaddrs(&ifaddr) < 0)
		errx(EXIT_FAILURE, "%s\n", "[Error] Was not possible get network interfaces");

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){

		if(ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET)
			continue;

		if(strncmp(iface, ifa->ifa_name, sizeof(char) * MAXIFACELEN) == 0){
			freeifaddrs(ifaddr);
			return 1;
		}
	}

	freeifaddrs(ifaddr);
	printf("%s\n", "Network Interface Doesn't Exist!");
	return 0;
}

int
eth_addr_isok(char *eth)
{
	int ok, i;

	ok = 1;

	if(strlen(eth) != MACLEN)
		ok = 0;
	if(ok){

		for(i = 0;;){
			if(! char_hexa(eth[i++]) || ! char_hexa(eth[i++]))
				ok = 0;

			if(i == MACLEN)
				break;

			if(eth[i++] != ':')
				ok = 0;

			if(! ok)
				break;
		}
	}

	if(! ok)
		printf("%s\n", "MAC Address Invalid!");

	return ok;
}

int
args_ok(int nargs, char *args[])
{

	if(nargs != NARGS)
		return 0;

	return iface_isok(args[NARGS - 2]) && eth_addr_isok(args[NARGS - 1]);

}

void
change_mac(char *iface, char *eth)
{
	/*
	 *HAS TO BE FINISHED!
	 */
	;
}

int
main(int argc, char *argv[])
{

	if(! args_ok(argc, argv))
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Usage Error");

	change_mac(argv[NARGS - 2], argv[NARGS - 1]);

	exit(EXIT_SUCCESS);
}
