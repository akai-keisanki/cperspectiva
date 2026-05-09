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
  size_t cell_count = size.x * size.y;
  self->size = size;
  self->matrix = calloc(cell_count, sizeof(char));
  self->background = malloc(sizeof(char) * cell_count);
  for (size_t i = 0; i < cell_count; ++i) self->background[i] = ' ';
  self->locked = calloc(cell_count, sizeof(bool));
  return self;
}

size_t pcmc_p2i(const pcmc_t *self, coord_t pos)
{
  return pos.x + pos.y * self->size.x;
}

coord_t pcmc_limit_pos(const pcmc_t *self, coord_t pos)
{
  return limit_coord(pos, self->size);
}

char *pcmc_ptr_at(pcmc_t *self, coord_t pos)
{
  pos = pcmc_limit_pos(self, pos);

  return &self->matrix[pcmc_p2i(self, pos)];
}

char pcmc_get_at(const pcmc_t *self, coord_t pos)
{
  pos = pcmc_limit_pos(self, pos);

  return self->matrix[pcmc_p2i(self, pos)];
}

void pcmc_set_at(pcmc_t *self, coord_t pos, const char c)
{
  pos = pcmc_limit_pos(self, pos);

  self->matrix[pcmc_p2i(self, pos)] = c;
}

void pcmc_fill(pcmc_t *self, char c)
{
  for (size_t i = 0; i < self->size.x * self->size.y; ++i)
    self->matrix[i] = c;
}

void pcmc_lock_area(pcmc_t *self, coord_t area_begin, coord_t area_end, const bool lock)
{
  area_begin = pcmc_limit_pos(self, area_begin);
  area_end = pcmc_limit_pos(self, area_end);

  for (size_t x = area_begin.x; x < area_end.x; ++x)
    for (size_t y = area_begin.y; y < area_end.y; ++y)
      self->locked[pcmc_p2i(self, mkcoord(x, y))] = lock;
}

void pcmc_print(const pcmc_t *self, FILE *stream)
{
  char c;

  setvbuf(stream, NULL, _IOFBF, (self->size.x + 1) * (self->size.y + 1));

  for (size_t y = 0; y < self->size.y; ++y)
  {
    for (size_t x = 0; x < self->size.x; ++x)
      if (c = pcmc_get_at(self, mkcoord(x, y)))
        fprintf(stream, "%c", c);
      else
        fprintf(stream, "%c", self->background[pcmc_p2i(self, mkcoord(x, y))]);
    fputc('\n', stream);
  }

  fflush(stream);
}

void pcmc_set_self_as_background(pcmc_t *self)
{
  memcpy(self->background, self->matrix, sizeof(char) * self->size.x * self->size.y);
}

void free_pcmc(pcmc_t *self)
{
  free(self->matrix);
  free(self->background);
  free(self->locked);
  free(self);
}
