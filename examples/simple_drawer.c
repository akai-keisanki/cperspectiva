/* 'simple_drawer' example Perspectiva (beta) application */

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
  char in_1stb;
  bool cursor_state;
  bool save;
  FILE *save_f;
};

signed int tui_init(pcmc_t *pcmc, void *tui_data)
{
  struct tui_data *data = tui_data;
  const coord_t size = pcmc_get_size(pcmc);

  *data = (struct tui_data){
      .p = div_coord(size, 2),
      .in_1stb = '\0',
      .cursor_state = 0,
      .save_f = fopen("paint.txt", "w"),
      .save = false
    };

  return EXIT_SUCCESS;
}

void tui_close(void *tui_data)
{
  struct tui_data *data = tui_data;

  fclose(data->save_f);
}

signed int tui_process(const pcmc_t *pcmc, void *tui_data, ptuia_input_t input)
{
  struct tui_data *data = tui_data;
  const coord_t size = pcmc_get_size(pcmc);
  coord_t drct = mkcoord(0, 0);

  data->save = false;
  
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

  data->p = sum_coords(data->p, drct);

  if (!strcmp(input.bytes, "\x1B"))
    return PTUIA_BREAK;

  if (data->p.x <= 0)
    data->p.x = size.x;
  if (data->p.y <= 0)
    data->p.y = size.y;
  if (data->p.x > size.x - 2)
    data->p.x = 1;
  if (data->p.y > size.y - 2)
    data->p.y = 1;

  data->in_1stb = input.bytes[0];

  data->cursor_state = !data->cursor_state;

  return PTUIA_CONTINUE;
}

void tui_draw(pcmc_t *pcmc, const void *tui_data)
{
  const struct tui_data *data = tui_data;
  const coord_t size = pcmc_get_size(pcmc);

  pcmc_fill(pcmc, '\0');

  if (data->save)
    pcmc_print(pcmc, data->save_f);

  pcmc_set_at(pcmc, data->p, '\0');

  if (data->in_1stb && data->in_1stb != '\x1B')
    pcmc_set_at(pcmc, data->p, data->in_1stb);

  pcmc_set_self_as_background(pcmc);

  pcmct_fill_area(pcmc, mkcoord(0, size.y - 1), mkcoord(size.x, size.y - 1), '_');
  pcmct_fill_area(pcmc, mkcoord(0, size.y), mkcoord(size.x, size.y), ' ');
  pcmct_write_str(pcmc, mkcoord(0, size.y), "Press ESC to exit. Use the arrow keys to move the cursor, type to paint. Enter to save to paint.txt", mkcoord(1, 0));

  if (data->cursor_state)
    pcmc_set_at(pcmc, data->p, '#');
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
