#define _POSIX_C_SOURCE 199309L

#include <signal.h>

#include <stdio.h>
#include <time.h>

#include "psi.h"
#include "debug.h"
#include "loop.h"
#include "input.h"
#include "plot.h"
#include "util.h"

static struct sigaction sigact = { 0 };
static int proceed = 1;

static void
handle_sigint(int _)
{
	(void)_;
	proceed = 0;
}

static void
install_signal_handler(void)
{
	sigact.sa_flags = 0;
	sigact.sa_handler = handle_sigint;
	sigaction(SIGINT, &sigact, NULL);
}

// TODO: add support for drawing multiple values simultaneously
// this functionality can be tested w/plot by specifying multiple files (-i) with values in them
void draw(struct plot *p){
	size_t i;
	if (!pdtry_all_buffers(p, 1)) {
		for (i = 0; i < p->datasets; i++) {
			if (feof(p->data[i].src.src)) {
				clearerr(p->data[i].src.src);
			}
		}
	}

	if (plot_plot(p)) {
		printf("\033[%dA", p->height);
		fflush(stdout);
	}
}

void
loop(struct plot *p, struct psi **ps, long ms)
{
	unsigned int height = p->height;

	if (p->x_label.every && p->x_label.side) {
		height += p->x_label.side == 3 ? 2 : 1;
	}

	struct timespec sleep = {
		.tv_sec = 0,
		.tv_nsec = 10000000,

	};

	install_signal_handler();

	printf("\033[?25l");

	while (proceed) {
		psi_observe(ps[0]);
		psi_observe(ps[1]);
		psi_observe(ps[2]);
		draw(p);
		nanosleep(&sleep, NULL);
	}

	printf("\033[%dB\033[?12l\033[?25h\n", height);
}
