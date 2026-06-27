/* 'simple_drawer' example Perspectiva (beta) application */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cpers/types.h>
#include <cpers/vals.h>
#include <cpers/pcmc.h>
#include <cpers/ptuia.h>
#include <cpers/pcmc_tricks.h>

struct tui_data
{
  coord_t p;
  char in_1stb;
  bool cursor_state;
  bool save;
  FILE *save_f;
};

signed int tui_init(pcmc_t *pcmc, void *tui_data, pcmc_t **components)
{
  struct tui_data *data = tui_data;

  const coord_t c0_sz = pcmc_get_size(components[0]);

  *data = (struct tui_data){
      .p = div_coord(c0_sz, 2),
      .in_1stb = '\0',
      .cursor_state = 0,
      .save_f = fopen("paint.txt", "w"),
      .save = false
    };

  pcmct_fill_background(components[0], ' ');

  return EXIT_SUCCESS;
}

void tui_close(void *tui_data)
{
  struct tui_data *data = tui_data;

  fclose(data->save_f);
}

signed int tui_process(const pcmc_t *pcmc, void *tui_data, const pcmc_t **components, ptuia_input_t input)
{
  struct tui_data *data = tui_data;
  coord_t drct = mkcoord(0, 0);

  data->save = false;

  data->in_1stb = '\0';
  
  if (!strcmp(input.bytes, "\x1B[A"))
    drct = mkcoord(0, -1);
  else if (!strcmp(input.bytes, "\x1B[B"))
    drct = mkcoord(0, 1);
  else if (!strcmp(input.bytes, "\x1B[C"))
    drct = mkcoord(1, 0);
  else if (!strcmp(input.bytes, "\x1B[D"))
    drct = mkcoord(-1, 0);
  else if (!strcmp(input.bytes, "\n"))
    data->save = true;
  else
    data->in_1stb = input.bytes[0];

  data->p = sum_coords(data->p, drct);
  limit_coord(data->p, pcmc_get_size(components[0]));

  if (!strcmp(input.bytes, "\x1B"))
    return PTUIA_BREAK;

  const coord_t c0_sz = pcmc_get_size(components[0]);

  if (data->p.x < 1)
    data->p.x = c0_sz.x;
  if (data->p.y < 1)
    data->p.y = c0_sz.y;
  if (data->p.x > c0_sz.x)
    data->p.x = 1;
  if (data->p.y > c0_sz.y)
    data->p.y = 1;

  data->cursor_state = !data->cursor_state;

  return PTUIA_CONTINUE;
}

void tui_draw(pcmc_t *pcmc, const void *tui_data, pcmc_t **components)
{
  const struct tui_data *data = tui_data;

  // component [0] (canvas)

  pcmct_fill(components[0], '\0');

  if (data->save)
    pcmc_print(components[0], data->save_f);

  if (data->in_1stb)
    pcmc_set_background_at(components[0], data->p, data->in_1stb);

  if (data->cursor_state)
    pcmc_set_at(components[0], data->p, '#');

  // component [1] (footer)

  const coord_t c1_sz = pcmc_get_size(components[1]);

  pcmct_fill(components[1], ' ');
  pcmct_fill_area(components[1], mkcoord(1, 1), mkcoord(c1_sz.x, 1), '_');
  pcmct_write_str(components[1], mkcoord(1, 2), "Press ESC to exit. Use the arrow keys to move the cursor, type to paint. Enter to save to paint.txt", mkcoord(1, 0));

  // draw components
  
  const coord_t size = pcmc_get_size(pcmc);

  pcmct_pcmc2pcmc_display(pcmc, mkcoord(1, 1), mkcoord(size.x, size.y - 2), components[0]);
  pcmct_pcmc2pcmc_foreground(pcmc, mkcoord(1, size.y - 1), size, components[1]);
}

signed int main(void)
{
  signed int exit_code = EXIT_SUCCESS;

  ptuia_t *ptuia = init_ptuia(sizeof(struct tui_data), tui_init, tui_close, tui_process, tui_draw, 2, (coord_t[2]){mkcoord(0, -2), mkcoord(0, 2)});

  if (ptuia == NULL)
  {
    puts("Could not start PTUIA.");

    return EXIT_FAILURE;
  }

  if (ptuia_run_with_stdio(ptuia) != EXIT_SUCCESS)
  {
    puts("TUI error!");

    exit_code = EXIT_FAILURE;
  }

  free_ptuia(ptuia);

  return exit_code;
}
