#include <cpers/ptuia.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <cpers/vals.h>
#include <cpers/time.h>
#include <cpers/pcmc.h>

struct ptuia
{
  pcmc_t *pcmc;
  void *data;
  tui_init_t *tui_init;
  tui_close_t *tui_close;
  tui_process_t *tui_process;
  tui_draw_t *tui_draw;
  size_t components_num;
  pcmc_t **components;
};

ptuia_t *init_ptuia(size_t data_struct_size, tui_init_t *tui_init, tui_close_t *tui_close, tui_process_t *tui_process, tui_draw_t *tui_draw, size_t components_num, coord_t *component_sizes)
{
  ptuia_t *self = malloc(sizeof(ptuia_t));
  if (self == NULL)
    return NULL;

  *self = (ptuia_t){
      .data = malloc(data_struct_size),
      .tui_init = tui_init,
      .tui_close = tui_close,
      .tui_process = tui_process,
      .tui_draw = tui_draw,
      .pcmc = init_pcmc(get_terminal_size()),
      .components_num = components_num,
      .components = malloc(sizeof(pcmc_t *) * components_num)
    };

  if (self->data == NULL || self->pcmc == NULL || self->components == NULL)
  {
    free_ptuia(self);
    return NULL;
  }

  coord_t size = pcmc_get_size(self->pcmc);

  for (size_t i = 0; i < components_num; ++i)
  {
    coord_t c_sz = component_sizes[i];
    if (c_sz.x <= 0)
      c_sz.x += size.x;
    if (c_sz.y <= 0)
      c_sz.y += size.y;

    self->components[i] = init_pcmc(c_sz);
    if (self->components[i] == NULL)
    {
      free_ptuia(self);
      return NULL;
    }
  }

  return self;
}

void free_ptuia(ptuia_t *self)
{
  if (self == NULL)
    return;

  if (self->data != NULL)
    free(self->data);
  if (self->pcmc != NULL)
    free_pcmc(self->pcmc);
  if (self->components != NULL)
  {
    for (size_t i = 0; i < self->components_num; ++i)
      if (self->components[i])
        free_pcmc(self->components[i]);
    free(self->components);
  }
  free(self);
}

signed int ptuia_run_with_stdio_loop(void *ctx)
{
  ptuia_t *self = ctx;

  signed int tui_process_code;
  tuiu_input_t input;

  if (self->tui_init(self->pcmc, self->data, self->components) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  while (tui_process_code != PTUIA_BREAK)
  {
    self->tui_draw(self->pcmc, self->data, self->components);

    tuiu_clear_stdout();
    pcmc_print_raw(self->pcmc, stdout);

    sleep_ms(30.0);

    input = tuiu_read_stdin();

    tui_process_code = self->tui_process(self->pcmc, self->data, (const pcmc_t **)self->components, input);
  }

  self->tui_close(self->data);

  pcmc_reset(self->pcmc);
  for (size_t i = 0; i < self->components_num; ++i)
    pcmc_reset(self->components[i]);

  return EXIT_SUCCESS;
}

signed int ptuia_run_with_stdio(ptuia_t *self)
{
  tuiu_call_with_tui_mode_in_stdio(ptuia_run_with_stdio_loop, self);
}
