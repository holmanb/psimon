#ifndef _PSI_OPTS_H
#define _PSI_OPTS_H

#define CPU 1
#define IO  2
#define MEM 4
#define ALL_RESOURCES (CPU|IO|MEM)

#define SOME 1
#define FULL 2
#define ALL_COUNTER_TYPES (SOME|FULL)



struct opts {
	char resources;
	char counter_types;
};

int parse_opts_psi(struct opts *, int argc, char **argv);
#endif
