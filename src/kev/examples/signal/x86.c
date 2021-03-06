/* $Id$
 * x86-specific code.
 *
 * Copyright (C) 2001-2004  Mikael Pettersson
 */
#define __USE_GNU /* enable symbolic names for gregset_t[] indices */
#include <sys/ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libperfctr.h"
#include "arch.h"

#ifdef __x86_64__
#ifndef REG_RIP
#define REG_RIP 16
#endif
#define REG_PC REG_RIP
#else /* !__x86_64__ */
#ifndef REG_EIP
#define REG_EIP 14
#endif
#define REG_PC REG_EIP
#endif

unsigned long mcontext_pc(const mcontext_t *mc)
{
    return mc->gregs[REG_PC];
}

void do_setup(const struct perfctr_info *info,
	      struct perfctr_cpu_control *cpu_control)
{
    unsigned int nractrs = 0;
    unsigned int pmc_map0 = 0, pmc_map1 = 1;
    unsigned int evntsel0, evntsel1;

    memset(cpu_control, 0, sizeof *cpu_control);

    switch( info->cpu_type ) {
#if !defined(__x86_64__)
      case PERFCTR_X86_INTEL_P6:
      case PERFCTR_X86_INTEL_PII:
      case PERFCTR_X86_INTEL_PIII:
      case PERFCTR_X86_INTEL_PENTM:
	/* FLOPS, USR, ENable, INT */
	evntsel0 = 0xC1 | (1 << 16) | (1 << 22) | (1 << 20);
	/* BR_TAKEN_RETIRED, USR, INT */
	evntsel1 = 0xC9 | (1 << 16) | (1 << 20);
	break;
      case PERFCTR_X86_AMD_K7:
	/* K7 can't count FLOPS. Count RETIRED_OPS instead. */
	evntsel0 = 0xC1 | (1 << 16) | (1 << 22) | (1 << 20);
	/* RETIRED_TAKEN_BRANCHES, USR, INT */
	evntsel1 = 0xC4 | (1 << 16) | (1 << 22) | (1 << 20);
	break;
      case PERFCTR_X86_INTEL_P4:
      case PERFCTR_X86_INTEL_P4M2:
	nractrs = 1;
	/* PMC(0) produces tagged x87_FP_uop:s (FLAME_CCCR0, FIRM_ESCR0) */
	cpu_control->pmc_map[0] = 0x8 | (1 << 31);
	cpu_control->evntsel[0] = (0x3 << 16) | (1 << 13) | (1 << 12);
	cpu_control->p4.escr[0] = (4 << 25) | (1 << 24) | (1 << 5) | (1 << 4) | (1 << 2);
	/* PMC(1) counts execution_event(X87_FP_retired) (IQ_CCCR0, CRU_ESCR2) */
	pmc_map0 = 0xC | (1 << 31);
	evntsel0 = (1 << 26) | (0x3 << 16) | (5 << 13) | (1 << 12);
	cpu_control->p4.escr[1] = (0xC << 25) | (1 << 9) | (1 << 2);
	/* PMC(2) counts branch_retired(TP,TM) (IQ_CCCR2, CRU_ESCR3) */
	pmc_map1 = 0xE | (1 << 31);
	evntsel1 = (1 << 26) | (0x3 << 16) | (5 << 13) | (1 << 12);
	cpu_control->p4.escr[2] = (6 << 25) | (((1 << 3)|(1 << 2)) << 9) | (1 << 2);
	break;
#endif
      case PERFCTR_X86_AMD_K8:
      case PERFCTR_X86_AMD_K8C:
	/* RETIRED_FPU_INSTRS, Unit Mask "x87 instrs", any CPL, Enable, INT */
	evntsel0 = 0xCB | (0x01 << 8) | (3 << 16) | (1 << 22) | (1 << 20);
	/* RETIRED_TAKEN_BRANCHES, USR, Enable, INT */
	evntsel1 = 0xC4 | (1 << 16) | (1 << 22) | (1 << 20);
	break;
      default:
	printf("%s: unsupported cpu type %u\n", __FUNCTION__, info->cpu_type);
	exit(1);
    }	
    cpu_control->tsc_on = 1;
    cpu_control->nractrs = nractrs;
    cpu_control->nrictrs = 2;
    cpu_control->pmc_map[nractrs+0] = pmc_map0;
    cpu_control->evntsel[nractrs+0] = evntsel0;
    cpu_control->ireset[nractrs+0] = -25;
    cpu_control->pmc_map[nractrs+1] = pmc_map1;
    cpu_control->evntsel[nractrs+1] = evntsel1;
    cpu_control->ireset[nractrs+1] = -25;
}
