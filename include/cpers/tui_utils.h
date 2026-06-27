#ifndef PERSPECTIVA_TUI_UTILS_H
#define PERSPECTIVA_TUI_UTILS_H

#define TUIU_INPUT_MAXSZ (size_t)0x400

#include <stddef.h>

struct tuiu_input
{
  size_t size;
  char bytes[TUIU_INPUT_MAXSZ];
};
typedef struct tuiu_input tuiu_input_t;

tuiu_input_t tuiu_read_stdin(void);
void tuiu_clear_stdout(void);

signed int tuiu_call_with_tui_mode_in_stdio(signed int (*callback)(void *ctx), void *ctx);

#endif
