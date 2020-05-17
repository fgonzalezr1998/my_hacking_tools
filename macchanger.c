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
wait_for_child(int child_pid)
{
	int status;
	if(waitpid(child_pid, &status, 0) < 0)
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Waitpid Failed!");

	if(WEXITSTATUS(status) != 0)
		fprintf(stderr, "%s\n", "[WARN] Child not finished well");

}

void
exec_macchanger(char *iface, char *eth)
{
	char *args[5];

	args[0] = "macchanger";
	if(strncmp(eth, "random", strlen("random")) == 0){
		args[1] = "-r";
		args[2] = iface;
		args[3] = NULL;
	}else{
		args[1] = "-m";
		args[2] = eth;
		args[3] = iface;
		args[4] = NULL;
	}
	execvp("macchanger", args);
}

void
change_mac(char *iface, char *eth)
{
	int pid;
	
	pid = fork();
	switch(pid){
		case -1:
			errx(EXIT_FAILURE, "%s\n", "[ERROR]Fork Failed!");
		case 0:
			exec_macchanger(iface, eth);
		default:
			wait_for_child(pid);
	}
}

void
show_help()
{
	printf("%s\n", "macchanger [-h] device new_mac");
}

int
main(int argc, char *argv[])
{
	if(geteuid() != 0)
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Make sudo!");

	if(strncmp(argv[1], "-h", strlen("-h")) == 0){
		show_help();
		exit(EXIT_SUCCESS);
	}

	if(! args_ok(argc, argv))
		errx(EXIT_FAILURE, "%s\n", "[ERROR] Usage Error");

	change_mac(argv[NARGS - 2], argv[NARGS - 1]);

	exit(EXIT_SUCCESS);
}
