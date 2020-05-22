#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <regex.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../lib/my_hacking_lib.h"

#define MINARGS 2
#define MAXIFACELEN 15

void change_mac(char *iface, char *eth);

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

	fprintf(stderr, "%s\n", "Network Interface Doesn't Exist!");
	return 0;
}

int
eth_addr_isok(char *eth)
{
	int ok, i;

	ok = 1;
	if(strncmp(eth, "random", strlen("random")) == 0)
		return 1;
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
		fprintf(stderr, "%s\n", "MAC Address Invalid!");

	return ok;
}

int
args_ok(int nargs, char *args[])
{

	if(nargs != MINARGS + 1)
		return 0;

	return iface_isok(args[MINARGS - 1]) && eth_addr_isok(args[MINARGS]);

}

void
exec_macchanger(char *iface, char *eth)
{
	char *args[6];

	args[0] = "ifconfig";
	args[1] = iface;
	args[2] = "hw";
	args[3] = "ether";
	if(strncmp(eth, "random", strlen("random")) == 0)
		mk_mac(eth, MACLEN);

	args[4] = eth;
	args[5] = NULL;

	execvp("ifconfig", args);
}

void
wait_for_child(int child_pid, char *iface, char *eth)
{
	int status;
	if(waitpid(child_pid, &status, 0) < 0)
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Waitpid Failed!");

	if(WEXITSTATUS(status) == 0)
		fprintf(stdout, "%s\n", "[INFO] MAC CHANGED SUCCESFULLY");
	else if(WEXITSTATUS(status) != -2)
		change_mac(iface, eth);

}

void
change_mac(char *iface, char *eth)
{
	int pid, fd;

	pid = fork();
	switch(pid){
		case -1:
			errx(EXIT_FAILURE, "%s\n", "[ERROR]Fork Failed!");
		case 0:
			fd = open("/dev/null", O_WRONLY);
			if(fd < 0){
				fprintf(stderr, "%s\n", "[ERROR] Can't open file for dup stderr");
				exit(-2);
			}
			dup2(fd, fileno(stderr));
			close(fd);
			exec_macchanger(iface, eth);
		default:
			wait_for_child(pid, iface, eth);
	}
}

void
show_help()
{
	printf("%s\n", "macchanger [-h] device new_mac");
	printf("%s\n", "new_mac maybe 'random' for choosing random mac address");
}

int
main(int argc, char *argv[])
{
	if(argc < MINARGS)
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Usage Error");
	if(geteuid() != 0)
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Make sudo!");

	if(strncmp(argv[1], "-h", strlen("-h")) == 0){
		show_help();
		exit(EXIT_SUCCESS);
	}

	if(! args_ok(argc, argv))
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Usage Error");

	change_mac(argv[MINARGS - 1], argv[MINARGS]);

	exit(EXIT_SUCCESS);
}
