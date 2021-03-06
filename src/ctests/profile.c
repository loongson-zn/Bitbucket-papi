/* 
* File:    profile.c
* CVS:     $Id$
* Author:  Philip Mucci
*          mucci@cs.utk.edu
* Mods:    Dan Terpstra
*          terpstra@cs.utk.edu
* Mods:    Maynard Johnson
*          maynardj@us.ibm.com
* Mods:    <your name here>
*          <your email address>
*/

/* This file performs the following test: profiling and program info option call

   - This tests the SVR4 profiling interface of PAPI. These are counted 
   in the default counting domain and default granularity, depending on 
   the platform. Usually this is the user domain (PAPI_DOM_USER) and 
   thread context (PAPI_GRN_THR).

     The Eventset contains:
     + PAPI_FP_INS (to profile)
     + PAPI_TOT_CYC

   - Set up profile
   - Start eventset 1
   - Do both (flops and reads)
   - Stop eventset 1
*/

#include "prof_utils.h"
#define PROFILE_ALL

static int do_profile(unsigned long plength, unsigned scale, int thresh, int bucket);

int main(int argc, char **argv)
{
   int num_tests = 6;
   long length;
   int mask;
   int retval;

   prof_init(argc, argv);
   mask = prof_events(num_tests);

#ifdef PROFILE_ALL
/* use these lines to profile entire code address space */
   start = prginfo->address_info.text_start;
   end = prginfo->address_info.text_end;
#else
/* use these lines to profile only do_flops address space */
   start = (caddr_t)do_flops;
   end = (caddr_t)fdo_flops;
/* Itanium and ppc64 processors return function descriptors instead of function addresses.
   You must dereference the descriptor to get the address.
*/
#if defined(ITANIUM1) || defined(ITANIUM2) || defined(__powerpc64__)
   start = (caddr_t)(((struct fdesc *)start)->ip);
   end = (caddr_t)(((struct fdesc *)end)->ip);
#endif
#endif

   length = end - start;
   if (length < 0)
      test_fail(__FILE__, __LINE__, "Profile length < 0!", length);

   prof_print_address(start, end,
               "Test case profile: POSIX compatible profiling with hardware counters.\n");
   prof_print_prof_info();
   retval = do_profile(length, FULL_SCALE, THRESHOLD, PAPI_PROFIL_BUCKET_16);
   if (retval)
      retval = do_profile(length, FULL_SCALE, THRESHOLD, PAPI_PROFIL_BUCKET_32);
   if (retval)
      retval = do_profile(length, FULL_SCALE, THRESHOLD, PAPI_PROFIL_BUCKET_64);

   remove_test_events(&EventSet, mask);

   if (retval)
      test_pass(__FILE__, values, num_tests);
   else
      test_fail(__FILE__, __LINE__, "No information in buffers", 1);
   exit(1);
}

static int do_profile(unsigned long plength, unsigned scale, int thresh, int bucket) {
   int i, retval;
   unsigned long blength;
   int num_buckets;

   char *profstr[5] = {"PAPI_PROFIL_POSIX",
                        "PAPI_PROFIL_RANDOM",
                        "PAPI_PROFIL_WEIGHTED",
                        "PAPI_PROFIL_COMPRESS",
                        "PAPI_PROFIL_<all>" };

   int profflags[5] = {PAPI_PROFIL_POSIX,
                       PAPI_PROFIL_POSIX | PAPI_PROFIL_RANDOM,
                       PAPI_PROFIL_POSIX | PAPI_PROFIL_WEIGHTED,
                       PAPI_PROFIL_POSIX | PAPI_PROFIL_COMPRESS,
                       PAPI_PROFIL_POSIX | PAPI_PROFIL_WEIGHTED |
                       PAPI_PROFIL_RANDOM | PAPI_PROFIL_COMPRESS };

   do_no_profile();
   blength = prof_size(plength, scale, bucket, &num_buckets);
   prof_alloc(5, blength);

   for (i=0;i<5;i++) {
      if (!TESTS_QUIET)
         printf("Test type   : \t%s\n", profstr[i]);

      if ((retval = PAPI_profil(profbuf[i], blength, start, scale,
                              EventSet, PAPI_event, thresh,
                              profflags[i] | bucket)) != PAPI_OK) {
         test_fail(__FILE__, __LINE__, "PAPI_profil", retval);
      }
      if ((retval = PAPI_start(EventSet)) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_start", retval);

      do_both(NUM_ITERS);

      if ((retval = PAPI_stop(EventSet, values[1])) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

      if (!TESTS_QUIET) {
         printf(TAB1, event_name, (values[1])[0]);
         printf(TAB1, "PAPI_TOT_CYC:", (values[1])[1]);
      }
      if ((retval = PAPI_profil(profbuf[i], blength, start, scale,
                              EventSet, PAPI_event, 0, profflags[i])) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_profil", retval);
   }

   prof_head(blength, bucket, num_buckets, 
      "address\t\t\tflat\trandom\tweight\tcomprs\tall\n");
   prof_out(5, bucket, num_buckets, scale);

   retval = prof_check(5, bucket, num_buckets);

   for (i=0;i<5;i++) {
      free(profbuf[i]);
   }

   return(retval);
}
