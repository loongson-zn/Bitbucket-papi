/* $Id$
 * x86-specific code.
 *
 * Copyright (C) 2000-2004  Mikael Pettersson
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libperfctr.h"
#include "arch.h"

void setup_control(const struct perfctr_info *info,
		   struct perfctr_cpu_control *control)
{
    unsigned int tsc_on = 1;
    unsigned int nractrs = 1;
    unsigned int pmc_map0 = 0;
    unsigned int evntsel0 = 0;

    memset(control, 0, sizeof *control);

    /* Attempt to set up control to count clocks via the TSC
       and FLOPS via PMC0. */
    switch( info->cpu_type ) {
      case PERFCTR_X86_GENERIC:
	nractrs = 0;		/* no PMCs available */
	break;
      case PERFCTR_X86_AMD_K8:
      case PERFCTR_X86_AMD_K8C:
	/* RETIRED_FPU_INSTRS, Unit Mask "x87 instrs", any CPL, Enable */
	evntsel0 = 0xCB | (0x01 << 8) | (3 << 16) | (1 << 22);
	break;
#if !defined(__x86_64__)
      case PERFCTR_X86_INTEL_P5:
      case PERFCTR_X86_INTEL_P5MMX:
      case PERFCTR_X86_CYRIX_MII:
	/* event 0x22 (FLOPS), any CPL */
	evntsel0 = 0x22 | (3 << 6);
	break;
      case PERFCTR_X86_INTEL_P6:
      case PERFCTR_X86_INTEL_PII:
      case PERFCTR_X86_INTEL_PIII:
      case PERFCTR_X86_INTEL_PENTM:
	/* note: FLOPS is only available in PERFCTR0 */
	/* event 0xC1 (FLOPS), any CPL, Enable */
	evntsel0 = 0xC1 | (3 << 16) | (1 << 22);
	break;
      case PERFCTR_X86_AMD_K7:
	/* K7 apparently can't count FLOPS. */
	counting_mips = 1;
	/* event 0xC0 (RETIRED_INSTRUCTIONS), any CPL, Enable */
	evntsel0 = 0xC0 | (3 << 16) | (1 << 22);
	break;
      case PERFCTR_X86_WINCHIP_C6:
	counting_mips = 1;	/* can't count FLOPS */
	tsc_on = 0;		/* no working TSC available */
	evntsel0 = 0x02;	/* X86_INSTRUCTIONS */
	break;
      case PERFCTR_X86_WINCHIP_2:
	counting_mips = 1;	/* can't count FLOPS */
	tsc_on = 0;		/* no working TSC available */
	evntsel0 = 0x16;	/* INSTRUCTIONS_EXECUTED */
	break;
      case PERFCTR_X86_VIA_C3:
	counting_mips = 1;	/* can't count FLOPS */
	pmc_map0 = 1;		/* redirect PMC0 to PERFCTR1 */
	evntsel0 = 0xC0;	/* INSTRUCTIONS_EXECUTED */
	break;
      case PERFCTR_X86_INTEL_P4:
      case PERFCTR_X86_INTEL_P4M2:
#endif
      case PERFCTR_X86_INTEL_P4M3:
	nractrs = 2;
	/* set up PMC(1) to produce tagged x87_FP_uop:s */
	control->pmc_map[1] = 0x8 | (1 << 31);
	control->evntsel[1] = (0x3 << 16) | (1 << 13) | (1 << 12);
	control->p4.escr[1] = (4 << 25) | (1 << 24) | (1 << 5) | (1 << 4) | (1 << 2);
	/* set up PMC(0) to count execution_event(X87_FP_retired) */
	pmc_map0 = 0xC | (1 << 31);
	evntsel0 = (0x3 << 16) | (5 << 13) | (1 << 12);
	control->p4.escr[0] = (0xC << 25) | (1 << 9) | (1 << 2);
	break;
      default:
	fprintf(stderr, "cpu_type %u (%s) not supported\n",
		info->cpu_type, perfctr_info_cpu_name(info));
	exit(1);
    }
    control->tsc_on = tsc_on;
    control->nractrs = nractrs;
    control->pmc_map[0] = pmc_map0;
    control->evntsel[0] = evntsel0;
}
