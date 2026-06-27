#ifndef PERSPECTIVA_PCMC_TRICKS_H
#define PERSPECTIVA_PCMC_TRICKS_H

#include <cpers/types.h>
#include <cpers/pcmc.h>

void pcmct_write_str(pcmc_t *pcmc, coord_t begin, const char *str, coord_t dir);

void pcmct_fill_area(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, char c);
void pcmct_frame_area(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, char c);
void pcmct_draw_line(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, char c);
void pcmct_draw_line_with_slope(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, char c);

void pcmct_pcmc2pcmc(pcmc_t *pcmc, coord_t area_begin, coord_t area_end, const pcmc_t *src);

#endif
