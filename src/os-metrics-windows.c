/*
 *  Operating system metrics
 *
 *  Copyright 2023 Alexander Diemand
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


#include <windows.h>
#include <psapi.h>

#include "os-metrics.h"

#define pow10to9 1000000000
#define pow10to6 1000000
#define pow10to3 1000

/*
unsigned long timeval2nanoseconds(struct timeval *tv) {
  unsigned long t = (tv->tv_sec * pow10to6 + tv->tv_usec) * pow10to3;
  return t;
}
*/

/*
int osm_read_proc_stats(struct OSM_Proc_stats *stats, int pid) {
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
*/

ULONGLONG ns_of_filetime(const FILETIME *ft);

ULONGLONG ns_of_filetime(const FILETIME *ft) {
        ULARGE_INTEGER lgi = {0};
        lgi.LowPart = ft->dwLowDateTime;
        lgi.HighPart = ft->dwHighDateTime;
        return lgi.QuadPart * 10;
}

int osm_read_sys_stats(struct OSM_Sys_stats *stats) {
    int res = 1;
    FILETIME usert={0,0};
    FILETIME kernelt={0,0};
    FILETIME idlet={0,0};
    if (! GetSystemTimes(&idlet, &kernelt, &usert)) { exit -1; }
    ULONGLONG idletime = ns_of_filetime(&idlet);
    stats->_user_time_ns = ns_of_filetime(&usert);
    stats->_system_time_ns = ns_of_filetime(&kernelt) - idletime;
    stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns;
    return 0;
}

int osm_read_proc_stats(struct OSM_Proc_stats *stats, int pid) {
    HANDLE hProc;
    hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProc == NULL) { return -1; }
    PROCESS_MEMORY_COUNTERS memcounters;
    if (! GetProcessMemoryInfo(hProc, &memcounters, sizeof(PROCESS_MEMORY_COUNTERS))) {
        CloseHandle(hProc); return -2; }
    stats->_resident_memory = memcounters.WorkingSetSize;
    FILETIME usert={0,0};
    FILETIME kernelt={0,0};
    FILETIME createt={0,0};
    FILETIME exitt={0,0};
    if (! GetProcessTimes(hProc, &createt, &exitt, &kernelt, &usert)) {
        CloseHandle(hProc); return -3; }
    CloseHandle(hProc);
    stats->_user_time_ns = ns_of_filetime(&usert);
    stats->_system_time_ns = ns_of_filetime(&kernelt);
    stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns;

    return 0;
}

