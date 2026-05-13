#ifndef PERSPECTIVA_PTUIA_H
#define PERSPECTIVA_PTUIA_H

/* Perspectiva Terminal User Interface Abstracter */

#include <cpers/types.h>
#include <cpers/pcmc.h>
#include <stddef.h>

#define PTUIA_CONTINUE (signed int)0
#define PTUIA_BREAK (signed int)1

struct ptuia;
typedef struct ptuia ptuia_t;

struct ptuia_input
{
  size_t size;
  char *bytes;
};
typedef struct ptuia_input ptuia_input_t;

typedef signed int tui_init_t(pcmc_t *pcmc, void *tui_data);
typedef void tui_close_t(void *tui_data);
typedef signed int tui_process_t(const pcmc_t *pcmc, void *tui_data, ptuia_input_t input);
typedef void tui_draw_t(pcmc_t *pcmc, const void *tui_data);

ptuia_t *init_ptuia(size_t data_struct_size, tui_init_t *tui_init, tui_close_t *tui_close, tui_process_t *tui_process, tui_draw_t *tui_draw);

signed int ptuia_run_with_stdio(ptuia_t *self);

void free_ptuia(ptuia_t *self);

#endif

