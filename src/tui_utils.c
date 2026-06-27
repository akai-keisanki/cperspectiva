#include <cpers/tui_utils.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <locale.h>

struct terminal_state_backup
{
  struct termios termios_backup;
  int fcntl_flags_backup;
  char locale_backup[0xFF];
};

void tuiu_set_stdio_raw(struct terminal_state_backup *tsb)
{
  tcgetattr(STDOUT_FILENO, &tsb->termios_backup);
  tsb->fcntl_flags_backup = fcntl(STDIN_FILENO, F_GETFL, 0);
  snprintf(tsb->locale_backup, sizeof(tsb->locale_backup), "%s", setlocale(LC_ALL, NULL));

  struct termios termios_raw = tsb->termios_backup;
  termios_raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDOUT_FILENO, TCSAFLUSH, &termios_raw);
  fcntl(STDIN_FILENO, F_SETFL, tsb->fcntl_flags_backup | O_NONBLOCK);

  setlocale(LC_ALL, "UTF-8");
}

void tuiu_set_stdio_back(struct terminal_state_backup tsb)
{
  tcsetattr(STDOUT_FILENO, TCSAFLUSH, &tsb.termios_backup);
  fcntl(STDIN_FILENO, F_SETFL, tsb.fcntl_flags_backup);

  setlocale(LC_ALL, tsb.locale_backup);
}

tuiu_input_t tuiu_read_stdin(void)
{
  tuiu_input_t input = {
    .size = 0,
    .bytes = {}
  };

  input.size = read(STDIN_FILENO, input.bytes, TUIU_INPUT_MAXSZ);

  if (input.size < 0)
    input.size = 0;
  if (input.size >= TUIU_INPUT_MAXSZ)
    input.size = TUIU_INPUT_MAXSZ - 1;

  input.bytes[input.size] = '\0';

  return input;
}

void tuiu_clear_stdout(void)
{
  write(STDOUT_FILENO, "\x1B[2J\x1B[H\x1B[3J", 11);
}

signed int tuiu_call_with_tui_mode_in_stdio(signed int (*callback)(void *ctx), void *ctx)
{
  signed int code;
  struct terminal_state_backup tsb;

  tuiu_set_stdio_raw(&tsb);

  tuiu_clear_stdout();

  code = callback(ctx);

  tuiu_clear_stdout();

  tuiu_set_stdio_back(tsb);

  return code;
}
