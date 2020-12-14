#include "loop.h"
#include "input.h"
#include "opts.h"
#include "plot.h"
#include "psi.h"
#include <unistd.h>

int
main(int argc, char **argv)
{
	
	int f[2];
	if (pipe(f)){
		printf("can't allocate anonymous pipe\n");
		exit(1);
	}
	FILE* fp = fdopen(f[0], "r");
	struct plot pl = { 0 };
	struct psi ps = { 0 };
	plot_init(&pl);

	// TODO: plumb some options for this
	psi_init(&ps, "/proc/pressure/cpu", f[1]);
		
	int lc = parse_opts(&pl, argc, argv);

	if (pl.datasets == 0) {
		plot_add(&pl, fp, lc);
	}

	set_input_buffer_size(8);
	loop(&pl, &ps, pl.follow_rate);
	

	psi_destroy(&ps);
	plot_destroy(&pl);

	fflush(stdout);

	return 0;
}
