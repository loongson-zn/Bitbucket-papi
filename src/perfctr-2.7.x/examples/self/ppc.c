/* $Id$
 * PPC32-specific code.
 *
 * Copyright (C) 2004  Mikael Pettersson
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libperfctr.h"
#include "arch.h"

void do_setup(const struct perfctr_info *info,
	      struct perfctr_cpu_control *cpu_control)
{
    memset(cpu_control, 0, sizeof *cpu_control);
    cpu_control->tsc_on = 1;
    if (info->cpu_type > PERFCTR_PPC_GENERIC) {
	cpu_control->nractrs = 1;
	cpu_control->pmc_map[0] = 0;
	cpu_control->evntsel[0] = 0x02; /* INSTRUCTIONS_COMPLETED */
	cpu_control->ppc.mmcr0 = (1 << (31-1)); /* don't count in kernel mode */
    }
}
