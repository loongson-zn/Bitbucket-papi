/* $Id$
 * PPC32-specific code.
 *
 * Copyright (C) 2004  Mikael Pettersson
 */
#include <sys/ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libperfctr.h"
#include "arch.h"

unsigned long mcontext_pc(const mcontext_t *mc)
{
    return mc->regs->nip;
}

void do_setup(const struct perfctr_info *info,
	      struct perfctr_cpu_control *cpu_control)
{
    memset(cpu_control, 0, sizeof *cpu_control);

    cpu_control->tsc_on = 1;
    cpu_control->nractrs = 0;
    cpu_control->nrictrs = 1;
    cpu_control->pmc_map[0] = 0;

    /* INSTRUCTIONS_COMPLETED */
    cpu_control->evntsel[0] = 0x02;

    /* overflow after 100 events */
    cpu_control->ireset[0] = 0x80000000-100;

    /* not kernel mode, enable interrupts, enable PMC1 interrupts */
    cpu_control->ppc.mmcr0 = (1<<(31-1)) | (1<<(31-5)) | (1<<(31-16));
}
