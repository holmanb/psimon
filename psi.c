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


/* some avg10=0.00 avg60=0.00 avg300=0.00 total=33239701
 * for now only parsing the "some" string and returning the total
 */
void parse(char *s, unsigned long long *n){

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
		debug("%s", "number parse error\n");
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	if (endptr == b2) {
		debug("%s", "No digits were found\n");
		exit(EXIT_FAILURE);
	}

	debug("val=%lld\n",val);
	if(val){
		*n = val;
		return;
	}
	debug("error parsing value %s from %s\n", 
			b2, 
			s);
}

void update(struct psi *p, unsigned long long *n, unsigned long long *diff){
	p->value_prev = p->value_curr;
	p->value_curr = *n;
	p->value_diff = p->value_curr - p->value_prev;
	*diff = p->value_diff;
}

void psi_init(struct psi *p, char *tgt, int snk){
	p->value_curr = 0;
	p->value_prev = 0;
	p->value_diff = 0;
	p->snk = snk;
	p->target = open(tgt, O_RDONLY);
	if (p->target < 0) {
              printf("%s: %s\n",
			tgt,
			strerror(errno));
              exit(1);
	}
}


int psi_observe(struct psi *p){
	char buf1[512] = "";
	char buf2[512] = "";
	//buf1[0]=buf2[0]='\0';
	unsigned long long val, diff;
	ssize_t r=0; 
	off_t o=0;

	// not all filesystems support seek, could close and reopen file for greater support
	o = lseek(p->target, 0, SEEK_CUR);
	if(o == -1){
		debug("error reading stats %s\n",
			strerror(errno));
	}
	r = read(p->target, buf1, 512);
	if (-1 == r){
		debug( "error reading stats %s\n",
			strerror(errno));
	}
	if(buf1[0] != '\0'){
		parse(buf1, &val);
		update(p, &val, &diff);
		snprintf(buf2, 512, "%lld", diff);
	}


	//debug("%s\n", buf2);
	write(p->snk, buf2, strlen(buf2+1));
	return 0;
}
void psi_destroy(struct psi *p){
	close(p->target);
}
