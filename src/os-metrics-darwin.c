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

int osm_read_proc_stats(struct OSM_Proc_stats *stats, int pid) {
    task_t task = MACH_PORT_NULL;
    if (task_for_pid(current_task(), pid, &task) != KERN_SUCCESS) {
        return -2;
    }
    struct mach_task_basic_info t_info;
    mach_msg_type_number_t t_info_count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(task, MACH_TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count) != KERN_SUCCESS) {
        return -1;
    }
    stats->_resident_memory = t_info.resident_size;
    stats->_user_time_ns = timeval2nanoseconds(t_info.user_time);
    stats->_system_time_ns = timeval2nanoseconds(t_info.system_time);
    return 0;
}

