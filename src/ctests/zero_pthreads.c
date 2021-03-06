/* This file performs the following test: start, stop and timer
functionality for 2 slave pthreads

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

#include <pthread.h>

#include "papi_test.h"

static const PAPI_hw_info_t *hw_info = NULL;

void *Thread(void *arg)
{
   int retval, num_tests = 1;
   int EventSet1=PAPI_NULL;
   int PAPI_event, mask1;
   int num_events1;
   long_long **values;
   long_long elapsed_us, elapsed_cyc;
   char event_name[PAPI_MAX_STR_LEN];

   printf("Thread 0x%x started\n", (int) pthread_self());

   /* add PAPI_TOT_CYC and one of the events in PAPI_FP_INS, PAPI_FP_OPS or
      PAPI_TOT_INS, depending on the availability of the event on the
      platform */
   EventSet1 = add_two_events(&num_events1, &PAPI_event, hw_info, &mask1);

   retval = PAPI_event_code_to_name(PAPI_event, event_name);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_event_code_to_name", retval);

   values = allocate_test_space(num_tests, num_events1);

   elapsed_us = PAPI_get_real_usec();

   elapsed_cyc = PAPI_get_real_cyc();

   retval = PAPI_start(EventSet1);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_start", retval);

   do_flops(*(int *) arg);

   retval = PAPI_stop(EventSet1, values[0]);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

   elapsed_us = PAPI_get_real_usec() - elapsed_us;

   elapsed_cyc = PAPI_get_real_cyc() - elapsed_cyc;

   remove_test_events(&EventSet1, mask1);

   if (!TESTS_QUIET) {
      printf("Thread 0x%x %-12s : \t%lld\n", (int) pthread_self(), event_name,
             (values[0])[0]);
      printf("Thread 0x%x PAPI_TOT_CYC : \t%lld\n", (int) pthread_self(), (values[0])[1]);
      printf("Thread 0x%x Real usec    : \t%lld\n", (int) pthread_self(), elapsed_us);
      printf("Thread 0x%x Real cycles  : \t%lld\n", (int) pthread_self(), elapsed_cyc);
   }

   free_test_space(values, num_tests);

#ifdef TEST_UNREGISTER
   retval = PAPI_unregister_thread();
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);
#endif

   return (NULL);
}

int main(int argc, char **argv)
{
   pthread_t e_th, f_th, g_th, h_th;
   int flops1, flops2, flops3, flops4;
   int retval, rc;
   pthread_attr_t attr;
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

   retval = PAPI_thread_init((unsigned long (*)(void)) (pthread_self));
   if (retval != PAPI_OK) {
      if (retval == PAPI_ESBSTR)
         test_skip(__FILE__, __LINE__, "PAPI_thread_init", retval);
      else
         test_fail(__FILE__, __LINE__, "PAPI_thread_init", retval);
   }

   elapsed_us = PAPI_get_real_usec();

   elapsed_cyc = PAPI_get_real_cyc();

   pthread_attr_init(&attr);
#ifdef PTHREAD_CREATE_UNDETACHED
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_UNDETACHED);
#endif
#ifdef PTHREAD_SCOPE_SYSTEM
   retval = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   if (retval != 0)
      test_skip(__FILE__, __LINE__, "pthread_attr_setscope", retval);
#endif

   flops1 = 1000000;
   rc = pthread_create(&e_th, &attr, Thread, (void *) &flops1);
   if (rc) {
      retval = PAPI_ESYS;
      test_fail(__FILE__, __LINE__, "pthread_create", retval);
   }
   flops2 = 2000000;
   rc = pthread_create(&f_th, &attr, Thread, (void *) &flops2);
   if (rc) {
      retval = PAPI_ESYS;
      test_fail(__FILE__, __LINE__, "pthread_create", retval);
   }

   flops3 = 4000000;
   rc = pthread_create(&g_th, &attr, Thread, (void *) &flops3);
   if (rc) {
      retval = PAPI_ESYS;
      test_fail(__FILE__, __LINE__, "pthread_create", retval);
   }

   flops4 = 8000000;
   rc = pthread_create(&h_th, &attr, Thread, (void *) &flops4);
   if (rc) {
      retval = PAPI_ESYS;
      test_fail(__FILE__, __LINE__, "pthread_create", retval);
   }

   pthread_attr_destroy(&attr);
   flops1 = 500000;
   Thread(&flops1);
   pthread_join(h_th, NULL);
   pthread_join(g_th, NULL);
   pthread_join(f_th, NULL);
   pthread_join(e_th, NULL);

   elapsed_cyc = PAPI_get_real_cyc() - elapsed_cyc;

   elapsed_us = PAPI_get_real_usec() - elapsed_us;

   if (!TESTS_QUIET) {
      printf("Master real usec   : \t%lld\n", elapsed_us);
      printf("Master real cycles : \t%lld\n", elapsed_cyc);
   }

   test_pass(__FILE__, NULL, 0);
   pthread_exit(NULL);
   exit(1);
}
