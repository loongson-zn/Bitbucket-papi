/* 
* File:    overflow_one_and_read.c : based on overflow_twoevents.c
* Mods:    Philip Mucci
*          mucci@cs.utk.edu
*          Kevin London
*          london@cs.utk.edu
*/

/* This file performs the following test: overflow dispatch on 1 counter.
 * In the handler read events.
*/

#include "papi_test.h"

#ifdef _CRAYT3E
#define OVER_FMT	"handler(%d, %x, %d, %lld, %d, %x) Overflow at %x!\n"
#define OUT_FMT		"%-12s : %16lld%16lld\n"
#elif defined(_WIN32)
#define OVER_FMT	"handler(%d, %x, %d, %I64d, %d, %p) Overflow at %p!\n"
#define OUT_FMT		"%-12s : %16I64d%16I64d\n"
#else
#define OVER_FMT	"handler(%d) Overflow at %p! vector=0x%llx\n"
#define OUT_FMT		"%-12s : %16lld%16lld\n"
#endif

typedef struct {
  long_long mask;
  int count;
} ocount_t;

/* there are three possible vectors, one counter overflows, the other 
   counter overflows, both overflow */
/*not used*/ ocount_t overflow_counts[3] = {{0,0}, {0,0}, {0,0}};
/*not used*/ int total_unknown = 0;

static const PAPI_hw_info_t *hw_info = NULL;
/*added*/ long_long dummyvalues[2];

void handler(int EventSet, void *address, long_long overflow_vector, void *context)
{
  int retval;

   if (!TESTS_QUIET) {
      fprintf(stderr, OVER_FMT, EventSet, address, overflow_vector);
   }

   if ((retval = PAPI_read(EventSet, dummyvalues)) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_read", retval);

   if (!TESTS_QUIET) {
        fprintf(stderr,TWO12, dummyvalues[0], dummyvalues[1], "(Reading  counters)\n");
   }
   if ( dummyvalues[1] == 0 )
      test_fail(__FILE__, __LINE__, "Total Cycles == 0", 1);
}

int main(int argc, char **argv)
{
   int EventSet;
   long_long **values = NULL;
   int retval;
   int PAPI_event;
   char event_name[PAPI_MAX_STR_LEN];
   int num_events1, mask1;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET) {
      retval = PAPI_set_debug(PAPI_VERB_ECONT);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);
   }

   hw_info = PAPI_get_hardware_info(); 
   if (hw_info == NULL)
     test_fail(__FILE__, __LINE__, "PAPI_get_hardware_info", 2);
      
    /* add PAPI_TOT_CYC and one of the events in PAPI_FP_INS, PAPI_FP_OPS or
      PAPI_TOT_INS, depends on the availability of the event on the
      platform */ 
/* NOTE: Only adding one overflow on PAPI_event -- no overflow for PAPI_TOT_CYC*/
   EventSet = add_two_events(&num_events1, &PAPI_event, hw_info, &mask1);

   values = allocate_test_space(2, num_events1);

   if ((retval = PAPI_event_code_to_name(PAPI_event, event_name)) != PAPI_OK)
     test_fail(__FILE__, __LINE__, "PAPI_event_code_to_name", retval);

   retval = PAPI_start(EventSet);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_start", retval);

   do_flops(NUM_FLOPS);

   retval = PAPI_stop(EventSet, values[0]);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

   retval = PAPI_overflow(EventSet, PAPI_event, THRESHOLD, 0, handler);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_overflow", retval);

   retval = PAPI_start(EventSet);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_start", retval);

   do_flops(NUM_FLOPS);

   retval = PAPI_stop(EventSet, values[1]);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

   remove_test_events(&EventSet, mask1);


   if (!TESTS_QUIET) {
     printf("Test case: Overflow dispatch of 1st event in set with 2 events.\n");
     printf("---------------------------------------------------------------\n");
     printf("Threshold for overflow is: %d\n", THRESHOLD);
     printf("Using %d iterations of c += a*b\n", NUM_FLOPS);
     printf("-----------------------------------------------\n");
  
     printf("Test type    : %16d%16d\n", 1, 2);
     printf(OUT_FMT, event_name, (values[0])[0], (values[1])[0]);
     printf(OUT_FMT, "PAPI_TOT_CYC", (values[0])[1], (values[1])[1]);
   }

   test_pass(__FILE__, NULL, 0);
   exit(1);
}

