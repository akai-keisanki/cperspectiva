#include <cpers/types.h>

coord_t mkcoord(size_t x, size_t y)
{
  return (coord_t){ .x = x, .y = y };
}

coord_t limit_coord(coord_t coord, coord_t maxs)
{
  if (coord.x > maxs.x) coord.x = maxs.x;
  if (coord.y > maxs.y) coord.y = maxs.y;

  return coord;
}

coord_t sum_coords(const coord_t a, const coord_t b)
{
  return mkcoord(a.x + b.x, a.y + b.y);
}

coord_t sub_coords(const coord_t a, const coord_t b)
{
  return mkcoord(a.x - b.x, a.y - b.y);
}

coord_t mul_coord(const coord_t coord, const size_t n)
{
  return mkcoord(coord.x * n, coord.y * n);
}

coord_t div_coord(const coord_t coord, const size_t n)
{
  return mkcoord(coord.x / n, coord.y / n);
}
