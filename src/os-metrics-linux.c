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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h> 

#include "os-metrics.h"

#define pow10to9 1000000000
#define pow10to6 1000000
#define pow10to3 1000

unsigned long timeval2nanoseconds(struct timeval *tv) {
  unsigned long t = (tv->tv_sec * pow10to6 + tv->tv_usec) * pow10to3;
  return t;
}

long sf_atol(const char *token) {
    if (token == NULL) { return -1; }
    char *endp;
    long res = strtol(token, &endp, 10);
    if (endp == token) { return -1; }
    else { return res; }
}

int osm_read_proc_stats(int pid, struct OSM_Proc_stats *stats) {
    struct rusage usage;
    int res = getrusage(RUSAGE_SELF, &usage);
    if (res == 0) {
        stats->_user_time_ns = timeval2nanoseconds(&usage.ru_utime);
        stats->_system_time_ns = timeval2nanoseconds(&usage.ru_stime);
        stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns;
        stats->_resident_memory = usage.ru_maxrss * 1024;
    }
    return res;
}

/* this returns the total CPU time spent;
   build a difference between measurements */
int osm_read_sys_stats(struct OSM_Sys_stats *stats) {
    int res = 1;
    long clktck = sysconf(_SC_CLK_TCK);
    FILE *fp = fopen("/proc/stat", "r");
    if (fp) {
        char buffer[256];
        if (fgets(buffer, 256, fp)) {
            char *strp = buffer; char *svptr; char *token;
            int idx = 0;
            const char *delim = " \t";
            while ((token = strtok_r(strp, delim, &svptr)) != NULL) {
                strp = NULL; idx++;
                if (idx == 2) {  // utime
                    stats->_user_time_ns = sf_atol(token) * pow10to9 / clktck;
                }
                if (idx == 3) {  // idle
                    stats->_user_time_ns += sf_atol(token) * pow10to9 / clktck;
                }
                if (idx == 4) {  // stime
                    stats->_system_time_ns = sf_atol(token) * pow10to9 / clktck;
                    stats->_cpu_time_ns = stats->_user_time_ns + stats->_system_time_ns;
                    res = 0;
                    break;
                }
            }
        }
        fclose(fp);
    }
    return res;
}

