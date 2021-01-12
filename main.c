#include <unistd.h>
#include "debug.h"
#include "loop.h"
#include "input.h"
#include "opts.h"
#include "plot.h"
#include "psi.h"
#include "alloc.h"
#include "opts_psi.h"

#define Y_LABEL_REQUESTED 2048
#define Y_LABEL  (MAX_HEIGHT-1) > Y_LABEL_REQUESTED ? Y_LABEL_REQUESTED : (MAX_HEIGHT-1)


/* TODO: plumb some options:
* - select between memory/io/cpu
* - different cgroups
* - define colors
* - configure speed/timestep?
* - configure y axis?
*/

int
main(int argc, char **argv)
{
	int i;
	struct opts o;
	struct psi ** metrics;
	struct plot pl = { 0 };
	FILE ** temp_files;

	// TODO randomize this
	char *tmp_filenames[] = {
		"/tmp/DEADBEAF.file0",
		"/tmp/DEADBEAF.file1",
		"/tmp/DEADBEAF.file2",
		"/tmp/DEADBEAF.file3",
		"/tmp/DEADBEAF.file4",
	};

	char *psi_filenames[] = {
		/* some */
		"/proc/pressure/cpu",
		"/proc/pressure/io",
		"/proc/pressure/memory",

		/* full */
		"/proc/pressure/io",
		"/proc/pressure/memory",
	};


	// ALLOC
	temp_files = ccalloc(16, sizeof(FILE *));
	metrics = ccalloc(5, sizeof(struct psi *));

	for(i=0; i<NUM_METRICS; i++){
		metrics[i] = ccalloc(1, sizeof(struct psi));
	}

	// initialize to zero
	for(i=0; i< NUM_METRICS; i++){
		metrics[i]->is_active = 0;
	}
	// INIT
	o.resources = 0;
	o.counter_types = 0;

	parse_opts_psi(&o, argc, argv);

	// fopen files
	for(i=0; i<NUM_METRICS; i++){
		temp_files[i] = fopen(tmp_filenames[i], "w");
	}

	// init  psi
	for(i=0; i<NUM_METRICS; i++){
		psi_init(metrics[i], psi_filenames[i], temp_files[i]);
	}

	// TEMPORARY FOR TESTING
	for(i=2; i<NUM_METRICS; i++){
		metrics[i]->is_active = 0;
	}

	// hardcoded array needs to be dynamically constructed based on filenames available
	char* argv_fake[] = {
		"plot", \
		"-m", \
		"-f", \
		"-c", "r", "-i",tmp_filenames[0], \
		"-c", "b", "-i",tmp_filenames[1], \
		"-c", "y", "-i",tmp_filenames[2], \
		"-c", "R", "-i",tmp_filenames[3], \
		"-c", "B", "-i",tmp_filenames[4]};



	// plot initialization
	plot_init(&pl);

	//parse_opts(&pl, 23, argv_fake);
	parse_opts(&pl, 11, argv_fake);
	set_input_buffer_size(8);

	// should probably construct a legend based on params passed
	printf("\n    psimon                   cpu=red     io=blue     mem=yell \n\n");

	// DO THINGS
	loop(&pl, metrics, pl.follow_rate);

	// FREE
	free(temp_files);

	for(i=0; i<NUM_METRICS; i++){
		psi_destroy(metrics[i]);
	}
	free(metrics);

	plot_destroy(&pl);

	fflush(stdout);

	return 0;
}
