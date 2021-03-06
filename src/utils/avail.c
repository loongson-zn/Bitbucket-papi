/* This file performs the following test: hardware info and which events are available */

#include "papi_test.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */

static void print_help(void)
{
   printf("This is the PAPI avail program.\n");
   printf("It provides availability and detail information\nfor PAPI preset and native events.  Usage:\n\n");
   printf("    avail [options] [event name]\n");
   printf("    avail TESTS_QUIET\n\n");
   printf("Options:\n\n");
   printf("  -a            display only available PAPI preset events\n");
   printf("  -d            display PAPI preset event info in detailed format\n");
   printf("  -e EVENTNAME  display full detail for named preset or native event\n");
   printf("  -h            print this help message\n");
   printf("  -t            display PAPI preset event info in tabular format (default)\n");
   printf("\n");
}

int main(int argc, char **argv)
{
   int i,j, k;
   int retval;
   int print_event_info = 0;
   char *name = NULL;
   int print_avail_only = 0;
   int print_tabular = 1;
   PAPI_event_info_t info;
   const PAPI_hw_info_t *hwinfo = NULL;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */
   if (argv[1]) {
      if (strstr(argv[1], "-e")) {
         print_event_info = 1;
         name = argv[2];
      }
   }
   for (i = 0; i < argc; i++)
      if (argv[i]) {
         if (strstr(argv[i], "-a"))
            print_avail_only = PAPI_PRESET_ENUM_AVAIL;
         if (strstr(argv[i], "-d"))
            print_tabular = 0;
         if (strstr(argv[i], "-h")) {
            print_help();
            exit(1);
         }
      }

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET) {
      retval = PAPI_set_debug(PAPI_VERB_ECONT);
      if (retval != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);
   }

   if ((hwinfo = PAPI_get_hardware_info()) == NULL)
      test_fail(__FILE__, __LINE__, "PAPI_get_hardware_info", 2);

   if (!TESTS_QUIET) {
      printf("Test case avail.c: Available events and hardware information.\n");
      printf
          ("-------------------------------------------------------------------------\n");
      printf("Vendor string and code   : %s (%d)\n", hwinfo->vendor_string,
             hwinfo->vendor);
      printf("Model string and code    : %s (%d)\n", hwinfo->model_string, hwinfo->model);
      printf("CPU Revision             : %f\n", hwinfo->revision);
      printf("CPU Megahertz            : %f\n", hwinfo->mhz);
      printf("CPU's in this Node       : %d\n", hwinfo->ncpu);
      printf("Nodes in this System     : %d\n", hwinfo->nnodes);
      printf("Total CPU's              : %d\n", hwinfo->totalcpus);
      printf("Number Hardware Counters : %d\n", PAPI_get_opt(PAPI_MAX_HWCTRS, NULL));
      printf("Max Multiplex Counters   : %d\n", PAPI_MPX_DEF_DEG);
      printf
          ("-------------------------------------------------------------------------\n");

      if (print_event_info) {
         if (PAPI_event_name_to_code(name, &i) == PAPI_OK) {
            if (PAPI_get_event_info(i, &info) == PAPI_OK) {
               PAPI_event_info_t n_info;
               if (i & PAPI_PRESET_MASK) {
                  printf("Event name:\t\t\t%s\nEvent Code:\t\t\t0x%-10x\nNumber of Native Events:\t%d\n",
		               info.symbol, info.event_code, info.count);
                  printf("Short Description:\t\t|%s|\nLong Description:\t\t|%s|\nDeveloper's Notes:\t\t|%s|\n",
		               info.short_descr, info.long_descr, info.note);
                     printf("Derived Type:\t\t\t|%s|\nPostfix Processing String:\t|%s|\n",
                     info.derived, info.postfix);
                  for (j=0;j<(int)info.count;j++) {
                     printf(" |Native Code[%d]: 0x%x  %s|\n",j,info.code[j], info.name[j]);
                     PAPI_get_event_info(info.code[j], &n_info);
                     printf(" |Number of Register Values: %d|\n", n_info.count);
                     for (k=0;k<(int)n_info.count;k++)
                        printf(" |Register[%d]: 0x%-10x  %s|\n",k, n_info.code[k], n_info.name[k]);
                     printf(" |Native Event Description: |%s|\n\n", n_info.long_descr);
                 }
               }
               else { /* must be a native event code */
                  printf("%s\t0x%x\n |%s|\n",
		               info.symbol,
		               info.event_code,
		               info.long_descr);
                  for (k=0;k<(int)info.count;k++)
                     printf(" |Register Value[%d]: 0x%-10x  %s|\n",k,info.code[k], info.name[k]);
               }
	         }
         }
         else printf("Sorry, an event by the name '%s' could not be found. Is it typed correctly?\n\n",name);
      }
      else {
         i = PAPI_PRESET_MASK;
         if (print_tabular) {
            if (print_avail_only) {
               printf("Name\t\tDerived\tDescription (Mgr. Note)\n");
            } else {
               printf("Name\t\tCode\t\tAvail\tDeriv\tDescription (Note)\n");
            }
         }
         else {
            printf("The following correspond to fields in the PAPI_event_info_t structure.\n");
            printf("Symbol\tEvent Code\tCount\n |Short Description|\n |Long Description|\n |Developer's Notes|\n Derived|\n |PostFix|\n");
         }
         do {
            if (PAPI_get_event_info(i, &info) == PAPI_OK) {
               if (print_tabular) {
                  if (print_avail_only) {
		               printf("%s\t%s\t%s (%s)\n",
			               info.symbol,
			               (info.count > 1 ? "Yes" : "No"),
			               info.long_descr, (info.note ? info.note : ""));
                  } else {
		               printf("%s\t0x%x\t%s\t%s\t%s (%s)\n",
		                     info.symbol,
		                     info.event_code,
		                     (info.count ? "Yes" : "No"),
		                     (info.count > 1 ? "Yes" : "No"),
		                     info.long_descr, (info.note ? info.note : ""));
                  }
               } else {
	               printf("%s\t0x%x\t%d\n |%s|\n |%s|\n |%s|\n |%s|\n |%s|\n",
		               info.symbol,
		               info.event_code,
		               info.count,
		               info.short_descr,
		               info.long_descr,
		               info.note,
                     info.derived,
                     info.postfix);
                  for (j=0;j<(int)info.count;j++) printf(" |Native Code[%d]: 0x%x  %s|\n",j,info.code[j], info.name[j]);
               }
	         }
         } while (PAPI_enum_event(&i, print_avail_only) == PAPI_OK);
      }
      printf
          ("-------------------------------------------------------------------------\n");
   }

   test_pass(__FILE__, NULL, 0);
   exit(1);
}
