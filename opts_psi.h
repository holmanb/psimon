#ifndef _PSI_OPTS_H
#define _PSI_OPTS_H

struct opts {
	char resources;
	// MEM | CPU | IO
	
//	char mem;
//	char cpu;
//	char io;

	// SOME | FULL
	char counter_types;
//	char some;
//	char full;
};

int parse_opts_psi(struct opts *, int argc, char **argv);
#endif
