#include "test5.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <stdlib.h>
#include <stdio.h>
#include "pixmap.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv) {
	/* DO NOT FORGET TO initialize service */
	sef_startup(); // Service initialized

	if (argc == 1) { /* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"Test_Init <decimal no.- mode> <decimal no.- delay>\"\n"
					"\t service run %s -args \"Test_Square <decimal no.- x> <decimal no. - y> <decimal no.- size> <decimal no.- color\"\n"
					"\t service run %s -args \"Test_Line <decimal no.- Xi> <decimal no.- Yi> <decimal no.- Xf> <decimal no.- Yf>\"\n"
					"\t service run %s -args \"Test_xpm <decimal no.- *xpm[]> <decimal no.- Xi> <decimal no.- Yi>\"\n"
					"\t service run %s -args \"Test_Move <decimal no.- *xpm[]> <decimal no.- Xi> <decimal no.- Yi> <decimal no.- Xf> <decimal no.- Yf> <decimal no.- Speed> <decimal no.- Frame Rate>\"\n"
					"\t service run %s -args \"Test_Controller\"\n", argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	unsigned short mode, delay, x, y, size, xi, yi, xf, yf;
	unsigned long color, speed, framerate;
	char ** xpm;

	if (strncmp(argv[1], "Test_Init", strlen("Test_Init")) == 0) {
		if (argc != 4) {
			printf(
					"VIDEO CARD:: wrong no. of arguments for video_test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16); /* Parses string to unsigned long */
		if (mode == ULONG_MAX)
			return 1;
		delay = parse_ulong(argv[3], 10);
		if (delay == ULONG_MAX)
			return 1;
		printf("Video Card::video_test_init(%x, %u)\n", mode, delay);
		return video_test_init(mode, delay);
	} else if (strncmp(argv[1], "Test_Square", strlen("Test_Square")) == 0) {
		if (argc != 6) {
			printf(
					"VIDEO CARD:: wrong no. of arguments for video_test_square()\n");
			return 1;
		}
		x = parse_ulong(argv[2], 10); /* Parses string to unsigned long */
		if (x == ULONG_MAX)
			return 1;
		y = parse_ulong(argv[3], 10);
		if (y == ULONG_MAX)
			return 1;
		size = parse_ulong(argv[4], 10);
		if (size == ULONG_MAX)
			return 1;
		color = parse_ulong(argv[5], 16);
		if (color == ULONG_MAX)
			return 1;

		printf("VIDEO CARD::video_test_square(%u, %u, %u, %u)\n", x, y, size,
				color);
		return video_test_square(x, y, size, color);
	} else if (strncmp(argv[1], "Test_Line", strlen("Test_Line")) == 0) {
		if (argc != 7) {
			printf(
					"VIDEO CARD:: wrong no of arguments for video_test_line()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10); /* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;
		yi = parse_ulong(argv[3], 10); /* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;
		xf = parse_ulong(argv[4], 10); /* Parses string to unsigned long */
		if (xf == ULONG_MAX)
			return 1;
		yf = parse_ulong(argv[5], 10); /* Parses string to unsigned long */
		if (yf == ULONG_MAX)
			return 1;
		color = parse_ulong(argv[6], 16); /* Parses string to unsigned long */
		if (color == ULONG_MAX)
			return 1;
		printf("VIDEO CARD::video_test_line(%u, %u, %u, %u, %u)\n", xi, yi, xf,
				yf, color);
		return video_test_line(xi, yi, xf, yf, color);
	} else if (strncmp(argv[1], "Test_xpm", strlen("Test_xpm")) == 0) {
		if (argc != 5) {
			printf("VIDEO CARD:: wrong no of arguments for test_xpm()\n");
			return 1;
		}
		xi = parse_ulong(argv[3], 10); /* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;
		yi = parse_ulong(argv[4], 10); /* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;
		printf("VIDEO CARD::test_xpm(%u, %u, %u)\n", argv[2], xi, yi);

		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0) {
			xpm = pic1;
		} else if (strncmp(argv[2], "pic2", strlen("pic2")) == 0) {
			xpm = pic2;
		} else if (strncmp(argv[2], "cross", strlen("cross")) == 0) {
			xpm = cross;
		} else if (strncmp(argv[2], "pic3", strlen("pic3")) == 0) {
			xpm = pic3;
		} else if (strncmp(argv[2], "penguin", strlen("penguin")) == 0) {
			xpm = penguin;
		}
		else
		{
			printf("video_card: invalid arguments for test_xpm(xi,yi,xpm)\n");
			return 1;
		}

		printf("test_xpm(%s,%d, %d)\n", xpm, xi, yi);
		return test_xpm(xpm, xi, yi);
	} else if (strncmp(argv[1], "Test_Move", strlen("Test_Move")) == 0) {
		if (argc != 9) {
			printf("VIDEO CARD:: wrong no of arguments for test_move()\n");
			return 1;
		}
		xi = parse_ulong(argv[3], 10); /* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;
		yi = parse_ulong(argv[4], 10); /* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;
		xf = parse_ulong(argv[5], 10); /* Parses string to unsigned long */
		if (xf == ULONG_MAX)
			return 1;
		yf = parse_ulong(argv[6], 10); /* Parses string to unsigned long */
		if (yf == ULONG_MAX)
			return 1;
		speed = parse_ulong(argv[7], 10); /* Parses string to unsigned long */
		if (speed == ULONG_MAX)
			return 1;
		framerate = parse_ulong(argv[8], 10); /* Parses string to unsigned long */
		if (framerate == ULONG_MAX)
			return 1;

		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0) {
					xpm = pic1;
				} else if (strncmp(argv[2], "pic2", strlen("pic2")) == 0) {
					xpm = pic2;
				} else if (strncmp(argv[2], "cross", strlen("cross")) == 0) {
					xpm = cross;
				} else if (strncmp(argv[2], "pic3", strlen("pic3")) == 0) {
					xpm = pic3;
				} else if (strncmp(argv[2], "penguin", strlen("penguin")) == 0) {
					xpm = penguin;
				} else {
					printf("video_card: invalid arguments for test_xpm(xpm,xi.yi)\n");
					return 1;
		}

		printf("VIDEO CARD::test_move(%u, %u, %u, %u, %u, %u, %u)\n", xpm, xi,
				yi, xf, yf, speed, framerate);
		return test_move(xpm, xi, yi, xf, yf, speed, framerate);
	} else if (strncmp(argv[1], "Test_Controller", strlen("Test_Controller"))
			== 0) {
		if (argc != 2) {
			printf(
					"VIDEO CARD:: wrong no. of arguments for test_controller()\n");
			return 1;
		}

		printf("VIDEO CARD::test_controller()\n");
		return test_controller();
	}

	else {
		printf("VIDEO CARD:: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
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
		printf("Video card: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
