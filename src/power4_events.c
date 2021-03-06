/* 
* File:    power4_events.c
* CVS:     
* Author:  Haihang You
*          you@cs.utk.edu
* Mods:    <your name here>
*          <your email address>
*/

#include "power4_events.h"

PWR4_native_map_t native_name_map[MAX_NATNAME_MAP_INDEX] = {
   {"PM_BIQ_IDU_FULL_CYC", -1}
   ,
   {"PM_BRQ_FULL_CYC", -1}
   ,
   {"PM_CR_MAP_FULL_CYC", -1}
   ,
   {"PM_DC_PREF_L2_CLONE_L3", -1}
   ,
   {"PM_DC_PREF_STREAM_ALLOC", -1}
   ,
   {"PM_DSLB_MISS", -1}
   ,
   {"PM_DTLB_MISS", -1}
   ,
   {"PM_FPR_MAP_FULL_CYC", -1}
   ,
   {"PM_FPU0_ALL", -1}
   ,
   {"PM_FPU0_DENORM", -1}
   ,
   {"PM_FPU0_FDIV", -1}
   ,
   {"PM_FPU0_FMA", -1}
   ,
   {"PM_FPU0_FSQRT", -1}
   ,
   {"PM_FPU0_FULL_CYC", -1}
   ,
   {"PM_FPU0_SINGLE", -1}
   ,
   {"PM_FPU0_STALL3", -1}
   ,
   {"PM_FPU0_STF", -1}
   ,
   {"PM_FPU1_ALL", -1}
   ,
   {"PM_FPU1_DENORM", -1}
   ,
   {"PM_FPU1_FDIV", -1}
   ,
   {"PM_FPU1_FMA", -1}
   ,
   {"PM_FPU1_FSQRT", -1}
   ,
   {"PM_FPU1_FULL_CYC", -1}
   ,
   {"PM_FPU1_SINGLE", -1}
   ,
   {"PM_FPU1_STALL3", -1}
   ,
   {"PM_FPU1_STF", -1}
   ,
   {"PM_GCT_FULL_CYC", -1}
   ,
   {"PM_GRP_DISP_REJECT", -1}
   ,
   {"PM_GRP_DISP_VALID", -1}
   ,
   {"PM_IC_PREF_INSTALL", -1}
   ,
   {"PM_IC_PREF_REQ", -1}
   ,
   {"PM_IERAT_XLATE_WR", -1}
   ,
   {"PM_INST_DISP", -1}
   ,
   {"PM_INST_FETCH_CYC", -1}
   ,
   {"PM_ISLB_MISS", -1}
   ,
   {"PM_ITLB_MISS", -1}
   ,
   {"PM_L1_DCACHE_RELOAD_VALID", -1}
   ,
   {"PM_L2SA_MOD_INV", -1}
   ,
   {"PM_L2SA_MOD_TAG", -1}
   ,
   {"PM_L2SA_SHR_INV", -1}
   ,
   {"PM_L2SA_SHR_MOD", -1}
   ,
   {"PM_L2SB_MOD_INV", -1}
   ,
   {"PM_L2SB_MOD_TAG", -1}
   ,
   {"PM_L2SB_SHR_INV", -1}
   ,
   {"PM_L2SB_SHR_MOD", -1}
   ,
   {"PM_L2SC_MOD_INV", -1}
   ,
   {"PM_L2SC_MOD_TAG", -1}
   ,
   {"PM_L2SC_SHR_INV", -1}
   ,
   {"PM_L2SC_SHR_MOD", -1}
   ,
   {"PM_L3B0_DIR_MIS", -1}
   ,
   {"PM_L3B0_DIR_REF", -1}
   ,
   {"PM_L3B1_DIR_MIS", -1}
   ,
   {"PM_L3B1_DIR_REF", -1}
   ,
   {"PM_LR_CTR_MAP_FULL_CYC", -1}
   ,
   {"PM_LSU0_DERAT_MISS", -1}
   ,
   {"PM_LSU0_FLUSH_LRQ", -1}
   ,
   {"PM_LSU0_FLUSH_SRQ", -1}
   ,
   {"PM_LSU0_FLUSH_ULD", -1}
   ,
   {"PM_LSU0_FLUSH_UST", -1}
   ,
   {"PM_LSU0_SRQ_STFWD", -1}
   ,
   {"PM_LSU1_DERAT_MISS", -1}
   ,
   {"PM_LSU1_FLUSH_LRQ", -1}
   ,
   {"PM_LSU1_FLUSH_SRQ", -1}
   ,
   {"PM_LSU1_FLUSH_ULD", -1}
   ,
   {"PM_LSU1_FLUSH_UST", -1}
   ,
   {"PM_LSU1_SRQ_STFWD", -1}
   ,
   {"PM_LSU_LMQ_FULL_CYC", -1}
   ,
   {"PM_LSU_LMQ_LHR_MERGE", -1}
   ,
   {"PM_LSU_LRQ_S0_ALLOC", -1}
   ,
   {"PM_LSU_LRQ_S0_VALID", -1}
   ,
   {"PM_LSU_SRQ_S0_ALLOC", -1}
   ,
   {"PM_LSU_SRQ_S0_VALID", -1}
   ,
   {"PM_MRK_IMR_RELOAD", -1}
   ,
   {"PM_MRK_LD_MISS_L1_LSU0", -1}
   ,
   {"PM_MRK_LD_MISS_L1_LSU1", -1}
   ,
   {"PM_MRK_STCX_FAIL", -1}
   ,
   {"PM_MRK_ST_MISS_L1", -1}
   ,
   {"PM_SNOOP_TLBIE", -1}
   ,
   {"PM_STCX_FAIL", -1}
   ,
   {"PM_ST_MISS_L1", -1}
   ,
   {"PM_XER_MAP_FULL_CYC", -1}
   ,
   {"PM_CYC", -1}
   ,
   {"PM_DATA_FROM_L3", -1}
   ,
   {"PM_FPU_DENORM", -1}
   ,
   {"PM_FPU_FDIV", -1}
   ,
   {"PM_GCT_EMPTY_CYC", -1}
   ,
   {"PM_INST_CMPL", -1}
   ,
   {"PM_INST_FROM_MEM", -1}
   ,
   {"PM_LSU_FLUSH_ULD", -1}
   ,
   {"PM_LSU_SRQ_STFWD", -1}
   ,
   {"PM_MRK_DATA_FROM_L3", -1}
   ,
   {"PM_MRK_GRP_DISP", -1}
   ,
   {"PM_MRK_LD_MISS_L1", -1}
   ,
   {"PM_MRK_ST_CMPL", -1}
   ,
   {"PM_RUN_CYC", -1}
   ,
   {"PM_DATA_FROM_MEM", -1}
   ,
   {"PM_FPU_FMA", -1}
   ,
   {"PM_FPU_STALL3", -1}
   ,
   {"PM_GRP_DISP", -1}
   ,
   {"PM_INST_FROM_L25_L275", -1}
   ,
   {"PM_LSU_FLUSH_UST", -1}
   ,
   {"PM_LSU_LMQ_SRQ_EMPTY_CYC", -1}
   ,
   {"PM_MRK_BRU_FIN", -1}
   ,
   {"PM_MRK_DATA_FROM_MEM", -1}
   ,
   {"PM_THRESH_TIMEO", -1}
   ,
   {"PM_WORK_HELD", -1}
   ,
   {"PM_1INST_CLB_CYC", -1}
   ,
   {"PM_2INST_CLB_CYC", -1}
   ,
   {"PM_3INST_CLB_CYC", -1}
   ,
   {"PM_4INST_CLB_CYC", -1}
   ,
   {"PM_5INST_CLB_CYC", -1}
   ,
   {"PM_6INST_CLB_CYC", -1}
   ,
   {"PM_7INST_CLB_CYC", -1}
   ,
   {"PM_8INST_CLB_CYC", -1}
   ,
   {"PM_BR_ISSUED", -1}
   ,
   {"PM_BR_MPRED_CR", -1}
   ,
   {"PM_BR_MPRED_TA", -1}
   ,
   {"PM_CRQ_FULL_CYC", -1}
   ,
   {"PM_DATA_TABLEWALK_CYC", -1}
   ,
   {"PM_DC_INV_L2", -1}
   ,
   {"PM_DC_PREF_OUT_STREAMS", -1}
   ,
   {"PM_EE_OFF", -1}
   ,
   {"PM_EE_OFF_EXT_INT", -1}
   ,
   {"PM_FAB_CMD_ISSUED", -1}
   ,
   {"PM_FAB_CMD_RETRIED", -1}
   ,
   {"PM_LSU0_LDF", -1}
   ,
   {"PM_LSU1_LDF", -1}
   ,
   {"PM_FPU0_FEST", -1}
   ,
   {"PM_FPU0_FIN", -1}
   ,
   {"PM_FPU0_FMOV_FEST", -1}
   ,
   {"PM_FPU0_FPSCR", -1}
   ,
   {"PM_FPU0_FRSP_FCONV", -1}
   ,
   {"PM_FPU1_FEST", -1}
   ,
   {"PM_FPU1_FIN", -1}
   ,
   {"PM_FPU1_FMOV_FEST", -1}
   ,
   {"PM_FPU1_FRSP_FCONV", -1}
   ,
   {"PM_FXLS0_FULL_CYC", -1}
   ,
   {"PM_FXU0_FIN", -1}
   ,
   {"PM_FXU1_FIN", -1}
   ,
   {"PM_GPR_MAP_FULL_CYC", -1}
   ,
   {"PM_GRP_DISP_BLK_SB_CYC", -1}
   ,
   {"PM_L1_PREF", -1}
   ,
   {"PM_L1_WRITE_CYC", -1}
   ,
   {"PM_L2SA_ST_HIT", -1}
   ,
   {"PM_L2SA_ST_REQ", -1}
   ,
   {"PM_L2SB_ST_HIT", -1}
   ,
   {"PM_L2SB_ST_REQ", -1}
   ,
   {"PM_L2SC_ST_HIT", -1}
   ,
   {"PM_L2SC_ST_REQ", -1}
   ,
   {"PM_L2_PREF", -1}
   ,
   {"PM_LARX_LSU0", -1}
   ,
   {"PM_LARX_LSU1", -1}
   ,
   {"PM_LD_MISS_L1_LSU0", -1}
   ,
   {"PM_LD_MISS_L1_LSU1", -1}
   ,
   {"PM_LD_REF_L1_LSU0", -1}
   ,
   {"PM_LD_REF_L1_LSU1", -1}
   ,
   {"PM_LSU0_BUSY", -1}
   ,
   {"PM_LSU1_BUSY", -1}
   ,
   {"PM_LSU_LMQ_S0_ALLOC", -1}
   ,
   {"PM_LSU_LMQ_S0_VALID", -1}
   ,
   {"PM_LSU_LRQ_FULL_CYC", -1}
   ,
   {"PM_LSU_SRQ_FULL_CYC", -1}
   ,
   {"PM_LSU_SRQ_SYNC_CYC", -1}
   ,
   {"PM_MRK_L1_RELOAD_VALID", -1}
   ,
   {"PM_MRK_LSU0_FLUSH_LRQ", -1}
   ,
   {"PM_MRK_LSU0_FLUSH_SRQ", -1}
   ,
   {"PM_MRK_LSU0_FLUSH_ULD", -1}
   ,
   {"PM_MRK_LSU0_FLUSH_UST", -1}
   ,
   {"PM_MRK_LSU0_INST_FIN", -1}
   ,
   {"PM_MRK_LSU1_FLUSH_LRQ", -1}
   ,
   {"PM_MRK_LSU1_FLUSH_SRQ", -1}
   ,
   {"PM_MRK_LSU1_FLUSH_ULD", -1}
   ,
   {"PM_MRK_LSU1_FLUSH_UST", -1}
   ,
   {"PM_MRK_LSU1_INST_FIN", -1}
   ,
   {"PM_MRK_LSU_SRQ_INST_VALID", -1}
   ,
   {"PM_STCX_PASS", -1}
   ,
   {"PM_ST_REF_L1_LSU0", -1}
   ,
   {"PM_ST_REF_L1_LSU1", -1}
   ,
   {"PM_DATA_FROM_L35", -1}
   ,
   {"PM_FPU_FEST", -1}
   ,
   {"PM_FXU_FIN", -1}
   ,
   {"PM_INST_FROM_L2", -1}
   ,
   {"PM_LD_MISS_L1", -1}
   ,
   {"PM_MRK_DATA_FROM_L35", -1}
   ,
   {"PM_MRK_LSU_FLUSH_LRQ", -1}
   ,
   {"PM_MRK_ST_CMPL_INT", -1}
   ,
   {"PM_STOP_COMPLETION", -1}
   ,
   {"PM_HV_CYC", -1}
   ,
   {"PM_DATA_FROM_L2", -1}
   ,
   {"PM_FPU_FIN", -1}
   ,
   {"PM_FXU1_BUSY_FXU0_IDLE", -1}
   ,
   {"PM_INST_FROM_L35", -1}
   ,
   {"PM_LARX", -1}
   ,
   {"PM_LSU_BUSY", -1}
   ,
   {"PM_LSU_SRQ_EMPTY_CYC", -1}
   ,
   {"PM_MRK_CRU_FIN", -1}
   ,
   {"PM_MRK_DATA_FROM_L2", -1}
   ,
   {"PM_MRK_GRP_CMPL", -1}
   ,
   {"PM_MRK_LSU_FLUSH_SRQ", -1}
   ,
   {"PM_1PLUS_PPC_CMPL", -1}
   ,
   {"PM_DATA_FROM_L25_SHR", -1}
   ,
   {"PM_FPU_ALL", -1}
   ,
   {"PM_FPU_FULL_CYC", -1}
   ,
   {"PM_FPU_SINGLE", -1}
   ,
   {"PM_FXU_IDLE", -1}
   ,
   {"PM_GRP_DISP_SUCCESS", -1}
   ,
   {"PM_GRP_MRK", -1}
   ,
   {"PM_INST_FROM_L3", -1}
   ,
   {"PM_LSU_FLUSH_SRQ", -1}
   ,
   {"PM_MRK_DATA_FROM_L25_SHR", -1}
   ,
   {"PM_MRK_GRP_TIMEO", -1}
   ,
   {"PM_DATA_FROM_L275_SHR", -1}
   ,
   {"PM_FPU_FSQRT", -1}
   ,
   {"PM_FPU_STF", -1}
   ,
   {"PM_FXU_BUSY", -1}
   ,
   {"PM_INST_FROM_L1", -1}
   ,
   {"PM_LSU_DERAT_MISS", -1}
   ,
   {"PM_LSU_FLUSH_LRQ", -1}
   ,
   {"PM_MRK_DATA_FROM_L275_SHR", -1}
   ,
   {"PM_MRK_FXU_FIN", -1}
   ,
   {"PM_MRK_GRP_ISSUED", -1}
   ,
   {"PM_MRK_ST_GPS", -1}
   ,
   {"PM_DATA_FROM_L275_MOD", -1}
   ,
   {"PM_FPU_FRSP_FCONV", -1}
   ,
   {"PM_FXU0_BUSY_FXU1_IDLE", -1}
   ,
   {"PM_GRP_CMPL", -1}
   ,
   {"PM_INST_FROM_PREF", -1}
   ,
   {"PM_MRK_DATA_FROM_L275_MOD", -1}
   ,
   {"PM_MRK_FPU_FIN", -1}
   ,
   {"PM_MRK_INST_FIN", -1}
   ,
   {"PM_MRK_LSU_FLUSH_UST", -1}
   ,
   {"PM_ST_REF_L1", -1}
   ,
   {"PM_0INST_FETCH", -1}
   ,
   {"PM_DATA_FROM_L25_MOD", -1}
   ,
   {"PM_EXT_INT", -1}
   ,
   {"PM_FPU_FMOV_FEST", -1}
   ,
   {"PM_LSU_LDF", -1}
   ,
   {"PM_FXLS_FULL_CYC", -1}
   ,
   {"PM_LD_REF_L1", -1}
   ,
   {"PM_MRK_DATA_FROM_L25_MOD", -1}
   ,
   {"PM_MRK_LSU_FIN", -1}
   ,
   {"PM_MRK_LSU_FLUSH_ULD", -1}
   ,
   {"PM_TB_BIT_TRANS", -1}
};
#ifdef PM_INITIALIZE
pm_info2_t pminfo;
#else
pm_info_t pminfo;
#endif
pm_groups_info_t pmgroups;
native_event_entry_t native_table[PAPI_MAX_NATIVE_EVENTS];
hwd_groups_t group_map[MAX_GROUPS] = { 0 };

/* to initialize the native_table */
void initialize_native_table()
{
   int i, j;

   memset(native_table, 0, PAPI_MAX_NATIVE_EVENTS * sizeof(native_event_entry_t));
   for (i = 0; i < PAPI_MAX_NATIVE_EVENTS; i++) {
      for (j = 0; j < MAX_COUNTERS; j++)
         native_table[i].resources.counter_cmd[j] = -1;
   }
}

/* to setup native_table group value */
static void PWR4_setup_gps(int total)
{
   int i, j, gnum;

   for (i = 0; i < total; i++) {
      for (j = 0; j < MAX_COUNTERS; j++) {
         /*      native_table[i].resources.rgg[j]=-1; */
         if (native_table[i].resources.selector & (1 << j)) {
            for (gnum = 0; gnum < pmgroups.maxgroups; gnum++) {
               if (native_table[i].resources.counter_cmd[j] ==
                   pmgroups.event_groups[gnum].events[j]) {
                  /* could use gnum instead of pmgroups.event_groups[gnum].group_id */
                  native_table[i].resources.group[pmgroups.event_groups[gnum].group_id /
                                                  32] |=
                      1 << (pmgroups.event_groups[gnum].group_id % 32);
                  /*native_table[i].resources.rgg[j]=gnum; */
               }
            }
         }
      }
   }

   for (gnum = 0; gnum < pmgroups.maxgroups; gnum++) {
      for (i = 0; i < MAX_COUNTERS; i++)
         group_map[gnum].counter_cmd[i] = pmgroups.event_groups[gnum].events[i];
   }
}

/* to setup native_table values, and return number of entries */
void power4_setup_native_table()
{
#ifdef PM_INITIALIZE
   PMEVENTS_T *wevp;
   PMINFO_T *info;
#else
   pm_events_t *wevp;
   pm_info_t *info;
#endif
   int pmc, ev, i, j, index;

   info = &pminfo;
   index = 0;
   initialize_native_table();
   for (pmc = 0; pmc < info->maxpmcs; pmc++) {
      wevp = info->list_events[pmc];
      for (ev = 0; ev < info->maxevents[pmc]; ev++, wevp++) {
         for (i = 0; i < index; i++) {
            if (strcmp(wevp->short_name, native_table[i].name) == 0) {
               native_table[i].resources.selector |= 1 << pmc;
               native_table[i].resources.counter_cmd[pmc] = wevp->event_id;
               break;
            }
         }
         if (i == index) {
            /*native_table[i].index=i; */
            native_table[i].resources.selector |= 1 << pmc;
            native_table[i].resources.counter_cmd[pmc] = wevp->event_id;
            native_table[i].name = wevp->short_name;
            native_table[i].description = wevp->description;
            index++;
            for (j = 0; j < MAX_NATNAME_MAP_INDEX; j++) {
               if (strcmp(native_table[i].name, native_name_map[j].name) == 0)
                  native_name_map[j].index = i;
            }
         }
      }
   }
   PWR4_setup_gps(index);
}
