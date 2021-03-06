/*
 * File:    linux.c
 * CVS:     $Id$
 * Author:  Kevin London
 *          london@cs.utk.edu
 * Mods:    <Your name here>
 *          <Your email here>
 */

#include "papi.h"
#include "papi_internal.h"
#include "papi_vector.h"
#include "papi_memory.h"

#ifndef PAPI_NO_VECTOR 
papi_vectors_t *_papi_vector_table;


/* Prototypes */
int vec_int_ok_dummy ();
int vec_int_one_dummy();
int vec_int_dummy ();
void vec_void_dummy();
void * vec_void_star_dummy();
long_long vec_long_long_dummy();
char * vec_char_star_dummy();
long vec_long_dummy();
u_long_long vec_dummy_get_virt_cycles (void * zero);
u_long_long vec_dummy_get_virt_usec (void * zero);
u_long_long vec_dummy_get_real_usec (void);
u_long_long vec_dummy_get_real_cycles (void);


u_long_long vec_dummy_get_real_usec (void)
{
  struct timeval tv;
    gettimeofday(&tv,NULL);  return((tv.tv_sec * 1000000) + tv.tv_usec);
} 
        
u_long_long vec_dummy_get_real_cycles (void)
{
  float usec, cyc;

  usec = (float)vec_dummy_get_real_usec();
  cyc = usec * _papi_hwi_system_info.hw_info.mhz;
  return((long long)cyc);
}

u_long_long vec_dummy_get_virt_usec (void * zero)
{
  long long retval;
  struct tms buffer;

#ifdef __CATAMOUNT__
  retval = 0;
#else
  times(&buffer);
  retval = (long long)buffer.tms_utime*(long long)(1000000/sysconf(_SC_CLK_TCK));
#endif
  return(retval);
}

u_long_long vec_dummy_get_virt_cycles (void * zero)
{
  float usec, cyc;

  usec = (float)vec_dummy_get_virt_usec(zero);
  cyc = usec * _papi_hwi_system_info.hw_info.mhz;
  return((long long)cyc);
}

int vec_int_ok_dummy (){
  return PAPI_OK;
}

int vec_int_one_dummy (){
  return 1;
}

int vec_int_dummy (){
  return PAPI_ESBSTR;
}

void * vec_void_star_dummy(){
  return NULL;
}

void vec_void_dummy(){
  return;
}

long_long vec_long_long_dummy(){
  return PAPI_ESBSTR;
}

char * vec_char_star_dummy(){
  return NULL;
}

long vec_long_dummy(){
  return PAPI_ESBSTR;
}

int _papi_hwi_setup_vector_table(papi_vectors_t *table, papi_svector_t *stable)
{
  int i;
 
  if ( !table || !stable ) return (PAPI_EINVAL);

  for(i=0; (stable[i].func_type)!=VEC_PAPI_END; i++ ){
     if ( !stable[i].func ) continue;
     switch( stable[i].func_type ){
       case VEC_PAPI_HWD_READ:
        table->_vec_papi_hwd_read = (int (*) (void *, void *, long_long **, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_DISPATCH_TIMER:
        table->_vec_papi_hwd_dispatch_timer = (void (*) (int, void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_START:
        table->_vec_papi_hwd_start = (int (*) (void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_STOP:
        table->_vec_papi_hwd_stop = (int (*) (void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_GET_REAL_CYCLES:
        table->_vec_papi_hwd_get_real_cycles = (long_long (*) ()) stable[i].func;
         break;
       case VEC_PAPI_HWD_GET_REAL_USEC:
        table->_vec_papi_hwd_get_real_usec = (long_long (*) ()) stable[i].func;
         break;
       case VEC_PAPI_HWD_GET_VIRT_CYCLES:
        table->_vec_papi_hwd_get_virt_cycles = (long_long (*) ()) stable[i].func;
         break;
       case VEC_PAPI_HWD_GET_VIRT_USEC:
        table->_vec_papi_hwd_get_virt_usec = (long_long (*) ()) stable[i].func;
         break;
       case VEC_PAPI_HWD_RESET:
        table->_vec_papi_hwd_reset = (int (*) (void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_WRITE:
        table->_vec_papi_hwd_write = (int (*) (void *, void *, long_long[])) stable[i].func;
         break;
       case VEC_PAPI_HWD_STOP_PROFILING:
        table->_vec_papi_hwd_stop_profiling = (int (*) (ThreadInfo_t *, EventSetInfo_t *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_INIT:
        table->_vec_papi_hwd_init = (int (*) (void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_INIT_CONTROL_STATE:
        table->_vec_papi_hwd_init_control_state = (void (*)(void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_UPDATE_SHLIB_INFO:
        table->_vec_papi_hwd_update_shlib_info = (int (*) ()) stable[i].func;
         break;
       case VEC_PAPI_HWD_GET_MEMORY_INFO:
        table->_vec_papi_hwd_get_memory_info = (int (*) (PAPI_hw_info_t *, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_UPDATE_CONTROL_STATE:
        table->_vec_papi_hwd_update_control_state = (int (*) (void *, NativeInfo_t *, int, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_CTL:
        table->_vec_papi_hwd_ctl = (int (*) (void *, int, _papi_int_option_t *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_SET_OVERFLOW:
        table->_vec_papi_hwd_set_overflow = (int (*) (EventSetInfo_t *, int, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_SET_PROFILE:
        table->_vec_papi_hwd_set_profile = (int (*) (EventSetInfo_t *, int, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_SET_DOMAIN:
        table->_vec_papi_hwd_set_domain = (int (*) (void *, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_NTV_ENUM_EVENTS:
        table->_vec_papi_hwd_ntv_enum_events = (int (*) (unsigned int *, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_NTV_CODE_TO_NAME:
        table->_vec_papi_hwd_ntv_code_to_name = (char * (*) (unsigned int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_NTV_CODE_TO_DESCR:
        table->_vec_papi_hwd_ntv_code_to_descr = (char * (*) (unsigned int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_NTV_CODE_TO_BITS:
        table->_vec_papi_hwd_ntv_code_to_bits = (int (*) (unsigned int, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_NTV_BITS_TO_INFO:
        table->_vec_papi_hwd_ntv_bits_to_info = (int (*) (void *, char *, unsigned int *, int, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_ALLOCATE_REGISTERS:
        table->_vec_papi_hwd_allocate_registers = (int (*) (EventSetInfo_t *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_BPT_MAP_AVAIL:
        table->_vec_papi_hwd_bpt_map_avail = (int (*) (void *, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_BPT_MAP_SET:
        table->_vec_papi_hwd_bpt_map_set = (void (*) (void *, int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_BPT_MAP_EXCLUSIVE:
        table->_vec_papi_hwd_bpt_map_exclusive = (int (*) (void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_BPT_MAP_SHARED:
        table->_vec_papi_hwd_bpt_map_shared = (int (*) (void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_BPT_MAP_PREEMPT:
        table->_vec_papi_hwd_bpt_map_preempt = (void (*) (void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_BPT_MAP_UPDATE:
        table->_vec_papi_hwd_bpt_map_update = (void (*) (void *, void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_GET_DMEM_INFO:
        table->_vec_papi_hwd_get_dmem_info = (long (*) (int)) stable[i].func;
         break;
       case VEC_PAPI_HWD_SHUTDOWN:
        table->_vec_papi_hwd_shutdown = (int (*) (void *)) stable[i].func;
         break;
       case VEC_PAPI_HWD_SHUTDOWN_GLOBAL:
        table->_vec_papi_hwd_shutdown_global = (int (*) (void)) stable[i].func;
         break;
       case VEC_PAPI_HWD_USER:
        table->_vec_papi_hwd_user =(int(*)(int, void *, void *))stable[i].func;
         break;
       default:
         INTDBG("Invalid function type: %d in the vectortable->\n", stable[i].func_type);
         return(PAPI_EINVAL);
     }
  }
  return(PAPI_OK);
}

int _papi_hwi_initialize_vector_table(papi_vectors_t *table){
 if ( !table ) return (PAPI_EINVAL);
 table->_vec_papi_hwd_read = (int (*)(void *,void *, long_long **, int)) vec_int_dummy;
 table->_vec_papi_hwd_dispatch_timer = (void (*)(int, void *, void *)) vec_void_dummy;
 table->_vec_papi_hwd_start = (int (*) (void *, void *)) vec_int_dummy;
 table->_vec_papi_hwd_stop = (int (*) (void *, void *)) vec_int_dummy;
 table->_vec_papi_hwd_get_real_cycles = (long_long (*) ()) vec_dummy_get_real_cycles;
 table->_vec_papi_hwd_get_real_usec = (long_long (*) ()) vec_dummy_get_real_usec;
 table->_vec_papi_hwd_get_virt_cycles = (long_long (*) (void *)) vec_dummy_get_virt_cycles;
 table->_vec_papi_hwd_get_virt_usec = (long_long (*) (void *)) vec_dummy_get_virt_usec;
 table->_vec_papi_hwd_reset  = (int (*) (void *, void *)) vec_int_dummy;
 table->_vec_papi_hwd_write = (int (*) (void *, void *, long_long[])) vec_int_dummy;
 table->_vec_papi_hwd_stop_profiling = (int (*) (ThreadInfo_t *, EventSetInfo_t *)) vec_int_dummy;
 table->_vec_papi_hwd_init = (int (*) (void *)) vec_int_ok_dummy;
 table->_vec_papi_hwd_init_control_state = (void (*) (void *)) vec_void_dummy;
 table->_vec_papi_hwd_update_shlib_info = (int (*) (void)) vec_int_dummy;
 table->_vec_papi_hwd_get_system_info = (int (*) ()) vec_int_dummy;
 table->_vec_papi_hwd_get_memory_info = (int (*) (PAPI_hw_info_t *, int)) vec_int_dummy;
 table->_vec_papi_hwd_update_control_state = (int (*) (void *, NativeInfo_t *, int, void *)) vec_int_dummy;
 table->_vec_papi_hwd_ctl = (int (*) (void *, int, _papi_int_option_t *)) vec_int_dummy;
 table->_vec_papi_hwd_set_overflow = (int (*) (EventSetInfo_t *, int, int)) vec_int_dummy;
 table->_vec_papi_hwd_set_profile = (int (*) (EventSetInfo_t *, int, int)) vec_int_dummy;
 table->_vec_papi_hwd_set_domain = (int (*) (void *, int)) vec_int_dummy;
 table->_vec_papi_hwd_ntv_enum_events = (int (*) (unsigned int *, int)) vec_int_dummy;
 table->_vec_papi_hwd_ntv_code_to_name = (char * (*) (unsigned int)) vec_char_star_dummy;
 table->_vec_papi_hwd_ntv_code_to_descr = (char * (*) (unsigned int)) vec_char_star_dummy;
 table->_vec_papi_hwd_ntv_code_to_bits = (int (*) (unsigned int, void *)) vec_int_dummy;
 table->_vec_papi_hwd_ntv_bits_to_info = (int (*) (void *, char *, unsigned int *, int, int)) vec_int_dummy;
 table->_vec_papi_hwd_allocate_registers = (int (*) (EventSetInfo_t *)) vec_int_one_dummy;
 table->_vec_papi_hwd_bpt_map_avail = (int (*) (void *, int)) vec_int_dummy;
 table->_vec_papi_hwd_bpt_map_set = (void (*) (void *, int)) vec_void_dummy;
 table->_vec_papi_hwd_bpt_map_exclusive = (int (*) (void *)) vec_int_dummy;
 table->_vec_papi_hwd_bpt_map_shared = (int (*) (void *, void *)) vec_int_dummy;
 table->_vec_papi_hwd_bpt_map_preempt = (void (*) (void *, void *)) vec_void_dummy;
 table->_vec_papi_hwd_bpt_map_update = (void (*) (void *, void *)) vec_void_dummy;
 table->_vec_papi_hwd_get_dmem_info = (long (*) (int)) vec_long_dummy;
 table->_vec_papi_hwd_shutdown = (int (*) (void *)) vec_int_dummy;
 table->_vec_papi_hwd_shutdown_global = (int (*) (void)) vec_int_ok_dummy;
 table->_vec_papi_hwd_user = (int (*) (int, void *, void *)) vec_int_dummy;
  return PAPI_OK;
}

int PAPI_user(int func_num, void * input, void * output){
  return (_papi_vector_table[0]._vec_papi_hwd_user(func_num, input, output));
}

char * find_dummy(void * func, char **buf){
  void * ptr=NULL;

  if ( vec_int_ok_dummy == (int (*)())func){
    ptr = ( void *)vec_int_ok_dummy;
    *buf = papi_strdup("vec_int_ok_dummy");
  }
  else if ( vec_int_one_dummy == (int (*)())func ){
    ptr = ( void *)vec_int_one_dummy;
    *buf = papi_strdup("vec_int_one_dummy");
  }
  else if ( vec_int_dummy == (int (*)())func ){
    ptr = ( void *)vec_int_dummy;
    *buf = papi_strdup("vec_int_dummy");
  }
  else if ( vec_void_dummy == (void (*)())func ){
    ptr = (void *)vec_void_dummy;
    *buf = papi_strdup("vec_void_dummy");
  }
  else if ( vec_void_star_dummy == (void *(*)())func ){
    ptr = (void *)vec_void_star_dummy;
    *buf = papi_strdup("vec_void_star_dummy");
  }
  else if ( vec_long_long_dummy == (long_long (*)())func ){
    ptr = (void *)vec_long_long_dummy;
    *buf = papi_strdup("vec_long_long_dummy");
  }
  else if ( vec_char_star_dummy == (unsigned char*(*)())func ){
    ptr = (void *)vec_char_star_dummy;
    *buf = papi_strdup("vec_char_star_dummy");
  }
  else if ( vec_long_dummy == (long (*)())func ){
    ptr = (void *)vec_long_dummy;  
    *buf = papi_strdup("vec_long_dummy");
  }
  else if ( vec_dummy_get_real_usec == (unsigned long_long(*)())func ) {
    ptr = (void *)vec_dummy_get_real_usec;
    *buf = papi_strdup("vec_dummy_get_real_usec");
  }
  else if ( vec_dummy_get_real_cycles == (unsigned long_long(*)())func ) {
    ptr = (void *)vec_dummy_get_real_cycles;
    *buf = papi_strdup("vec_dummy_get_real_cycles");
  }
  else if ( vec_dummy_get_virt_usec == (unsigned long_long(*)())func ) {
    ptr = (void *)vec_dummy_get_virt_usec;
    *buf = papi_strdup("vec_dummy_get_virt_usec");
  }
  else if ( vec_dummy_get_virt_cycles == (unsigned long_long(*)())func ) {
    ptr = (void *)vec_dummy_get_virt_cycles;
    *buf = papi_strdup("vec_dummy_get_virt_cycles");
  }
  else {
    ptr = NULL;
  }
  return(ptr);
}

void vector_print_routine( void*func, char *fname, int pfunc){
  void * ptr=NULL;
  char  *buf=NULL;

  ptr = find_dummy(func, &buf);
  if ( ptr ){
    printf("%s: %s is mapped to %s.\n", (ptr?"DUMMY":"function"),fname, buf);
    papi_free(buf);
  }
  else if ( (!ptr && pfunc) )
    printf("%s: %s is mapped to %p.\n", (ptr?"DUMMY":"function"),fname, func);
}

void vector_print_table(papi_vectors_t *table, int print_func){

 if ( !table ) return;

 vector_print_routine(table->_vec_papi_hwd_read,"_papi_hwd_read",print_func);
 vector_print_routine(table->_vec_papi_hwd_dispatch_timer, "_papi_hwd_dispatch_timer",print_func);
 vector_print_routine(table->_vec_papi_hwd_start, "_papi_hwd_start",print_func);
 vector_print_routine(table->_vec_papi_hwd_stop, "_papi_hwd_stop",print_func);
 vector_print_routine(table->_vec_papi_hwd_get_real_cycles, "_papi_hwd_get_real_cycles",print_func);
 vector_print_routine(table->_vec_papi_hwd_get_real_usec, "_papi_hwd_get_real_usec",print_func);
 vector_print_routine(table->_vec_papi_hwd_get_virt_cycles, "_papi_hwd_get_virt_cycles",print_func);
 vector_print_routine(table->_vec_papi_hwd_get_virt_usec, "_papi_hwd_get_virt_usec",print_func);
 vector_print_routine(table->_vec_papi_hwd_reset, "_papi_hwd_reset",print_func);
 vector_print_routine(table->_vec_papi_hwd_write, "_papi_hwd_write",print_func);
 vector_print_routine(table->_vec_papi_hwd_stop_profiling, "_papi_hwd_stop_profiling",print_func);
 vector_print_routine(table->_vec_papi_hwd_init, "_papi_hwd_init",print_func);
 vector_print_routine(table->_vec_papi_hwd_init_control_state, "_papi_hwd_init_control_state",print_func);
 vector_print_routine(table->_vec_papi_hwd_ctl, "_papi_hwd_ctl",print_func);
 vector_print_routine(table->_vec_papi_hwd_set_overflow, "_papi_hwd_set_overflow",print_func);
 vector_print_routine(table->_vec_papi_hwd_set_profile, "_papi_hwd_set_profile",print_func);
 vector_print_routine(table->_vec_papi_hwd_set_domain, "_papi_hwd_set_domain",print_func);
 vector_print_routine(table->_vec_papi_hwd_ntv_enum_events, "_papi_hwd_ntv_enum_events",print_func);
 vector_print_routine(table->_vec_papi_hwd_ntv_code_to_name, "_papi_hwd_ntv_code_to_name",print_func);
 vector_print_routine(table->_vec_papi_hwd_ntv_code_to_descr, "_papi_hwd_ntv_code_to_descr",print_func);
 vector_print_routine(table->_vec_papi_hwd_ntv_code_to_bits, "_papi_hwd_ntv_code_to_bits",print_func);
 vector_print_routine(table->_vec_papi_hwd_ntv_bits_to_info, "_papi_hwd_ntv_bits_to_info",print_func);
 vector_print_routine(table->_vec_papi_hwd_allocate_registers, "_papi_hwd_allocate_registers",print_func);
 vector_print_routine(table->_vec_papi_hwd_bpt_map_avail, "_papi_hwd_bpt_map_avail",print_func);
 vector_print_routine(table->_vec_papi_hwd_bpt_map_set, "_papi_hwd_bpt_map_set",print_func);
 vector_print_routine(table->_vec_papi_hwd_bpt_map_exclusive, "_papi_hwd_bpt_map_exclusive",print_func);
 vector_print_routine(table->_vec_papi_hwd_bpt_map_shared, "_papi_hwd_bpt_shared",print_func);
 vector_print_routine(table->_vec_papi_hwd_bpt_map_update, "_papi_hwd_bpt_map_update",print_func);
 vector_print_routine(table->_vec_papi_hwd_get_dmem_info, "_papi_hwd_get_dmem_info",print_func);
 vector_print_routine(table->_vec_papi_hwd_shutdown, "_papi_hwd_shutdown",print_func);
 vector_print_routine(table->_vec_papi_hwd_shutdown_global, "_papi_hwd_shutdown_global",print_func);
 vector_print_routine(table->_vec_papi_hwd_user, "_papi_hwd_user",print_func);
}
#endif
