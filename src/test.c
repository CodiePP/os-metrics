
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "os-metrics.h"

#ifdef _WIN64
#include <windows.h>
static LARGE_INTEGER _tm_freq;
#endif

int main (int argc, char **argv) {

  uint64_t t0 = osm_timestamp_ns();
#ifdef _WIN64
  QueryPerformanceFrequency(&_tm_freq);
  printf("Windows timer frequency: %lld\n", _tm_freq.QuadPart);
#endif

  struct OSM_Proc_stats _proc_stats;
  pid_t pid = getpid();

  int sz = 1024 * 8;
  char buffer1[sz];
  for (int i = 0; i < sz; i++) {
    buffer1[i] = i & 0xff; }
  char buffer2[sz];
  unsigned long limit = 1000000;
  for (int i = 0; i < limit; i++) {
    // do some work
    memcpy(buffer2, buffer1, sz);
  }

  int res = 0;
  if ((res = osm_read_proc_stats(pid, &_proc_stats)) == 0)
  {
    printf("os-metrics on process with pid %d\n  resident size = %u bytes\n  user time = %" PRIu64 " ns\n  system time = %" PRIu64 " ns\n  total cpu time = %" PRIu64 " ns\n", (int)pid, _proc_stats._resident_memory, _proc_stats._user_time_ns, _proc_stats._system_time_ns, _proc_stats._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (1): %d!\n", res);
  }

  struct OSM_Sys_stats _sys_stats0, _sys_stats1, _sys_stats2, _sys_stats3;
  if ((res = osm_read_sys_stats(&_sys_stats0)) != 0) {
    printf("something went wrong (2a): %d!\n", res);
  }
  sleep(2);
  if ((res = osm_read_sys_stats(&_sys_stats1)) == 0)
  {
    printf("os-metrics on system\n  user time = %" PRIu64 " ns\n  system time = %" PRIu64 " ns\n  total cpu time = %" PRIu64 " ns\n", _sys_stats1._user_time_ns-_sys_stats0._user_time_ns, _sys_stats1._system_time_ns-_sys_stats0._system_time_ns, _sys_stats1._cpu_time_ns-_sys_stats0._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (2b): %d!\n", res);
  }
  //sleep(2);
  unsigned long long _t={0};
  for (int i=0; i<1000; i++) {
          for (int j=0; j<1000000; j++) {
                  _t += j*i;
          }
  }
  if (_t > 0) {
    printf("√\n");
  };

  if ((res = osm_read_sys_stats(&_sys_stats2)) == 0)
  {
    printf("os-metrics on system\n  user time = %" PRIu64 " ns\n  system time = %" PRIu64 " ns\n  total cpu time = %" PRIu64 " ns\n", _sys_stats2._user_time_ns-_sys_stats1._user_time_ns, _sys_stats2._system_time_ns-_sys_stats1._system_time_ns, _sys_stats2._cpu_time_ns-_sys_stats1._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (2c): %d!\n", res);
  }
  sleep(2);
  if ((res = osm_read_sys_stats(&_sys_stats3)) == 0)
  {
    printf("os-metrics on system\n  user time = %" PRIu64 " ns\n  system time = %" PRIu64 " ns\n  total cpu time = %" PRIu64 " ns\n", _sys_stats3._user_time_ns-_sys_stats2._user_time_ns, _sys_stats3._system_time_ns-_sys_stats2._system_time_ns, _sys_stats3._cpu_time_ns-_sys_stats2._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (2d): %d!\n", res);
  }

  if ((res = osm_read_proc_stats(pid, &_proc_stats)) == 0)
  {
    printf("os-metrics on process with pid %d\n  resident size = %u bytes\n  user time = %" PRIu64 " ns\n  system time = %" PRIu64 " ns\n  total cpu time = %" PRIu64 " ns\n", (int)pid, _proc_stats._resident_memory, _proc_stats._user_time_ns, _proc_stats._system_time_ns, _proc_stats._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (1): %d!\n", res);
  }

  uint64_t t1 = osm_timestamp_ns();
  uint64_t td = t1 - t0;

  printf("time elapsed: %" PRIu64 " ns\n", td);

  return 0;
}
