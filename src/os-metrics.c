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


#if defined(__linux__)
#include "os-metrics-linux.c"
#elif defined(__APPLE__)
#include "os-metrics-darwin.c"
#elif defined(__FreeBSD__)
#include "os-metrics-freebsd.c"
#elif defined(_WIN64)
#include "os-metrics-windows.c"
#else
#error not known platform
#endif

#if !defined(_WIN64)

#include <time.h>

uint64_t osm_timestamp_ns() {
   struct timespec ts;
   timespec_get(&ts, TIME_UTC);
   return ts.tv_sec * 1E9 + ts.tv_nsec;
}

#endif