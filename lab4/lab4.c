#include "test4.h"
#include "i8254.h"
#include "i8042.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/drivers.h>


static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);


int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup(); // Service initialized

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"Test_Packet <decimal no.- COUNTS>\"\n"
			"\t service run %s -args \"Test_Remote <decimal no.- PERIOD> <decimal no. - COUNTS\"\n"
			"\t service run %s -args \"Test_Async <decimal no.- SECONDS_IDLE>\"\n"
			"\t service run %s -args \"Test_Gesture <decimal no.- LENGTH>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short count, seconds_idle, period;
	short length;

	if (strncmp(argv[1], "Test_Packet", strlen("Test_Packet")) == 0) {
		if (argc != 3) {
			printf("MOUSE:: wrong no. of arguments for mouse_test_packet()\n");
			return 1;
		}
		count = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (count == ULONG_MAX)
			return 1;
		printf("Mouse::mouse_test_packet(%d)\n", count);
		return mouse_test_packet(count);
	}
	else if (strncmp(argv[1], "Test_Remote", strlen("Test_Remote")) == 0) {
		if (argc != 4) {
			printf("MOUSE:: wrong no. of arguments for mouse_test_async()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (period == ULONG_MAX)
			return 1;
		count = parse_ulong(argv[3],10);
		if(count == ULONG_MAX)
			return 1;
		printf("MOUSE::mouse_test_remote(%d, %d)\n", period, count);
		return mouse_test_remote(period, count);
	}
	else if (strncmp(argv[1], "Test_Async", strlen("Test_Async")) == 0) {
		if (argc != 3) {
			printf("MOUSE:: wrong no of arguments for mouse_test_async()\n");
			return 1;
		}
		seconds_idle = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (seconds_idle == ULONG_MAX)
			return 1;
		printf("MOUSE::mouse_test_async(%d)\n", seconds_idle);
		return mouse_test_async(seconds_idle);
	}else if (strncmp(argv[1], "Test_Gesture", strlen("Test_Gesture")) == 0) {
		if (argc != 3) {
			printf("MOUSE:: wrong no of arguments for mouse_test_gesture()\n");
			return 1;
		}
		length = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (length == ULONG_MAX)
			return 1;
		printf("MOUSE::mouse_test_gesture(%d)\n", length);
		return mouse_test_gesture(length);
	}
	else {
		printf("MOUSE:: %s - no valid function!\n", argv[1]);
		return 1;
	}
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
		printf("Mouse: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
