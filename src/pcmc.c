#include <cpers/pcmc.h>
#include <stdlib.h>
#include <string.h>

struct pcmc
{
  coord_t size;
  char *matrix;
  char *background;
  bool *locked;
};

pcmc_t *init_pcmc(coord_t size)
{
  pcmc_t *self = malloc(sizeof(struct pcmc));
  if (self == NULL)
    return NULL;

  size_t cell_count = size.x * size.y;

  *self = (pcmc_t){
      .size = size,
      .matrix = calloc(cell_count, sizeof(char)),
      .background = malloc(sizeof(char) * cell_count),
      .locked = calloc(cell_count, sizeof(bool))
    };
  if (self->matrix == NULL || self->background == NULL || self->locked == NULL)
  {
    free_pcmc(self);
    return NULL;
  }

  for (size_t i = 0; i < cell_count; ++i)
    self->background[i] = ' ';

  return self;
}

void free_pcmc(pcmc_t *self)
{
  if (self == NULL)
    return;

  if (self->matrix != NULL)
    free(self->matrix);
  if (self->background != NULL)
    free(self->background);
  if (self->locked != NULL)
    free(self->locked);
  free(self);
}

coord_t pcmc_get_size(const pcmc_t *self)
{
  return self->size;
}

size_t pcmc_p2i(const pcmc_t *self, coord_t pos)
{
  pos = pcmc_limit_pos(self, pos);

  return (pos.x - 1) + (pos.y - 1) * self->size.x;
}

coord_t pcmc_limit_pos(const pcmc_t *self, coord_t pos)
{
  if (pos.x > self->size.x)
    pos.x = self->size.x;

  if (pos.x <= 0)
    pos.x = 1;

  if (pos.y > self->size.y)
    pos.y = self->size.y;

  if (pos.y <= 0)
    pos.y = 1;

  return pos;
}

char pcmc_get_at(const pcmc_t *self, coord_t pos)
{
  pos = pcmc_limit_pos(self, pos);

  return self->matrix[pcmc_p2i(self, pos)];
}

char pcmc_get_display_at(const pcmc_t *self, coord_t pos)
{
  char c;

  if (!(c = pcmc_get_at(self, pos)))
    c = self->background[pcmc_p2i(self, pos)];

  return c;
}

void pcmc_set_at(pcmc_t *self, coord_t pos, const char c)
{
  pos = pcmc_limit_pos(self, pos);

  if (!self->locked[pcmc_p2i(self, pos)])
    self->matrix[pcmc_p2i(self, pos)] = c;
}

void pcmc_fill(pcmc_t *self, char c)
{
  for (size_t x = 1; x <= self->size.x; ++x)
    for (size_t y = 1; y <= self->size.y; ++y)
      pcmc_set_at(self, mkcoord(x, y), c);
}

void pcmc_lock_area(pcmc_t *self, coord_t area_begin, coord_t area_end, const bool lock)
{
  area_begin = pcmc_limit_pos(self, area_begin);
  area_end = pcmc_limit_pos(self, area_end);

  for (size_t x = area_begin.x; x <= area_end.x; ++x)
    for (size_t y = area_begin.y; y <= area_end.y; ++y)
      self->locked[pcmc_p2i(self, mkcoord(x, y))] = lock;
}

void pcmc_print(const pcmc_t *self, FILE *stream)
{
  setvbuf(stream, NULL, _IOFBF, (self->size.x + 1) * (self->size.y + 1));

  for (size_t y = 0; y <= self->size.y; ++y)
  {
    for (size_t x = 0; x <= self->size.x; ++x)
      fprintf(stream, "%c", pcmc_get_display_at(self, mkcoord(x, y)));
    fputc('\n', stream);
  }

  fflush(stream);

  setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
}

void pcmc_print_raw(const pcmc_t *self, FILE *stream)
{
  setvbuf(stream, NULL, _IOFBF, (self->size.x + 1) * (self->size.y + 1));

  for (size_t x = 1; x <= self->size.x; ++x)
    for (size_t y = 1; y <= self->size.y; ++y)
      fprintf(stream, "\x1B[%zu;%zuH%c", y, x, pcmc_get_display_at(self, mkcoord(x, y)));

  fflush(stream);
}

void pcmc_set_self_as_background(pcmc_t *self)
{
  for (size_t x = 1; x <= self->size.x; ++x)
    for (size_t y = 1; y <= self->size.y; ++y)
      self->background[pcmc_p2i(self, mkcoord(x, y))] = pcmc_get_display_at(self, mkcoord(x, y));
}

void pcmc_reset(pcmc_t *self)
{
  size_t cell_count = self->size.x * self->size.y;

  for (size_t i = 0; i < cell_count; ++i)
  {
    self->matrix[i] = '\0';
    self->background[i] = ' ';
    self->locked[i] = false;
  }
}
