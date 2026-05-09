#include <cpers/time.h>
#include <time.h>

void sleep_ms(double milliseconds)
{
  struct timespec timespec = { 0, milliseconds * 1e6 };
  nanosleep(&timespec, NULL);
}
