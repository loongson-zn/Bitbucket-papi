/****************************/
/* THIS IS OPEN SOURCE CODE */
/****************************/

/*
* File:    perfctr-ppc64.h
* Author:  Maynard Johnson
*          maynardj@us.ibm.com
* Mods:    <your name here>
*          <your email address>
*/

#ifndef _PAPI_PERFCTR_PPC64            /* _PAPI_PERFCTR_PPC64 */
#define _PAPI_PERFCTR_PPC64

#ifdef _POWER5
#define MAX_COUNTERS 6
#define NUM_COUNTER_MASKS 4	
#define GROUP_INTS 5
/* masks for PMC1-4 should be AND'ed into MMCR1 */
#define PMC1_SEL_MASK	0xFFFFFFFF00FFFFFFULL
#define PMC2_SEL_MASK	0xFFFFFFFFFF00FFFFULL
#define PMC3_SEL_MASK	0xFFFFFFFFFFFF00FFULL
#define PMC4_SEL_MASK	0xFFFFFFFFFFFFFF00ULL
/* PMC5_6_FREEZE should be OR'ed into MMCR0 */
#define PMC5_PMC6_FREEZE	0x00000010UL
#else
#define GROUP_INTS 2
#define MAX_COUNTERS 8
#define NUM_COUNTER_MASKS	MAX_COUNTERS+1
#ifdef _POWER4
#define PMC1_SEL_MASK	0xFFFFE0FFUL
#define PMC2_SEL_MASK	0xFFFFFFC1UL
#define PMC3_SEL_MASK	0xFFFFFFFF07FFFFFFULL
#define PMC4_SEL_MASK	0xFFFFFFFFF83FFFFFULL
#define PMC5_SEL_MASK	0xFFFFFFFFFFC1FFFFULL
#define PMC6_SEL_MASK	0xFFFFFFFFFFFE0FFFULL
#define PMC7_SEL_MASK	0xFFFFFFFFFFFFF07FULL
#define PMC8_SEL_MASK	0xFFFFFFFFFFFFFF83ULL
#define PMC8a_SEL_MASK	0xFFFDFFFFUL
#else
/* assume ppc970 for now */
#define PMC1_SEL_MASK	0xFFFFF0FFUL
#define PMC2_SEL_MASK	0xFFFFFFE1UL
#define PMC3_SEL_MASK	0xFFFFFFFF87FFFFFFULL
#define PMC4_SEL_MASK	0xFFFFFFFFFC3FFFFFULL
#define PMC5_SEL_MASK	0xFFFFFFFFFFE1FFFFULL
#define PMC6_SEL_MASK	0xFFFFFFFFFFFF0FFFULL
#define PMC7_SEL_MASK	0xFFFFFFFFFFFFF87FULL
#define PMC8_SEL_MASK	0xFFFFFFFFFFFFFFC3ULL
#define PMC8a_SEL_MASK	0xFFFDFFFFUL
#endif
#endif



#include "papi.h"
#include ARCH_EVTS
#include "linux-ppc64.h"
#include "papi_preset.h"
#include "libperfctr.h"
#include "ppc64.h"

#define HW_OVERFLOW 1
//#define PAPI_MAX_STR_LEN 129


#ifdef __GNUC__
#define HAVE_FFSLL
#define _GNU_SOURCE
#define __USE_GNU
#endif

// control bits MMCR0
#define PERF_INT_ENABLE			0x0000C000 // enables interrupts on PMC1 as well as PMC2-PMCj (2<=j<=MAX_COUNTERS)
#define PMC_OVFL				0x80000000
#define PERF_OS_ONLY			0x20000000
#define PERF_USR_ONLY			0x40000000
#define PERF_USR_AND_OS			0x00000000
#define PERF_CONTROL_MASK		0xFFFFE000

#define AI_ERROR "No support for a-mode counters after adding an i-mode counter"
#define VOPEN_ERROR "vperfctr_open() returned NULL"
#define GOPEN_ERROR "gperfctr_open() returned NULL"
#define VINFO_ERROR "vperfctr_info() returned < 0"
#define VCNTRL_ERROR "vperfctr_control() returned < 0"
#define GCNTRL_ERROR "gperfctr_control() returned < 0"
#define FOPEN_ERROR "fopen(%s) returned NULL"
#define STATE_MAL_ERROR "Error allocating perfctr structures"
#define MODEL_ERROR "This is not a PowerPC"
#define EVENT_INFO_FILE_ERROR "Event info file error"

#define MUTEX_LOCKED 1
#define MUTEX_OPEN 0

extern unsigned int lock[];

#include <unistd.h>

// similar to __arch_compare_and_exchange_val_32_acq() from libc's atomic.h
static inline unsigned long _papi_hwd_trylock(unsigned int *lock)
{
	unsigned long tmp, tmp2;
	__asm__ volatile("              li              %1,%3\n"
                     "1:            lwarx           %0,0,%2\n"
                     "              cmpwi           0,%0,%4\n"
                     "              bne-            2f\n"
                     "              stwcx.          %1,0,%2\n"
                     "              bne-            1b\n"
                     "              isync\n"
                     "2:"
                     : "=&r"(tmp), "=&r"(tmp2)
                     : "b"(lock), "i"(MUTEX_LOCKED), "i"(MUTEX_OPEN)
                     : "cr0", "memory");
    return tmp;
}

#define _papi_hwd_lock(locknum)          \
	do { } while (_papi_hwd_trylock((unsigned int *)(&(lock[(locknum)]))) != MUTEX_OPEN)

#define _papi_hwd_unlock(locknum) \
	do { \
		__asm__ volatile("lwsync": : :"memory"); \
		lock[(locknum)] = MUTEX_OPEN; \
	} while(0)


// prototypes
int setup_ppc64_native_table(void);

typedef struct hwd_native {
   /* index in the native table, required */
   int index;
   /* Which counters can be used?  */
   unsigned int selector;
   /* Rank determines how many counters carry each metric */
   unsigned char rank;
   /* which counter this native event stays */
   int position;
   int mod;
   int link;
} hwd_native_t;

typedef struct ppc64_reg_alloc {
   int ra_position;
   unsigned int ra_group[GROUP_INTS];
   int ra_counter_cmd[MAX_COUNTERS];
} ppc64_reg_alloc_t;


/* typedefs to conform to hardware independent PAPI code. */
typedef ppc64_reg_alloc_t hwd_reg_alloc_t;

typedef struct ppc64_perfctr_control {
	
// the members below are from power4.h	
   /* Buffer to pass to the kernel to control the counters */
   int group_id;
   /* Interrupt interval */
   int timer_ms;

	
// the members below are from perfctr-p3.h	
   hwd_native_t native[MAX_COUNTERS];
   int native_idx;
   unsigned char master_selector;
   hwd_register_t allocated_registers;
   struct vperfctr_control control;
   struct perfctr_sum_ctrs state;
} ppc64_perfctr_control_t;


typedef struct ppc64_perfctr_context {
   struct vperfctr *perfctr;
} ppc64_perfctr_context_t;

/* typedefs to conform to hardware independent PAPI code. */
typedef ppc64_perfctr_control_t hwd_control_state_t ;
typedef ppc64_perfctr_context_t hwd_context_t;
#define hwd_pmc_control vperfctr_control

typedef struct ntv_event {
	char symbol[PAPI_MAX_STR_LEN];
	unsigned int event_num;
	char * short_description;
	char * description;
} ntv_event_t;

typedef struct ntv_event_info {
	int maxevents[MAX_COUNTERS];
	int maxpmcs;
	ntv_event_t * wev[MAX_COUNTERS];
} ntv_event_info_t;


typedef struct event_group {
	int group_id;
	unsigned int mmcr0;
	unsigned int mmcr1L;
	unsigned int mmcr1U;
	unsigned int mmcra;
	unsigned int events[MAX_COUNTERS];
} event_group_t;

typedef struct ntv_event_group_info {
	int maxgroups;
	event_group_t * event_groups[MAX_GROUPS];
} ntv_event_group_info_t;


// prototypes
ntv_event_info_t * perfctr_get_native_evt_info(void);
ntv_event_group_info_t * perfctr_get_native_group_info(void);
int _papi_hwd_get_memory_info(PAPI_hw_info_t * mem_info, int cpu_type);

#endif                          /* _PAPI_PERFCTR_PPC64 */
