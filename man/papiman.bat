REM PAPI Utility Pages
perl manServer_papi.pl -o html man1/papi_avail.1
perl manServer_papi.pl -o html man1/papi_clockres.1
perl manServer_papi.pl -o html man1/papi_cost.1
perl manServer_papi.pl -o html man1/papi_command_line.1
perl manServer_papi.pl -o html man1/papi_decode.1
perl manServer_papi.pl -o html man1/papi_event_chooser.1
perl manServer_papi.pl -o html man1/papi_mem_info.1
perl manServer_papi.pl -o html man1/papi_native_avail.1

REM PAPI Miscellaneous Pages
perl manServer_papi.pl -o html man3/PAPI.3
perl manServer_papi.pl -o html man3/PAPI_help.3
perl manServer_papi.pl -o html man3/PAPI_presets.3
perl manServer_papi.pl -o html man3/PAPI_native.3

REM PAPI Low Level Interface
perl manServer_papi.pl -o html man3/PAPI_accum.3
perl manServer_papi.pl -o html man3/PAPI_add_event.3
perl manServer_papi.pl -o html man3/PAPI_add_events.3
perl manServer_papi.pl -o html man3/PAPI_cleanup_eventset.3
perl manServer_papi.pl -o html man3/PAPI_create_eventset.3
perl manServer_papi.pl -o html man3/PAPI_destroy_eventset.3
perl manServer_papi.pl -o html man3/PAPI_encode_events.3
perl manServer_papi.pl -o html man3/PAPI_enum_event.3
perl manServer_papi.pl -o html man3/PAPI_event_code_to_name.3
perl manServer_papi.pl -o html man3/PAPI_event_name_to_code.3
perl manServer_papi.pl -o html man3/PAPI_get_dmem_info.3
perl manServer_papi.pl -o html man3/PAPI_get_event_info.3
perl manServer_papi.pl -o html man3/PAPI_get_executable_info.3
perl manServer_papi.pl -o html man3/PAPI_get_hardware_info.3
perl manServer_papi.pl -o html man3/PAPI_get_multiplex.3
perl manServer_papi.pl -o html man3/PAPI_get_opt.3
perl manServer_papi.pl -o html man3/PAPI_get_overflow_event_index.3
perl manServer_papi.pl -o html man3/PAPI_get_real_cyc.3
perl manServer_papi.pl -o html man3/PAPI_get_real_usec.3
perl manServer_papi.pl -o html man3/PAPI_get_shared_lib_info.3
perl manServer_papi.pl -o html man3/PAPI_get_thr_specific.3
perl manServer_papi.pl -o html man3/PAPI_get_virt_cyc.3
perl manServer_papi.pl -o html man3/PAPI_get_virt_usec.3
perl manServer_papi.pl -o html man3/PAPI_is_initialized.3
perl manServer_papi.pl -o html man3/PAPI_library_init.3
perl manServer_papi.pl -o html man3/PAPI_list_events.3
perl manServer_papi.pl -o html man3/PAPI_lock.3
perl manServer_papi.pl -o html man3/PAPI_multiplex_init.3
perl manServer_papi.pl -o html man3/PAPI_num_events.3
perl manServer_papi.pl -o html man3/PAPI_num_hwctrs.3
perl manServer_papi.pl -o html man3/PAPI_overflow.3
perl manServer_papi.pl -o html man3/PAPI_perror.3
perl manServer_papi.pl -o html man3/PAPI_profil.3
perl manServer_papi.pl -o html man3/PAPI_query_event.3
perl manServer_papi.pl -o html man3/PAPI_read.3
perl manServer_papi.pl -o html man3/PAPI_register_thread.3
perl manServer_papi.pl -o html man3/PAPI_remove_event.3
perl manServer_papi.pl -o html man3/PAPI_remove_events.3
perl manServer_papi.pl -o html man3/PAPI_reset.3
perl manServer_papi.pl -o html man3/PAPI_set_debug.3
perl manServer_papi.pl -o html man3/PAPI_set_domain.3
perl manServer_papi.pl -o html man3/PAPI_set_event_info.3
perl manServer_papi.pl -o html man3/PAPI_set_granularity.3
perl manServer_papi.pl -o html man3/PAPI_set_multiplex.3
perl manServer_papi.pl -o html man3/PAPI_set_opt.3
perl manServer_papi.pl -o html man3/PAPI_set_thr_specific.3
perl manServer_papi.pl -o html man3/PAPI_shutdown.3
perl manServer_papi.pl -o html man3/PAPI_sprofil.3
perl manServer_papi.pl -o html man3/PAPI_start.3
perl manServer_papi.pl -o html man3/PAPI_state.3
perl manServer_papi.pl -o html man3/PAPI_stop.3
perl manServer_papi.pl -o html man3/PAPI_strerror.3
perl manServer_papi.pl -o html man3/PAPI_thread_id.3
perl manServer_papi.pl -o html man3/PAPI_thread_init.3
perl manServer_papi.pl -o html man3/PAPI_unlock.3
perl manServer_papi.pl -o html man3/PAPI_write.3

REM PAPI High Level Interface
perl manServer_papi.pl -o html man3/PAPI_accum_counters.3
perl manServer_papi.pl -o html man3/PAPI_flips.3
perl manServer_papi.pl -o html man3/PAPI_flops.3
perl manServer_papi.pl -o html man3/PAPI_ipc.3
perl manServer_papi.pl -o html man3/PAPI_num_counters.3
perl manServer_papi.pl -o html man3/PAPI_read_counters.3
perl manServer_papi.pl -o html man3/PAPI_start_counters.3
perl manServer_papi.pl -o html man3/PAPI_stop_counters.3

REM PAPI Fortran Specific Documentation
perl manServer_papi.pl -o html man3/PAPIF.3
perl manServer_papi.pl -o html man3/PAPIF_get_clockrate.3
perl manServer_papi.pl -o html man3/PAPIF_get_domain.3
perl manServer_papi.pl -o html man3/PAPIF_get_exe_info.3
perl manServer_papi.pl -o html man3/PAPIF_get_granularity.3
perl manServer_papi.pl -o html man3/PAPIF_get_preload.3
perl manServer_papi.pl -o html man3/PAPIF_set_event_domain.3
