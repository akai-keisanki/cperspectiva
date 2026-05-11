#include <cpers/time.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

void sleep_ms(double milliseconds)
{
  unsigned long long int nsi = fmod(milliseconds * 1e6, 1e9);
  unsigned long long int si = milliseconds * 1e-3;
  
  struct timespec timespec = { si,  nsi };
  nanosleep(&timespec, NULL);
}
