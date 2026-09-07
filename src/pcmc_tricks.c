#include <cpers/pcmc_tricks.h>

#include <stddef.h>
#include <stdlib.h>
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

void pcmct_draw_polygon_fill(pcmc_t *pcmc, const unsigned int vertex_count, const coord_t *vertices, const char c)
{
  signed long int cp;
  unsigned long int ni;
  bool cp_sign, in_polygon;
  coord_t side_vec, p_vec;
  coord_t minc = vertices[0];
  coord_t maxc = vertices[0];

  for (unsigned int i = 0; i < vertex_count; ++i)
  {
    minc.x = pers_min_li(minc.x, vertices[i].x);
    minc.y = pers_min_li(minc.y, vertices[i].y);
    maxc.x = pers_max_li(maxc.x, vertices[i].x);
    maxc.y = pers_max_li(maxc.y, vertices[i].y);
  }

  for (signed long int x = minc.x; x <= maxc.x; ++x)
    for (signed long int y = minc.y; y <= maxc.y; ++y)
    {
      in_polygon = true;
      for (unsigned int i = 0; i < vertex_count; ++i)
      {
        ni = i == vertex_count - 1 ? 0 : i + 1;

        side_vec = mkcoord(vertices[ni].x - vertices[i].x, vertices[ni].y - vertices[i].y);
        p_vec = mkcoord(x - vertices[i].x, y - vertices[i].y);

        cp = coords_cross_product_2d(side_vec, p_vec);

        if (i == 0)
        {
          cp_sign = cp < 0;
          continue;
        }
        if (cp == 0 || cp_sign != (cp < 0))
        {
          in_polygon = false;
          break;
        }
      }
      if (in_polygon)
        pcmc_set_at(pcmc, mkcoord(x, y), c);
    }
}

void pcmct_draw_polygon_border(pcmc_t *pcmc, const unsigned int vertex_count, const coord_t *vertices, const char c)
{
  signed long int cp;
  unsigned long int ni;
  bool cp_sign, in_border;
  coord_t side_vec, p_vec;
  coord_t minc = vertices[0];
  coord_t maxc = vertices[0];

  for (unsigned int i = 0; i < vertex_count; ++i)
  {
    minc.x = pers_min_li(minc.x, vertices[i].x);
    minc.y = pers_min_li(minc.y, vertices[i].y);
    maxc.x = pers_max_li(maxc.x, vertices[i].x);
    maxc.y = pers_max_li(maxc.y, vertices[i].y);
  }

  for (signed long int x = minc.x; x <= maxc.x; ++x)
    for (signed long int y = minc.y; y <= maxc.y; ++y)
    {
      in_border = false;
      for (unsigned long int i = 0; i < vertex_count; ++i)
      {
        ni = i == vertex_count - 1 ? 0 : i + 1;

        side_vec = mkcoord(vertices[ni].x - vertices[i].x, vertices[ni].y - vertices[i].y);
        p_vec = mkcoord(x - vertices[i].x, y - vertices[i].y);

        cp = coords_cross_product_2d(side_vec, p_vec);

        if (cp * cp <= coord_modulo_square(side_vec))
        {
          in_border = true;
          continue;
        }
        if (i == 0)
        {
          cp_sign = cp < 0;
          continue;
        }
        if (cp_sign != (cp < 0))
        {
          in_border = false;
          break;
        }
      }
      if (in_border)
        pcmc_set_at(pcmc, mkcoord(x, y), c);
    }
}

void pcmct_draw_polygon(pcmc_t *pcmc, const unsigned int vertex_count, const coord_t *vertices, const char c)
{
  pcmct_draw_polygon_fill(pcmc, vertex_count, vertices, c);
  pcmct_draw_polygon_border(pcmc, vertex_count, vertices, c);
}

void pcmct_draw_line_with_slope(pcmc_t *pcmc, coord_t begin, coord_t end, const char c)
{
  unsigned long int per;

  order_coords(&begin, &end);

  if (end.x == begin.x)
  {
    for (size_t y = begin.y; y < end.y; ++y)
      pcmc_set_at(pcmc, mkcoord(begin.x, y), c);

    return;
  }

  if (end.y == begin.y)
  {
    for (size_t x = begin.x; x < end.x; ++x)
      pcmc_set_at(pcmc, mkcoord(x, begin.y), c);

    return;
  }

  double slope = ((double)end.y - begin.y) / ((double)end.x - begin.x);

  for (size_t x = begin.x; x < end.x; ++x)
  {
    per = round(((double)x - begin.x) * slope + begin.y);
    pcmc_set_at(pcmc, mkcoord(x, per), c);
  }

  slope = ((double)end.x - begin.x) / ((double)end.y - begin.y);

  for (size_t y = begin.y; y != end.y; begin.y < end.y ? ++y : --y)
  {
    per = round(((double)y - begin.y) * slope + begin.x);
    pcmc_set_at(pcmc, mkcoord(per, y), c);
  }
}

void pcmct_draw_line(pcmc_t *pcmc, coord_t begin, coord_t end, const char c)
{
  pcmct_draw_polygon_border(pcmc, 2, (const coord_t[2]){begin, end}, c);
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
