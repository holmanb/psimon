#ifndef _LOOP_H
#define _LOOP_H
struct plot;
struct psi;

void draw(struct plot *);
void loop(struct plot *, struct psi **, long);
#endif
