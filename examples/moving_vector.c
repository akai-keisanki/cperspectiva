/* 'moving_vector' example Perspectiva (beta) application */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cpers/types.h>
#include <cpers/pcmc.h>
#include <cpers/ptuia.h>
#include <cpers/pcmc_tricks.h>

struct tui_data
{
  coord_t p;
  coord_t drct;
  char *input_bytes;
};

signed int tui_init(pcmc_t *pcmc, void *tui_data)
{
  struct tui_data *data = tui_data;
  const coord_t size = pcmc_get_size(pcmc);

  *data = (struct tui_data){
      .p = div_coord(size, 2),
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

signed int tui_process(const pcmc_t *pcmc, void *tui_data, ptuia_input_t input)
{
  struct tui_data *data = tui_data;
  const coord_t size = pcmc_get_size(pcmc);
  size_t i;

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

  data->p = sum_coords(data->p, data->drct);

  if (data->p.x <= 0)
    data->p.x = size.x;
  if (data->p.y <= 0)
    data->p.y = size.y;
  if (data->p.x > size.x)
    data->p.x = 1;
  if (data->p.y > size.y)
    data->p.y = 1;

  strncpy(data->input_bytes, input.bytes, 255);
  data->input_bytes[255] = 0;

  return PTUIA_CONTINUE;
}

void tui_draw(pcmc_t *pcmc, const void *tui_data)
{
  const struct tui_data *data = tui_data;
  const coord_t size = pcmc_get_size(pcmc);

  pcmc_fill(pcmc, ' ');

  pcmct_draw_line(pcmc, div_coord(size, 2), data->p, '#');

  pcmct_fill_area(pcmc, mkcoord(0, size.y - 2), mkcoord(size.x, size.y - 2), '_');
  pcmct_fill_area(pcmc, mkcoord(0, size.y - 1), mkcoord(size.x, size.y), ' ');
  pcmct_write_str(pcmc, mkcoord(0, size.y - 1), "Press q to exit. Use the arrow keys to move the vector.", mkcoord(1, 0));
  if (data->input_bytes)
    for (size_t i = 0; data->input_bytes[i]; ++i)
      pcmc_set_at(pcmc, mkcoord(i + 1, size.y), data->input_bytes[i]);
}

signed int main(void)
{
  signed int exit_code = EXIT_SUCCESS;
  ptuia_t *ptuia = init_ptuia(sizeof(struct tui_data), tui_init, tui_close, tui_process, tui_draw);

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
