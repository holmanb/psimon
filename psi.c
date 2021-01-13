#include "psi.h"
#include "debug.h"
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define STRSIZE 512

/* sample string:
 * some avg10=0.00 avg60=0.00 avg300=0.00 total=33239701
 *
 * for now only parsing the "some" string and returning the total
 * TODO - add support for collecting "total" blocked counters
 */
void parse_some(char *s, unsigned long long *some){
	return;
}

void psi_parse(char *s, unsigned long long *some, unsigned long long *full){

	// grab "some" line for io/memory
	char *endptr, *b1, *b2, *b3;
	unsigned long long val;
	debug("parsing value from:\n[%s]\n",s);

	b1 = strtok(s, "\n");
	debug("b1=[%s]\n",b1);

	b2 = strtok(b1, "l");
	debug("b2=[%s]\n",b2);

	b3 = strtok(NULL, "=");
	debug("b3=[%s]\n", b3);

	errno = 0;
	val = strtoull(b3, &endptr, 10);

	if (errno != 0){
		perror("strtol");
		die("%s", "number parse error\n");
	}

	if (endptr == b2) {
		die("%s", "No digits were found\n");
	}

	debug("val=%lld\n",val);
	*some = val;
	return;
}

void psi_update(struct psi *p, unsigned long long *n, unsigned long long *diff){
	static int init = 1;
	if(init){
		init = 0;
		*diff = 0;
		p->counter.value_curr = *n;
		return;
	}
	*diff = *n - p->counter.value_curr;
	p->counter.value_curr = *n;
	debug("\ncurr %lld \ndiff %lld \n",
			p->counter.value_curr,
			*diff);
}

void psi_init(struct psi *p, char *tgt, FILE *s){

	p->is_active = 1;
	p->counter.value_curr = 0;
	p->counter.value_prev = 0;
	p->counter.value_diff = 0;
	p->snk = s;
	p->target = open(tgt, O_RDONLY);
	if (p->target < 0) {
              die("%s: %s\n",
			tgt,
			strerror(errno));
	}
}

int psi_observe(struct psi *p){
	char buf1[STRSIZE] = "";
	char buf2[STRSIZE] = "";
	unsigned long long some, full, diff;
	ssize_t r = 0;
	off_t o = 0;

	o = lseek(p->target, 0, SEEK_SET);
	if(o == -1){
		die("seeking %s\n",strerror(errno));
	}
	r = read(p->target, buf1, STRSIZE);
	if (-1 == r){
		die("error reading stats %s\n",strerror(errno));
	} else if (0 == r){
		die("%s"," read in zero bytes from file\n");
	}
	if(buf1[0] != '\0'){
		psi_parse(buf1, &some, &full);
		psi_update(p, &some, &diff);
		snprintf(buf2, STRSIZE, "%lld", diff);
	} else {
		die("%s", "unexpected empty buffer\n");
	}
	if(strlen(buf2) == 0){
		die("%s","parsing error\n");
	}
	fprintf(p->snk, "%s\n", buf2);

	fflush(p->snk);
	return 0;
}

void psi_destroy(struct psi *p){
	close(p->target);
}
