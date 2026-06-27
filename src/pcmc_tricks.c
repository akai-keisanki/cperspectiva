#include <cpers/pcmc_tricks.h>

#include <stddef.h>
#include <math.h>

void pcmct_write_str(pcmc_t *pcmc, const coord_t begin, const char *str, const coord_t dir)
{
  if (!str) return;

  coord_t p = begin;

  for (size_t i = 0; str[i]; ++i)
  {
    p = pcmc_limit_pos(pcmc, p);

    pcmc_set_at(pcmc, p, str[i]);
    p = sum_coords(p, dir);
  }
}

void pcmct_fill(pcmc_t *pcmc, char c)
{
  coord_t size = pcmc_get_size(pcmc);

  for (size_t x = 1; x <= size.x; ++x)
    for (size_t y = 1; y <= size.y; ++y)
      pcmc_set_at(pcmc, mkcoord(x, y), c);
}

void pcmct_fill_background(pcmc_t *pcmc, char c)
{
  coord_t size = pcmc_get_size(pcmc);

  for (size_t x = 1; x <= size.x; ++x)
    for (size_t y = 1; y <= size.y; ++y)
      pcmc_set_background_at(pcmc, mkcoord(x, y), c);
}

void pcmct_set_self_as_background(pcmc_t *pcmc)
{
  coord_t size = pcmc_get_size(pcmc);

  for (size_t x = 1; x <= size.x; ++x)
    for (size_t y = 1; y <= size.y; ++y)
      pcmc_set_background_at(pcmc, mkcoord(x, y), pcmc_get_display_at(pcmc, mkcoord(x, y)));
}

void pcmct_fill_area(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const char c)
{
  area_begin = pcmc_limit_pos(pcmc, area_begin);
  area_end = pcmc_limit_pos(pcmc, area_end);
  order_rectangle_limit_coords(&area_begin, &area_end);

  for (size_t x = area_begin.x; x <= area_end.x; ++x)
    for (size_t y = area_begin.y; y <= area_end.y; ++y)
      pcmc_set_at(pcmc, mkcoord(x, y), c);
}

void pcmct_frame_area(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const char c)
{
  area_begin = pcmc_limit_pos(pcmc, area_begin);
  area_end = pcmc_limit_pos(pcmc, area_end);
  order_rectangle_limit_coords(&area_begin, &area_end);

  for (size_t x = area_begin.x; x <= area_end.x; ++x)
  {
    pcmc_set_at(pcmc, mkcoord(x, area_begin.y), c);
    pcmc_set_at(pcmc, mkcoord(x, area_end.y), c);
  }

  for (size_t y = area_begin.y; y <= area_end.y; ++y)
  {
    pcmc_set_at(pcmc, mkcoord(area_begin.x, y), c);
    pcmc_set_at(pcmc, mkcoord(area_end.x, y), c);
  }
}

void pcmct_draw_line(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const char c)
{
  // TODO: WIP
  
  pcmct_draw_line_with_slope(pcmc, area_begin, area_end, c);
}

void pcmct_draw_line_with_slope(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const char c)
{
  unsigned long int per;

  order_coords(&area_begin, &area_end);

  if (area_end.x == area_begin.x)
  {
    for (size_t y = area_begin.y; y < area_end.y; ++y)
      pcmc_set_at(pcmc, mkcoord(area_begin.x, y), c);

    return;
  }

  if (area_end.y == area_begin.y)
  {
    for (size_t x = area_begin.x; x < area_end.x; ++x)
      pcmc_set_at(pcmc, mkcoord(x, area_begin.y), c);

    return;
  }

  double slope = ((double)area_end.y - area_begin.y) / ((double)area_end.x - area_begin.x);

  for (size_t x = area_begin.x; x < area_end.x; ++x)
  {
    per = round(((double)x - area_begin.x) * slope + area_begin.y);
    pcmc_set_at(pcmc, mkcoord(x, per), c);
  }

  slope = ((double)area_end.x - area_begin.x) / ((double)area_end.y - area_begin.y);

  for (size_t y = area_begin.y; y != area_end.y; area_begin.y < area_end.y ? ++y : --y)
  {
    per = round(((double)y - area_begin.y) * slope + area_begin.x);
    pcmc_set_at(pcmc, mkcoord(per, y), c);
  }
}

void pcmct_pcmc2pcmc_base(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const pcmc_t *src, char (*pcmc_get_at_f)(const pcmc_t *, coord_t))
{
  area_begin = pcmc_limit_pos(pcmc, area_begin);
  area_end = pcmc_limit_pos(pcmc, area_end);
  order_rectangle_limit_coords(&area_begin, &area_end);

  const coord_t src_sz = pcmc_get_size(src);
  
  if (area_end.x - area_begin.x > src_sz.x)
    area_end.x = area_begin.x + src_sz.x;
  if (area_end.y - area_begin.y > src_sz.y)
    area_end.y = area_begin.y + src_sz.y;

  for (size_t x = area_begin.x, x2 = 1; x <= area_end.x; (++x, ++x2))
    for (size_t y = area_begin.y, y2 = 1; y <= area_end.y; (++y, ++y2))
      pcmc_set_at(pcmc, mkcoord(x, y), pcmc_get_at_f(src, mkcoord(x2, y2)));
}

void pcmct_pcmc2pcmc_display(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const pcmc_t *src)
{
  pcmct_pcmc2pcmc_base(pcmc, area_begin, area_end, src, pcmc_get_display_at);
}

void pcmct_pcmc2pcmc_foreground(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const pcmc_t *src)
{
  pcmct_pcmc2pcmc_base(pcmc, area_begin, area_end, src, pcmc_get_at);
}
