/* $Id$ */

/* This file performs the following test: start, stop and timer
functionality for 2 slave native SMP threads

   - It attempts to use the following two counters. It may use less
depending on hardware counter resource limitations. These are counted
in the default counting domain and default granularity, depending on
the platform. Usually this is the user domain (PAPI_DOM_USER) and
thread context (PAPI_GRN_THR).

     + PAPI_FP_INS
     + PAPI_TOT_CYC

Each of 2 slave pthreads:
   - Get cyc.
   - Get us.
   - Start counters
   - Do flops
   - Stop and read counters
   - Get us.
   - Get cyc.

Master pthread:
   - Get us.
   - Get cyc.
   - Fork threads
   - Wait for threads to exit
   - Get us.
   - Get cyc.
*/


#include "papi_test.h"

#if defined(sun) && defined(sparc)
#include <thread.h>
#elif defined(mips) && defined(sgi) && defined(unix)
#include <mpc.h>
#elif defined(_AIX)
#include <pthread.h>
#endif

extern int TESTS_QUIET;         /* Declared in test_utils.c */
const PAPI_hw_info_t *hw_info = NULL;


void Thread(int t, int n)
{
   int retval, num_tests = 1;
   int EventSet1=PAPI_NULL;
   int PAPI_event, mask1;
   int num_events1;
   long_long **values;
   long_long elapsed_us, elapsed_cyc;
   char event_name[PAPI_MAX_STR_LEN];

   /* add PAPI_TOT_CYC and one of the events in PAPI_FP_INS, PAPI_FP_OPS or
      PAPI_TOT_INS, depending on the availability of the event on the
      platform */
   EventSet1 = add_two_events(&num_events1, &PAPI_event, hw_info, &mask1);

   retval = PAPI_event_code_to_name(PAPI_event, event_name);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_event_code_to_name", retval);

   values = allocate_test_space(num_tests, num_events1);

   retval = PAPI_start(EventSet1);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_start", retval);

   elapsed_us = PAPI_get_real_usec();

   elapsed_cyc = PAPI_get_real_cyc();

   do_flops(n);

   elapsed_us = PAPI_get_real_usec() - elapsed_us;

   elapsed_cyc = PAPI_get_real_cyc() - elapsed_cyc;

   retval = PAPI_stop(EventSet1, values[0]);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

   remove_test_events(&EventSet1, mask1);

   if (!TESTS_QUIET) {
      printf("Thread 0x%x %-12s : \t%lld\n", t, event_name, (values[0])[0]);
      printf("Thread 0x%x PAPI_TOT_CYC : \t%lld\n", t, (values[0])[1]);
   }

   free_test_space(values, num_tests);
   if (!TESTS_QUIET) {
      printf("Thread 0x%x Real usec    : \t%lld\n", t, elapsed_us);
      printf("Thread 0x%x Real cycles  : \t%lld\n", t, elapsed_cyc);
   }
}

int main(int argc, char **argv)
{
   int i, retval;
   long long elapsed_us, elapsed_cyc;

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

   elapsed_us = PAPI_get_real_usec();

   elapsed_cyc = PAPI_get_real_cyc();

#if defined(_AIX)
   retval = PAPI_thread_init((unsigned long (*)(void)) (pthread_self));
   if (retval != PAPI_OK) {
      if (retval == PAPI_ESBSTR)
         test_skip(__FILE__, __LINE__, "PAPI_thread_init", retval);
      else
         test_fail(__FILE__, __LINE__, "PAPI_thread_init", retval);
   }
#pragma ibm parallel_loop
#elif defined(sgi) && defined(mips)
   retval = PAPI_thread_init((unsigned long (*)(void)) (mp_my_threadnum));
   if (retval != PAPI_OK) {
      test_fail(__FILE__, __LINE__, "PAPI_thread_init", retval);
   }
#pragma parallel
#pragma local(i)
#pragma pfor
#elif defined(sun) && defined(sparc)
   retval = PAPI_thread_init((unsigned long (*)(void)) (thr_self));
   if (retval != PAPI_OK) {
      test_fail(__FILE__, __LINE__, "PAPI_thread_init", retval);
   }
#pragma MP taskloop private(i)
#else
#error "Architecture not included in this test file yet."
#endif
   for (i = 1; i < 3; i++)
      Thread(i, 10000000 * i);

   elapsed_cyc = PAPI_get_real_cyc() - elapsed_cyc;

   elapsed_us = PAPI_get_real_usec() - elapsed_us;

   if (!TESTS_QUIET) {
      printf("Master real usec   : \t%lld\n", elapsed_us);
      printf("Master real cycles : \t%lld\n", elapsed_cyc);
   }
   test_pass(__FILE__, NULL, 0);
   exit(1);
}
