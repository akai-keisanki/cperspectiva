#ifndef PERSPECTIVA_TYPES_H
#define PERSPECTIVA_TYPES_H

#include <stddef.h>
#include <stdbool.h>

struct coord;
typedef struct coord coord_t;

coord_t mkcoord(signed long int x, signed long int y);
coord_t limit_coord(coord_t coord, coord_t maxs);

coord_t sum_coords(coord_t a, coord_t b);
coord_t sub_coords(coord_t a, coord_t b);
coord_t mul_coord(coord_t coord, signed long int n);
coord_t div_coord(coord_t coord, signed long int n);

void pers_swap_li(size_t *a, size_t *b);

void swap_coords(coord_t *a, coord_t *b);
void order_coords(coord_t *a, coord_t *b);
void order_rectangle_limit_coords(coord_t *a, coord_t *b);

struct coord
{
  signed long int x;
  signed long int y;
};

#endif
