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
