#include <cpers/vals.h>
#include <unistd.h>
#include <sys/ioctl.h>


coord_t get_terminal_size(void)
{
  struct winsize winsize;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);

  return mkcoord(winsize.ws_col, winsize.ws_row);
}
