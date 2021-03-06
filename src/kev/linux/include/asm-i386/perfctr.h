/* $Id$
 * x86 Performance-Monitoring Counters driver
 *
 * Copyright (C) 1999-2004  Mikael Pettersson
 */
#ifndef _ASM_I386_PERFCTR_H
#define _ASM_I386_PERFCTR_H

struct perfctr_sum_ctrs {
	unsigned long long tsc;
	unsigned long long pmc[18];
};

struct perfctr_cpu_control {
	unsigned int tsc_on;
	unsigned int nractrs;		/* # of a-mode counters */
	unsigned int nrictrs;		/* # of i-mode counters */
	unsigned int pmc_map[18];
	unsigned int evntsel[18];	/* one per counter, even on P5 */
	struct {
		unsigned int escr[18];
		unsigned int pebs_enable;	/* for replay tagging */
		unsigned int pebs_matrix_vert;	/* for replay tagging */
	} p4;
	int ireset[18];			/* < 0, for i-mode counters */
	unsigned int _reserved1;
	unsigned int _reserved2;
	unsigned int _reserved3;
	unsigned int _reserved4;
};

struct perfctr_cpu_state {
	unsigned int cstatus;
	struct {	/* k1 is opaque in the user ABI */
		unsigned int id;
		const void *isuspend_cpu;
	} k1;
	/* The two tsc fields must be inlined. Placing them in a
	   sub-struct causes unwanted internal padding on x86-64. */
	unsigned int tsc_start;
	unsigned long long tsc_sum;
	struct {
		unsigned int map;
		unsigned int start;
		unsigned long long sum;
	} pmc[18];	/* the size is not part of the user ABI */
#ifdef __KERNEL__
	struct perfctr_cpu_control control;
	unsigned int p4_escr_map[18];
#endif
};

/* cstatus is a re-encoding of control.tsc_on/nractrs/nrictrs
   which should have less overhead in most cases */

static inline
unsigned int perfctr_mk_cstatus(unsigned int tsc_on, unsigned int nractrs,
				unsigned int nrictrs)
{
	return (tsc_on<<31) | (nrictrs<<16) | ((nractrs+nrictrs)<<8) | nractrs;
}

static inline unsigned int perfctr_cstatus_enabled(unsigned int cstatus)
{
	return cstatus;
}

static inline int perfctr_cstatus_has_tsc(unsigned int cstatus)
{
	return (int)cstatus < 0;	/* test and jump on sign */
}

static inline unsigned int perfctr_cstatus_nractrs(unsigned int cstatus)
{
	return cstatus & 0x7F;		/* and with imm8 */
}

static inline unsigned int perfctr_cstatus_nrctrs(unsigned int cstatus)
{
	return (cstatus >> 8) & 0x7F;
}

static inline unsigned int perfctr_cstatus_has_ictrs(unsigned int cstatus)
{
	return cstatus & (0x7F << 16);
}

/*
 * 'struct siginfo' support for perfctr overflow signals.
 * In unbuffered mode, si_code is set to SI_PMC_OVF and a bitmask
 * describing which perfctrs overflowed is put in si_pmc_ovf_mask.
 * A bitmask is used since more than one perfctr can have overflowed
 * by the time the interrupt handler runs.
 *
 * glibc's <signal.h> doesn't seem to define __SI_FAULT or __SI_CODE(),
 * and including <asm/siginfo.h> as well may cause redefinition errors,
 * so the user and kernel values are different #defines here.
 */
#ifdef __KERNEL__
#define SI_PMC_OVF	(__SI_FAULT|'P')
#else
#define SI_PMC_OVF	('P')
#endif
#define si_pmc_ovf_mask	_sifields._pad[0] /* XXX: use an unsigned field later */

/* version number for user-visible CPU-specific data */
#define PERFCTR_CPU_VERSION	0x0500	/* 5.0 */

#ifdef __KERNEL__

#if defined(CONFIG_PERFCTR) || defined(CONFIG_PERFCTR_MODULE)

/* Driver init/exit. */
extern int perfctr_cpu_init(void);
extern void perfctr_cpu_exit(void);

/* CPU type name. */
extern char *perfctr_cpu_name;

/* Hardware reservation. */
extern const char *perfctr_cpu_reserve(const char *service);
extern void perfctr_cpu_release(const char *service);

/* PRE: state has no running interrupt-mode counters.
   Check that the new control data is valid.
   Update the driver's private control data.
   is_global should be zero for per-process counters and non-zero
   for global-mode counters. This matters for HT P4s, alas.
   Returns a negative error code if the control data is invalid. */
extern int perfctr_cpu_update_control(struct perfctr_cpu_state *state, int is_global);

/* Read a-mode counters. Subtract from start and accumulate into sums.
   Must be called with preemption disabled. */
extern void perfctr_cpu_suspend(struct perfctr_cpu_state *state);

/* Write control registers. Read a-mode counters into start.
   Must be called with preemption disabled. */
extern void perfctr_cpu_resume(struct perfctr_cpu_state *state);

/* Perform an efficient combined suspend/resume operation.
   Must be called with preemption disabled. */
extern void perfctr_cpu_sample(struct perfctr_cpu_state *state);

/* The type of a perfctr overflow interrupt handler.
   It will be called in IRQ context, with preemption disabled. */
typedef void (*perfctr_ihandler_t)(unsigned long pc);

#if defined(CONFIG_X86_LOCAL_APIC)
#define PERFCTR_INTERRUPT_SUPPORT 1
#endif

#if PERFCTR_INTERRUPT_SUPPORT
extern void perfctr_cpu_set_ihandler(perfctr_ihandler_t);
extern void perfctr_cpu_ireload(struct perfctr_cpu_state*);
extern unsigned int perfctr_cpu_identify_overflow(struct perfctr_cpu_state*);
#else
static inline void perfctr_cpu_set_ihandler(perfctr_ihandler_t x) { }
#endif

#if defined(CONFIG_SMP)
/* CPUs in `perfctr_cpus_forbidden_mask' must not use the
   performance-monitoring counters. TSC use is unrestricted.
   This is needed to prevent resource conflicts on hyper-threaded P4s.
   The declaration of `perfctr_cpus_forbidden_mask' is in the driver's
   private compat.h, since it needs to handle cpumask_t incompatibilities. */
#define PERFCTR_CPUS_FORBIDDEN_MASK_NEEDED 1
#endif

#endif	/* CONFIG_PERFCTR */

#if defined(CONFIG_KPERFCTR) && PERFCTR_INTERRUPT_SUPPORT
asmlinkage void perfctr_interrupt(struct pt_regs*);
#define perfctr_vector_init()	\
	set_intr_gate(LOCAL_PERFCTR_VECTOR, perfctr_interrupt)
#else
#define perfctr_vector_init()	do{}while(0)
#endif

#endif	/* __KERNEL__ */

#endif	/* _ASM_I386_PERFCTR_H */
