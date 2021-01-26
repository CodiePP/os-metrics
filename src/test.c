
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os-metrics.h"

int main (int argc, char **argv) {

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
  if ((res = osm_read_proc_stats(&_proc_stats, pid)) == 0)
  {
    printf("os-metrics on process with pid %d\n  resident size = %ld bytes\n  user time = %ld ns\n  system time = %ld ns\n  total cpu time = %ld ns\n", pid, _proc_stats._resident_memory, _proc_stats._user_time_ns, _proc_stats._system_time_ns, _proc_stats._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (1): %d!\n", res);
  }

  struct OSM_Sys_stats _sys_stats;
  if ((res = osm_read_sys_stats(&_sys_stats)) == 0)
  {
    printf("os-metrics on system\n  user time = %ld ns\n  system time = %ld ns\n  total cpu time = %ld ns\n", _sys_stats._user_time_ns, _sys_stats._system_time_ns, _sys_stats._cpu_time_ns);
  }
  else
  {
    printf("something went wrong (2): %d!\n", res);
  }

  return 0;
}
