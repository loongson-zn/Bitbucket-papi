/* 
* File:    	tenth.c
* Mods: 	Maynard Johnson
*			maynardj@us.ibm.com
*/
#define ITERS 100

/* This file performs the following test: start, stop and timer functionality for 
   PAPI_L1_TCM derived event

   - They are counted in the default counting domain and default
     granularity, depending on the platform. Usually this is 
     the user domain (PAPI_DOM_USER) and thread context (PAPI_GRN_THR).
   - Get us.
   - Start counters
   - Do flops
   - Stop and read counters
   - Get us.
*/


#if defined(sun) && defined(sparc)
#define CACHE_LEVEL "PAPI_L2_TCM"
#define EVT1		  PAPI_L2_TCM
#define EVT2		  PAPI_L2_TCA
#define EVT3		  PAPI_L2_TCH
#define EVT1_STR	  "PAPI_L2_TCM: "
#define EVT2_STR	  "PAPI_L2_TCA: "
#define EVT3_STR	  "PAPI_L2_TCH: "
#define MASK1		  MASK_L2_TCM
#define MASK2		  MASK_L2_TCA
#define MASK3		  MASK_L2_TCH
#else
#if defined(_POWER4) || defined(_POWER5) || defined(_PPPC970)
#define CACHE_LEVEL "PAPI_L1_DCA"
#define EVT1		  PAPI_L1_DCA
#define EVT2		  PAPI_L1_DCW
#define EVT3		  PAPI_L1_DCR
#define EVT1_STR	  "PAPI_L1_DCA: "
#define EVT2_STR	  "PAPI_L1_DCW: "
#define EVT3_STR	  "PAPI_L1_DCR: "
#define MASK1		  MASK_L1_DCA
#define MASK2		  MASK_L1_DCW
#define MASK3		  MASK_L1_DCR
#else
#define CACHE_LEVEL "PAPI_L1_TCM"
#define EVT1		  PAPI_L1_TCM
#define EVT2		  PAPI_L1_ICM
#define EVT3		  PAPI_L1_DCM
#define EVT1_STR	  "PAPI_L1_TCM: "
#define EVT2_STR	  "PAPI_L1_ICM: "
#define EVT3_STR	  "PAPI_L1_DCM: "
#define MASK1		  MASK_L1_TCM
#define MASK2		  MASK_L1_ICM
#define MASK3		  MASK_L1_DCM
#endif
#endif

#include "papi_test.h"

extern int TESTS_QUIET;         /* Declared in test_utils.c */

int main(int argc, char **argv)
{
   int retval, num_tests = 30, tmp;
   int EventSet1=PAPI_NULL;
   int EventSet2=PAPI_NULL;
   int EventSet3=PAPI_NULL;
   int mask1 = MASK1;
   int mask2 = MASK2;
   int mask3 = MASK3;
   int num_events1;
   int num_events2;
   int num_events3;
   long_long **values;
   int i, j;
   long_long min[3];
   long_long max[3];
   long_long sum[3];

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET) {
      retval = PAPI_set_debug(PAPI_VERB_ECONT);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);
   }

   /* Make sure that required resources are available */
   retval = PAPI_query_event(EVT1);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, EVT1_STR, retval);

   retval = PAPI_query_event(EVT2);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, EVT2_STR, retval);

   retval = PAPI_query_event(EVT3);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, EVT3_STR, retval);


   EventSet1 = add_test_events(&num_events1, &mask1);
   EventSet2 = add_test_events(&num_events2, &mask2);
   EventSet3 = add_test_events(&num_events3, &mask3);

   values = allocate_test_space(num_tests, 1);

   /* Warm me up */
   do_l1misses(ITERS);
   do_misses(1, 1024*1024*4);

   for (i=0; i<10; i++) {
      retval = PAPI_start(EventSet1);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_start", retval);

      do_l1misses(ITERS);
      do_misses(1, 1024*1024*4);

      retval = PAPI_stop(EventSet1, values[(i*3)+0]);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

      retval = PAPI_start(EventSet2);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_start", retval);

      do_l1misses(ITERS);
      do_misses(1, 1024*1024*4);

      retval = PAPI_stop(EventSet2, values[(i*3)+1]);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

      retval = PAPI_start(EventSet3);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_start", retval);

      do_l1misses(ITERS);
      do_misses(1, 1024*1024*4);

      retval = PAPI_stop(EventSet3, values[(i*3)+2]);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_stop", retval);
   }

   remove_test_events(&EventSet1, mask1);
   remove_test_events(&EventSet2, mask2);
   remove_test_events(&EventSet3, mask3);

   for (j=0;j<3;j++) {
      min[j] = 65535;
      max[j] = sum[j] = 0;
   }
   for (i=0;i<10;i++) {
      for (j=0;j<3;j++) {
         if (min[j] > values[(i*3)+j][0]) min[j] = values[(i*3)+j][0];
         if (max[j] < values[(i*3)+j][0]) max[j] = values[(i*3)+j][0];
         sum[j] += values[(i*3)+j][0];
      }
   }

   if (!TESTS_QUIET) {
      printf("Test case 10: start, stop for derived event %s.\n", CACHE_LEVEL);
      printf("--------------------------------------------------------\n");
      tmp = PAPI_get_opt(PAPI_DEFDOM, NULL);
      printf("Default domain is: %d (%s)\n", tmp, stringify_domain(tmp));
      tmp = PAPI_get_opt(PAPI_DEFGRN, NULL);
      printf("Default granularity is: %d (%s)\n", tmp, stringify_granularity(tmp));
      printf("Using %d iterations of c += a*b\n", ITERS);
      printf("Repeated 10 times\n");
      printf
          ("-------------------------------------------------------------------------\n");
/* 
      for (i=0;i<10;i++) {
         printf("Test type   : %12s%13s%13s\n", "1", "2", "3");
         printf(TAB3, EVT1_STR, values[(i*3)+0][0], (long_long)0, (long_long)0);
         printf(TAB3, EVT2_STR, (long_long)0, values[(i*3)+1][0], (long_long)0);
         printf(TAB3, EVT3_STR, (long_long)0, (long_long)0, values[(i*3)+2][0]);
         printf
            ("-------------------------------------------------------------------------\n");
      }
*/
      printf("Test type   : %12s%13s%13s\n", "min", "max", "sum");
      printf(TAB3, EVT1_STR, min[0], max[0], sum[0]);
      printf(TAB3, EVT2_STR, min[1], max[1], sum[1]);
      printf(TAB3, EVT3_STR, min[2], max[2], sum[2]);
      printf
         ("-------------------------------------------------------------------------\n");
      printf("Verification:\n");
#if defined(sun) && defined(sparc)
      printf(TAB1, "Sum 1 approximately equals sum 2 - sum 3 or", (sum[1] - sum[2]));
#else
      printf(TAB1, "Sum 1 approximately equals sum 2 + sum 3 or", (sum[1] + sum[2]));
#endif
   }

   {
      long_long tmin, tmax;

#if defined(sun) && defined(sparc)
      tmax = (long_long) (sum[1] - sum[2]);
#else
      tmax = (long_long) (sum[1] + sum[2]);
#endif

      printf ("percent error: %f\n", (float)(abs((int)(tmax - sum[0]))*100)/sum[0]);
      tmin = (long_long) (tmax * 0.8);
      tmax = (long_long) (tmax * 1.2);
      if (sum[0] > tmax || sum[0] < tmin)
         test_fail(__FILE__, __LINE__, CACHE_LEVEL, 1);
   }
   test_pass(__FILE__, values, num_tests);
   exit(1);
}
