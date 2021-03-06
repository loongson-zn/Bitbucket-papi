/*
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
#ifndef __PFMLIB_PRIV_H__
#define __PFMLIB_PRIV_H__

#include <perfmon/pfmlib.h>

#include "pfmlib_priv_comp.h"

typedef struct {
	char 		*pmu_name;
	int		pmu_type;
	unsigned int	pme_count; /* number of events */
	int		(*get_event_code)(unsigned int i);
	unsigned long	(*get_event_vcode)(unsigned int i);
	char		*(*get_event_name)(unsigned int i);
	void		(*get_event_counters)(unsigned int i, pfmlib_regmask_t *counters);
	int		(*print_info)(unsigned int v, int (*pf)(const char *fmt,...));
	int 		(*dispatch_events)(pfmlib_input_param_t *p, void *model_in, pfmlib_output_param_t *q, void *model_out);
	void		(*get_num_counters)(unsigned int *num);
	void		(*get_num_pmcs)(unsigned int *num);
	void		(*get_num_pmds)(unsigned int *num);
	int 		(*pmu_detect)(void);
	void		(*get_impl_pmcs)(pfmlib_regmask_t *impl_pmcs);
	void		(*get_impl_pmds)(pfmlib_regmask_t *impl_pmds);
	void		(*get_impl_counters)(pfmlib_regmask_t *impl_counters);
	void		(*get_hw_counter_width)(unsigned int *width);
} pfm_pmu_support_t;

typedef struct {
	pfmlib_options_t	options;
	pfm_pmu_support_t	*current;
} pfm_config_t;	

#define PFMLIB_INITIALIZED()	(pfm_config.current != NULL)

extern pfm_config_t pfm_config;

#define PFMLIB_DEBUG()		pfm_config.options.pfm_debug
#define PFMLIB_VERBOSE()	pfm_config.options.pfm_verbose
#define pfm_current		pfm_config.current

extern void __pfm_vbprintf(const char *fmt,...);

#ifdef PFMLIB_DEBUG
#define DPRINT(a) \
	do { \
		if (pfm_config.options.pfm_debug) { \
			printf("%s (%s.%d): ", __FILE__, __FUNCTION__, __LINE__); printf a; } \
	} while (0)
#else
#define DPRINT(a)
#endif

#define ALIGN_DOWN(a,p)	((a) & ~((1UL<<(p))-1))
#define ALIGN_UP(a,p)	((((a) + ((1UL<<(p))-1))) & ~((1UL<<(p))-1))

#endif /* __PFMLIB_PRIV_H__ */
