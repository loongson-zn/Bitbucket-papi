/* 
* File:    prof_utils.c
* CVS:     $Id$
* Author:  Dan Terpstra
*          terpstra@cs.utk.edu
* Mods:    <your name here>
*          <your email address>
*/

/* This file contains utility functions useful for all profiling tests
   It can be used by:
   - profile.c,
   - sprofile.c,
   - profile_pthreads.c,
   - profile_twoevents.c,
   - earprofile.c,
   - future profiling tests.
*/

#include "papi_test.h"

/* variables global to profiling tests */
long_long **values;
const PAPI_exe_info_t *prginfo = NULL;
const PAPI_hw_info_t *hw_info;
char event_name[PAPI_MAX_STR_LEN];
int PAPI_event;
int EventSet = PAPI_NULL;
caddr_t start, end;
void *profbuf[5];

/* This function does the generic initialization stuff found at the top of most
   profile tests (most tests in general). This includes:
   - setting the QUIET flag;
   - initing the PAPI library;
   - setting the debug level;
   - getting hardware and executable info.
   It assumes that prginfo and hw_info are global to the parent routine.
*/
void prof_init(int argc, char **argv) {
   int retval;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */

   if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET)
      if ((retval = PAPI_set_debug(PAPI_VERB_ECONT)) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);

   hw_info = PAPI_get_hardware_info();
   if (hw_info == NULL)
     test_fail(__FILE__, __LINE__, "PAPI_get_hardware_info", 2);

   if ((prginfo = PAPI_get_executable_info()) == NULL)
      test_fail(__FILE__, __LINE__, "PAPI_get_executable_info", 1);
}

/* Many profiling tests count one of {FP_INS, FP_OPS, TOT_INS} and TOT_CYC.
   This function creates an event set containing the appropriate pair of events.
   It also initializes the global event_name string to the event selected.
   Assumed globals: EventSet, PAPI_event, hw_info, event_name.
*/
int prof_events(int num_tests) {
   int retval;
   int num_events, mask;

  /* add PAPI_TOT_CYC and one of the events in PAPI_FP_INS, PAPI_FP_OPS or
      PAPI_TOT_INS, depends on the availability of the event on the
      platform */
   EventSet = add_two_events(&num_events, &PAPI_event, hw_info, &mask);

   values = allocate_test_space(num_tests, num_events);

   if ((retval = PAPI_event_code_to_name(PAPI_event, event_name)) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_event_code_to_name", retval);

   return (mask);
}

/* This function displays info from the prginfo structure in a standardized format.
*/
void prof_print_address(caddr_t start, caddr_t end, char *title) {
   printf(title);
   printf("----------------------------------------------------------------\n");
   printf("Text start: %p, Text end: %p, Text length: 0x%x\n",
            prginfo->address_info.text_start, prginfo->address_info.text_end,
            (unsigned int)(prginfo->address_info.text_end - prginfo->address_info.text_start));
   printf("Data start: %p, Data end: %p\n",
            prginfo->address_info.data_start, prginfo->address_info.data_end);
   printf("BSS start : %p, BSS end : %p\n",
            prginfo->address_info.bss_start, prginfo->address_info.bss_end);

   printf("----------------------------------------------------------------\n");
}
/* This function displays profining information useful for several profile tests.
   It (probably inappropriately) assumes use of a common THRESHOLD. This should
   probably be a passed parameter.
   Assumed globals: event_name, start, stop.
*/
void prof_print_prof_info(void) {
   printf("Profiling event  : %s\n", event_name);
   printf("Profile Threshold: %d\n", THRESHOLD);
   printf("Profile Addresses: begins: %p\n", start);
   printf("                   ends  : %p\n", end);
   printf("----------------------------------------------------------------\n");
   printf("\n");
}
/* Most profile tests begin by counting the eventset with no profiling enabled.
   This function does that work. It assumes that the 'work' routine is do_both().
   A better implementation would pass a pointer to the work function.
   Assumed globals: EventSet, values, event_name.
*/
void do_no_profile (void) {
   int retval;

   if ((retval = PAPI_start(EventSet)) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_start", retval);

   do_both(NUM_ITERS);

   if ((retval = PAPI_stop(EventSet, values[0])) != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_stop", retval);

   printf("Test type   : \tNo profiling\n");
   printf(TAB1, event_name, (values[0])[0]);
   printf(TAB1, "PAPI_TOT_CYC:", (values[0])[1]);
}

/* This routine allocates and initializes up to 5 equal sized profiling buffers.
   They need to be freed when profiling is completed.
   The number and size are passed parameters.
   The profbuf[] array of void * pointers is an assumed global.
   It should be cast to the required type by the parent routine.
*/
void prof_alloc(int num, unsigned long blength) {
   int i;

   for (i=0;i<num;i++) {
      profbuf[i] = malloc(blength);
      if (profbuf[i] == NULL) {
         test_fail(__FILE__, __LINE__, "malloc", PAPI_ESYS);
      }
      memset(profbuf[i], 0x00, blength );
   }
}

/* Given the profiling type (16, 32, or 64) this function returns the 
   bucket size in bytes. NOTE: the bucket size does not ALWAYS correspond
   to the expected value, esp on architectures like Cray with weird data types.
   This is necessary because the posix_profile routine in extras.c relies on
   the data types and sizes produced by the compiler.
*/
int prof_buckets(int bucket) {
   int bucket_size;
   switch (bucket) {
      case PAPI_PROFIL_BUCKET_16:
         bucket_size = sizeof(short);
         break;
      case PAPI_PROFIL_BUCKET_32:
         bucket_size = sizeof(int);
         break;
      case PAPI_PROFIL_BUCKET_64:
         bucket_size = sizeof(u_long_long);
         break;
      default:
         bucket_size = 0;
         break;
   }
   return(bucket_size);
}

/* A standardized header printing routine. No assumed globals.
*/
void prof_head(unsigned long blength, int bucket, int num_buckets, char *header) {
   int bucket_size = prof_buckets(bucket);
   printf("\n------------------------------------------------------------\n");
   printf("PAPI_profil() hash table, Bucket size: %d bits.\n", bucket_size*8);
   printf("Number of buckets: %d.\nLength of buffer: %ld bytes.\n", num_buckets, blength);
   printf("------------------------------------------------------------\n");
   printf(header);
}

/* This function prints a standardized profile output based on the bucket size.
   A row consisting of an address and 'n' data elements is displayed for each
   address with at least one non-zero bucket.
   Assumes global profbuf[] array pointers.
*/
void prof_out(int n, int bucket, int num_buckets, int scale) {
   int i,j;
   unsigned short buf_16;
   unsigned int   buf_32;
   u_long_long  buf_64;
   unsigned short **buf16 = (unsigned short **)profbuf;
   unsigned int   **buf32 = (unsigned int **)profbuf;
   u_long_long    **buf64 = (u_long_long **)profbuf;

   if (!TESTS_QUIET) {
      /* printf("0x%lx\n",(unsigned long) start + (unsigned long) (2 * i)); */
      switch (bucket) {
         case PAPI_PROFIL_BUCKET_16:
            for (i = 0; i < num_buckets; i++) {
               for(j=0,buf_16=0;j<n;j++) buf_16 |= (buf16[j])[i];
               if (buf_16) {
                  printf("%-16p", start + (int)(((long_long)i * scale)>>17)); 
                  for(j=0,buf_16=0;j<n;j++)
                     printf("\t%d", (buf16[j])[i]);
                  printf("\n");
              }
            }
            break;
         case PAPI_PROFIL_BUCKET_32:
            for (i = 0; i < num_buckets; i++) {
               for(j=0,buf_32=0;j<n;j++) buf_32 |= (buf32[j])[i];
               if (buf_32) {
                  printf("%-16p", start + (int)(((long_long)i * scale)>>17));
                  for(j=0,buf_32=0;j<n;j++)
                     printf("\t%d", (buf32[j])[i]);
                  printf("\n");
               }
            }
            break;
         case PAPI_PROFIL_BUCKET_64:
            for (i = 0; i < num_buckets; i++) {
               for(j=0,buf_64=0;j<n;j++) buf_64 |= (buf64[j])[i];
               if (buf_64) {
                  printf("%-16p", start + (int)(((long_long)i * scale)>>17));
                  for(j=0,buf_64=0;j<n;j++)
                     printf("\t%lld", (buf64[j])[i]);
                  printf("\n");
              }
            }
            break;
      }
      printf("------------------------------------------------------------\n\n");
   }
}
/* This function checks to make sure that some buffer value somewhere is nonzero.
   If all buffers are empty, zero is returned. This usually indicates a profiling
   failure. Assumes global profbuf[].
*/
int prof_check(int n, int bucket, int num_buckets) {
   int i,j;
   int retval = 0;
   unsigned short **buf16 = (unsigned short **)profbuf;
   unsigned int   **buf32 = (unsigned int **)profbuf;
   u_long_long    **buf64 = (u_long_long **)profbuf;

   switch (bucket) {
      case PAPI_PROFIL_BUCKET_16:
         for (i = 0; i < num_buckets; i++)
            for (j=0;j<n;j++)
               retval = retval || buf16[j][i];
         break;
      case PAPI_PROFIL_BUCKET_32:
            for (i = 0; i < num_buckets; i++)
            for (j=0;j<n;j++)
               retval = retval || buf32[j][i];
         break;
      case PAPI_PROFIL_BUCKET_64:
            for (i = 0; i < num_buckets; i++)
            for (j=0;j<n;j++)
               retval = retval || buf64[j][i];
         break;
   }
   return(retval);
}

/* Computes the length (in bytes) of the buffer required for profiling.
   'plength' is the profile length, or address range to be profiled.
   By convention, it is assumed that there are half as many buckets as addresses.
   The scale factor is a fixed point fraction in which 0xffff = ~1
                                                         0x8000 = 1/2
                                                         0x4000 = 1/4, etc.
   Thus, the number of profile buckets is (plength/2) * (scale/65536),
   and the length (in bytes) of the profile buffer is buckets * bucket size.
   */
unsigned long prof_size(unsigned long plength, unsigned scale, int bucket, int *num_buckets) {
   unsigned long blength;
   long_long llength = ((long_long)plength * scale);
   int bucket_size = prof_buckets(bucket);
   *num_buckets = (llength / 65536) / 2;
   blength = (unsigned long)(*num_buckets) * bucket_size;
   return(blength);
}

