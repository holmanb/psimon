#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "opts_psi.h"
#include "debug.h"

static void
print_usage(FILE *f)
{
	fprintf(f,
		"usage: psimon [counter type] [resource]\n"
		"opts:\n"
		"counter type:\n"
		"  -f - full\n"
		"  -s - some\n"
		"resource:\n"
		"  -c - cpu\n"
		"  -i - io\n"
		"  -m - mem\n"
		"\n"
		"ex.\n"
		"psimon -fim\n\n"
		"note: the kernel doesn't report cpu/full counters, so requesting -cf alone will fail explicitly\n"
		"combinations such as -sf -ci will silently ignore the request for cpu/full counters and happily \n"
		"provide cpu/some as well as io/some and io/full counters\n"
		);
}



int
parse_opts_psi(struct opts * o, int argc, char **argv)
{
/*
 * default = smci (some, compute, io, memory)
 */
	char opt;

	while ((opt = getopt(argc, argv, "f:s:m:c:i:h")) != -1) {
		switch (opt) {
		case 'f':
			o->counter_types = FULL;
			break;
		case 's':
			o->counter_types = SOME;
			break;
		case 'm':
			o->resources = MEM;
			break;
		case 'c':
			o->resources = CPU;
			break;
		case 'i':
			o->resources = IO;
			break;
		case 'h':
			print_usage(stdout);
			exit(EXIT_SUCCESS);
		default:
			print_usage(stderr);
			exit(EXIT_FAILURE);
		}
	}
	// reasonable defaults
	if(!o->resources){
		o->resources = ALL_RESOURCES;
	}
	if(!o->counter_types){
		o->counter_types= SOME;
	}
	if((o->counter_types & FULL) && (o->resources & CPU)) die("%s\n", "CPU/full is an invalid counter combination, since the kernel does not report this metric\n");


	if(o->counter_types & FULL) die("%s\n", "full counter type not yet implemented");

	return 0;
}

