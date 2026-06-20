#include <cpers/ptuia.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cpers/pcmc.h>
#include <cpers/time.h>
#include <cpers/vals.h>

#define INPUT_MAXSIZE (size_t)1024

struct ptuia
{
  pcmc_t *pcmc;
  void *data;
  tui_init_t *tui_init;
  tui_close_t *tui_close;
  tui_process_t *tui_process;
  tui_draw_t *tui_draw;
};

ptuia_t *init_ptuia(size_t data_struct_size, tui_init_t *tui_init, tui_close_t *tui_close, tui_process_t *tui_process, tui_draw_t *tui_draw)
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
      .pcmc = init_pcmc(get_terminal_size())
    };

  if (self->data == NULL || self->pcmc == NULL)
    return NULL;

  return self;
}

struct terminal_state_backup
{
  struct termios termios_backup;
  int fcntl_flags_backup;
  char locale_backup[256];
};

void ptuia_set_stdio_raw(struct terminal_state_backup *tsb)
{
  tcgetattr(STDOUT_FILENO, &tsb->termios_backup);
  tsb->fcntl_flags_backup = fcntl(STDIN_FILENO, F_GETFL, 0);
  snprintf(tsb->locale_backup, sizeof(tsb->locale_backup), "%s", setlocale(LC_ALL, NULL));

  struct termios termios_raw = tsb->termios_backup;
  termios_raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDOUT_FILENO, TCSAFLUSH, &termios_raw);
  fcntl(STDIN_FILENO, F_SETFL, tsb->fcntl_flags_backup | O_NONBLOCK);

  setlocale(LC_ALL, "");
}

void ptuia_set_stdio_back(struct terminal_state_backup tsb)
{
  tcsetattr(STDOUT_FILENO, TCSAFLUSH, &tsb.termios_backup);
  fcntl(STDIN_FILENO, F_SETFL, tsb.fcntl_flags_backup);

  setlocale(LC_ALL, tsb.locale_backup);
}

signed int ptuia_run_with_stdio(ptuia_t *self)
{
  struct terminal_state_backup tsb;
  signed int tui_process_code;
  ptuia_input_t input;

  input.bytes = malloc(sizeof(char) * INPUT_MAXSIZE + sizeof(int));
  if (input.bytes == NULL)
    return EXIT_FAILURE;

  if (self->tui_init(self->pcmc, self->data) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  ptuia_set_stdio_raw(&tsb);
    
  while (tui_process_code != PTUIA_BREAK)
  {
    self->tui_draw(self->pcmc, self->data);

    write(STDOUT_FILENO, "\x1B[2J\x1B[H\x1B[3J", 11);
    pcmc_print_raw(self->pcmc, stdout);

    sleep_ms(30.0);

    input.size = read(STDIN_FILENO, input.bytes, INPUT_MAXSIZE);
    if (input.size >= INPUT_MAXSIZE) input.size = 0;
    input.bytes[input.size] = 0;

    tui_process_code = self->tui_process(self->pcmc, self->data, input);
  }

  self->tui_close(self->data);

  free(input.bytes);

  write(STDOUT_FILENO, "\x1B[2J\x1B[H\x1B[3J", 11);

  ptuia_set_stdio_back(tsb);

  return EXIT_SUCCESS;
}

void free_ptuia(ptuia_t *self)
{
  free(self->data);
  free_pcmc(self->pcmc);
  free(self);
}
