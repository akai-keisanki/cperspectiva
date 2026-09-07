#ifndef PERSPECTIVA_TYPES_H
#define PERSPECTIVA_TYPES_H

#include <stddef.h>
#include <stdbool.h>

void pers_swap_li(signed long int *a, signed long int *b);
signed long int pers_min_li(signed long int a, signed long int b);
signed long int pers_max_li(signed long int a, signed long int b);
double pers_no_zero_lf(double x);

struct coord;
typedef struct coord coord_t;

coord_t mkcoord(signed long int x, signed long int y);
coord_t limit_coord(coord_t coord, coord_t maxs);

coord_t sum_coords(coord_t a, coord_t b);
coord_t sub_coords(coord_t a, coord_t b);
coord_t mul_coord(coord_t coord, signed long int n);
coord_t div_coord(coord_t coord, signed long int n);

void swap_coords(coord_t *a, coord_t *b);
void order_coords(coord_t *a, coord_t *b);
void order_rectangle_limit_coords(coord_t *a, coord_t *b);

signed long int coord_modulo_square(coord_t coord);
double coord_modulo(coord_t coord);
signed long int coords_dot_product(coord_t a, coord_t b);
signed long int coords_cross_product_2d(coord_t a, coord_t b);

struct coord
{
  signed long int x;
  signed long int y;
};

#endif
