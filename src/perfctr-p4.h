#ifndef _PAPI_PENTIUM4_H
#define _PAPI_PENTIUM4_H

#define HAVE_FFSLL
#define _GNU_SOURCE
#define __USE_GNU
#define __USE_UNIX98

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <malloc.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ucontext.h>
#include <linux/unistd.h>

#include "p4_events.h"

#ifdef _WIN32
#include "cpuinfo.h"
#include "pmclib.h"
#else
#include "libperfctr.h"
#define inline_static inline static
#endif

/* Per event data structure for each event */

typedef struct P4_perfctr_event {
   unsigned pmc_map;
   unsigned evntsel;
   unsigned evntsel_aux;
   unsigned pebs_enable;
   unsigned pebs_matrix_vert;
   unsigned ireset;
} P4_perfctr_event_t;


#define MAX_COUNTERS	       18
#define MAX_COUNTER_TERMS	8

/*
The name and description fields should be self-explanatory.
The counter and escr fields are what must be used for register allocation.
There are 18 possible counters. There are 45 possible escrs.
Each native event requires and consumes one counter and one escr.
Multiple counters (2 or 3) are valid with each escr, 
and typically 2 escrs are valid with each event.
Thus, you must track and optimize two different selectors: one for the 18 counters,
and a second for the 45 escrs. 
The bits set in counter[0] correspond to the counters available for use with escr[0].
The value of escr[0] is the bit position in a long_long selector determining which escrs
have been allocated.
Example:
Native event PNE_branch_retired_all can live on 6 counters
counter[0] = MSR_IQ_COUNTER014 = COUNTER(12) | COUNTER(13) | COUNTER(16)
counter[1] = MSR_IQ_COUNTER235 = COUNTER(14) | COUNTER(15) | COUNTER(17)
This event also MUST live on one of two escrs:
escr[0] = MSR_CRU_ESCR2 = 41
escr[1] = MSR_CRU_ESCR3 = 42
If one of counter[0] is chosen, then escr[0] must be chosen.
Likewise for counter[1] and escr[1].
So if this event is assigned to counter 12, bit 12 of counter_selector must be set, 
and bit 41 of escr_selector must be set. These resources are then not available for
any other native event.
*/

typedef struct P4_perfctr_codes {
   P4_perfctr_event_t data[MAX_COUNTER_TERMS];
} P4_perfctr_preset_t;

typedef struct P4_register {
   unsigned counter[2];         // bitmap of valid counters for each escr
   unsigned escr[2];            // bit offset for each of 2 valid escrs
   unsigned cccr;               // value to be loaded into cccr register
   unsigned event;              // value defining event to be loaded into escr register
   unsigned pebs_enable;        // flag for PEBS counting
   unsigned pebs_matrix_vert;   // flag for PEBS_MATRIX_VERT, whatever that is 
   unsigned ireset;             // I don't really know what this does
} P4_register_t;

/* defines the fields needed by _papi_hwd_allocate_registers
   to map the counter set */
typedef struct P4_reg_alloc {
   P4_register_t ra_bits;       /* Info about this native event mapping */
   unsigned ra_selector;        /* Bit mask showing which counters can carry this metric */
   unsigned ra_rank;            /* How many counters can carry this metric */
   unsigned ra_escr[2];         /* Bit field array showing which (of 45) esc registers can carry this metric */
} P4_reg_alloc_t;

typedef struct hwd_p4_native_map {
   char *name;                  // ASCII name of the native event
   char *description;           // ASCII description of the native event
   P4_register_t bits;          // description of resources needed by this event
   int mask;                    // contains all valid mask bits for this event group
   int synonym;                 // index of next synonym if event can be multiply encoded 
} hwd_p4_native_map_t;

typedef struct hwd_p4_mask {
   int bit_pos;                 // bit position of mask bit
   char *name;                  // ASCII name of the native event
   char *description;           // ASCII description of the native event
} hwd_p4_mask_t;

typedef struct P4_perfctr_control {
   struct vperfctr_control control;
   struct perfctr_sum_ctrs state;
} P4_perfctr_control_t;

/* Per thread data structure for thread level counters */

typedef struct P4_perfctr_context {
   struct vperfctr *perfctr;
/*  P4_perfctr_control_t start; */
} P4_perfctr_context_t;

typedef P4_reg_alloc_t hwd_reg_alloc_t;

typedef P4_perfctr_control_t hwd_control_state_t;

typedef P4_register_t hwd_register_t;

typedef P4_perfctr_context_t hwd_context_t;

typedef P4_perfctr_event_t hwd_event_t;

#define AI_ERROR "No support for a-mode counters after adding an i-mode counter"
#define VOPEN_ERROR "vperfctr_open() returned NULL"
#define GOPEN_ERROR "gperfctr_open() returned NULL"
#define VINFO_ERROR "vperfctr_info() returned < 0"
#define VCNTRL_ERROR "vperfctr_control() returned < 0"
#define GCNTRL_ERROR "gperfctr_control() returned < 0"
#define FOPEN_ERROR "fopen(%s) returned NULL"
#define STATE_MAL_ERROR "Error allocating perfctr structures"
#define MODEL_ERROR "This is not a Pentium 4"

typedef siginfo_t hwd_siginfo_t;
typedef ucontext_t hwd_ucontext_t;

#ifdef __x86_64__
#define GET_OVERFLOW_ADDRESS(ctx) (caddr_t)(((struct sigcontext *)(&ctx->ucontext->uc_mcontext))->rip)
#else
#define GET_OVERFLOW_ADDRESS(ctx)  (caddr_t)(((struct sigcontext *)(&ctx->ucontext->uc_mcontext))->eip)
#endif

/* Undefined identifiers in executable */

extern caddr_t _start, _init, _etext, _fini, _end, _edata, __bss_start;
extern int _papi_hwd_get_system_info(void);

#endif
