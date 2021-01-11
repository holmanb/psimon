#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "opts_psi.h"

static void
print_usage(FILE *f)
{
	fprintf(f,
		"usage: psimon [opts]\n"
		"opts\n"
		"  -a show all values\n"
		"  -h - duh...\n"
		"\n"
		"ex.\n"
		"psimon\n"
		);
}



int
parse_opts_psi(struct opts * o, int argc, char **argv)
{
	char opt;
	int lc = 0;

	while ((opt = getopt(argc, argv, "a")) != -1) {
		switch (opt) {
		case 'a':
			o->all = 1;
			break;
		case 'h':
			print_usage(stdout);
			exit(EXIT_SUCCESS);
		default:
			print_usage(stderr);
			exit(EXIT_FAILURE);
		}
	}

	return lc;
}

