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


/* TODO: plumb some options: 
* - select between memory/io/cpu  (or maybe support them simultaniously)
* - different cgroups
* - define colors
* - configure speed/timestep?
* - configure y axis?
*/

int
main(int argc, char **argv)
{
	printf("\n    psimon               ");

	printf("    cpu=red ");
	printf("    io=blue ");
	printf("    mem=yell \n\n");

	char *filenames[] = { "/tmp/file1", "/tmp/file2", "/tmp/file3" };

	char b[32];
	snprintf(b, 32, "0:%d", Y_LABEL);
	debug("Y axis is size: %s\n", b);
	char* argv_fake[] = { "psimon", "-b", b, "-m", \
		 "-c", "r", "-i", filenames[0], \
		 "-c", "b", "-i", filenames[1], \
		 "-c", "y", "-i", filenames[2], \
		 "-f"};

	struct plot pl = { 0 };

	FILE ** temp_files = calloc(16, sizeof(FILE *));
	if(temp_files == NULL) {die("%s", "couldn't allocate mem\n");}
	temp_files[0] = fopen(filenames[0], "w");
	temp_files[1] = fopen(filenames[1], "w");
	temp_files[2] = fopen(filenames[2], "w");

	plot_init(&pl);
	parse_opts(&pl, sizeof(argv_fake)/sizeof(argv_fake[0]) - (2 * DYNAMIC_AXIS_TMP), argv_fake);

	struct psi ps1 = { 0 };
	struct psi ps2 = { 0 };
	struct psi ps3 = { 0 };

	psi_init(&ps1, "/proc/pressure/cpu", temp_files[0]);
	psi_init(&ps2, "/proc/pressure/io", temp_files[1]);
	psi_init(&ps3, "/proc/pressure/memory", temp_files[2]);

	struct psi ** metrics = NULL;
	metrics = calloc(3, sizeof(struct psi *));
	if(metrics == NULL) {die("%s", "couldn't allocate mem\n");}

	metrics[0] = &ps1;
	metrics[1] = &ps2;
	metrics[2] = &ps3;

	set_input_buffer_size(8);
	loop(&pl, metrics, pl.follow_rate);

	psi_destroy(&ps1);
	psi_destroy(&ps2);
	psi_destroy(&ps3);
	plot_destroy(&pl);

	fflush(stdout);

	return 0;
}
