/* 'moving_vector' example Perspectiva (beta) application */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cpers/full.h>

struct tui_data
{
  coord_t p;
  coord_t drct;
  char *input_bytes;
};

signed int tui_init(pcmc_t *pcmc, void *tui_data, pcmc_t **components)
{
  struct tui_data *data = tui_data;
  const coord_t c0_sz = pcmc_get_size(components[0]);

  *data = (struct tui_data){
      .p = div_coord(c0_sz, 2),
      .drct = mkcoord(0, 0),
      .input_bytes = malloc(sizeof(char) * 256)
    };

  if (data->input_bytes == NULL)
    return EXIT_FAILURE;

  data->input_bytes[0] = '\0';

  return EXIT_SUCCESS;
}

void tui_close(void *tui_data)
{
  struct tui_data *data = tui_data;

  free(data->input_bytes);
}

signed int tui_process(const pcmc_t *pcmc, void *tui_data, const pcmc_t **components, tuiu_input_t input)
{
  struct tui_data *data = tui_data;

  // process input

  if (!strcmp(input.bytes, "\x1B[A"))
    data->drct = mkcoord(0, -1);
  else if (!strcmp(input.bytes, "\x1B[B"))
    data->drct = mkcoord(0, 1);
  else if (!strcmp(input.bytes, "\x1B[C"))
    data->drct = mkcoord(2, 0);
  else if (!strcmp(input.bytes, "\x1B[D"))
    data->drct = mkcoord(-2, 0);

  if (!strcmp(input.bytes, "q"))
    return PTUIA_BREAK;

  // movement

  data->p = sum_coords(data->p, data->drct);

  const coord_t c0_sz = pcmc_get_size(components[0]);

  if (data->p.x < 1)
    data->p.x = c0_sz.x;
  if (data->p.y < 1)
    data->p.y = c0_sz.y;
  if (data->p.x > c0_sz.x)
    data->p.x = 1;
  if (data->p.y > c0_sz.y)
    data->p.y = 1;

  // store input for display

  strncpy(data->input_bytes, input.bytes, 255);
  data->input_bytes[255] = 0;

  return PTUIA_CONTINUE;
}

void tui_draw(pcmc_t *pcmc, const void *tui_data, pcmc_t **components)
{
  const struct tui_data *data = tui_data;

  // component [0] (canvas)

  const coord_t c0_sz = pcmc_get_size(components[0]);

  pcmct_fill(components[0], ' ');

  pcmct_draw_line(components[0], div_coord(c0_sz, 2), data->p, '#');

  // component [1] (footer)

  const coord_t c1_sz = pcmc_get_size(components[1]);

  pcmct_fill(components[1], ' ');
  pcmct_fill_area(components[1], mkcoord(1, 1), mkcoord(c1_sz.x, 1), '_');

  pcmct_write_str(components[1], mkcoord(1, 2), "Press q to exit. Use the arrow keys to move the vector.", mkcoord(1, 0));
  pcmct_write_str(components[1], mkcoord(1, 3), data->input_bytes, mkcoord(1, 0));

  // draw components
  
  const coord_t size = pcmc_get_size(pcmc);

  pcmct_pcmc2pcmc_display(pcmc, mkcoord(1, 1), mkcoord(size.x, size.y - 3), components[0]);
  pcmct_pcmc2pcmc_foreground(pcmc, mkcoord(1, size.y - 2), size, components[1]);
}

signed int main(void)
{
  signed int exit_code = EXIT_SUCCESS;
  ptuia_t *ptuia = init_ptuia(sizeof(struct tui_data), tui_init, tui_close, tui_process, tui_draw, 2, (coord_t[2]){mkcoord(0, -3), mkcoord(0, 3)});

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

