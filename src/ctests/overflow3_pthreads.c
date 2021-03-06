/* 
* File:    overflow3_pthreads.c
* CVS:     $Id$
* Author:  Philip Mucci
*          mucci@cs.utk.edu
* Mods:    
*          
*/

/* This file tests the overflow functionality when there are
 * threads in which the application isn't calling PAPI (and only
 * one thread that is calling PAPI.)
 */

#include <pthread.h>
#include "papi_test.h"

int total = 0;

void *thread_fn(void *dummy)
{
   while (1){
      do_both(NUM_ITERS);
   }
}

void handler(int EventSet, void *address, long_long overflow_vector, void *context)
{
   if (!TESTS_QUIET) {
#ifdef _CRAYT3E
      fprintf(stderr, "handler(%d ) Overflow at %x, thread 0x%x!\n",
              EventSet, address, PAPI_thread_id());
#else
      fprintf(stderr, "handler(%d ) Overflow at %p, thread 0x%lux!\n",
              EventSet, address, PAPI_thread_id());
#endif
   }
   total++;
}

void mainloop(int arg)
{
   int retval, num_tests = 1;
   int EventSet1=PAPI_NULL;
   int mask1 = 0x0;
   int num_events1;
   long long **values;
   const PAPI_hw_info_t *hw_info;
   int PAPI_event;
   char event_name[PAPI_MAX_STR_LEN];

   if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET)
      if ((retval = PAPI_set_debug(PAPI_VERB_ECONT)) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);

   hw_info = PAPI_get_hardware_info();
   if (hw_info == NULL)
      test_fail(__FILE__, __LINE__, "PAPI_get_hardware_info", 2);

   /* add PAPI_TOT_CYC and one of the events in PAPI_FP_INS, PAPI_FP_OPS or
      PAPI_TOT_INS, depending on the availability of the event on the
      platform */
   EventSet1 = add_two_events(&num_events1, &PAPI_event, hw_info, &mask1);

   values = allocate_test_space(num_tests, num_events1);

   if ((retval = PAPI_overflow(EventSet1, PAPI_event, THRESHOLD, 0, handler)) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_overflow", retval);

   if ((retval = PAPI_start(EventSet1)) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_start", retval);

   do_both(arg);

   if ((retval = PAPI_stop(EventSet1, values[0])) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

   /* clear the papi_overflow event */
   if ((retval = PAPI_overflow(EventSet1, PAPI_event, 0, 0, NULL)) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_overflow", retval);

   if ((retval = PAPI_event_code_to_name(PAPI_event, event_name)) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_event_code_to_name", retval);

   if (!TESTS_QUIET) {
      printf("Thread 0x%x %s : \t%lld\n", (int) pthread_self(),
                     event_name, (values[0])[0]);
      printf("Thread 0x%x PAPI_TOT_CYC: \t%lld\n", (int) pthread_self(), 
         (values[0])[1]);
   }

   retval = PAPI_cleanup_eventset(EventSet1);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_cleanup_eventset", retval);

   retval = PAPI_destroy_eventset(&EventSet1);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_destroy_eventset", retval);

   free_test_space(values, num_tests);
   PAPI_shutdown();
}

int main(int argc, char **argv)
{
   int i, rc, retval;
   pthread_t id[NUM_THREADS];
   pthread_attr_t attr;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */

   printf("%s: Using %d threads\n\n", argv[0], NUM_THREADS);
   printf("Does non-threaded overflow work with extraneous threads present?\n");

   pthread_attr_init(&attr);
#ifdef PTHREAD_CREATE_UNDETACHED
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_UNDETACHED);
#endif
#ifdef PTHREAD_SCOPE_SYSTEM
   retval = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   if (retval != 0)
      test_skip(__FILE__, __LINE__, "pthread_attr_setscope", retval);
#endif

   for (i = 0; i < NUM_THREADS; i++) {
      rc = pthread_create(&id[i], &attr, thread_fn, NULL);
      if (rc)
         test_fail(__FILE__, __LINE__, "pthread_create", rc);
   }
   pthread_attr_destroy(&attr);

   mainloop(NUM_ITERS);

   test_pass(__FILE__, NULL, 0);
   exit(1);
}
