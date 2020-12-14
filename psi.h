#ifndef _PSI_H
#define _PSI_H

struct psi {
	int snk;
	int target;
	unsigned long long value_curr;
	unsigned long long value_prev;
	unsigned long long value_diff;
};

void psi_init(struct psi *, char *, int);
int psi_observe(struct psi *p);
void psi_destroy(struct psi *);
void parse(char *, unsigned long long *);
void update(struct psi *, unsigned long long *, unsigned long long *);
#endif
