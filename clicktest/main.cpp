#include <SDL/SDL.h>
#include <curses.h>

#include "al_input.h"

int main ( int argc, char* args[] ) {
  keypad(initscr(),1);
  raw ();
  nodelay(stdscr,0);

  init_input();
  printw ("Running...\n"); napms(1); refresh();
  while (1) {
    switch (get_click()) {
      case NONE: break;
      case CLICK: beep(); printw ("Click\n"); break;
      case DCLICK: beep(); printw ("DClick\n"); break;
      case HOLD: beep(); printw ("Hold\n"); break;
    }
    napms(1); refresh();
  }
  deinit_input();
  return 0;
}
