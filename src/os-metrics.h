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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OSM_Proc_stats {
  uint64_t _user_time_ns;
  uint64_t _system_time_ns;
  uint64_t _cpu_time_ns;
  uint32_t _resident_memory;
};

int osm_read_proc_stats(int pid, struct OSM_Proc_stats *);


struct OSM_Sys_stats {
  uint64_t _user_time_ns;
  uint64_t _system_time_ns;
  uint64_t _cpu_time_ns;
};

int osm_read_sys_stats(struct OSM_Sys_stats *);

uint64_t osm_timestamp_ns();

#ifdef __cplusplus
}
#endif
