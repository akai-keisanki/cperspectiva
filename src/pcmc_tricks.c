#include <cpers/pcmc_tricks.h>

#include <stddef.h>

void pcmct_write_str(pcmc_t *pcmc, const coord_t begin, const char *str, const coord_t dir)
{
  if (!str) return;

  coord_t p = begin;

  for (size_t i = 0; str[i]; ++i)
  {
    pcmc_set_at(pcmc, p, str[i]);
    p = sum_coords(p, dir);
  }
}
