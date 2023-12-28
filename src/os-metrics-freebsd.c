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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/resource.h> 
#include <libutil.h>

#include "os-metrics.h"

#define pow10to9 1000000000
#define pow10to6 1000000
#define pow10to3 1000

unsigned long timeval2nanoseconds(struct timeval *tv) {
  unsigned long t = (tv->tv_sec * pow10to6 + tv->tv_usec) * pow10to3;
  return t;
}

int osm_read_proc_stats(int pid, struct OSM_Proc_stats *stats) {
    int res = 1;
    struct kinfo_proc *usage = kinfo_getproc(pid);
    if (usage != NULL) {
        stats->_user_time_ns = timeval2nanoseconds(&usage->ki_rusage.ru_utime);
        stats->_system_time_ns = timeval2nanoseconds(&usage->ki_rusage.ru_stime);
        stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns;
        stats->_resident_memory = usage->ki_rssize * 4096;
	res = 0;
    }
    return res;
}


int osm_read_sys_stats(struct OSM_Sys_stats *stats) {
    int res = 1;
    // tbd
    return res;
}

