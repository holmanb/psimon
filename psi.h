#ifndef _PSI_H
#define _PSI_H
#include<stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_METRICS 5

struct psi {
	char is_active; // toggled depending on user input
	int target;
	FILE *snk;
	unsigned long long value_curr;
	unsigned long long value_prev;
	unsigned long long value_diff;
};

void psi_init(struct psi *, char *, FILE*);
int psi_observe(struct psi *p);
void psi_destroy(struct psi *);
void psi_parse(char *, unsigned long long *, unsigned long long *);
void psi_update(struct psi *, unsigned long long *, unsigned long long *);
#endif
