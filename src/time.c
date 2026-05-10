#include <cpers/time.h>
#include <time.h>
#include <stdio.h>

void sleep_ms(double milliseconds)
{
  unsigned long long int nsi = milliseconds * 1e6;
  unsigned long long int si = nsi / 1e9;
  nsi %= (unsigned long long int)1e9;
  
  struct timespec timespec = { si,  nsi };
  nanosleep(&timespec, NULL);
}
