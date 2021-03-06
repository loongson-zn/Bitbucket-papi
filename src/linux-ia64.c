/*
* File:    linux-ia64.c
* CVS:     $Id$
* Author:  Philip Mucci
*          mucci@cs.utk.edu
* Mods:	   Kevin London
*	   london@cs.utk.edu
*          Per Ekman
*          pek@pdc.kth.se
*          Zhou Min
*          min@cs.utk.edu
*/


#define IN_SUBSTRATE

#include "papi.h"
#include "papi_internal.h"
#include "linux-ia64.h"
#include "papi_protos.h"
#include "papi_vector.h"
#include "pfmwrap.h"
#include "threads.h"
#include "papi_memory.h"

static int sidx;

#ifndef ITANIUM2
static itanium_preset_search_t ia1_preset_search_map[] = {
   {PAPI_L1_TCM, DERIVED_ADD, {"L1D_READ_MISSES_RETIRED", "L2_INST_DEMAND_READS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_ICM, 0, {"L2_INST_DEMAND_READS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCM, 0, {"L1D_READ_MISSES_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_TCM, 0, {"L2_MISSES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCM, DERIVED_SUB, {"L2_MISSES", "L3_READS_INST_READS_ALL", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_ICM, 0, {"L3_READS_INST_READS_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_TCM, 0, {"L3_MISSES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICM, 0, {"L3_READS_INST_READS_MISS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCM, DERIVED_ADD,
    {"L3_READS_DATA_READS_MISS", "L3_WRITES_DATA_WRITES_MISS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_LDM, 0, {"L3_READS_DATA_READS_MISS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_STM, 0, {"L3_WRITES_DATA_WRITES_MISS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_LDM, 0, {"L1D_READ_MISSES_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_LDM, 0, {"L3_READS_DATA_READS_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_STM, 0, {"L3_WRITES_ALL_WRITES_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCH, DERIVED_ADD,
    {"L3_READS_DATA_READS_HIT", "L3_WRITES_DATA_WRITES_HIT", 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCH, DERIVED_SUB, {"L1D_READS_RETIRED", "L1D_READ_MISSES_RETIRED", 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCA, 0, {"L1D_READS_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCA, 0, {"L2_DATA_REFERENCES_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCA, DERIVED_ADD,
    {"L3_READS_DATA_READS_ALL", "L3_WRITES_DATA_WRITES_ALL", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCR, 0, {"L2_DATA_REFERENCES_READS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCR, 0, {"L3_READS_DATA_READS_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCW, 0, {"L2_DATA_REFERENCES_WRITES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCW, 0, {"L3_WRITES_DATA_WRITES_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICH, 0, {"L3_READS_INST_READS_HIT", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_ICR, DERIVED_ADD, {"L1I_PREFETCH_READS", "L1I_DEMAND_READS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_ICR, DERIVED_ADD, {"L2_INST_DEMAND_READS", "L2_INST_PREFETCH_READS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICR, 0, {"L3_READS_INST_READS_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_TLB_DM, 0, {"DTLB_MISSES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_TLB_IM, 0, {"ITLB_MISSES_FETCH", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_MEM_SCY, 0, {"MEMORY_CYCLE", PAPI_NULL, 0, 0, 0, 0, 0, 0}},
   {PAPI_STL_ICY, 0, {"UNSTALLED_BACKEND_CYCLE", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_BR_INS, 0, {"BRANCH_EVENT", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_BR_PRC, 0, {"BRANCH_PREDICTOR_ALL_CORRECT_PREDICTIONS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_BR_MSP, DERIVED_ADD,
    {"BRANCH_PREDICTOR_ALL_WRONG_PATH", "BRANCH_PREDICTOR_ALL_WRONG_TARGET", 0, 0, 0, 0, 0, 0}},
   {PAPI_TOT_CYC, 0, {"CPU_CYCLES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_FP_OPS, DERIVED_ADD, {"FP_OPS_RETIRED_HI", "FP_OPS_RETIRED_LO", 0, 0, 0, 0, 0, 0}},
   {PAPI_TOT_INS, 0, {"IA64_INST_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_LD_INS, 0, {"LOADS_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_SR_INS, 0, {"STORES_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_LST_INS, DERIVED_ADD, {"LOADS_RETIRED", "STORES_RETIRED", 0, 0, 0, 0, 0, 0}},
   {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}}
};
#define ia_preset_search_map ia1_preset_search_map
#else
static itanium_preset_search_t ia2_preset_search_map[] = {
   {PAPI_CA_SNP, 0, {"BUS_SNOOPS_SELF", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_CA_INV, DERIVED_ADD, {"BUS_MEM_READ_BRIL_SELF", "BUS_MEM_READ_BIL_SELF", 0, 0, 0, 0, 0, 0}},
   {PAPI_TLB_TL, DERIVED_ADD, {"ITLB_MISSES_FETCH_L2ITLB", "L2DTLB_MISSES", 0, 0, 0, 0, 0, 0}},
   {PAPI_STL_ICY, 0, {"DISP_STALLED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_STL_CCY, 0, {"BACK_END_BUBBLE_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_TOT_IIS, 0, {"INST_DISPERSED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_RES_STL, 0, {"BE_EXE_BUBBLE_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_FP_STAL, 0, {"BE_EXE_BUBBLE_FRALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_TCR, DERIVED_ADD,
    {"L2_DATA_REFERENCES_L2_DATA_READS", "L2_INST_DEMAND_READS", "L2_INST_PREFETCHES",
     0, 0, 0, 0, 0}},
   {PAPI_L1_TCM, DERIVED_ADD, {"L2_INST_DEMAND_READS", "L1D_READ_MISSES_ALL", 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_ICM, 0, {"L2_INST_DEMAND_READS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCM, 0, {"L1D_READ_MISSES_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_TCM, 0, {"L2_MISSES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCM, DERIVED_SUB, {"L2_MISSES", "L3_READS_INST_FETCH_ALL", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_ICM, 0, {"L3_READS_INST_FETCH_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_TCM, 0, {"L3_MISSES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICM, 0, {"L3_READS_INST_FETCH_MISS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCM, DERIVED_ADD,
    {"L3_READS_DATA_READ_MISS", "L3_WRITES_DATA_WRITE_MISS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_LDM, 0, {"L3_READS_ALL_MISS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_STM, 0, {"L3_WRITES_DATA_WRITE_MISS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_LDM, DERIVED_ADD, {"L1D_READ_MISSES_ALL", "L2_INST_DEMAND_READS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_LDM, 0, {"L3_READS_ALL_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_STM, 0, {"L3_WRITES_ALL_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCH, DERIVED_SUB, {"L1D_READS_SET1", "L1D_READ_MISSES_ALL", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCH, DERIVED_SUB, {"L2_DATA_REFERENCES_L2_ALL", "L2_MISSES", 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCH, DERIVED_ADD,
    {"L3_READS_DATA_READ_HIT", "L3_WRITES_DATA_WRITE_HIT", 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCA, 0, {"L1D_READS_SET1", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCA, 0, {"L2_DATA_REFERENCES_L2_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCA, 0, {"L3_REFERENCES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_DCR, 0, {"L1D_READS_SET1", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCR, 0, {"L2_DATA_REFERENCES_L2_DATA_READS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCR, 0, {"L3_READS_DATA_READ_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_DCW, 0, {"L2_DATA_REFERENCES_L2_DATA_WRITES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_DCW, 0, {"L3_WRITES_DATA_WRITE_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICH, 0, {"L3_READS_DINST_FETCH_HIT", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_ICR, DERIVED_ADD, {"L1I_PREFETCHES", "L1I_READS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_ICR, DERIVED_ADD, {"L2_INST_DEMAND_READS", "L2_INST_PREFETCHES", 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICR, 0, {"L3_READS_INST_FETCH_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_ICA, DERIVED_ADD, {"L1I_PREFETCHES", "L1I_READS", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_TCH, DERIVED_SUB, {"L2_REFERENCES", "L2_MISSES", 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_TCH, DERIVED_SUB, {"L3_REFERENCES", "L3_MISSES", 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_TCA, 0, {"L2_REFERENCES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_TCA, 0, {"L3_REFERENCES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_TCR, 0, {"L3_READS_ALL_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_TCW, 0, {"L3_WRITES_ALL_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_TLB_DM, 0, {"L2DTLB_MISSES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_TLB_IM, 0, {"ITLB_MISSES_FETCH_L2ITLB", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_BR_INS, 0, {"BRANCH_EVENT", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_BR_PRC, 0, {"BR_MISPRED_DETAIL_ALL_CORRECT_PRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_BR_MSP, DERIVED_ADD,
    {"BR_MISPRED_DETAIL_ALL_WRONG_PATH", "BR_MISPRED_DETAIL_ALL_WRONG_TARGET", 0, 0, 0, 0, 0, 0}},
   {PAPI_TOT_CYC, 0, {"CPU_CYCLES", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_FP_OPS, 0, {"FP_OPS_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_TOT_INS, DERIVED_ADD, {"IA64_INST_RETIRED", "IA32_INST_RETIRED", 0, 0, 0, 0, 0, 0}},
   {PAPI_LD_INS, 0, {"LOADS_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_SR_INS, 0, {"STORES_RETIRED", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L2_ICA, 0, {"L2_INST_DEMAND_READS", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L3_ICA, 0, {"L3_READS_INST_FETCH_ALL", 0, 0, 0, 0, 0, 0, 0}},
   {PAPI_L1_TCR, DERIVED_ADD, {"L1D_READS_SET0", "L1I_READS", 0, 0, 0, 0, 0, 0}}, 
   {PAPI_L1_TCA, DERIVED_ADD, {"L1D_READS_SET0", "L1I_READS", 0, 0, 0, 0, 0, 0}}, 
   {PAPI_L2_TCW, 0, {"L2_DATA_REFERENCES_L2_DATA_WRITES", 0, 0, 0, 0, 0, 0, 0}},

   {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}}
};
#define ia_preset_search_map ia2_preset_search_map
#endif

#define NUM_OF_PRESET_EVENTS (sizeof(ia_preset_search_map)/sizeof(itanium_preset_search_t)-1)
hwi_search_t ia_preset_search_map_bycode[NUM_OF_PRESET_EVENTS + 1];
hwi_search_t *preset_search_map = ia_preset_search_map_bycode;

#ifdef ALTIX
static unsigned long mmdev_clicks_per_tick;
static volatile unsigned long *mmdev_timer_addr;
#endif

/* Machine info structure. -1 is unused. */
extern papi_mdi_t _papi_hwi_system_info;
extern hwi_preset_data_t _papi_hwi_preset_data[PAPI_MAX_PRESET_EVENTS];

extern void dispatch_profile(EventSetInfo_t * ESI, void *context,
                             long_long over, int profile_index);

/* This substrate should never malloc anything. All allocation should be
   done by the high level API. */

/* The values defined in this file may be X86-specific (2 general 
   purpose counters, 1 special purpose counter, etc.*/

/* PAPI stuff */

/* Low level functions, should not handle errors, just return codes. */

/* I want to keep the old way to define the preset search map.
   In Itanium2, there are more than 400 native events, if I use the
   index directly, it will be difficult for people to debug, so I
   still keep the old way to define preset search table, but 
   I add this function to generate the preset search map in papi3 
*/
int generate_preset_search_map(itanium_preset_search_t * oldmap)
{
   int pnum, i, cnt;
   char **findme;

   pnum = 0;                    /* preset event counter */
   memset(ia_preset_search_map_bycode, 0x0, sizeof(ia_preset_search_map_bycode));
   for (i = 0; i <= PAPI_MAX_PRESET_EVENTS; i++) {
      if (oldmap[i].preset == 0)
         break;
      pnum++;
      preset_search_map[i].event_code = oldmap[i].preset;
      preset_search_map[i].data.derived = oldmap[i].derived;
      strcpy(preset_search_map[i].data.operation,oldmap[i].operation);
      findme = oldmap[i].findme;
      cnt = 0;
      while (*findme != NULL) {
         if (cnt == MAX_COUNTER_TERMS){
	    PAPIERROR("Count (%d) == MAX_COUNTER_TERMS (%d)\n",cnt,MAX_COUNTER_TERMS);
            return(PAPI_ENOEVNT);
         }
         if (pfm_find_event_byname(*findme, (unsigned int *)&preset_search_map[i].data.native[cnt]) !=
             PFMLIB_SUCCESS)
            return (PAPI_ENOEVNT);
         else
            preset_search_map[i].data.native[cnt] ^= PAPI_NATIVE_MASK;
         findme++;
         cnt++;
      }
      preset_search_map[i].data.native[cnt] = PAPI_NULL;
   }
   if (NUM_OF_PRESET_EVENTS != pnum){
     PAPIERROR("NUM_OF_PRESET_EVENTS %d != pnum %d\n", (int)NUM_OF_PRESET_EVENTS,pnum);
     return(PAPI_ENOEVNT);
   }
   return (PAPI_OK);
}


static inline char *search_cpu_info(FILE * f, char *search_str, char *line)
{
   /* This code courtesy of our friends in Germany. Thanks Rudolph Berrendorf! */
   /* See the PCL home page for the German version of PAPI. */

   char *s;

   while (fgets(line, 256, f) != NULL) {
      if (strstr(line, search_str) != NULL) {
         /* ignore all characters in line up to : */
         for (s = line; *s && (*s != ':'); ++s);
         if (*s)
            return (s);
      }
   }
   return (NULL);

   /* End stolen code */
}

inline_static unsigned long get_cycles(void)
{
   unsigned long tmp;
#ifdef ALTIX
   tmp = mmdev_clicks_per_tick * (*mmdev_timer_addr);
#elif defined(__INTEL_COMPILER)
   tmp = __getReg(_IA64_REG_AR_ITC);
#else                           /* GCC */
   /* XXX: need more to adjust for Itanium itc bug */
   __asm__ __volatile__("mov %0=ar.itc":"=r"(tmp)::"memory");
#endif
   return tmp;
}

inline static int set_domain(hwd_control_state_t * this_state, int domain)
{
   int mode = 0, did = 0, i;
   pfmw_param_t *evt = &this_state->evt;

   if (domain & PAPI_DOM_USER) {
      did = 1;
      mode |= PFM_PLM3;
   }
   if (domain & PAPI_DOM_KERNEL) {
      did = 1;
      mode |= PFM_PLM0;
   }

   if (domain == PAPI_DOM_OTHER) 
      return (PAPI_ESBSTR);

   if (!did)
      return (PAPI_EINVAL);

   PFMW_PEVT_DFLPLM(evt) = mode;

   /* Bug fix in case we don't call pfmw_dispatch_events after this code */
   for (i = 0; i < MAX_COUNTERS; i++) {
      if (PFMW_PEVT_PFPPC_REG_NUM(evt,i)) {
         pfmw_arch_pmc_reg_t value;
         SUBDBG("slot %d, register %lud active, config value 0x%lx\n",
                i, (unsigned long) (PFMW_PEVT_PFPPC_REG_NUM(evt,i)),
                PFMW_PEVT_PFPPC_REG_VAL(evt,i));

         PFMW_ARCH_REG_PMCVAL(value) = PFMW_PEVT_PFPPC_REG_VAL(evt,i);
         PFMW_ARCH_REG_PMCPLM(value) = mode;
         PFMW_PEVT_PFPPC_REG_VAL(evt,i) = PFMW_ARCH_REG_PMCVAL(value);

         SUBDBG("new config value 0x%lx\n", PFMW_PEVT_PFPPC_REG_VAL(evt,i));
      }
   }

   return (PAPI_OK);
}

inline static int sub_mdi_init(int idx) 
{
  /* Name of the substrate we're using */
  strcpy(_papi_hwi_substrate_info[idx].substrate, "$Id$");          
  
  _papi_hwi_substrate_info[idx].num_cntrs = MAX_COUNTERS;
  _papi_hwi_substrate_info[idx].num_gp_cntrs = MAX_COUNTERS;
  _papi_hwi_substrate_info[idx].supports_hw_overflow = 1;
  _papi_hwi_substrate_info[idx].supports_hw_profile = 0;
  _papi_hwi_substrate_info[idx].supports_64bit_counters = 1;
  _papi_hwi_substrate_info[idx].supports_inheritance = 1;
  _papi_hwi_substrate_info[idx].context_size  = sizeof(hwd_context_t);
  _papi_hwi_substrate_info[idx].register_size = sizeof(hwd_register_t);
  _papi_hwi_substrate_info[idx].reg_alloc_size = sizeof(hwd_reg_alloc_t);
  _papi_hwi_substrate_info[idx].control_state_size =sizeof(hwd_control_state_t);
  _papi_hwi_system_info.supports_real_usec = 1;
  _papi_hwi_system_info.supports_real_cyc = 1;
 
  return(PAPI_OK);
}

inline static int set_granularity(hwd_control_state_t * this_state, int domain)
{
   switch (domain) {
   case PAPI_GRN_PROCG:
   case PAPI_GRN_SYS:
   case PAPI_GRN_SYS_CPU:
   case PAPI_GRN_PROC:
      return(PAPI_ESBSTR);
   case PAPI_GRN_THR:
      break;
   default:
      return (PAPI_EINVAL);
   }
   return (PAPI_OK);
}

/* This function should tell your kernel extension that your children
   inherit performance register information and propagate the values up
   upon child exit and parent wait. */

inline static int set_inherit(int arg)
{
   return (PAPI_ESBSTR);
}

inline static int set_default_domain(hwd_control_state_t * this_state, int domain)
{
   return (set_domain(this_state, domain));
}

inline static int set_default_granularity(hwd_control_state_t * this_state,
                                          int granularity)
{
   return (set_granularity(this_state, granularity));
}


volatile unsigned int lock[PAPI_MAX_LOCK];

static void lock_init(void)
{
   int i;
   for (i = 0; i < PAPI_MAX_LOCK; i++)
      lock[i] = MUTEX_OPEN;
}

/* this function is called by PAPI_library_init */
papi_svector_t _linux_ia64_table[] = {
 {(void (*)())_papi_hwd_update_shlib_info, VEC_PAPI_HWD_UPDATE_SHLIB_INFO},
 {(void (*)())_papi_hwd_init, VEC_PAPI_HWD_INIT},
 {(void (*)())_papi_hwd_init_control_state, VEC_PAPI_HWD_INIT_CONTROL_STATE},
 {(void (*)())_papi_hwd_dispatch_timer, VEC_PAPI_HWD_DISPATCH_TIMER},
 {(void (*)())_papi_hwd_ctl, VEC_PAPI_HWD_CTL},
 {(void (*)())_papi_hwd_get_real_usec, VEC_PAPI_HWD_GET_REAL_USEC},
 {(void (*)())_papi_hwd_get_real_cycles, VEC_PAPI_HWD_GET_REAL_CYCLES},
 {(void (*)())_papi_hwd_get_virt_cycles, VEC_PAPI_HWD_GET_VIRT_CYCLES},
 {(void (*)())_papi_hwd_get_virt_usec, VEC_PAPI_HWD_GET_VIRT_USEC},
 {(void (*)())_papi_hwd_update_control_state,VEC_PAPI_HWD_UPDATE_CONTROL_STATE}, {(void (*)())_papi_hwd_start, VEC_PAPI_HWD_START },
 {(void (*)())_papi_hwd_stop, VEC_PAPI_HWD_STOP },
 {(void (*)())_papi_hwd_read, VEC_PAPI_HWD_READ },
 {(void (*)())_papi_hwd_shutdown, VEC_PAPI_HWD_SHUTDOWN },
 {(void (*)())_papi_hwd_reset, VEC_PAPI_HWD_RESET},
 {(void (*)())_papi_hwd_set_profile, VEC_PAPI_HWD_SET_PROFILE},
 {(void (*)())_papi_hwd_get_dmem_info, VEC_PAPI_HWD_GET_DMEM_INFO},
 {(void (*)())_papi_hwd_set_overflow, VEC_PAPI_HWD_SET_OVERFLOW},
 {(void (*)())_papi_hwd_ntv_enum_events, VEC_PAPI_HWD_NTV_ENUM_EVENTS},
 {(void (*)())_papi_hwd_ntv_code_to_name, VEC_PAPI_HWD_NTV_CODE_TO_NAME},
 {(void (*)())_papi_hwd_ntv_code_to_descr, VEC_PAPI_HWD_NTV_CODE_TO_DESCR},
 {NULL, VEC_PAPI_END}
};

int _papi_hwd_init_substrate(papi_vectors_t *vtable, int idx)
{
   int retval, type;
   unsigned int version;
   pfmlib_options_t pfmlib_options;

   sidx = idx;

  /* Setup the vector entries that the OS knows about */
#ifndef PAPI_NO_VECTOR
  retval = _papi_hwi_setup_vector_table( vtable, _linux_ia64_table);
  if ( retval != PAPI_OK ) return(retval);
#endif

   /* Opened once for all threads. */
   if (pfm_initialize() != PFMLIB_SUCCESS)
      return (PAPI_ESYS);

   if (pfm_get_pmu_type(&type) != PFMLIB_SUCCESS)
      return (PAPI_ESYS);

#ifdef ITANIUM2
   if (type != PFMLIB_ITANIUM2_PMU) {
      PAPIERROR("Intel Itanium I is not supported by this substrate");
      return (PAPI_ESBSTR);
   }
#else
   if (type != PFMLIB_ITANIUM_PMU) {
      PAPIERROR("Intel Itanium II is not supported by this substrate");
      return (PAPI_ESBSTR);
   }
#endif

   if (pfm_get_version(&version) != PFMLIB_SUCCESS)
      return (PAPI_ESBSTR);

   if (PFM_VERSION_MAJOR(version) != PFM_VERSION_MAJOR(PFMLIB_VERSION)) {
      PAPIERROR("Version mismatch of libpfm: compiled %x vs. installed %x",
              PFM_VERSION_MAJOR(PFMLIB_VERSION), PFM_VERSION_MAJOR(version));
      return (PAPI_ESBSTR);
   }

   memset(&pfmlib_options, 0, sizeof(pfmlib_options));
#ifdef DEBUG
   if (ISLEVEL(DEBUG_SUBSTRATE)) {
      pfmlib_options.pfm_debug = 1;
      pfmlib_options.pfm_verbose = 1;
   }
#endif

   if (pfm_set_options(&pfmlib_options))
      return (PAPI_ESYS);

   /* Initialize outstanding values in machine info structure */
   if (sub_mdi_init(idx) != PAPI_OK) {
      return (PAPI_ESBSTR);
   }

   /* Fill in what we can of the papi_system_info. */
   retval = _papi_hwd_get_system_info();
   if (retval)
      return (retval);

#if defined(ALTIX)
   {
     int fd;
     unsigned long femtosecs_per_tick = 0;
     int offset;
     
     if((fd = open(MMTIMER_FULLNAME, O_RDONLY)) == -1) {
       PAPIERROR("Failed to open MM timer");
       return(PAPI_ESBSTR);
     }
     if ((offset = ioctl(fd, MMTIMER_GETOFFSET, 0)) == -ENOSYS) {
       PAPIERROR("Failed to get offset of MM timer");
       return(PAPI_ESBSTR);
     }
     if ((mmdev_timer_addr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fd, 0)) == NULL) {
       PAPIERROR("Failed to mmap MM timer");
       return(PAPI_ESBSTR);
     }

     mmdev_timer_addr += offset;
     ioctl(fd, MMTIMER_GETRES, &femtosecs_per_tick);
     mmdev_clicks_per_tick = (_papi_hwi_system_info.hw_info.mhz * 1.0e-9) * femtosecs_per_tick;
     close(fd);
   }
#endif

   _papi_hwi_system_info.hw_info.vendor = PAPI_VENDOR_INTEL;

   /* Setup presets */

   retval = generate_preset_search_map((itanium_preset_search_t *)ia_preset_search_map);
   if (retval)
      return (retval);

   retval = _papi_hwi_setup_all_presets(preset_search_map, NULL, idx);
   if (retval)
      return (retval);

   /* get_memory_info has a CPU model argument that is not used,
    * fakining it here with hw_info.model which is not set by this
    * substrate 
    */
   retval = _papi_hwd_get_memory_info(&_papi_hwi_system_info.hw_info,
                            _papi_hwi_system_info.hw_info.model);
   if (retval)
      return (retval);

   lock_init();
   
   return (PAPI_OK);
}

static int _papi_hwd_init(hwd_context_t * zero)
{
  return(pfmw_create_context(zero));
}

static long_long _papi_hwd_get_real_usec(void) {
   return((long_long)get_cycles() / (long_long)_papi_hwi_system_info.hw_info.mhz);
}
                                                                                
static
long_long _papi_hwd_get_real_cycles(void) {
   return((long_long)get_cycles());
}

static long_long _papi_hwd_get_virt_usec(hwd_context_t * zero)
{
   long_long retval;
   struct tms buffer;

   times(&buffer);
   SUBDBG("user %d system %d\n",(int)buffer.tms_utime,(int)buffer.tms_stime);
   retval = (long_long)((buffer.tms_utime+buffer.tms_stime)*
     (1000000/sysconf(_SC_CLK_TCK)));
   /* NOT CLOCKS_PER_SEC as in the headers! */
   return (retval);
}

static long_long _papi_hwd_get_virt_cycles(hwd_context_t * zero)
{
   return (_papi_hwd_get_virt_usec(zero) * (long_long)_papi_hwi_system_info.hw_info.mhz);
}

/* reset the hardware counters */
static int _papi_hwd_reset(hwd_context_t * ctx, hwd_control_state_t * machdep)
{
   pfarg_reg_t writeem[MAX_COUNTERS];
   int i;

   pfmw_stop(ctx);
   memset(writeem, 0, sizeof writeem);
   for (i = 0; i < MAX_COUNTERS; i++) {
      /* Writing doesn't matter, we're just zeroing the counter. */
      writeem[i].reg_num = MAX_COUNTERS + i;
   }
   if (pfmw_perfmonctl(ctx->tid, ctx->fd, PFM_WRITE_PMDS, writeem, MAX_COUNTERS) == -1) {
      PAPIERROR("perfmonctl(PFM_WRITE_PMDS) errno %d", errno);
      return PAPI_ESYS;
   }
   pfmw_start(ctx);
   return (PAPI_OK);
}

static int _papi_hwd_read(hwd_context_t * ctx, hwd_control_state_t * machdep,
                   long_long ** events, int flags)
{
   int i;
   pfarg_reg_t readem[MAX_COUNTERS];

   memset(readem, 0x0, sizeof readem);

/* read the 4 counters, the high level function will process the 
   mapping for papi event to hardware counter 
*/
   for (i = 0; i < MAX_COUNTERS; i++) {
      readem[i].reg_num = MAX_COUNTERS + i;
   }

   if (pfmw_perfmonctl(ctx->tid, ctx->fd, PFM_READ_PMDS, readem, MAX_COUNTERS) == -1) {
      SUBDBG("perfmonctl error READ_PMDS errno %d\n", errno);
      return PAPI_ESYS;
   }

   for (i = 0; i < _papi_hwi_substrate_info[sidx].num_cntrs; i++) {
      machdep->counters[i] = readem[i].reg_value;
      SUBDBG("read counters is %ld\n", readem[i].reg_value);
   }

#ifdef ITANIUM
   pfmw_param_t *pevt= &(machdep->evt);
   pfmw_arch_pmc_reg_t flop_hack;
   /* special case, We need to scale FP_OPS_HI */
   for (i = 0; i < PFMW_PEVT_EVTCOUNT(pevt); i++) {
      PFMW_ARCH_REG_PMCVAL(flop_hack) = 
                      PFMW_PEVT_PFPPC_REG_VAL(pevt,i);
      if (PFMW_ARCH_REG_PMCES(flop_hack) == 0xa)
         machdep->counters[i] *= 4;
   }
#endif

   *events = machdep->counters;
   return PAPI_OK;
}


static int _papi_hwd_start(hwd_context_t * ctx, hwd_control_state_t * current_state)
{
   int i;
   pfmw_param_t *pevt = &(current_state->evt);

   pfmw_stop(ctx);

/* write PMCS */
   if (pfmw_perfmonctl(ctx->tid, ctx->fd, PFM_WRITE_PMCS,
        PFMW_PEVT_PFPPC(pevt), 
        PFMW_PEVT_PFPPC_COUNT(pevt)) == -1) {
      PAPIERROR("perfmonctl(PFM_WRITE_PMCS) errno %d", errno);
      return (PAPI_ESYS);
   }

/* set the initial value of the hardware counter , if PAPI_overflow or
  PAPI_profil are called, then the initial value is the threshold
*/
   for (i = 0; i < MAX_COUNTERS; i++)
      current_state->pd[i].reg_num = MAX_COUNTERS + i;

   if (pfmw_perfmonctl(ctx->tid, ctx->fd, 
           PFM_WRITE_PMDS, current_state->pd, MAX_COUNTERS) == -1) {
      PAPIERROR("perfmonctl(WRITE_PMDS) errno %d", errno);
      return (PAPI_ESYS);
   }

   pfmw_start(ctx);

   return PAPI_OK;
}

static int _papi_hwd_stop(hwd_context_t * ctx, hwd_control_state_t * zero)
{
   pfmw_stop(ctx);
   return PAPI_OK;
}

static int _papi_hwd_ctl(hwd_context_t * zero, int code, _papi_int_option_t * option)
{
   switch (code) {
   case PAPI_DEFDOM:
      return (set_default_domain(option->domain.ESI->machdep, option->domain.domain));
   case PAPI_DOMAIN:
      return (set_domain(option->domain.ESI->machdep, option->domain.domain));
   case PAPI_DEFGRN:
      return (set_default_granularity
              (option->domain.ESI->machdep, option->granularity.granularity));
   case PAPI_GRANUL:
      return (set_granularity
              (option->granularity.ESI->machdep, option->granularity.granularity));
   default:
      return (PAPI_EINVAL);
   }
}

static int _papi_hwd_shutdown(hwd_context_t * ctx)
{
   return (pfmw_destroy_context(ctx));
}

#ifdef PFM20
/* This function set the parameters which needed by DATA EAR */
int set_dear_ita_param(pfmw_ita_param_t * ita_lib_param, int EventCode)
{
#ifdef ITANIUM2
   ita_lib_param->pfp_magic = PFMLIB_ITA2_PARAM_MAGIC;
   ita_lib_param->pfp_ita2_dear.ear_used = 1;
   pfm_ita2_get_ear_mode(EventCode, &ita_lib_param->pfp_ita2_dear.ear_mode);
   ita_lib_param->pfp_ita2_dear.ear_plm = PFM_PLM3;
   ita_lib_param->pfp_ita2_dear.ear_ism = PFMLIB_ITA2_ISM_IA64; /* ia64 only */
   pfm_ita2_get_event_umask(EventCode, &ita_lib_param->pfp_ita2_dear.ear_umask);
#else
   ita_lib_param->pfp_magic = PFMLIB_ITA_PARAM_MAGIC;
   ita_lib_param->pfp_ita_dear.ear_used = 1;
   ita_lib_param->pfp_ita_dear.ear_is_tlb = pfm_ita_is_dear_tlb(EventCode);
   ita_lib_param->pfp_ita_dear.ear_plm = PFM_PLM3;
   ita_lib_param->pfp_ita_dear.ear_ism = PFMLIB_ITA_ISM_IA64;   /* ia64 only */
   pfm_ita_get_event_umask(EventCode, &ita_lib_param->pfp_ita_dear.ear_umask);
#endif
   return PAPI_OK;
}

static unsigned long check_btb_reg(pfmw_arch_pmd_reg_t reg)
{
#ifdef ITANIUM2
   int is_valid = reg.pmd8_15_ita2_reg.btb_b == 0
       && reg.pmd8_15_ita2_reg.btb_mp == 0 ? 0 : 1;
#else
   int is_valid = reg.pmd8_15_ita_reg.btb_b == 0 && reg.pmd8_15_ita_reg.btb_mp
       == 0 ? 0 : 1;
#endif

   if (!is_valid)
      return 0;

#ifdef ITANIUM2
   if (reg.pmd8_15_ita2_reg.btb_b) {
      unsigned long addr;

      addr = reg.pmd8_15_ita2_reg.btb_addr << 4;
      addr |= reg.pmd8_15_ita2_reg.btb_slot < 3 ? reg.pmd8_15_ita2_reg.btb_slot : 0;
      return addr;
   } else
      return 0;
#else
   if (reg.pmd8_15_ita_reg.btb_b) {
      unsigned long addr;

      addr = reg.pmd8_15_ita_reg.btb_addr << 4;
      addr |= reg.pmd8_15_ita_reg.btb_slot < 3 ? reg.pmd8_15_ita_reg.btb_slot : 0;
      return addr;
   } else
      return 0;
#endif
}

static unsigned long check_btb(pfmw_arch_pmd_reg_t * btb, pfmw_arch_pmd_reg_t * pmd16)
{
   int i, last;
   unsigned long addr, lastaddr;

#ifdef ITANIUM2
   i = (pmd16->pmd16_ita2_reg.btbi_full) ? pmd16->pmd16_ita2_reg.btbi_bbi : 0;
   last = pmd16->pmd16_ita2_reg.btbi_bbi;
#else
   i = (pmd16->pmd16_ita_reg.btbi_full) ? pmd16->pmd16_ita_reg.btbi_bbi : 0;
   last = pmd16->pmd16_ita_reg.btbi_bbi;
#endif

   addr = 0;
   do {
      lastaddr = check_btb_reg(btb[i]);
      if (lastaddr)
         addr = lastaddr;
      i = (i + 1) % 8;
   } while (i != last);
   if (addr)
      return addr;
   else
      return PAPI_ESYS;
}

static int ia64_process_profile_entry(void *papiContext)
{
   ThreadInfo_t *thread;
   EventSetInfo_t *ESI;
   pfmw_smpl_hdr_t *hdr;
   pfmw_smpl_entry_t *ent;
   unsigned long buf_pos;
   unsigned long entry_size;
   int i, ret, reg_num, overflow_vector, count, pos;
   int EventCode=0, eventindex=0, native_index=0;
   _papi_hwi_context_t *ctx = (_papi_hwi_context_t *) papiContext;
   struct sigcontext *info = (struct sigcontext *) ctx->ucontext;
   hwd_control_state_t *this_state;
   pfmw_arch_pmd_reg_t *reg;

   thread = _papi_hwi_lookup_thread();
   if (thread == NULL)
     return(PAPI_EBUG);

   if ((ESI = thread->running_eventset[0]) == NULL)
     return(PAPI_EBUG);

   if ((ESI->state & PAPI_PROFILING) == 0)
     return(PAPI_EBUG);

   this_state = &ESI->machdep;

   hdr = (pfmw_smpl_hdr_t *) this_state->smpl_vaddr;
   /*
    * Make sure the kernel uses the format we understand
    */
   if (PFM_VERSION_MAJOR(hdr->hdr_version) != PFM_VERSION_MAJOR(PFM_SMPL_VERSION)) {
      PAPIERROR("Perfmon v%u.%u sampling format is not supported",
              PFM_VERSION_MAJOR(hdr->hdr_version), PFM_VERSION_MINOR(hdr->hdr_version));
   }
   entry_size = hdr->hdr_entry_size;

   /*
    * walk through all the entries recorded in the buffer
    */
   buf_pos = (unsigned long) (hdr + 1);
   for (i = 0; i < hdr->hdr_count; i++) {
      ret = 0;
      ent = (pfmw_smpl_entry_t *) buf_pos;
      if (ent->regs == 0) {
         buf_pos += entry_size;
         continue;
      }

      /* record  each register's overflow times  */
      ESI->profile.overflowcount++;

      overflow_vector = ent->regs;
      while (overflow_vector) {
         reg_num = ffs(overflow_vector) - 1;
         /* find the event code */
         for (count = 0; count < ESI->profile.event_counter; count++) {
            eventindex = ESI->profile.EventIndex[count];
            pos= ESI->EventInfoArray[eventindex].pos[0];
            if (pos + PMU_FIRST_COUNTER == reg_num) {
               EventCode = ESI->profile.EventCode[count];
               native_index= ESI->NativeInfoArray[pos].ni_event 
                                 & PAPI_NATIVE_AND_MASK;
               break;
            }
         }
         /* something is wrong */
         if (count == ESI->profile.event_counter) {
	    PAPIERROR("wrong count: %d vs. ESI->profile.event_counter %d", count, ESI->profile.event_counter);
            return(PAPI_EBUG);
          }

         /* * print entry header */
         info->sc_ip = ent->ip;
#ifdef ITANIUM2
         if (pfm_ita2_is_dear(native_index)) {
#else
         if (pfm_ita_is_dear(native_index)) {
#endif
            reg = (pfmw_arch_pmd_reg_t *) (ent + 1);
            reg++;
            reg++;
#ifdef ITANIUM2
            info->sc_ip = ((reg->pmd17_ita2_reg.dear_iaddr +
                            reg->pmd17_ita2_reg.dear_bn) << 4)
                | reg->pmd17_ita2_reg.dear_slot;

#else
            info->sc_ip = (reg->pmd17_ita_reg.dear_iaddr << 4)
                | (reg->pmd17_ita_reg.dear_slot);
#endif
         };
#ifdef ITANIUM2
         if (pfm_ita2_is_btb(native_index)
             || EventCode == PAPI_BR_INS) {
#else
         if (pfm_ita_is_btb(native_index)
             || EventCode == PAPI_BR_INS) {
#endif
            reg = (pfmw_arch_pmd_reg_t *) (ent + 1);
            info->sc_ip = check_btb(reg, reg + 8);
         }

         dispatch_profile(ESI, papiContext, (long_long) 0, count);
         overflow_vector ^= (1 << reg_num);
      }



/*
        printf("Entry %d PID:%d CPU:%d regs:0x%lx IIP:0x%016lx\n",
            smpl_entry++,
            ent->pid,
            ent->cpu,
            ent->regs,
            info->sc_ip);
*/


      /*  move to next entry */
      buf_pos += entry_size;

   }                            /* end of for loop */
   return (PAPI_OK);
}

#else   /* PFM30 */
static int ia64_process_profile_entry(void *papiContext)
{
   ThreadInfo_t *thread;
   EventSetInfo_t *ESI;
   pfmw_smpl_hdr_t *hdr;
   pfmw_smpl_entry_t *ent;
   unsigned long buf_pos;
   unsigned long entry_size;
   int i, ret, reg_num, count, pos;
   int EventCode=0, eventindex, native_index=0;
   _papi_hwi_context_t *ctx = (_papi_hwi_context_t *) papiContext;
   struct sigcontext *info = (struct sigcontext *) ctx->ucontext;
   hwd_control_state_t *this_state;
   pfmw_arch_pmd_reg_t *reg;

   thread = _papi_hwi_lookup_thread();
   if (thread == NULL)
     return (PAPI_EBUG);

   if ((ESI = thread->running_eventset[sidx]) == NULL)
     return(PAPI_EBUG);

   if ((ESI->state & PAPI_PROFILING) == 0)
     return(PAPI_EBUG);

   this_state = ESI->machdep;

   hdr = (pfmw_smpl_hdr_t *) this_state->smpl_vaddr;
   entry_size = sizeof(pfmw_smpl_entry_t);

   /*
    * walk through all the entries recorded in the buffer
    */
   buf_pos = (unsigned long) (hdr + 1);
   for (i = 0; i < hdr->hdr_count; i++) {
      ret = 0;
      ent = (pfmw_smpl_entry_t *) buf_pos;
      if (ent->ovfl_pmd == 0) {
         buf_pos += entry_size;
         continue;
      }
/*
        printf("Entry %d PID:%d CPU:%d ovfl_pmd:0x%x IIP:0x%016lx\n",
            smpl_entry++,
            ent->pid,
            ent->cpu,
            ent->ovfl_pmd,
            ent->ip);
*/

      /* record  each register's overflow times  */
      ESI->profile.overflowcount++;

      if (ent->ovfl_pmd) 
      {
         reg_num = ent->ovfl_pmd;
         /* find the event code */
         for (count = 0; count < ESI->profile.event_counter; count++) {
            eventindex = ESI->profile.EventIndex[count];
            pos= ESI->EventInfoArray[eventindex].pos[0];
            if (pos + PMU_FIRST_COUNTER == reg_num) {
               EventCode = ESI->profile.EventCode[count];
               native_index= ESI->NativeInfoArray[pos].ni_event 
                                 & PAPI_NATIVE_AND_MASK;
               break;
            }
         }
         /* something is wrong */
         if (count == ESI->profile.event_counter)
	   {
	     PAPIERROR("wrong count: %d vs. ESI->profile.event_counter %d\n", count, ESI->profile.event_counter);
	     return(PAPI_EBUG);
	   }

         /* * print entry header */
         info->sc_ip = ent->ip;
         printf("ent->ip = 0x%lx \n", ent->ip);
#ifdef ITANIUM2
         if (pfm_ita2_is_dear(native_index)) {
#else
         if (pfm_ita_is_dear(native_index)) {
#endif
            reg = (pfmw_arch_pmd_reg_t *) (ent + 1);
            reg++;
            reg++;
#ifdef ITANIUM2
            info->sc_ip = ((reg->pmd17_ita2_reg.dear_iaddr +
                            reg->pmd17_ita2_reg.dear_bn) << 4)
                | reg->pmd17_ita2_reg.dear_slot;

#else
            info->sc_ip = (reg->pmd17_ita_reg.dear_iaddr << 4)
                | (reg->pmd17_ita_reg.dear_slot);
#endif
            printf("info->sc_ip = 0x%lx \n", info->sc_ip);
            /* adjust pointer position */
            buf_pos += (hweight64(DEAR_REGS_MASK)<<3);
         };

         dispatch_profile(ESI, papiContext, (long_long) 0, count);
      }

      /*  move to next entry */
      buf_pos += entry_size;

   }                            /* end of if */
   return (PAPI_OK);
}

#endif


/* This function only used when hardware overflows ARE working */

static void _papi_hwd_dispatch_timer(int signal, siginfo_t * info, void *context)
 {
   _papi_hwi_context_t ctx;
   ThreadInfo_t *t = NULL;
   caddr_t pc;

   ctx.si = (hwd_siginfo_t *) info;
   ctx.ucontext = (hwd_ucontext_t *) context;

   pc = GET_OVERFLOW_ADDRESS(context);
   _papi_hwi_dispatch_overflow_signal((void *) &ctx, NULL, (long_long) 0, 0, &t, pc, sidx);
   return;
 }

#ifdef PFM20
static void ia64_process_sigprof(int n, hwd_siginfo_t * info, struct sigcontext
                                 *context)
{
   _papi_hwi_context_t ctx;

   ctx.si = info;
   ctx.ucontext = context;

   if (info->sy_code != PROF_OVFL) {
      PAPIERROR("received spurious SIGPROF, si_code = %d", info->sy_code);
      return;
   }
   ia64_process_profile_entry(&ctx);
   if (pfmw_perfmonctl(info->sy_pid, 0, PFM_RESTART, NULL, 0) == -1) {
     PAPIERROR("perfmonctl(PFM_RESTART) errno %d", errno);
   }
}

static void ia64_dispatch_sigprof(int n, hwd_siginfo_t * info, struct sigcontext *context)
{
   _papi_hwi_context_t ctx;
   ThreadInfo_t *master = NULL;

   ctx.si = info;
   ctx.ucontext = context;

   SUBDBG("pid=%d @0x%lx bv=0x%lx\n", info->sy_pid, context->sc_ip, info->sy_pfm_ovfl[0]);
   if (info->sy_code != PROF_OVFL) {
      PAPIERROR("received spurious SIGPROF, si_code = %d", info->sy_code);
      return;
   }
   _papi_hwi_dispatch_overflow_signal((void *) &ctx, NULL, 
                     info->sy_pfm_ovfl[0]>>PMU_FIRST_COUNTER, 0, &master);

   if (pfmw_perfmonctl(info->sy_pid, 0, PFM_RESTART, 0, 0) == -1) {
     PAPIERROR("perfmonctl(PFM_RESTART) errno %d", errno);
      return;
   }
}
#else  /* PFM30 */
static void ia64_process_sigprof(int n, hwd_siginfo_t * info, struct sigcontext
                                 *context)
{
   _papi_hwi_context_t ctx;

   ctx.si = info;
   ctx.ucontext = context;

   ia64_process_profile_entry(&ctx);

   if (pfmw_perfmonctl(0, info->si_fd, PFM_RESTART, NULL, 0) == -1) {
     PAPIERROR("perfmonctl(PFM_RESTART) errno %d", errno);
     return;
   }
}

static void ia64_dispatch_sigprof(int n, hwd_siginfo_t * info, struct sigcontext *sc)
{
   _papi_hwi_context_t ctx;
   pfm_msg_t msg;
   int ret, fd;
   ThreadInfo_t *master = NULL;
   caddr_t pc;

   ctx.si = info;
   ctx.ucontext = sc;
   fd = info->si_fd;
   ret = read(fd, &msg, sizeof(msg));

   if (ret != sizeof(msg)) {
      PAPIERROR("read(overflow message): errno %d", errno);
      return;
   }

   if (msg.pfm_gen_msg.msg_type != PFM_MSG_OVFL) {
      PAPIERROR("unexpected msg type %d",msg.pfm_gen_msg.msg_type);
      return;
   }

   pc = GET_OVERFLOW_ADDRESS(sc);
   _papi_hwi_dispatch_overflow_signal((void *) &ctx, NULL, 
          msg.pfm_ovfl_msg.msg_ovfl_pmds[0]>>PMU_FIRST_COUNTER, 0, &master, pc, sidx);
 
  if (pfmw_perfmonctl(0, fd, PFM_RESTART, 0, 0) == -1) {
      PAPIERROR("perfmonctl(PFM_RESTART) errno %d", errno);
      return;
   }

}
#endif

static int set_notify(EventSetInfo_t * ESI, int index, int value)
{
   int *pos, count, hwcntr, i;
   pfmw_param_t *pevt = &(((hwd_control_state_t *)ESI->machdep)->evt);

   pos = ESI->EventInfoArray[index].pos;
   count = 0;
   while (pos[count] != -1 && count < MAX_COUNTERS) {
      hwcntr = pos[count] + PMU_FIRST_COUNTER;
      for (i = 0; i < MAX_COUNTERS; i++) {
         if ( PFMW_PEVT_PFPPC_REG_NUM(pevt,i) == hwcntr) {
            SUBDBG("Found hw counter %d in %d, flags %d\n", hwcntr, i, value);
            PFMW_PEVT_PFPPC_REG_FLG(pevt,i) = value;
/*
         #ifdef PFM30
            if (value)
               pevt->pc[i].reg_reset_pmds[0] = 1UL << pevt->pc[i].reg_num;
            else 
               pevt->pc[i].reg_reset_pmds[0] = 0;
         #endif
*/
            break;
         }
      }
      count++;
   }
   return (PAPI_OK);
}

static int _papi_hwd_stop_profiling(ThreadInfo_t * master, EventSetInfo_t * ESI)
{
   _papi_hwi_context_t ctx;
   struct sigcontext info;

   ctx.ucontext = &info;
   pfmw_stop(master->context[sidx]);
   ESI->profile.overflowcount = 0;
   ia64_process_profile_entry(&ctx);
   return (PAPI_OK);
}


static int _papi_hwd_set_profile(EventSetInfo_t * ESI, int EventIndex, int threshold)
{
   struct sigaction act;
   void *tmp;
   int i;
   hwd_control_state_t *this_state = ESI->machdep;
   hwd_context_t *ctx = ESI->master->context[sidx];

   if (threshold == 0) {
/* unset notify */
      set_notify(ESI, EventIndex, 0);
/* reset the initial value */
      i = ESI->EventInfoArray[EventIndex].pos[0];
      SUBDBG("counter %d used in overflow, threshold %d\n",
             i + PMU_FIRST_COUNTER, threshold);
      this_state->pd[i].reg_value = 0;
      this_state->pd[i].reg_long_reset = 0;
      this_state->pd[i].reg_short_reset = 0;

/* remove the signal handler */
      if (ESI->profile.event_counter == 0)
         if (sigaction(OVFL_SIGNAL, NULL, NULL) == -1)
            return (PAPI_ESYS);
   } else {
      tmp = (void *) signal(OVFL_SIGNAL, SIG_IGN);
      if ((tmp != (void *) SIG_DFL) && (tmp != (void *) ia64_process_sigprof))
         return (PAPI_ESYS);

      /* Set up the signal handler */

      memset(&act, 0x0, sizeof(struct sigaction));
      act.sa_handler = (sig_t) ia64_process_sigprof;
      act.sa_flags = SA_RESTART;
      if (sigaction(OVFL_SIGNAL, &act, NULL) == -1)
         return (PAPI_ESYS);

/* set initial value in pd array */
      i = ESI->EventInfoArray[EventIndex].pos[0];
      SUBDBG("counter %d used in overflow, threshold %d\n",
             i + PMU_FIRST_COUNTER, threshold);
      this_state->pd[i].reg_value = (~0UL) - (unsigned long) threshold + 1;
      this_state->pd[i].reg_long_reset = (~0UL) - (unsigned long) threshold + 1;
      this_state->pd[i].reg_short_reset = (~0UL)-(unsigned long) threshold + 1;

      /* clear the overflow counters */
      ESI->profile.overflowcount = 0;

      /* in order to rebuild the context, we must destroy the old context */
      if (pfmw_destroy_context(ctx) == PAPI_ESYS) {
         return (PAPI_ESYS);
      }
      /* need to rebuild the context */
      if( pfmw_recreate_context(ESI,&this_state->smpl_vaddr, EventIndex)
                ==PAPI_ESYS)
         return(PAPI_ESYS);

      /* Set up the overflow notifier on the proper event.  */

      set_notify(ESI, EventIndex, PFM_REGFL_OVFL_NOTIFY);
   }
   return (PAPI_OK);
}

static int _papi_hwd_set_overflow(EventSetInfo_t * ESI, int EventIndex, int threshold)
{
   hwd_control_state_t *this_state = (hwd_control_state_t *) ESI->machdep;
   int j, retval = PAPI_OK, *pos;

   if (threshold == 0) {
      /* Remove the overflow notifier on the proper event. 
       */
      set_notify(ESI, EventIndex, 0);

      pos = ESI->EventInfoArray[EventIndex].pos;
      j = pos[0];
      SUBDBG("counter %d used in overflow, threshold %d\n",
             j + PMU_FIRST_COUNTER, threshold);
      this_state->pd[j].reg_value = 0;
      this_state->pd[j].reg_long_reset = 0;
      this_state->pd[j].reg_short_reset = 0;

      /* Remove the signal handler */

      _papi_hwi_lock(INTERNAL_LOCK);
      _papi_hwi_using_signal--;
      SUBDBG("_papi_hwi_using_signal=%d\n", _papi_hwi_using_signal);
      if (_papi_hwi_using_signal == 0) {

         if (sigaction(OVFL_SIGNAL, NULL, NULL) == -1)
            retval = PAPI_ESYS;
      }
      _papi_hwi_unlock(INTERNAL_LOCK);
   } else {
      struct sigaction act;

      /* Set up the signal handler */

      memset(&act, 0x0, sizeof(struct sigaction));
      act.sa_handler = (sig_t) ia64_dispatch_sigprof;
      act.sa_flags = SA_RESTART|SA_SIGINFO;
      if (sigaction(OVFL_SIGNAL, &act, NULL) == -1)
         return (PAPI_ESYS);

      /*Set the overflow notifier on the proper event. Remember that selector
       */
      set_notify(ESI, EventIndex, PFM_REGFL_OVFL_NOTIFY);

/* set initial value in pd array */

      pos = ESI->EventInfoArray[EventIndex].pos;
      j = pos[0];
      SUBDBG("counter %d used in overflow, threshold %d\n",
             j + PMU_FIRST_COUNTER, threshold);
      this_state->pd[j].reg_value = (~0UL) - (unsigned long) threshold + 1;
      this_state->pd[j].reg_short_reset = (~0UL)-(unsigned long) threshold+1;
      this_state->pd[j].reg_long_reset = (~0UL) - (unsigned long) threshold + 1;

      _papi_hwi_lock(INTERNAL_LOCK);
      _papi_hwi_using_signal++;
      _papi_hwi_unlock(INTERNAL_LOCK);
   }
   return (retval);
}

static char *_papi_hwd_ntv_code_to_name(unsigned int EventCode)
{
   return(pfmw_get_event_name(EventCode^PAPI_NATIVE_MASK));
}

static char *_papi_hwd_ntv_code_to_descr(unsigned int EventCode)
{
   return (_papi_hwd_ntv_code_to_name(EventCode));
}

static int _papi_hwd_ntv_enum_events(unsigned int *EventCode, int modifer)
{
   int index = *EventCode & PAPI_NATIVE_AND_MASK;

   if (index < MAX_NATIVE_EVENT - 1) {
      *EventCode = *EventCode + 1;
      return (PAPI_OK);
   } else
      return (PAPI_ENOEVNT);
}

static void _papi_hwd_init_control_state(hwd_control_state_t * ptr)
{
   set_domain(ptr, _papi_hwi_substrate_info[sidx].default_domain);
/* set library parameter pointer */
#ifdef PFM20
#ifdef ITANIUM2
   ptr->ita_lib_param.pfp_magic = PFMLIB_ITA2_PARAM_MAGIC;
#else
   ptr->ita_lib_param.pfp_magic = PFMLIB_ITA_PARAM_MAGIC;
#endif
   ptr->evt.pfp_model = &ptr->ita_lib_param;
#endif
}

static void _papi_hwd_remove_native(hwd_control_state_t * this_state, NativeInfo_t * nativeInfo)
{
   return;
}

static int _papi_hwd_update_control_state(hwd_control_state_t * this_state,
                   NativeInfo_t * native, int count, hwd_context_t * zero )
{
   int i, org_cnt;
   pfmw_param_t *evt = &this_state->evt;
   int events[MAX_COUNTERS];
   int index;

   if (count == 0) {
      for (i = 0; i < MAX_COUNTERS; i++)
         PFMW_PEVT_EVENT(evt,i) = 0;
      PFMW_PEVT_EVTCOUNT(evt) = 0;
      memset(PFMW_PEVT_PFPPC(evt), 0, sizeof(PFMW_PEVT_PFPPC(evt)));
      return (PAPI_OK);
   }

/* save the old data */
   org_cnt = PFMW_PEVT_EVTCOUNT(evt);
   for (i = 0; i < MAX_COUNTERS; i++)
      events[i] = PFMW_PEVT_EVENT(evt,i);

   for (i = 0; i < MAX_COUNTERS; i++)
         PFMW_PEVT_EVENT(evt,i) = 0;
   PFMW_PEVT_EVTCOUNT(evt) = 0;
   memset(PFMW_PEVT_PFPPC(evt), 0, sizeof(PFMW_PEVT_PFPPC(evt)));


   SUBDBG(" original count is %d\n", org_cnt);

/* add new native events to the evt structure */
   for (i = 0; i < count; i++) {
      index = native[i].ni_event & PAPI_NATIVE_AND_MASK;
#ifdef PFM20
#ifdef ITANIUM2
      if (pfm_ita2_is_dear(index))
#else
      if (pfm_ita_is_dear(index))
#endif
         set_dear_ita_param(&this_state->ita_lib_param, index);
#endif
      PFMW_PEVT_EVENT(evt,i) = index;
   }
   PFMW_PEVT_EVTCOUNT(evt) = count;
   /* Recalcuate the pfmlib_param_t structure, may also signal conflict */
   if (pfmw_dispatch_events(evt)) {
      /* recover the old data */
      PFMW_PEVT_EVTCOUNT(evt) = org_cnt;
      for (i = 0; i < MAX_COUNTERS; i++)
         PFMW_PEVT_EVENT(evt,i) = events[i];
      return (PAPI_ECNFLCT);
   }
   SUBDBG("event_count=%d\n", PFMW_PEVT_EVTCOUNT(evt));

   for (i = 0; i < PFMW_PEVT_EVTCOUNT(evt); i++) {
      native[i].ni_position = PFMW_PEVT_PFPPC_REG_NUM(evt,i) 
                              - PMU_FIRST_COUNTER;
      SUBDBG("event_code is %d, reg_num is %d\n", native[i].ni_event & PAPI_NATIVE_AND_MASK,
             native[i].ni_position);
   }

   return (PAPI_OK);
}

static int _papi_hwd_update_shlib_info(void)
{
   char fname[PAPI_HUGE_STR_LEN];
   unsigned long t_index = 0, d_index = 0, b_index = 0, counting = 1;
   PAPI_address_map_t *tmp = NULL;
   FILE *f;
                                                                                
   sprintf(fname, "/proc/%ld/maps", (long) _papi_hwi_system_info.pid);
   f = fopen(fname, "r");
                                                                                
   if (!f)
     {
         PAPIERROR("fopen(%s) returned < 0", fname);
         return(PAPI_OK);
     }
                                                                                
 again:
   while (!feof(f)) {
      char buf[PAPI_HUGE_STR_LEN+PAPI_HUGE_STR_LEN], perm[5], dev[6], mapname[PATH_MAX], lastmapname[PAPI_HUGE_STR_LEN];
      unsigned long begin, end, size, inode, foo;
                                                                                
      if (fgets(buf, sizeof(buf), f) == 0)
         break;
      if (strlen(mapname))
        strcpy(lastmapname,mapname);
      else
        lastmapname[0] = '\0';
      mapname[0] = '\0';
      sscanf(buf, "%lx-%lx %4s %lx %5s %ld %s", &begin, &end, perm,
             &foo, dev, &inode, mapname);
      size = end - begin;
                                                                                
      /* the permission string looks like "rwxp", where each character can
       * be either the letter, or a hyphen.  The final character is either
       * p for private or s for shared. */
                                                                                
      if (counting)
        {
          if ((perm[2] == 'x') && (perm[0] == 'r') && (inode != 0))
            {
              if  (strcmp(_papi_hwi_system_info.exe_info.fullname,mapname) == 0)                {
                  _papi_hwi_system_info.exe_info.address_info.text_start = (caddr_t) begin;
                  _papi_hwi_system_info.exe_info.address_info.text_end =
                    (caddr_t) (begin + size);
                }
              t_index++;
            }
          else if ((perm[0] == 'r') && (perm[1] == 'w') && (inode != 0) && (strcmp(_papi_hwi_system_info.exe_info.fullname,mapname) == 0))
            {
              _papi_hwi_system_info.exe_info.address_info.data_start = (caddr_t) begin;
              _papi_hwi_system_info.exe_info.address_info.data_end =
                (caddr_t) (begin + size);
              d_index++;
            }
          else if ((perm[0] == 'r') && (perm[1] == 'w') && (inode == 0) && (strcmp(_papi_hwi_system_info.exe_info.fullname,lastmapname) == 0))
            {
              _papi_hwi_system_info.exe_info.address_info.bss_start = (caddr_t) begin;
              _papi_hwi_system_info.exe_info.address_info.bss_end =
                (caddr_t) (begin + size);
              b_index++;
            }
        }
      else if (!counting)
        {
          if ((perm[2] == 'x') && (perm[0] == 'r') && (inode != 0))
            {
              if (strcmp(_papi_hwi_system_info.exe_info.fullname,mapname) != 0)
                {
              t_index++;
                  tmp[t_index-1 ].text_start = (caddr_t) begin;
                  tmp[t_index-1 ].text_end = (caddr_t) (begin + size);
                  strncpy(tmp[t_index-1 ].name, mapname, PAPI_MAX_STR_LEN);
                }
            }
          else if ((perm[0] == 'r') && (perm[1] == 'w') && (inode != 0))
            {
              if ( (strcmp(_papi_hwi_system_info.exe_info.fullname,mapname) != 0)
               && (t_index >0 ) && (tmp[t_index-1 ].data_start == 0))
                {
                  tmp[t_index-1 ].data_start = (caddr_t) begin;
                  tmp[t_index-1 ].data_end = (caddr_t) (begin + size);
                }
            }
          else if ((perm[0] == 'r') && (perm[1] == 'w') && (inode == 0))
            {
              if ((t_index > 0 ) && (tmp[t_index-1].bss_start == 0))
                {
                  tmp[t_index-1].bss_start = (caddr_t) begin;
                  tmp[t_index-1].bss_end = (caddr_t) (begin + size);
                }
            }
        }
   }
                                                                                
   if (counting) {
      /* When we get here, we have counted the number of entries in the map
         for us to allocate */
                                                                                
      tmp = (PAPI_address_map_t *) papi_calloc(t_index-1, sizeof(PAPI_address_map_t));
      if (tmp == NULL)
        { PAPIERROR("Error allocating shared library address map"); return(PAPI_ENOMEM); }
      t_index = 0;
      rewind(f);
      counting = 0;
      goto again;
   } else {
      if (_papi_hwi_system_info.shlib_info.map)
         papi_free(_papi_hwi_system_info.shlib_info.map);
      _papi_hwi_system_info.shlib_info.map = tmp;
      _papi_hwi_system_info.shlib_info.count = t_index;
                                                                                
      fclose(f);
   }
   return (PAPI_OK);
}
                                                                                

static int _papi_hwd_get_system_info(void)
{
   int tmp, retval;
   char maxargs[PAPI_HUGE_STR_LEN], *t, *s;
   pid_t pid;
   float mhz = 0.0;
   FILE *f;
                                                                                
   /* Software info */
                                                                                
   /* Path and args */
                                                                                
   pid = getpid();
   if (pid < 0)
     { PAPIERROR("getpid() returned < 0"); return(PAPI_ESYS); }
   _papi_hwi_system_info.pid = pid;
                                                                                
   sprintf(maxargs, "/proc/%d/exe", (int) pid);
   if (readlink(maxargs, _papi_hwi_system_info.exe_info.fullname, PAPI_HUGE_STR_LEN) < 0)
     { PAPIERROR("readlink(%s) returned < 0", maxargs); return(PAPI_ESYS); }
                                                                                
   /* basename can modify it's argument */
   strcpy(maxargs,_papi_hwi_system_info.exe_info.fullname);
   strcpy(_papi_hwi_system_info.exe_info.address_info.name, basename(maxargs));
                                                                                
   /* Executable regions, may require reading /proc/pid/maps file */
                                                                                
   retval = _papi_hwd_update_shlib_info();
                                                                                
   /* PAPI_preload_option information */
                                                                                
   strcpy(_papi_hwi_system_info.preload_info.lib_preload_env, "LD_PRELOAD");
   _papi_hwi_system_info.preload_info.lib_preload_sep = ' ';
   strcpy(_papi_hwi_system_info.preload_info.lib_dir_env, "LD_LIBRARY_PATH");
   _papi_hwi_system_info.preload_info.lib_dir_sep = ':';
                                                                                
   SUBDBG("Executable is %s\n", _papi_hwi_system_info.exe_info.address_info.name);
   SUBDBG("Full Executable is %s\n", _papi_hwi_system_info.exe_info.fullname);
   SUBDBG("Text: Start %p, End %p, length %d\n",
          _papi_hwi_system_info.exe_info.address_info.text_start,
          _papi_hwi_system_info.exe_info.address_info.text_end,
          (int)(_papi_hwi_system_info.exe_info.address_info.text_end -
          _papi_hwi_system_info.exe_info.address_info.text_start));
   SUBDBG("Data: Start %p, End %p, length %d\n",
          _papi_hwi_system_info.exe_info.address_info.data_start,
          _papi_hwi_system_info.exe_info.address_info.data_end,
          (int)(_papi_hwi_system_info.exe_info.address_info.data_end -
          _papi_hwi_system_info.exe_info.address_info.data_start));
   SUBDBG("Bss: Start %p, End %p, length %d\n",
          _papi_hwi_system_info.exe_info.address_info.bss_start,
          _papi_hwi_system_info.exe_info.address_info.bss_end,
          (int)(_papi_hwi_system_info.exe_info.address_info.bss_end -
          _papi_hwi_system_info.exe_info.address_info.bss_start));
                                                                                
   /* Hardware info */
                                                                                
   _papi_hwi_system_info.hw_info.ncpu = sysconf(_SC_NPROCESSORS_ONLN);
   _papi_hwi_system_info.hw_info.nnodes = 1;
   _papi_hwi_system_info.hw_info.totalcpus = sysconf(_SC_NPROCESSORS_CONF);
   _papi_hwi_system_info.hw_info.vendor = -1;
                                                                                
   if ((f = fopen("/proc/cpuinfo", "r")) == NULL)
     { PAPIERROR("fopen(/proc/cpuinfo) errno %d",errno); return(PAPI_ESYS); }
                                                                                
   /* All of this information maybe overwritten by the substrate */
                                                                                
   /* MHZ */
                                                                                
   rewind(f);
   s = search_cpu_info(f, "cpu MHz", maxargs);
   if (s)
      sscanf(s + 1, "%f", &mhz);
   _papi_hwi_system_info.hw_info.mhz = mhz;
                                                                                
   /* Vendor Name */
                                                                                
   rewind(f);
   s = search_cpu_info(f, "vendor_id", maxargs);
   if (s && (t = strchr(s + 2, '\n')))
     {
      *t = '\0';
      strcpy(_papi_hwi_system_info.hw_info.vendor_string, s + 2);
     }
   else
     {
       rewind(f);
       s = search_cpu_info(f, "vendor", maxargs);
       if (s && (t = strchr(s + 2, '\n'))) {
         *t = '\0';
         strcpy(_papi_hwi_system_info.hw_info.vendor_string, s + 2);
       }
     }
                                                                                
   /* Revision */
                                                                                
   rewind(f);
   s = search_cpu_info(f, "stepping", maxargs);
   if (s)
      {
        sscanf(s + 1, "%d", &tmp);
        _papi_hwi_system_info.hw_info.revision = (float) tmp;
      }
   else
     {
       rewind(f);
       s = search_cpu_info(f, "revision", maxargs);
       if (s)
         {
           sscanf(s + 1, "%d", &tmp);
           _papi_hwi_system_info.hw_info.revision = (float) tmp;
         }
     }
                                                                                
   /* Model Name */
                                                                                
   rewind(f);
   s = search_cpu_info(f, "family", maxargs);
   if (s && (t = strchr(s + 2, '\n')))
     {
       *t = '\0';
       strcpy(_papi_hwi_system_info.hw_info.model_string, s + 2);
     }
   else
     {
       rewind(f);
       s = search_cpu_info(f, "vendor", maxargs);
       if (s && (t = strchr(s + 2, '\n')))
         {
           *t = '\0';
           strcpy(_papi_hwi_system_info.hw_info.vendor_string, s + 2);
         }
     }
                                                                                
   rewind(f);
   s = search_cpu_info(f, "model", maxargs);
   if (s)
      {
        sscanf(s + 1, "%d", &tmp);
        _papi_hwi_system_info.hw_info.model = tmp;
      }
                                                                                
   fclose(f);
                                                                                
   SUBDBG("Found %d %s(%d) %s(%d) CPU's at %f Mhz.\n",
          _papi_hwi_system_info.hw_info.totalcpus,
          _papi_hwi_system_info.hw_info.vendor_string,
          _papi_hwi_system_info.hw_info.vendor,
          _papi_hwi_system_info.hw_info.model_string,
          _papi_hwi_system_info.hw_info.model, _papi_hwi_system_info.hw_info.mhz);
                                                                                
   return (PAPI_OK);
}

