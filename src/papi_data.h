/****************************/
/* THIS IS OPEN SOURCE CODE */
/****************************/

/* 
* File:    papi_data.h
* CVS:     $Id$
* Author:  dan terpstra
*          terpstra@cs.utk.edu
* Mods:    Min Zhou
*          min@cs.utk.edu
* Mods:    Kevin London
*	   london@cs.utk.edu
* Mods:    Per Ekman
*          pek@pdc.kth.se
* Mods:    <your name here>
*          <your email address>
*/
#ifndef _PAPI_DATA_H
#define _PAPI_DATA_H  1

/********************/
/*  BEGIN GLOBALS   */
/********************/

/* Static preinitialized table of text information for all preset events */
hwi_preset_info_t _papi_hwi_preset_info[PAPI_MAX_PRESET_EVENTS] = {
   /*  0*/ {"PAPI_L1_DCM",  "L1D cache misses", "Level 1 data cache misses"},
   /*  1*/ {"PAPI_L1_ICM",  "L1I cache misses", "Level 1 instruction cache misses"},
   /*  2*/ {"PAPI_L2_DCM",  "L2D cache misses", "Level 2 data cache misses"},
   /*  3*/ {"PAPI_L2_ICM",  "L2I cache misses", "Level 2 instruction cache misses"},
   /*  4*/ {"PAPI_L3_DCM",  "L3D cache misses", "Level 3 data cache misses"},
   /*  5*/ {"PAPI_L3_ICM",  "L3I cache misses", "Level 3 instruction cache misses"},
   /*  6*/ {"PAPI_L1_TCM",  "L1 cache misses", "Level 1 cache misses"},
   /*  7*/ {"PAPI_L2_TCM",  "L2 cache misses", "Level 2 cache misses"},
   /*  8*/ {"PAPI_L3_TCM",  "L3 cache misses", "Level 3 cache misses"},
   /*  9*/ {"PAPI_CA_SNP",  "Snoop Requests", "Requests for a snoop"},
   /* 10*/ {"PAPI_CA_SHR",  "Ex Acces shared CL", "Requests for exclusive access to shared cache line"},
   /* 11*/ {"PAPI_CA_CLN",  "Ex Access clean CL", "Requests for exclusive access to clean cache line"},
   /* 12*/ {"PAPI_CA_INV",  "Cache ln invalid", "Requests for cache line invalidation"},
   /* 13*/ {"PAPI_CA_ITV",  "Cache ln intervene", "Requests for cache line intervention"},
   /* 14*/ {"PAPI_L3_LDM",  "L3 load misses", "Level 3 load misses"},
   /* 15*/ {"PAPI_L3_STM",  "L3 store misses", "Level 3 store misses"},
   /* 16*/ {"PAPI_BRU_IDL", "Branch idle cycles", "Cycles branch units are idle"},
   /* 17*/ {"PAPI_FXU_IDL", "IU idle cycles", "Cycles integer units are idle"},
   /* 18*/ {"PAPI_FPU_IDL", "FPU idle cycles", "Cycles floating point units are idle"},
   /* 19*/ {"PAPI_LSU_IDL", "L/SU idle cycles", "Cycles load/store units are idle"},
   /* 20*/ {"PAPI_TLB_DM",  "Data TLB misses", "Data translation lookaside buffer misses"},
   /* 21*/ {"PAPI_TLB_IM",  "Instr TLB misses", "Instruction translation lookaside buffer misses"},
   /* 22*/ {"PAPI_TLB_TL",  "Total TLB misses", "Total translation lookaside buffer misses"},
   /* 23*/ {"PAPI_L1_LDM",  "L1 load misses", "Level 1 load misses"},
   /* 24*/ {"PAPI_L1_STM",  "L1 store misses", "Level 1 store misses"},
   /* 25*/ {"PAPI_L2_LDM",  "L2 load misses", "Level 2 load misses"},
   /* 26*/ {"PAPI_L2_STM",  "L2 store misses", "Level 2 store misses"},
   /* 27*/ {"PAPI_BTAC_M",  "Br targt addr miss", "Branch target address cache misses"},
   /* 28*/ {"PAPI_PRF_DM",  "Data prefetch miss", "Data prefetch cache misses"},
   /* 29*/ {"PAPI_L3_DCH",  "L3D cache hits", "Level 3 data cache hits"},
   /* 30*/ {"PAPI_TLB_SD",  "TLB shootdowns", "Translation lookaside buffer shootdowns"},
   /* 31*/ {"PAPI_CSR_FAL", "Failed store cond", "Failed store conditional instructions"},
   /* 32*/ {"PAPI_CSR_SUC", "Good store cond", "Successful store conditional instructions"},
   /* 33*/ {"PAPI_CSR_TOT", "Total store cond", "Total store conditional instructions"},
   /* 34*/ {"PAPI_MEM_SCY", "Stalled mem cycles", "Cycles Stalled Waiting for memory accesses"},
   /* 35*/ {"PAPI_MEM_RCY", "Stalled rd cycles", "Cycles Stalled Waiting for memory Reads"},
   /* 36*/ {"PAPI_MEM_WCY", "Stalled wr cycles", "Cycles Stalled Waiting for memory writes"},
   /* 37*/ {"PAPI_STL_ICY", "No instr issue", "Cycles with no instruction issue"},
   /* 38*/ {"PAPI_FUL_ICY", "Max instr issue", "Cycles with maximum instruction issue"},
   /* 39*/ {"PAPI_STL_CCY", "No instr done", "Cycles with no instructions completed"},
   /* 40*/ {"PAPI_FUL_CCY", "Max instr done", "Cycles with maximum instructions completed"},
   /* 41*/ {"PAPI_HW_INT",  "Hdw interrupts", "Hardware interrupts"},
   /* 42*/ {"PAPI_BR_UCN",  "Uncond branch", "Unconditional branch instructions"},
   /* 43*/ {"PAPI_BR_CN",   "Cond branch", "Conditional branch instructions"},
   /* 44*/ {"PAPI_BR_TKN",  "Cond branch taken", "Conditional branch instructions taken"},
   /* 45*/ {"PAPI_BR_NTK",  "Cond br not taken", "Conditional branch instructions not taken"},
   /* 46*/ {"PAPI_BR_MSP",  "Cond br mspredictd", "Conditional branch instructions mispredicted"},
   /* 47*/ {"PAPI_BR_PRC",  "Cond br predicted", "Conditional branch instructions correctly predicted"},
   /* 48*/ {"PAPI_FMA_INS", "FMAs completed", "FMA instructions completed"},
   /* 49*/ {"PAPI_TOT_IIS", "Instr issued", "Instructions issued"},
   /* 50*/ {"PAPI_TOT_INS", "Instr completed", "Instructions completed"},
   /* 51*/ {"PAPI_INT_INS", "Int instructions", "Integer instructions"},
   /* 52*/ {"PAPI_FP_INS",  "FP instructions", "Floating point instructions"},
   /* 53*/ {"PAPI_LD_INS",  "Loads", "Load instructions"},
   /* 54*/ {"PAPI_SR_INS",  "Stores", "Store instructions"},
   /* 55*/ {"PAPI_BR_INS",  "Branches", "Branch instructions"},
   /* 56*/ {"PAPI_VEC_INS", "Vector/SIMD instr", "Vector/SIMD instructions"},
   /* 57*/ {"PAPI_RES_STL", "Stalled res cycles", "Cycles stalled on any resource"},
   /* 58*/ {"PAPI_FP_STAL", "Stalled FPU cycles", "Cycles the FP unit(s) are stalled"},
   /* 59*/ {"PAPI_TOT_CYC", "Total cycles", "Total cycles"},
   /* 60*/ {"PAPI_LST_INS", "L/S completed", "Load/store instructions completed"},
   /* 61*/ {"PAPI_SYC_INS", "Syncs completed", "Synchronization instructions completed"},
   /* 62*/ {"PAPI_L1_DCH",  "L1D cache hits", "Level 1 data cache hits"},
   /* 63*/ {"PAPI_L2_DCH",  "L2D cache hits", "Level 2 data cache hits"},
   /* 64*/ {"PAPI_L1_DCA",  "L1D cache accesses", "Level 1 data cache accesses"},
   /* 65*/ {"PAPI_L2_DCA",  "L2D cache accesses", "Level 2 data cache accesses"},
   /* 66*/ {"PAPI_L3_DCA",  "L3D cache accesses", "Level 3 data cache accesses"},
   /* 67*/ {"PAPI_L1_DCR",  "L1D cache reads", "Level 1 data cache reads"},
   /* 68*/ {"PAPI_L2_DCR",  "L2D cache reads", "Level 2 data cache reads"},
   /* 69*/ {"PAPI_L3_DCR",  "L3D cache reads", "Level 3 data cache reads"},
   /* 70*/ {"PAPI_L1_DCW",  "L1D cache writes", "Level 1 data cache writes"},
   /* 71*/ {"PAPI_L2_DCW",  "L2D cache writes", "Level 2 data cache writes"},
   /* 72*/ {"PAPI_L3_DCW",  "L3D cache writes",  "Level 3 data cache writes"},
   /* 73*/ {"PAPI_L1_ICH",  "L1I cache hits", "Level 1 instruction cache hits"},
   /* 74*/ {"PAPI_L2_ICH",  "L2I cache hits", "Level 2 instruction cache hits"},
   /* 75*/ {"PAPI_L3_ICH",  "L3I cache hits", "Level 3 instruction cache hits"},
   /* 76*/ {"PAPI_L1_ICA",  "L1I cache accesses", "Level 1 instruction cache accesses"},
   /* 77*/ {"PAPI_L2_ICA",  "L2I cache accesses", "Level 2 instruction cache accesses"},
   /* 78*/ {"PAPI_L3_ICA",  "L3I cache accesses", "Level 3 instruction cache accesses"},
   /* 79*/ {"PAPI_L1_ICR",  "L1I cache reads", "Level 1 instruction cache reads"},
   /* 80*/ {"PAPI_L2_ICR",  "L2I cache reads", "Level 2 instruction cache reads"},
   /* 81*/ {"PAPI_L3_ICR",  "L3I cache reads", "Level 3 instruction cache reads"},
   /* 82*/ {"PAPI_L1_ICW",  "L1I cache writes", "Level 1 instruction cache writes"},
   /* 83*/ {"PAPI_L2_ICW",  "L2I cache writes", "Level 2 instruction cache writes"},
   /* 84*/ {"PAPI_L3_ICW",  "L3I cache writes", "Level 3 instruction cache writes"},
   /* 85*/ {"PAPI_L1_TCH",  "L1 cache hits", "Level 1 total cache hits"},
   /* 86*/ {"PAPI_L2_TCH",  "L2 cache hits", "Level 2 total cache hits"},
   /* 87*/ {"PAPI_L3_TCH",  "L3 cache hits", "Level 3 total cache hits"},
   /* 88*/ {"PAPI_L1_TCA",  "L1 cache accesses", "Level 1 total cache accesses"},
   /* 89*/ {"PAPI_L2_TCA",  "L2 cache accesses", "Level 2 total cache accesses"},
   /* 90*/ {"PAPI_L3_TCA",  "L3 cache accesses", "Level 3 total cache accesses"},
   /* 91*/ {"PAPI_L1_TCR",  "L1 cache reads", "Level 1 total cache reads"},
   /* 92*/ {"PAPI_L2_TCR",  "L2 cache reads", "Level 2 total cache reads"},
   /* 93*/ {"PAPI_L3_TCR",  "L3 cache reads", "Level 3 total cache reads"},
   /* 94*/ {"PAPI_L1_TCW",  "L1 cache writes", "Level 1 total cache writes"},
   /* 95*/ {"PAPI_L2_TCW",  "L2 cache writes", "Level 2 total cache writes"},
   /* 96*/ {"PAPI_L3_TCW",  "L3 cache writes", "Level 3 total cache writes"},
   /* 97*/ {"PAPI_FML_INS", "FPU multiply", "Floating point multiply instructions"},
   /* 98*/ {"PAPI_FAD_INS", "FPU add", "Floating point add instructions"},
   /* 99*/ {"PAPI_FDV_INS", "FPU divide", "Floating point divide instructions"},
   /*100*/ {"PAPI_FSQ_INS", "FPU square root", "Floating point square root instructions"},
   /*101*/ {"PAPI_FNV_INS", "FPU inverse", "Floating point inverse instructions"},
   /*102*/ {"PAPI_FP_OPS",  "FP operations", "Floating point operations"},
   /* empty entries are now null pointers instead of pointers to empty strings */
   /*103*/ {NULL, NULL, NULL},
   /*104*/ {NULL, NULL, NULL},
   /*105*/ {NULL, NULL, NULL},
   /*106*/ {NULL, NULL, NULL},
   /*107*/ {NULL, NULL, NULL},
   /*108*/ {NULL, NULL, NULL},
   /*109*/ {NULL, NULL, NULL},
   /*110*/ {NULL, NULL, NULL},
   /*111*/ {NULL, NULL, NULL},
   /*112*/ {NULL, NULL, NULL},
   /*113*/ {NULL, NULL, NULL},
   /*114*/ {NULL, NULL, NULL},
   /*115*/ {NULL, NULL, NULL},
   /*116*/ {NULL, NULL, NULL},
   /*117*/ {NULL, NULL, NULL},
   /*118*/ {NULL, NULL, NULL},
   /*119*/ {NULL, NULL, NULL},
   /*120*/ {NULL, NULL, NULL},
   /*121*/ {NULL, NULL, NULL},
   /*122*/ {NULL, NULL, NULL},
   /*123*/ {NULL, NULL, NULL},
   /*124*/ {NULL, NULL, NULL},
   /*125*/ {NULL, NULL, NULL},
   /*126*/ {NULL, NULL, NULL},
   /*127*/ {NULL, NULL, NULL}
};


const hwi_describe_t _papi_hwi_err[PAPI_NUM_ERRORS] = {
   /* 0*/ {PAPI_OK, "PAPI_OK", "No error"},
   /* 1*/ {PAPI_EINVAL, "PAPI_EINVAL", "Invalid argument"},
   /* 2*/ {PAPI_ENOMEM, "PAPI_ENOMEM", "Insufficient memory"},
   /* 3*/ {PAPI_ESYS, "PAPI_ESYS", "A System/C library call failed"},
   /* 4*/ {PAPI_ESBSTR, "PAPI_ESBSTR", "Not supported by substrate"},
   /* 5*/ {PAPI_ECLOST, "PAPI_ECLOST", "Access to the counters was lost or interrupted"},
   /* 6*/ {PAPI_EBUG, "PAPI_EBUG", "Internal error, please send mail to the developers"},
   /* 7*/ {PAPI_ENOEVNT, "PAPI_ENOEVNT", "Event does not exist"},
   /* 8*/ {PAPI_ECNFLCT, "PAPI_ECNFLCT", "Event exists, but cannot be counted due to hardware resource limits"},
   /* 9*/ {PAPI_ENOTRUN, "PAPI_ENOTRUN", "EventSet is currently not running"},
   /*10*/ {PAPI_EISRUN, "PAPI_EISRUN", "EventSet is currently counting"},
   /*11*/ {PAPI_ENOEVST, "PAPI_ENOEVST", "No such EventSet available"},
   /*12*/ {PAPI_ENOTPRESET, "PAPI_ENOTPRESET", "Event in argument is not a valid preset"},
   /*13*/ {PAPI_ENOCNTR, "PAPI_ENOCNTR", "Hardware does not support performance counters"},
   /*14*/ {PAPI_EMISC, "PAPI_EMISC", "Unknown error code"},
   /*15*/ {PAPI_EPERM, "PAPI_EPERM", "Permission level does not permit operation"}
};

/********************/
/*    END GLOBALS   */
/********************/
#endif
