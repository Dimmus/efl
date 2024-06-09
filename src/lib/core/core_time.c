#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#define EFL_LOOP_PROTECTED

#include <stdlib.h>
#include <sys/time.h>

#if defined(__APPLE__) && defined(__MACH__)
#  include <mach/mach_time.h>
#endif

#include <time.h>

#ifdef _WIN32
#  include <evil_private.h> /* gettimeofday */
#endif

#include "Efl_Core.h"
#include "core_private.h"

#if defined(_WIN32)
static LONGLONG _core_time_freq;
#elif defined(HAVE_CLOCK_GETTIME)
static clockid_t _core_time_clock_id;
static Efl_Bool  _core_time_got_clock_id = EFL_FALSE;
#elif defined(__APPLE__) && defined(__MACH__)
static double _core_time_clock_conversion = 1e-9;
#endif

EAPI double
core_time_get(void)
{
#ifdef _WIN32
  LARGE_INTEGER count;

  QueryPerformanceCounter(&count);
  return (double)count.QuadPart / (double)_core_time_freq;
#elif defined(HAVE_CLOCK_GETTIME)
  struct timespec t;

  if (EINA_UNLIKELY(!_core_time_got_clock_id)) return core_time_unix_get();

  if (EINA_UNLIKELY(clock_gettime(_core_time_clock_id, &t)))
  {
    CRI("Cannot get current time");
    return 0.0;
  }

  return (double)t.tv_sec + (((double)t.tv_nsec) / 1000000000.0);
#elif defined(__APPLE__) && defined(__MACH__)
  return _core_time_clock_conversion * (double)mach_absolute_time();
#else
  return core_time_unix_get();
#endif
}

EAPI double
core_time_unix_get(void)
{
#ifdef HAVE_GETTIMEOFDAY
  struct timeval timev;

  gettimeofday(&timev, NULL);
  return (double)timev.tv_sec + (((double)timev.tv_usec) / 1000000);
#else
#  error "Your platform isn't supported yet"
#endif
}

EAPI double
core_loop_time_get(void)
{
  return efl_loop_time_get(ML_OBJ);
}

EAPI void
core_loop_time_set(double t)
{
  double tnow   = core_time_get();
  double tdelta = t - tnow;

  if (tdelta > 0.0)
  {
    fprintf(stderr,
            "Eccore: Trying to set loop time (%1.8f) %1.8fs too far in the "
            "future\n",
            t,
            tdelta);
    return;
  }
  efl_loop_time_set(ML_OBJ, t);
}

/*-********************   Internal methods   ********************************/

/* TODO: Documentation says "All  implementations  support  the  system-wide
 * real-time clock, which is identified by CLOCK_REALTIME. Check if the fallback
 * to unix time (without specifying the resolution) might be removed
 */
void
_core_time_init(void)
{
#if defined(_WIN32)
  LARGE_INTEGER freq;

  QueryPerformanceFrequency(&freq);
  _core_time_freq = freq.QuadPart;
#elif defined(HAVE_CLOCK_GETTIME)
  struct timespec t;

  if (_core_time_got_clock_id) return;

  if (!clock_gettime(CLOCK_MONOTONIC, &t))
  {
    _core_time_clock_id     = CLOCK_MONOTONIC;
    _core_time_got_clock_id = EFL_TRUE;
    DBG("using CLOCK_MONOTONIC");
  }
  else if (!clock_gettime(CLOCK_REALTIME, &t))
  {
        // may go backwards
    _core_time_clock_id     = CLOCK_REALTIME;
    _core_time_got_clock_id = EFL_TRUE;
    WRN("CLOCK_MONOTONIC not available. Fallback to CLOCK_REALTIME");
  }
  else
    CRI("Cannot get a valid clock_gettime() clock id! Fallback to unix "
        "time");
#else
#  if defined(__APPLE__) && defined(__MACH__)
  mach_timebase_info_data_t info;
  kern_return_t             err = mach_timebase_info(&info);
  if (err == 0)
      _core_time_clock_conversion =
      1e-9 * (double)info.numer / (double)info.denom;
  else WRN("Unable to get timebase info. Fallback to nanoseconds");
#  else
#    warning "Your platform isn't supported yet"
  CRI("Platform does not support clock_gettime. Fallback to unix time");
#  endif
#endif
  core_loop_time_set(core_time_get());
}
