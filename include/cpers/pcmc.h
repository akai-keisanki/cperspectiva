#ifndef PERSPECTIVA_PCMC_H
#define PERSPECTIVA_PCMC_H

/* Perspectiva Character Matrix Container */

#include <cpers/types.h>
#include <stdio.h>

struct pcmc;
typedef struct pcmc pcmc_t;

pcmc_t *init_pcmc(coord_t size);
void free_pcmc(pcmc_t *self);

coord_t pcmc_get_size(const pcmc_t *self);

coord_t pcmc_limit_pos(const pcmc_t *self, coord_t pos);

char pcmc_get_at(const pcmc_t *self, coord_t pos);
char pcmc_get_display_at(const pcmc_t *self, coord_t pos);
void pcmc_set_at(pcmc_t *self, coord_t pos, char c);

void pcmc_fill(pcmc_t *self, char c);

void pcmc_lock_area(pcmc_t *self, coord_t area_begin, coord_t area_end, bool lock);

void pcmc_print(const pcmc_t *self, FILE *stream);
void pcmc_print_raw(const pcmc_t *self, FILE *stream);

void pcmc_set_self_as_background(pcmc_t *self);

void pcmc_reset(pcmc_t *self);

#endif
