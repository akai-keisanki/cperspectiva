#ifndef PERSPECTIVA_TYPES_H
#define PERSPECTIVA_TYPES_H

#include <stddef.h>
#include <stdbool.h>

struct coord;
typedef struct coord coord_t;

coord_t mkcoord(size_t x, size_t y);
coord_t limit_coord(coord_t coord, coord_t maxs);

coord_t sum_coords(coord_t a, coord_t b);
coord_t sub_coords(coord_t a, coord_t b);
coord_t mul_coord(coord_t coord, size_t n);
coord_t div_coord(coord_t coord, size_t n);

struct coord
{
  size_t x;
  size_t y;
};

#endif
