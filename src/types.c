#include <cpers/types.h>

#include <math.h>

void pers_swap_li(signed long int *a, signed long int *b)
{
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
}

signed long int pers_min_li(const signed long int a, const signed long int b)
{
  return a < b ? a : b;
}

signed long int pers_max_li(const signed long int a, const signed long int b)
{
  return a < b ? b : a;
}

double pers_no_zero_lf(const double x)
{
  return x < 0.05 ? 0.05 : x;
}

coord_t mkcoord(const signed long int x, const signed long int y)
{
  return (coord_t){ .x = x, .y = y };
}

coord_t limit_coord(coord_t coord, const coord_t maxs)
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

coord_t mul_coord(const coord_t coord, const signed long int n)
{
  return mkcoord(coord.x * n, coord.y * n);
}

coord_t div_coord(const coord_t coord, const signed long int n)
{
  return mkcoord(coord.x / n, coord.y / n);
}

void swap_coords(coord_t *a, coord_t *b)
{
  pers_swap_li(&a->x, &b->x);
  pers_swap_li(&a->y, &b->y);
}

void order_coords(coord_t *a, coord_t *b)
{
  if (a->x > b->x)
  {
    swap_coords(a, b);

    return;
  }

  if (a->x == b->x && a->y > b->y)
  {
    swap_coords(a, b);
    
    return;
  }
}

void order_rectangle_limit_coords(coord_t *a, coord_t *b)
{
  if (a->x > b->x)
    pers_swap_li(&a->x, &b->x);

  if (a->y > b->y)
    pers_swap_li(&a->y, &b->y);
}

signed long int coord_modulo_square(const coord_t coord)
{
  return coord.x * coord.x + coord.y * coord.y;
}

double coord_modulo(const coord_t coord)
{
  return sqrt(coord_modulo_square(coord));
}

signed long int coords_dot_product(const coord_t a, const coord_t b)
{
  return a.x * b.x + a.y * b.y;
}

signed long int coords_cross_product_2d(const coord_t a, const coord_t b)
{
  return a.x * b.y - a.y * b.x;
}

coord_t coord_transform(const coord_t a, const coord_t t1, const coord_t t2)
{
  return sum_coords(mul_coord(t1, a.x), mul_coord(t2, a.y));
}

