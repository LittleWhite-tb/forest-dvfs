/*
 * syst.c - example of a simple system wide monitoring program
 *
 * Copyright (c) 2002-2006 Hewlett-Packard Development Company, L.P.
 * Contributed by Stephane Eranian <eranian@hpl.hp.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>

#include <perfmon/pfmlib.h>
#include <perfmon/pfmlib_perf_event.h>

#include "perf_util.h"

#define CPU 0
#define EVENTS "UNHALTED_CORE_CYCLES,INSTRUCTION_RETIRED,LLC_REFERENCES,LLC_MISSES"

static perf_event_desc_t **all_fds;
static int *num_fds;

void
setup_cpu(int cpu)
{
	perf_event_desc_t *fds;
	int i, ret;

	ret = perf_setup_list_events(EVENTS, &all_fds[cpu], &num_fds[cpu]);
	if (ret || (num_fds == 0))
		errx(1, "cannot setup events\n");
	fds = all_fds[cpu]; /* temp */

	fds[0].fd = -1;
	for(i=0; i < num_fds[cpu]; i++) {
		/* request timing information necessary for scaling counts */
		fds[i].hw.read_format = PERF_FORMAT_SCALE;
		fds[i].fd = perf_event_open(&fds[i].hw, -1, cpu, -1, 0);
		if (fds[i].fd == -1)
			err(1, "cannot attach event to CPU%d %s", cpu, fds[i].name);
	}
}

void
measure(void)
{
	perf_event_desc_t *fds;
	int cmin, cmax, ncpus;
	int i, ret;


	/* number of available CPUs */
	cmin = 0;
	cmax = (int) sysconf(_SC_NPROCESSORS_ONLN);
	ncpus = cmax;

	all_fds = calloc(ncpus, sizeof(perf_event_desc_t));
	num_fds = calloc(ncpus, sizeof(int));
	if (!all_fds || !num_fds)
		err(1, "cannot allocate memory for internal structures");

	setup_cpu(CPU);

	/*
	 * FIX this for hotplug CPU
	 */
	fds = all_fds[CPU];
	for(i=0; i < num_fds[CPU]; i++) {
		ret = ioctl(fds[i].fd, PERF_EVENT_IOC_ENABLE, 0);
		if (ret)
			err(1, "cannot enable event %s\n", fds[i].name);
	}
	
	uint64_t values[3];

	fds = all_fds[CPU];
	for(i=0; i < num_fds[CPU]; i++) {
		ret = read(fds[i].fd, values, sizeof(values));
		if (ret != sizeof(values)) {
			if (ret == -1)
				fprintf(stderr, "ERR: cannot read event %d:%d", i, ret);
			else
				fprintf(stderr, "WARN: could not read event%d", i);
		}

		/*
		 * scaling because we may be sharing the PMU and
		 * thus may be multiplexed
		 */
		fds[i].value = perf_scale(values);

		printf("CPU%d val=%-20lu %s\n",
				CPU,
				fds[i].value,
				fds[i].name);
	}
	
	fds = all_fds[CPU];
	for(i=0; i < num_fds[CPU]; i++)
		close(fds[i].fd);
	free(all_fds);
}

int
main(int argc, char **argv)
{
	int ret;

	ret = pfm_initialize();
	if (ret != PFM_SUCCESS)
		fprintf(stderr, "libpfm initialization failed: %s\n", pfm_strerror(ret));
	
	measure();

	/* free libpfm resources cleanly */
	pfm_terminate();

	return 0;
}
