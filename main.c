#include "debug.h"
#include "loop.h"
#include "input.h"
#include "opts.h"
#include "plot.h"
#include "psi.h"
#include <unistd.h>

#define DYNAMIC_AXIS_TMP 0

// TODO make file stuff more elegant (cleanup file on close, maybe an array or something
#define TMPFILE1 "/tmp/file1"
#define TMPFILE2 "/tmp/file2"

#define Y_LABEL_REQUESTED 1024
#define Y_LABEL  (MAX_HEIGHT-1) > Y_LABEL_REQUESTED ? Y_LABEL_REQUESTED : (MAX_HEIGHT-1)


int
main(int argc, char **argv)
{
	char b[32];
	printf("title\n");
	snprintf(b, 32, "0:%d", Y_LABEL);
	debug("Y axis is size: %s\n", b);
	char* argv_fake[] = { "psimon", "-b", b, "-m", \
		 "-c", "r", "-i", TMPFILE1, \
		 "-f"};
//		 "-c", "b", "-i", TMPFILE2, \


	struct plot pl = { 0 };

	FILE * file1 = fopen(TMPFILE1, "w");
	FILE * file2 = fopen(TMPFILE2, "w");

	plot_init(&pl);
	parse_opts(&pl, sizeof(argv_fake)/sizeof(argv_fake[0]) - (2 * DYNAMIC_AXIS_TMP), argv_fake);


	/* TODO: plumb some options: 
	* - select between memory/io/cpu  (or maybe support them simultaniously)
	* - different cgroups
	* - define colors
	* - configure speed/timestep?
	*/
	struct psi ps = { 0 };
	psi_init(&ps, "/proc/pressure/cpu", file1);
	//psi_init(&ps, "/proc/pressure/io", file2);
	//psi_init(&ps, "/proc/pressure/memory", file3);

	set_input_buffer_size(8);
	loop(&pl, &ps, pl.follow_rate);


	psi_destroy(&ps);
	plot_destroy(&pl);

	fflush(stdout);

	return 0;
}
