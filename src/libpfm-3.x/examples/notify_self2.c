/*
 * notify_self2.c - example of how you can use overflow notifications with F_SETSIG
 *
 * Copyright (C) 2002-2003 Hewlett-Packard Co
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
 *
 * This file is part of libpfm, a performance monitoring support library for
 * applications on Linux/ia64.
 */
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <perfmon/perfmon.h>
#include <perfmon/pfmlib.h>


#define SMPL_PERIOD	1000000000UL

/*
 * This array must contain at least 2 events
 */
static char *event_list[]={
	"cpu_cycles",
	"IA64_INST_RETIRED",
};
#define N_EVENTS	sizeof(event_list)/sizeof(const char *)

static volatile unsigned long notification_received;

#define NUM_PMCS PFMLIB_MAX_PMCS
#define NUM_PMDS PFMLIB_MAX_PMDS

static pfarg_reg_t pd[NUM_PMDS];
static int ctx_fd;

static void fatal_error(char *fmt,...) __attribute__((noreturn));

static void
fatal_error(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(1);
}


static void
sigio_handler(int n, struct siginfo *info, struct sigcontext *sc)
{
	pfm_msg_t msg;
	int fd = info->si_fd;
	int r;

	if (fd != ctx_fd) {
		fatal_error("handler does not get valid file descriptor\n");
	}

	if (perfmonctl(fd, PFM_READ_PMDS, pd+1, 1) == -1) {
		fatal_error("PFM_READ_PMDS: %s", strerror(errno));
	}

	r = read(fd, &msg, sizeof(msg));
	if (r != sizeof(msg)) {
		fatal_error("cannot read overflow message: %s\n", strerror(errno));
	}

	if (msg.pfm_gen_msg.msg_type != PFM_MSG_OVFL) {
		fatal_error("unexpected msg type: %d\n",msg.pfm_gen_msg.msg_type);
	}

	/*
	 * XXX: risky to do printf() in signal handler!
	 */
	printf("Notification %lu: %"PRIu64" %s\n",
		notification_received,
		pd[1].reg_value,
		event_list[1]);

	/*
	 * At this point, the counter used for the sampling period has already
	 * be reset by the kernel because we are in non-blocking mode, self-monitoring.
	 */

	/*
	 * increment our notification counter
	 */
	notification_received++;

	/*
	 * And resume monitoring
	 */
	if (perfmonctl(fd, PFM_RESTART,NULL, 0) == -1) {
		fatal_error("PFM_RESTART: %s", strerror(errno));
	}
}

/*
 * infinite loop waiting for notification to get out
 */
void
busyloop(void)
{
	/*
	 * busy loop to burn CPU cycles
	 */
	for(;notification_received < 3;) ;
}


int
main(int argc, char **argv)
{
	char **p;
	pfarg_context_t ctx[1];
	pfmlib_input_param_t inp;
	pfmlib_output_param_t outp;
	pfarg_reg_t pc[NUM_PMCS];
	pfarg_load_t load_args;
	pfmlib_options_t pfmlib_options;
	struct sigaction act;
	unsigned int i;
	int ret;

	/*
	 * Initialize pfm library (required before we can use it)
	 */
	if (pfm_initialize() != PFMLIB_SUCCESS) {
		printf("Can't initialize library\n");
		exit(1);
	}

	/*
	 * Install the signal handler (SIGIO)
	 */
	memset(&act, 0, sizeof(act));
	act.sa_handler = (sig_t)sigio_handler;
	sigaction (SIGIO, &act, 0);

	/*
	 * pass options to library (optional)
	 */
	memset(&pfmlib_options, 0, sizeof(pfmlib_options));
	pfmlib_options.pfm_debug = 0; /* set to 1 for debug */
	pfm_set_options(&pfmlib_options);

	memset(pc, 0, sizeof(pc));
	memset(ctx, 0, sizeof(ctx));
	memset(&load_args, 0, sizeof(load_args));
	memset(&inp,0, sizeof(inp));
	memset(&outp,0, sizeof(outp));

	p = event_list;
	for (i=0; i < N_EVENTS ; i++, p++) {
		if (pfm_find_event(*p, &inp.pfp_events[i].event) != PFMLIB_SUCCESS) {
			fatal_error("Cannot find %s event\n", *p);
		}
	}

	/*
	 * set the default privilege mode for all counters:
	 * 	PFM_PLM3 : user level only
	 */
	inp.pfp_dfl_plm = PFM_PLM3;

	/*
	 * how many counters we use
	 */
	inp.pfp_event_count = i;

	/*
	 * let the library figure out the values for the PMCS
	 */
	if ((ret=pfm_dispatch_events(&inp, NULL, &outp, NULL)) != PFMLIB_SUCCESS) {
		fatal_error("Cannot configure events: %s\n", pfm_strerror(ret));
	}

	/*
	 * now create the context for self monitoring/per-task
	 */
	if (perfmonctl(0, PFM_CREATE_CONTEXT, ctx, 1) == -1 ) {
		if (errno == ENOSYS) {
			fatal_error("Your kernel does not have performance monitoring support!\n");
		}
		fatal_error("Can't create PFM context %s\n", strerror(errno));
	}
	ctx_fd = ctx->ctx_fd;

	/*
	 * Now prepare the argument to initialize the PMDs and PMCS.
	 * We use pfp_pmc_count to determine the number of registers to
	 * setup. Note that this field can be >= pfp_event_count.
	 */

	for (i=0; i < outp.pfp_pmc_count; i++) {
		pc[i].reg_num   = outp.pfp_pmcs[i].reg_num;
		pc[i].reg_value = outp.pfp_pmcs[i].reg_value;
	}

	for (i=0; i < inp.pfp_event_count; i++) {
		pd[i].reg_num   = pc[i].reg_num;
	}
	/*
	 * We want to get notified when the counter used for our first
	 * event overflows
	 */
	pc[0].reg_flags 	|= PFM_REGFL_OVFL_NOTIFY;
	pc[0].reg_reset_pmds[0] |= 1UL << outp.pfp_pmcs[1].reg_num;

	/*
	 * we arm the first counter, such that it will overflow
	 * after SMPL_PERIOD events have been observed
	 */
	pd[0].reg_value       = (~0UL) - SMPL_PERIOD + 1;
	pd[0].reg_long_reset  = (~0UL) - SMPL_PERIOD + 1;
	pd[0].reg_short_reset = (~0UL) - SMPL_PERIOD + 1;

	/*
	 * Now program the registers
	 *
	 * We don't use the save variable to indicate the number of elements passed to
	 * the kernel because, as we said earlier, pc may contain more elements than
	 * the number of events we specified, i.e., contains more than counting monitors.
	 */
	if (perfmonctl(ctx_fd, PFM_WRITE_PMCS, pc, outp.pfp_pmc_count) == -1) {
		fatal_error("perfmonctl error PFM_WRITE_PMCS errno %d\n",errno);
	}

	if (perfmonctl(ctx_fd, PFM_WRITE_PMDS, pd, inp.pfp_event_count) == -1) {
		fatal_error("perfmonctl error PFM_WRITE_PMDS errno %d\n",errno);
	}

	/*
	 * we want to monitor ourself
	 */
	load_args.load_pid = getpid();

	if (perfmonctl(ctx_fd, PFM_LOAD_CONTEXT, &load_args, 1) == -1) {
		fatal_error("perfmonctl error PFM_WRITE_PMDS errno %d\n",errno);
	}

	/*
	 * setup asynchronous notification on the file descriptor
	 */
	ret = fcntl(ctx_fd, F_SETFL, fcntl(ctx_fd, F_GETFL, 0) | O_ASYNC);
	if (ret == -1) {
		fatal_error("cannot set ASYNC: %s\n", strerror(errno));
	}

	/*
	 * get ownership of the descriptor
	 */
	ret = fcntl(ctx_fd, F_SETOWN, getpid());
	if (ret == -1) {
		fatal_error("cannot setown: %s\n", strerror(errno));
	}
#ifndef _GNU_SOURCE
#error "this program must be compiled with -D_GNU_SOURCE"
#else
	/*
	 * when you explicitely declare that you want a particular signal,
	 * even with you use the default signal, the kernel will send more
	 * information concerning the event to the signal handler.
	 *
	 * In particular, it will send the file descriptor from which the
	 * event is originating which can be quite useful when monitoring
	 * multiple tasks from a single thread.
	 */
	ret = fcntl(ctx_fd, F_SETSIG, SIGIO);
	if (ret == -1) {
		fatal_error("cannot setsig: %s\n", strerror(errno));
	}
#endif
	/*
	 * Let's roll now
	 */
	pfm_self_start(ctx_fd);

	busyloop();

	pfm_self_stop(ctx_fd);

	/*
	 * free our context
	 */
	close(ctx_fd);

	return 0;
}
