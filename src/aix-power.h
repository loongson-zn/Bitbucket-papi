#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <libgen.h>
#include <sys/systemcfg.h>
#include <sys/processor.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <procinfo.h>
#include <sys/atomic_op.h>

#define ANY_THREAD_GETS_SIGNAL
#include <dlfcn.h>

#include "pmapi.h"
#define POWER_MAX_COUNTERS MAX_COUNTERS
#define GROUP_INTS 2
#define MAX_GROUPS (GROUP_INTS * 32)
#define INVALID_EVENT -2

#include "papi.h"

typedef struct hwd_control_state {
  /* Indices into preset map for event in order of addition */
  /* if !PRESET_MASK then native event and counter # */
  unsigned int preset[POWER_MAX_COUNTERS];
  /* bitmap with all counters currently used */
  unsigned char master_selector;  
  /* bitmap with which counters used for which event */
  unsigned char selector[POWER_MAX_COUNTERS];  
  /* Buffer to pass to the kernel to control the counters */
  pm_prog_t counter_cmd;
  /* Interrupt interval */
  int timer_ms;  
} hwd_control_state_t;

#include "papi_internal.h"

typedef struct hwd_preset {
  /* Is this event derived? */
  unsigned char derived;
  
#ifdef _POWER4
  /* How many metrics? */
  unsigned char metric_count;
  /* Bits encode the groups this event lives in */
  unsigned int gps[GROUP_INTS];  
#else
  /* Which counters can be used? Bits encode counters available
      Separate selectors for each metric in a derived event;
      Rank determines how many counters carry each metric */
  unsigned char selector[POWER_MAX_COUNTERS];  
  unsigned char rank[POWER_MAX_COUNTERS];
#endif

  /* Buffers containing counter cmds for each possible metric */
  unsigned char counter_cmd[POWER_MAX_COUNTERS][POWER_MAX_COUNTERS];
  /* If it exists, then this is the description of this event */
  char note[PAPI_MAX_STR_LEN];
} hwd_preset_t;

typedef struct hwd_groups {
  /* group number from the pmapi pm_groups_t struct */
  int group_id;
  /* Buffer containing counter cmds for this group */
  unsigned char counter_cmd[POWER_MAX_COUNTERS];
} hwd_groups_t;

typedef struct pmapi_search {
  /* Preset code */
  int preset;
  /* Derived code */
  int derived;
  /* Strings to look for */
  char *(findme[POWER_MAX_COUNTERS]);
} pmapi_search_t;

extern _etext;
extern _edata;
extern _end;
extern _data;
extern int (*thread_kill_fn)(int, int);