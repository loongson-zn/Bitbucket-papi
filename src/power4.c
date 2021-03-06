/* This substrate should never malloc anything. All allocation should be
   done by the high level API. */

#include "papi.h"
#include "papi_internal.h"
#include "papi_vector.h"

extern hwd_groups_t group_map[];

static hwi_search_t _papi_hwd_PWR4_preset_map[] = {
   {PAPI_L1_DCM, {DERIVED_ADD, {PNE_PM_LD_MISS_L1, PNE_PM_ST_MISS_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Level 1 data cache misses */
   {PAPI_L1_DCA, {DERIVED_ADD, {PNE_PM_LD_REF_L1, PNE_PM_ST_REF_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},        /*Level 1 data cache access */
   {PAPI_FXU_IDL, {0, {PNE_PM_FXU_IDLE, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Cycles integer units are idle */
   {PAPI_L1_LDM, {0, {PNE_PM_LD_MISS_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},     /*Level 1 load misses */
   {PAPI_L1_STM, {0, {PNE_PM_ST_MISS_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},     /*Level 1 store misses */
   {PAPI_L1_DCW, {0, {PNE_PM_ST_REF_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Level 1 D cache write */
   {PAPI_L1_DCR, {0, {PNE_PM_LD_REF_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Level 1 D cache read */
   {PAPI_FMA_INS, {0, {PNE_PM_FPU_FMA, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},       /*FMA instructions completed */
   {PAPI_TOT_IIS, {0, {PNE_PM_INST_DISP, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},     /*Total instructions issued */
   {PAPI_TOT_INS, {0, {PNE_PM_INST_CMPL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},     /*Total instructions executed */
   {PAPI_INT_INS, {0, {PNE_PM_FXU_FIN, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},       /*Integer instructions executed */
   {PAPI_FP_OPS, {DERIVED_POSTFIX, {PNE_PM_FPU0_FIN, PNE_PM_FPU1_FIN, PNE_PM_FPU_FMA, PNE_PM_FPU_STF, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, "N0|N1|+|N2|+|N3|-|"}},      /*Floating point instructions executed */
   {PAPI_FP_INS, {0, {PNE_PM_FPU_FIN, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Floating point instructions executed */
   {PAPI_TOT_CYC, {0, {PNE_PM_CYC, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},   /*Total cycles */
   {PAPI_FDV_INS, {0, {PNE_PM_FPU_FDIV, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*FD ins */
   {PAPI_FSQ_INS, {0, {PNE_PM_FPU_FSQRT, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},     /*FSq ins */
   {PAPI_TLB_DM, {0, {PNE_PM_DTLB_MISS, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Data translation lookaside buffer misses */
   {PAPI_TLB_IM, {0, {PNE_PM_ITLB_MISS, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Instr translation lookaside buffer misses */
   {PAPI_TLB_TL, {DERIVED_ADD, {PNE_PM_DTLB_MISS, PNE_PM_ITLB_MISS,PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},        /*Total translation lookaside buffer misses */
   {PAPI_HW_INT, {0, {PNE_PM_EXT_INT, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},        /*Hardware interrupts */
   {PAPI_STL_ICY, {0, {PNE_PM_0INST_FETCH, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},   /*Cycles with No Instruction Issue */
   {PAPI_LD_INS, {0, {PNE_PM_LD_REF_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Load instructions*/
   {PAPI_SR_INS, {0, {PNE_PM_ST_REF_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Store instructions*/
   {PAPI_LST_INS, {DERIVED_ADD, {PNE_PM_ST_REF_L1, PNE_PM_LD_REF_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},      /*Load and Store instructions*/
/* Start editing here */
   {PAPI_BR_INS, {0, {PNE_PM_BR_ISSUED, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},
   {PAPI_BR_MSP, {DERIVED_ADD, {PNE_PM_BR_MPRED_CR, PNE_PM_BR_MPRED_TA, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}},
   {PAPI_L1_DCH, {DERIVED_POSTFIX, {PNE_PM_LD_REF_L1, PNE_PM_LD_MISS_L1, PNE_PM_ST_REF_L1, PNE_PM_ST_MISS_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, "N0|N1|-|N2|+|N3|-|"}}, /* Level 1 data cache hits */
   /* no PAPI_L2_STM, PAPI_L2_DCW nor PAPI_L2_DCA since stores/writes to L2 aren't countable */
   {PAPI_L2_DCR, {DERIVED_ADD, {PNE_PM_DATA_FROM_L2, PNE_PM_DATA_FROM_L25_MOD, PNE_PM_DATA_FROM_L25_SHR, PNE_PM_DATA_FROM_L275_MOD, PNE_PM_DATA_FROM_L275_SHR, PNE_PM_DATA_FROM_L3, PNE_PM_DATA_FROM_L35, PNE_PM_DATA_FROM_MEM}, 0}}, /* Level 2 data cache reads */
   {PAPI_L2_DCH, {DERIVED_ADD, {PNE_PM_DATA_FROM_L2, PNE_PM_DATA_FROM_L25_MOD, PNE_PM_DATA_FROM_L25_SHR, PNE_PM_DATA_FROM_L275_MOD, PNE_PM_DATA_FROM_L275_SHR, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 2 data cache hits */
   {PAPI_L2_DCM, {DERIVED_ADD, {PNE_PM_DATA_FROM_L3, PNE_PM_DATA_FROM_L35, PNE_PM_DATA_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 2 data cache misses (reads & writes) */
   {PAPI_L2_LDM, {DERIVED_ADD, {PNE_PM_DATA_FROM_L3, PNE_PM_DATA_FROM_L35, PNE_PM_DATA_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 2 data cache read misses */
   /* no PAPI_L3_STM, PAPI_L3_DCW nor PAPI_L3_DCA since stores/writes to L3 aren't countable */
   {PAPI_L3_DCR, {DERIVED_ADD, {PNE_PM_DATA_FROM_L3, PNE_PM_DATA_FROM_L35, PNE_PM_DATA_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 data cache reads */
   {PAPI_L3_DCH, {DERIVED_ADD, {PNE_PM_DATA_FROM_L3, PNE_PM_DATA_FROM_L35, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 data cache hits */
   {PAPI_L3_DCM, {0, {PNE_PM_DATA_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 data cache misses (reads & writes) */
   {PAPI_L3_LDM, {0, {PNE_PM_DATA_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 data cache read misses */

   {PAPI_L1_ICA, {DERIVED_ADD, {PNE_PM_INST_FROM_L1, PNE_PM_INST_FROM_L2, PNE_PM_INST_FROM_L25_L275, PNE_PM_INST_FROM_L3, PNE_PM_INST_FROM_L35, PNE_PM_INST_FROM_MEM, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 1 inst cache accesses */
   {PAPI_L1_ICH, {0, {PNE_PM_INST_FROM_L1, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 1 inst cache hits */
   {PAPI_L1_ICM, {DERIVED_ADD, {PNE_PM_INST_FROM_L2, PNE_PM_INST_FROM_L25_L275, PNE_PM_INST_FROM_L3, PNE_PM_INST_FROM_L35, PNE_PM_INST_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 1 inst cache misses */

   {PAPI_L2_ICA, {DERIVED_ADD, {PNE_PM_INST_FROM_L2, PNE_PM_INST_FROM_L25_L275, PNE_PM_INST_FROM_L3, PNE_PM_INST_FROM_L35, PNE_PM_INST_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 2 inst cache accesses */
   {PAPI_L2_ICH, {DERIVED_ADD, {PNE_PM_INST_FROM_L2, PNE_PM_INST_FROM_L25_L275, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 2 inst cache hits */
   {PAPI_L2_ICM, {DERIVED_ADD, {PNE_PM_INST_FROM_L3, PNE_PM_INST_FROM_L35, PNE_PM_INST_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 2 inst cache misses */

   {PAPI_L3_ICA, {DERIVED_ADD, {PNE_PM_INST_FROM_L3, PNE_PM_INST_FROM_L35, PNE_PM_INST_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 inst cache accesses */
   {PAPI_L3_ICH, {DERIVED_ADD, {PNE_PM_INST_FROM_L3, PNE_PM_INST_FROM_L35, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 inst cache hits */
   {PAPI_L3_ICM, {0, {PNE_PM_INST_FROM_MEM, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}, /* Level 3 inst cache misses */
   
/* Stop editing here */
   {0, {0, {PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL, PAPI_NULL}, 0}}        /* end of list */
};
hwi_search_t *preset_search_map;

/* Reports the elements of the hwd_register_t struct as an array of names and a matching array of values.
   Maximum string length is name_len; Maximum number of values is count.
*/
static void copy_value(unsigned int val, char *nam, char *names, unsigned int *values, int len)
{
   *values = val;
   strncpy(names, nam, len);
   names[len-1] = '\0';
}

int _papi_hwd_ntv_bits_to_info(hwd_register_t *bits, char *names,
                               unsigned int *values, int name_len, int count)
{
   int i = 0;
   copy_value(bits->selector, "Power4 event code", &names[i*name_len], &values[i], name_len);
   if (++i == count) return(i);
   copy_value((unsigned int)bits->counter_cmd, "Power4 counter_cmd code", &names[i*name_len], &values[i], name_len);
   return(++i);
}


/*#define DEBUG_SETUP*/
/* the following bpt functions are empty functions in POWER4 */
/* This function examines the event to determine
    if it can be mapped to counter ctr. 
    Returns true if it can, false if it can't.
*/
int _papi_hwd_bpt_map_avail(hwd_reg_alloc_t * dst, int ctr)
{
}

/* This function forces the event to
    be mapped to only counter ctr. 
    Returns nothing.
*/
void _papi_hwd_bpt_map_set(hwd_reg_alloc_t * dst, int ctr)
{
}

/* This function examines the event to determine
    if it has a single exclusive mapping. 
    Returns true if exlusive, false if non-exclusive.
*/
int _papi_hwd_bpt_map_exclusive(hwd_reg_alloc_t * dst)
{
}

/* This function compares the dst and src events
    to determine if any counters are shared. Typically the src event
    is exclusive, so this detects a conflict if true.
    Returns true if conflict, false if no conflict.
*/
int _papi_hwd_bpt_map_shared(hwd_reg_alloc_t * dst, hwd_reg_alloc_t * src)
{
}

/* This function removes the counters available to the src event
    from the counters available to the dst event,
    and reduces the rank of the dst event accordingly. Typically,
    the src event will be exclusive, but the code shouldn't assume it.
    Returns nothing.
*/
void _papi_hwd_bpt_map_preempt(hwd_reg_alloc_t * dst, hwd_reg_alloc_t * src)
{
}

/* This function updates the selection status of 
    the dst event based on information in the src event.
    Returns nothing.
*/
void _papi_hwd_bpt_map_update(hwd_reg_alloc_t * dst, hwd_reg_alloc_t * src)
{
}

/* initialize preset_search_map table by type of CPU */
#ifdef PM_INITIALIZE
int _papi_hwd_init_preset_search_map(PMINFO_T * info)
#else
int _papi_hwd_init_preset_search_map(pm_info_t * info)
#endif
{
   preset_search_map = _papi_hwd_PWR4_preset_map;
   return 1;
}

/* this function recusively does Modified Bipartite Graph counter allocation 
     success  return 1
	 fail     return 0
*/
static int do_counter_allocation(PWR4_reg_alloc_t * event_list, int size)
{
   int i, j, group = -1;
   unsigned int map[GROUP_INTS];

   for (i = 0; i < GROUP_INTS; i++)
      map[i] = event_list[0].ra_group[i];

   for (i = 1; i < size; i++) {
      for (j = 0; j < GROUP_INTS; j++)
         map[j] &= event_list[i].ra_group[j];
   }

   for (i = 0; i < GROUP_INTS; i++) {
      if (map[i]) {
         group = ffs(map[i]) - 1 + i * 32;
         break;
      }
   }

   if (group < 0)
      return group;             /* allocation fail */
   else {
      for (i = 0; i < size; i++) {
         for (j = 0; j < MAX_COUNTERS; j++) {
            if (event_list[i].ra_counter_cmd[j] >= 0
                && event_list[i].ra_counter_cmd[j] == group_map[group].counter_cmd[j])
               event_list[i].ra_position = j;
         }
      }
      return group;
   }
}


/* this function will be called when there are counters available 
     success  return 1
	 fail     return 0
*/
int _papi_hwd_allocate_registers(EventSetInfo_t * ESI)
{
   hwd_control_state_t *this_state = &ESI->machdep;
   unsigned char selector;
   int i, j, natNum, index;
   PWR4_reg_alloc_t event_list[MAX_COUNTERS];
   int position, group;


   /* not yet successfully mapped, but have enough slots for events */

   /* Initialize the local structure needed 
      for counter allocation and optimization. */
   natNum = ESI->NativeCount;
   for (i = 0; i < natNum; i++) {
      /* CAUTION: Since this is in the hardware layer, it's ok 
         to access the native table directly, but in general this is a bad idea */
      event_list[i].ra_position = -1;
      /* calculate native event rank, which is number of counters it can live on, this is power3 specific */
      for (j = 0; j < MAX_COUNTERS; j++) {
         if ((index =
              native_name_map[ESI->NativeInfoArray[i].ni_event & PAPI_NATIVE_AND_MASK].index) <
             0)
            return 0;
         event_list[i].ra_counter_cmd[j] = native_table[index].resources.counter_cmd[j];
      }
      for (j = 0; j < GROUP_INTS; j++) {
         if ((index =
              native_name_map[ESI->NativeInfoArray[i].ni_event & PAPI_NATIVE_AND_MASK].index) <
             0)
            return 0;
         event_list[i].ra_group[j] = native_table[index].resources.group[j];
      }
      /*event_list[i].ra_mod = -1; */
   }

   if ((group = do_counter_allocation(event_list, natNum)) >= 0) {      /* successfully mapped */
      /* copy counter allocations info back into NativeInfoArray */
      this_state->group_id = group;
      for (i = 0; i < natNum; i++)
         ESI->NativeInfoArray[i].ni_position = event_list[i].ra_position;
      /* update the control structure based on the NativeInfoArray */
      /*_papi_hwd_update_control_state(this_state, ESI->NativeInfoArray, natNum);*/
      return 1;
   } else {
      return 0;
   }
}


/* This used to be init_config, static to the substrate.
   Now its exposed to the hwi layer and called when an EventSet is allocated.
*/
void _papi_hwd_init_control_state(hwd_control_state_t * ptr)
{
   int i;

   for (i = 0; i < _papi_hwi_system_info.num_cntrs; i++) {
      ptr->counter_cmd.events[i] = COUNT_NOTHING;
   }
   ptr->counter_cmd.mode.b.is_group = 1;

   set_domain(ptr, _papi_hwi_system_info.default_domain);
   set_granularity(ptr, _papi_hwi_system_info.default_granularity);
   /*setup_native_table();*/
}


/* This function updates the control structure with whatever resources are allocated
    for all the native events in the native info structure array. */
int _papi_hwd_update_control_state(hwd_control_state_t * this_state,
                                   NativeInfo_t * native, int count,
				   hwd_context_t *context)
{

   this_state->counter_cmd.events[0] = this_state->group_id;
   return PAPI_OK;
}

papi_svector_t _power4_mips_table[] = {
 { (void (*)())_papi_hwd_init_control_state, VEC_PAPI_HWD_INIT_CONTROL_STATE },
 { (void (*)())_papi_hwd_update_control_state, VEC_PAPI_HWD_UPDATE_CONTROL_STATE},
 { (void (*)())_papi_hwd_bpt_map_set, VEC_PAPI_HWD_BPT_MAP_SET },
 { (void (*)())_papi_hwd_bpt_map_avail, VEC_PAPI_HWD_BPT_MAP_AVAIL },
 { (void (*)())_papi_hwd_bpt_map_exclusive, VEC_PAPI_HWD_BPT_MAP_EXCLUSIVE },
 { (void (*)())_papi_hwd_bpt_map_shared, VEC_PAPI_HWD_BPT_MAP_SHARED },
 { (void (*)())_papi_hwd_bpt_map_preempt, VEC_PAPI_HWD_BPT_MAP_PREEMPT },
 { (void (*)())_papi_hwd_bpt_map_update, VEC_PAPI_HWD_BPT_MAP_UPDATE },
 { (void (*)())_papi_hwd_allocate_registers, VEC_PAPI_HWD_ALLOCATE_REGISTERS },
 { (void (*)())_papi_hwd_ntv_bits_to_info, VEC_PAPI_HWD_NTV_BITS_TO_INFO},
 { NULL, VEC_PAPI_END}
};
                                                                                
int power4_setup_vector_table(papi_vectors_t *vtable){
  int retval=PAPI_OK;
#ifndef PAPI_NO_VECTOR
  retval = _papi_hwi_setup_vector_table( vtable, _power4_mips_table);
#endif
  return(retval);
}

