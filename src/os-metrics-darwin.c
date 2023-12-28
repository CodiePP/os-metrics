/*
 *  Operating system metrics
 *
 *  Copyright 2021 Alexander Diemand
 *
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/task.h>
#include <mach/task_info.h>

#include "os-metrics.h"

unsigned long timeval2nanoseconds(struct time_value tv) {
  unsigned long t = (tv.seconds * 1000000 + tv.microseconds) * 1000;
  return t;
}

int osm_read_proc_stats(int pid, struct OSM_Proc_stats *stats) {
    task_t task = MACH_PORT_NULL;
    if (task_for_pid(current_task(), pid, &task) != KERN_SUCCESS) {
        return -1;
    }
    struct mach_task_basic_info t_info;
    mach_msg_type_number_t t_info_count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(task, MACH_TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count) != KERN_SUCCESS) {
        return -2;
    }
    stats->_resident_memory = t_info.resident_size;
    /* the following are: "run time for terminated threads within the task" */
    //stats->_user_time_ns = timeval2nanoseconds(t_info.user_time);
    //stats->_system_time_ns = timeval2nanoseconds(t_info.system_time);
    stats->_user_time_ns = 0;
    stats->_system_time_ns = 0;

    thread_array_t thr_list;
    mach_msg_type_number_t thr_count;
    if (task_threads(task, &thr_list, &thr_count) != KERN_SUCCESS) {
        return -3;
    }

    struct thread_basic_info thr_info;
    mach_msg_type_number_t thr_info_count;
    for (int thr = 0; thr < thr_count; thr++) {
        thr_info_count = THREAD_INFO_MAX;
        if (thread_info(thr_list[thr], THREAD_BASIC_INFO,
                        (thread_info_t)&thr_info, &thr_info_count) != KERN_SUCCESS) {
            return -4;
        }
        if (!(thr_info.flags & TH_FLAGS_IDLE)) {
            stats->_user_time_ns += timeval2nanoseconds(thr_info.user_time);
            stats->_system_time_ns += timeval2nanoseconds(thr_info.system_time);
        }
    }
    stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns;

    if (vm_deallocate(task, (vm_offset_t)thr_list, thr_count * sizeof(thread_t))
        != KERN_SUCCESS) {
        return -5;
    }
    return 0;
}

int osm_read_sys_stats(struct OSM_Sys_stats *stats) {
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    host_cpu_load_info_data_t r_load;

    mach_port_t host_port = mach_host_self();
    if (host_statistics(host_port, HOST_CPU_LOAD_INFO,
                            (host_info_t)&r_load, &count) != KERN_SUCCESS) {
        return -1;
    }
    mach_port_deallocate(mach_task_self(), host_port);
    stats->_user_time_ns = r_load.cpu_ticks[CPU_STATE_USER] * 100000 / CLK_TCK;
    stats->_system_time_ns = r_load.cpu_ticks[CPU_STATE_SYSTEM] * 100000 / CLK_TCK;
    stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns +
                          r_load.cpu_ticks[CPU_STATE_NICE] * 100000 / CLK_TCK;
    return 0;
}

