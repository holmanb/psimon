#include "debug.h"
#include "loop.h"
#include "input.h"
#include "opts.h"
#include "plot.h"
#include "psi.h"
#include <unistd.h>

int
main(int argc, char **argv)
{
	char *prog = argv[0];	
	char* argv_fake[] = { prog, "-f" };
	struct plot pl = { 0 };
	int lc = parse_opts(&pl, 2, argv_fake);
	plot_init(&pl);

	int f[2];
	if (pipe(f)){
		printf("can't allocate anonymous pipe\n");
		exit(1);
	}
	FILE* fp = fdopen(f[0], "r");
	if(fp == NULL) {
		debug("%s", "problem opening pipe\n");
	}

	if (pl.datasets == 0) {
		plot_add(&pl, fp, lc);
	}

	// TODO: plumb some options for this
	struct psi ps = { 0 };
	psi_init(&ps, "/proc/pressure/cpu", &f[1]);

	set_input_buffer_size(8);
	loop(&pl, &ps, pl.follow_rate);
	

	psi_destroy(&ps);
	plot_destroy(&pl);

	fflush(stdout);

	return 0;
}
