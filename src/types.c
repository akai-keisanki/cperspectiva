#include <cpers/types.h>

coord_t mkcoord(signed long int x, size_t y)
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

coord_t mul_coord(const coord_t coord, const signed long int n)
{
  return mkcoord(coord.x * n, coord.y * n);
}

coord_t div_coord(const coord_t coord, const signed long int n)
{
  return mkcoord(coord.x / n, coord.y / n);
}

void pers_swap_li(size_t *a, size_t *b)
{
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
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
