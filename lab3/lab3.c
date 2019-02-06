#include "i8042.h" //TOCOMPLETE
#include "test3.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/drivers.h>
#include "KBD.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup(); // Service initialized

	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(SELF);

	/* Prints usage of the program if no arguments are passed */
	if (argc == 1)
	{
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"Test_Scan <0 for C // 1 for Assembly>\"\n"
			"\t service run %s -args \"Polling\"\n"
			"\t service run %s -args \"Timed_Scan <decimal no. - time>\"\n",
			argv[0], argv[0], argv[0]);
}


static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("Keyboard: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}


static int proc_args(int argc, char **argv)
{
	unsigned long assmbl, time; //Only 2 args since poll doesn't accept them.

	if (strncmp(argv[1], "Test_Scan", strlen("Test_Scan")) == 0)
	{
		if (argc != 3)
		{
			printf("Keyboard: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		assmbl = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		//In case user didn't press 0 or 1
		if (assmbl != 1 && assmbl != 0)
		{
			printf("User can only press 0 or 1 (C or Assembly)!");
					return 1;
		}
		if (assmbl == ULONG_MAX)
			return 1;
		printf("KBD::kbd_test_scan(%lu)\n", assmbl);
		return kbd_test_scan(assmbl);
	}

	else if (strncmp(argv[1], "Polling", strlen("Polling")) == 0)
	{
		if (argc != 2)
		{
			printf("Keyboard: wrong no. of arguments for kbd_test_poll()\n");
			return 1;
		}
		printf("KBD::kbd_test_poll()\n");
		return kbd_test_poll();
	}

	else if (strncmp(argv[1], "Timed_Scan", strlen("Timed_Scan")) == 0)
	{
		if (argc != 3)
		{
			printf("Keyboard: wrong no of arguments for kbd_test_timed_scan()\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("KBD::kbd_test_timed_scan(%lu)\n", time);
		return kbd_test_timed_scan(time);
	}
	else {
		printf("Keyboard: %s - no valid function!\n", argv[1]);
		return 1;
	}
}
