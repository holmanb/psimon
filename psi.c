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
void psi_parse(char *s, unsigned long long *n){

	// grab "some" line for io/memory
	char *endptr, *b1, *b2, *b3;
	unsigned long long val;

	b1 = strtok(s, "\n");
	//debug("b1=[%s]\n",b1);

	b2 = strtok(b1, "l");
	//debug("b2=[%s]\n",b2);

	b3 = strtok(NULL, "=");
	//debug("b3=[%s]\n", b3);
	

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
	if(val){
		*n = val;
		return;
	}
	die("error parsing value %s from %s\n", 
			b2, 
			s);
}

void psi_update(struct psi *p, unsigned long long *n, unsigned long long *diff){
	// don't print initial value as diff value
	static int init = 1;
	if(init){
		init = 0;
		*diff = 0;
		p->value_curr = *n;
		return;
	}
	*diff = *n - p->value_curr;
	p->value_curr = *n;
	debug("\ncurr %lld \ndiff %lld \n",
			p->value_curr,
			*diff);
}

void psi_init(struct psi *p, char *tgt, FILE *s){
	p->value_curr = 0;
	p->value_prev = 0;
	p->value_diff = 0;
	p->snk = s;
	p->target = open(tgt, O_RDONLY);
	if (p->target < 0) {
              die("%s: %s\n",
			tgt,
			strerror(errno));
	}
	int init=0;
	while(++init<15){
		fprintf(p->snk,"%s", "0\n");
	}
}


int psi_observe(struct psi *p){
	debug("%s","\n");
	char buf1[STRSIZE] = "";
	char buf2[STRSIZE] = "";
	unsigned long long val, diff;
	ssize_t r=0; 
	off_t o=0;

	o = lseek(p->target, 0, SEEK_SET);
	if(o == -1){
		die("seeking %s\n",
			strerror(errno));
	}
	r = read(p->target, buf1, STRSIZE);
	if (-1 == r){
		die("error reading stats %s\n",
			strerror(errno));
	} else if (0 == r){
		die("%s"," read in zero bytes from file\n");
	}
	if(buf1[0] != '\0'){
		psi_parse(buf1, &val);
		psi_update(p, &val, &diff);
		snprintf(buf2, STRSIZE, "%lld", diff);
	} else {
		die("%s", "empty buffer?  the heck?\n");
	}
	debug("writing buffer [%s]\n",
			buf2);
	if(strlen(buf2) == 0){
		die("%s","what happened here?\n");
	}
	fprintf(stdout, "[%s]", buf2);
	int ret=0;
	ret = fprintf(p->snk, "%s\n", buf2);
	fprintf(stdout, "return value of [%d]\n", ret);

	fflush(p->snk);
	return 0;
}
void psi_destroy(struct psi *p){
	close(p->target);
}
