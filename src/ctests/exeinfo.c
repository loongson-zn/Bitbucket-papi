/* 
* File:    profile.c
* CVS:     $Id$
* Author:  Philip Mucci
*          mucci@cs.utk.edu
* Mods:    <your name here>
*          <your email address>
*/
#include <stdlib.h>
#include <stdio.h>
#include "papi_test.h"

int main(int argc, char **argv)
{
   int retval;

   const PAPI_exe_info_t *exeinfo;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */

   if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   if (!TESTS_QUIET)
      if ((retval = PAPI_set_debug(PAPI_VERB_ECONT)) != PAPI_OK)
         test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);

   if ((exeinfo = PAPI_get_executable_info()) == NULL) 
     test_fail(__FILE__, __LINE__, "PAPI_get_executable_info", retval);

   printf("Path+Program: %s\n",exeinfo->fullname);
   printf("Program: %s\n",exeinfo->address_info.name);
   printf("Text start: %p, Text end: %p\n",exeinfo->address_info.text_start,exeinfo->address_info.text_end);
   printf("Data start: %p, Data end: %p\n",exeinfo->address_info.data_start,exeinfo->address_info.data_end);
   printf("Bss start: %p, Bss end: %p\n",exeinfo->address_info.bss_start,exeinfo->address_info.bss_end);
   
   if ((exeinfo->fullname == NULL) || (strlen(exeinfo->fullname) == 0))
     test_fail(__FILE__, __LINE__, "PAPI_get_executable_lib_info",1);
   if ((exeinfo->address_info.name == NULL) || (strlen(exeinfo->address_info.name) == 0))
     test_fail(__FILE__, __LINE__, "PAPI_get_executable_lib_info",1);
   if ((exeinfo->address_info.text_start == 0x0) || (exeinfo->address_info.text_end == 0x0) ||
       (exeinfo->address_info.text_start >= exeinfo->address_info.text_end))
     test_fail(__FILE__, __LINE__, "PAPI_get_executable_lib_info",1);
   if ((exeinfo->address_info.data_start == 0x0) || (exeinfo->address_info.data_end == 0x0) ||
       (exeinfo->address_info.data_start >= exeinfo->address_info.data_end))
     test_fail(__FILE__, __LINE__, "PAPI_get_executable_lib_info",1);
/*
   if ((exeinfo->address_info.bss_start == 0x0) || (exeinfo->address_info.bss_end == 0x0) ||
       (exeinfo->address_info.bss_start >= exeinfo->address_info.bss_end))
     test_fail(__FILE__, __LINE__, "PAPI_get_executable_lib_info",1);
*/

   sleep(1); /* Needed for debugging, so you can ^Z and stop the process, inspect /proc to see if it's right */
   
   test_pass(__FILE__, NULL, 0);
   exit(0);
}

