/****************************/
/* THIS IS OPEN SOURCE CODE */
/****************************/

/* 
* File:    papi_internal.h
* CVS:     $Id$
* Author:  Philip Mucci
*          mucci@cs.utk.edu
* Mods:    dan terpstra
*          terpstra.utk.edu
* Mods:    Kevin London
*	   london@cs.utk.edu
*          Haihang You
*          you@cs.utk.edu
* Mods:    <your name here>
*          <your email address>
*/

#ifndef _PAPI_INTERNAL_H
#define _PAPI_INTERNAL_H

#ifdef DEBUG

#ifdef __GNUC__
#define FUNC __FUNCTION__
#elif defined(__func__)
#define FUNC __func__
#else
#define FUNC "?"
#endif


  /* Debug Levels */

#define DEBUG_SUBSTRATE         0x2
#define DEBUG_API               0x4
#define DEBUG_INTERNAL          0x8
#define DEBUG_THREADS           0x10
#define DEBUG_MULTIPLEX         0x20
#define DEBUG_OVERFLOW          0x40
#define DEBUG_PROFILE           0x80
#define DEBUG_ALL               (DEBUG_SUBSTRATE|DEBUG_API|DEBUG_INTERNAL|DEBUG_THREADS|DEBUG_MULTIPLEX|DEBUG_OVERFLOW|DEBUG_PROFILE)

  /* Please get rid of the DBG macro from your code */

extern int _papi_hwi_debug; 
extern unsigned long int (*_papi_hwi_thread_id_fn)(void);

#define DEBUGLABEL(a) if (_papi_hwi_thread_id_fn) fprintf(stderr, "%s:%s:%s:%d:0x%lx ",a,__FILE__, FUNC, __LINE__,_papi_hwi_thread_id_fn()); else fprintf(stderr, "%s:%s:%s:%d: ",a,__FILE__, FUNC, __LINE__)
#define ISLEVEL(a) (_papi_hwi_debug&a)

#define DEBUGLEVEL(a) ((a&DEBUG_SUBSTRATE)?"SUBSTRATE":(a&DEBUG_API)?"API":(a&DEBUG_INTERNAL)?"INTERNAL":(a&DEBUG_THREADS)?"THREADS":(a&DEBUG_MULTIPLEX)?"MULTIPLEX":(a&DEBUG_OVERFLOW)?"OVERFLOW":(a&DEBUG_PROFILE)?"PROFILE":"UNKNOWN")

#ifndef NO_VARARG_MACRO         /* Has variable arg macro support */
#define PAPIDEBUG(level,format, args...) { if(_papi_hwi_debug&level){DEBUGLABEL(DEBUGLEVEL(level));fprintf(stderr,format, ## args);}}

 /* Macros */

#define SUBDBG(format, args...) (PAPIDEBUG(DEBUG_SUBSTRATE,format, ## args))
#define APIDBG(format, args...) (PAPIDEBUG(DEBUG_API,format, ## args))
#define INTDBG(format, args...) (PAPIDEBUG(DEBUG_INTERNAL,format, ## args))
#define THRDBG(format, args...) (PAPIDEBUG(DEBUG_THREADS,format, ## args))
#define MPXDBG(format, args...) (PAPIDEBUG(DEBUG_MULTIPLEX,format, ## args))
#define OVFDBG(format, args...) (PAPIDEBUG(DEBUG_OVERFLOW,format, ## args))
#define PRFDBG(format, args...) (PAPIDEBUG(DEBUG_PROFILE,format, ## args))
#endif

#else
#ifndef NO_VARARG_MACRO         /* Has variable arg macro support */
#define SUBDBG(format, args...) { ; }
#define APIDBG(format, args...) { ; }
#define INTDBG(format, args...) { ; }
#define THRDBG(format, args...) { ; }
#define MPXDBG(format, args...) { ; }
#define OVFDBG(format, args...) { ; }
#define PRFDBG(format, args...) { ; }
#define PAPIDEBUG(level, format, args...) { ; }
#endif
#endif

#define DEADBEEF 0xdedbeef
extern int papi_num_substrates;

  /********************************************************/
/* This block provides general strings used in PAPI     */
/* If a new string is needed for PAPI prompts           */
/* it should be placed in this file and referenced by   */
/* label.                                               */
/********************************************************/
#define PAPI_ERROR_CODE_str      "Error Code"
#define PAPI_SHUTDOWN_str	      "PAPI_shutdown: PAPI is not initialized"
#define PAPI_SHUTDOWN_SYNC_str	"PAPI_shutdown: other threads still have running EventSets"


/* some members of structs and/or function parameters may or may not be
   necessary, but at this point, we have included anything that might 
   possibly be useful later, and will remove them as we progress */

/* Signal used for overflow delivery */

#define PAPI_ITIMER ITIMER_PROF
#define PAPI_SIGNAL SIGPROF
#define PAPI_ITIMER_MS 1

/* Commands used to compute derived events */

#define NOT_DERIVED      0x0    /* Do nothing */
#define DERIVED_ADD      0x1    /* Add counters */
#define DERIVED_PS       0x2    /* Divide by the cycle counter and convert to seconds */
#define DERIVED_ADD_PS   0x4    /* Add 2 counters then divide by the cycle counter and xl8 to secs. */
#define DERIVED_CMPD     0x8    /* Event lives in operand index but takes 2 or more codes */
#define DERIVED_SUB      0x10   /* Sub all counters from counter with operand_index */
#define DERIVED_POSTFIX  0x20   /* Process counters based on specified postfix string */

/* Thread related: thread local storage */

#define LOWLEVEL_TLS		PAPI_NUM_TLS+0
#define NUM_INNER_TLS   	1
#define PAPI_MAX_TLS		(NUM_INNER_TLS+PAPI_NUM_TLS)

/* Thread related: locks */

#define INTERNAL_LOCK      	PAPI_NUM_LOCK+0       /* papi_internal.c */
#define MULTIPLEX_LOCK     	PAPI_NUM_LOCK+1       /* multiplex.c */
#define THREADS_LOCK		PAPI_NUM_LOCK+2       /* threads.c */
#define HIGHLEVEL_LOCK		PAPI_NUM_LOCK+3       /* papi_hl.c */
#define MEMORY_LOCK		PAPI_NUM_LOCK+4       /* papi_memory.c*/
#define SUBSTRATE_LOCK          PAPI_NUM_LOCK+5       /* <substrate.c> */
#define GLOBAL_LOCK          	PAPI_NUM_LOCK+6       /* papi.c for global variable (static and non) initialization/shutdown */
#define NUM_INNER_LOCK         	7
#define PAPI_MAX_LOCK         	(NUM_INNER_LOCK+PAPI_NUM_LOCK)

/* extras related */

#define NEED_CONTEXT		1
#define DONT_NEED_CONTEXT 	0

#define PAPI_MAX_COUNTER_TERMS	8

/* DEFINES END HERE */

#include OS_HEADER
#include "papi_preset.h"

typedef struct _EventSetDomainInfo {
   int domain;
} EventSetDomainInfo_t;

typedef struct _EventSetGranularityInfo {
   int granularity;
} EventSetGranularityInfo_t;

typedef struct _EventSetOverflowInfo {
   long_long *deadline;
   int *threshold;
   int *EventIndex;
   int *EventCode;
   int count;
   int event_counter;
   int flags;
   int timer_ms;
   PAPI_overflow_handler_t handler;
} EventSetOverflowInfo_t;

typedef struct _EventSetProfileInfo {
   PAPI_sprofil_t **prof;
   int *count;     /* Number of buffers */
   int *threshold;
   int *EventIndex;
   int *EventCode;
   int flags;
   int overflowcount;           /* number of overflows */
   int event_counter;
} EventSetProfileInfo_t;

/* This contains info about an individual event added to the EventSet.
   The event can be either PRESET or NATIVE, and either simple or derived.
   If derived, it can consist of up to PAPI_MAX_COUNTER_TERMS native events.
   An EventSet contains a pointer to an array of these structures to define
   each added event.
 */

typedef struct _EventInfo {
   unsigned int event_code;     /* Preset or native code for this event as passed to PAPI_add_event() */
   /* should this be PAPI_MAX_COUNTER_TERMS instead of MAX_COUNTERS ?? (dkt 10/9/03) */
   int pos[PAPI_MAX_COUNTER_TERMS];   /* position in the counter array for this events components */
   char *ops;                   /* operation string of preset */
   int derived;                 /* Counter derivation command used for derived events */
} EventInfo_t;

/* This contains info about each native event added to the EventSet.
   An EventSet contains an array of MAX_COUNTERS of these structures 
   to define each native event in the set.
 */

typedef struct _NativeInfo {
   void * ni_bits;      /* Substrate defined resources used by this native event hwd_register_t*/
   int ni_event;        /* native event code; always non-zero unless empty */
   int ni_position;     /* counter array position where this native event lives */
   int ni_owners;       /* specifies how many owners share this native event */
} NativeInfo_t;


/* Multiplex definitions */

/* This contains only the information about an event that
 * would cause two events to be counted separately.  Options
 * that don't affect an event aren't included here.
 */

typedef struct _papi_info {
   int event_type;
   int domain;
   int granularity;
} PapiInfo;

typedef struct _masterevent {
   int uses;
   int active;
   int is_a_rate;
   int papi_event;
   PapiInfo pi;
   long_long count;
   long_long cycles;
   long_long handler_count;
   long_long prev_total_c;
   long_long count_estimate;
   double rate_estimate;
   struct _threadlist *mythr;
   struct _masterevent *next;
} MasterEvent;

typedef struct _threadlist {
#ifdef PTHREADS
   pthread_t thr;
#else
   unsigned long int tid;
#endif
   /* Total cycles for this thread */
   long_long total_c;
   /* Pointer to event in use */
   MasterEvent *cur_event;
   /* List of multiplexing events for this thread */
   MasterEvent *head;
   /* Pointer to next thread */
   struct _threadlist *next;
} Threadlist;

/* Structure contained in the EventSet structure that
   holds information about multiplexing. */

typedef enum { MPX_STOPPED, MPX_RUNNING } MPX_status;

typedef struct _MPX_EventSet {
   MPX_status status;
   /* Pointer to this thread's structure */
   struct _threadlist *mythr;
   /* Pointers to this EventSet's MPX entries in the master list for this thread */
   struct _masterevent *(mev[PAPI_MPX_DEF_DEG]);
   /* Number of entries in above list */
   int num_events;
   /* Not sure... */
   long_long start_c, stop_c;
   long_long start_values[PAPI_MPX_DEF_DEG];
   long_long stop_values[PAPI_MPX_DEF_DEG];
   long_long start_hc[PAPI_MPX_DEF_DEG];
} MPX_EventSet;

typedef MPX_EventSet *EventSetMultiplexInfo_t;

/* Opaque struct, not defined yet...due to threads.h <-> papi_internal.h */

struct _ThreadInfo;

typedef struct _EventSetInfo {
   unsigned long int tid;       /* Thread ID, only used if PAPI_thread_init() is called  */

   int EventSetIndex;           /* Index of the EventSet in the array  */
   int SubstrateIndex;		/* Which Substrate this EventSet Belongs too */

   int NumberOfEvents;          /* Number of events added to EventSet */

   void * machdep; /* A chunk of memory of sizeof
                                   _papi_hwi_system_info bytes. This 
                                   will contain the encoding necessary for the 
                                   hardware to set the counters to the appropriate
                                   conditions */

   long_long *hw_start;         /* Array of length _papi_hwi_system_info.num_cntrs that contains
                                   unprocessed, out of order, long_long counter registers */

   long_long *sw_stop;          /* Array of length ESI->NumberOfCounters that contains
                                   processed, in order, PAPI counter values when used or stopped */

   int state;                   /* The state of this entire EventSet; can be
                                   PAPI_RUNNING or PAPI_STOPPED plus flags */

   int NativeCount;             /* How many native events in the array below. */
   NativeInfo_t *NativeInfoArray;/*Info about each native event in the set */

   EventInfo_t *EventInfoArray; /* This array contains the mapping from 
                                   events added into the API into hardware 
                                   specific encoding as returned by the 
                                   kernel or the code that directly 
                                   accesses the counters. */

   EventSetDomainInfo_t domain;

   EventSetGranularityInfo_t granularity;

   EventSetOverflowInfo_t overflow;

   EventSetMultiplexInfo_t multiplex;

   EventSetProfileInfo_t profile;

   struct _ThreadInfo *master;

} EventSetInfo_t;

typedef struct _dynamic_array {
   EventSetInfo_t **dataSlotArray;      /* array of ptrs to EventSets */
   int totalSlots;              /* number of slots in dataSlotArrays      */
   int availSlots;              /* number of open slots in dataSlotArrays */
   int fullSlots;               /* number of full slots in dataSlotArray    */
   int lowestEmptySlot;         /* index of lowest empty dataSlotArray    */
} DynamicArray_t;

/* Substrate option types for _papi_hwd_ctl. */

typedef struct _papi_int_defdomain {
   int defdomain;
} _papi_int_defdomain_t;

typedef struct _papi_int_domain {
   int domain;
   int eventset;
   EventSetInfo_t *ESI;
} _papi_int_domain_t;

typedef struct _papi_int_granularity {
   int granularity;
   int eventset;
   EventSetInfo_t *ESI;
} _papi_int_granularity_t;

typedef struct _papi_int_overflow {
   EventSetInfo_t *ESI;
   EventSetOverflowInfo_t overflow;
} _papi_int_overflow_t;

typedef struct _papi_int_profile {
   EventSetInfo_t *ESI;
   EventSetProfileInfo_t profile;
} _papi_int_profile_t;

typedef union _papi_int_option_t {
   _papi_int_overflow_t overflow;
   _papi_int_profile_t profile;
   _papi_int_domain_t domain;
   _papi_int_defdomain_t defdomain;
   _papi_int_granularity_t granularity;
} _papi_int_option_t;

typedef struct {
   void * si;
   void * ucontext;
} _papi_hwi_context_t;

typedef struct _papi_mdi {
   pid_t pid;                   /* Process identifier */
   PAPI_hw_info_t hw_info;      /* See definition in papi.h */
   PAPI_exe_info_t exe_info;    /* See definition in papi.h */
   PAPI_shlib_info_t shlib_info;    /* See definition in papi.h */
   PAPI_preload_info_t preload_info; /* See definition in papi.h */ 


   /* Begin public feature flags */
   int supports_multiple_threads;     /* hardware counters support multiple threads */

   int supports_real_usec;      /* We can use the real_usec call */
   int supports_real_cyc;       /* We can use the real_cyc call */
   int supports_virt_usec;      /* We can use the virt_usec call */
   int supports_virt_cyc;       /* We can use the virt_cyc call */

   /* End public feature flags */

   

   /* End private Info */
   DynamicArray_t global_eventset_map;  /* Global structure to maintain int<->EventSet mapping */
} papi_mdi_t;

typedef struct _papi_substrate_mdi {
   int substrate_index;
   char substrate[81];          /* Name of the substrate we're using */
   float version;               /* Version of this substrate */

   int default_domain;          /* The default domain when this substrate is used */

   int default_granularity;     /* The default granularity when this substrate is used */

   /* Begin public feature flags */

   /* The following variables define the length of the arrays in the 
      EventSetInfo_t structure. Each array is of length num_gp_cntrs + 
      num_sp_cntrs * sizeof(long_long) */

   int num_cntrs;               /* Number of counters returned by a substrate read/write */

   int num_gp_cntrs;            /* Number of general purpose counters or counters
                                   per group */
   int grouped_counters;        /* Number of counter groups, zero for no groups */
   int num_sp_cntrs;            /* Number of special purpose counters, like 
                                   Time Stamp Counter on IBM or Pentium */

   int supports_program;        /* We can use programmable events */
   int supports_write;          /* We can write the counters */
   int supports_hw_overflow;    /* Needs overflow to be emulated */
   int supports_hw_profile;     /* Needs profile to be emulated */

   int supports_64bit_counters; /* Only limited precision is available from hardware */
   int supports_inheritance;    /* We can pass on and inherit child counters/values */
   int supports_attach;         /* We can attach PAPI to another process */
   /* End public feature flags */

   /* Begin private feature flags */

   int supports_read_reset;     /* The read call from the kernel resets the counters */

   /* End private feature flags */

   /* Begin private Info */
   int context_size;
   int register_size;
   int reg_alloc_size;
   int control_state_size;
} papi_substrate_mdi_t;

extern papi_substrate_mdi_t * _papi_hwi_substrate_info;
extern papi_mdi_t _papi_hwi_system_info;
extern int _papi_hwi_error_level;
extern volatile int _papi_hwi_using_signal;

/*
 * Debug functions for platforms without vararg macro support
 */

#ifdef NO_VARARG_MACRO
inline_static void PAPIDEBUG(int level, char *format, ...)
{
#ifdef DEBUG
   va_list args;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void SUBDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_SUBSTRATE;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void APIDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_API;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void INTDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_INTERNAL;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void THRDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_THREADS;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void MPXDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_MULTIPLEX;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void OVFDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_OVERFLOW;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}

inline_static void PRFDBG(char *format, ...)
{
#ifdef DEBUG
   va_list args;
   int level = DEBUG_PROFILE;

   if (ISLEVEL(level)) {
      va_start(args, format);
      DEBUGLABEL(DEBUGLEVEL(level));
      vfprintf(stderr, format, args);
      va_end(args);
   } else
#endif
      return;
}
#endif

#include "threads.h"
#include "papi_vector.h"

inline_static EventSetInfo_t *_papi_hwi_lookup_EventSet(int eventset)
{
   const DynamicArray_t *map = &_papi_hwi_system_info.global_eventset_map;
   EventSetInfo_t *set;

   if ((eventset < 0) || (eventset > map->totalSlots))
      return (NULL);
   
   set = map->dataSlotArray[eventset];
#ifdef DEBUG
   if ((ISLEVEL(DEBUG_THREADS)) && (_papi_hwi_thread_id_fn) && (set->master->tid != _papi_hwi_thread_id_fn()))
     return(NULL);
#endif

   return (set);
}

extern char *const init_str[];
#endif                          /* PAPI_INTERNAL_H */
