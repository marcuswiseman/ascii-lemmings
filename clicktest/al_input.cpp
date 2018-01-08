#include <SDL/SDL.h>

#include "al_input.h"

unsigned int double_click_time = 150; // Max time allowed to read for double click or hold in ms
unsigned int hold_time = 200; // Max time allowed to read for a hold in ,s

enum {KEYBOARD, MOUSE, JOYSTICK};

Uint32 lastclick, lastrelease;
int buttondown = false;
SDL_Event event;
//SDL_Joystick *stick = NULL;
//int joysticks = 0;
int lastkey = 0;
int lastvalue[3] = {0, 0, 0};
int sentclick = 1;

clickTypes downclick (int value, int type) {
  sentclick = 0;
  lastvalue[0] = lastvalue[1] = lastvalue[2] = 0;
  if (SDL_GetTicks() - lastrelease < double_click_time) {
    sentclick = 1;
    return DCLICK;
  }
  lastclick = SDL_GetTicks();
  lastvalue[type] = value;
  return NONE;
}

clickTypes upclick (int value, int type) {
  if (lastvalue[type] == value) {
    lastvalue[0] = lastvalue[1] = lastvalue[2] = 0;
    if (SDL_GetTicks() - lastclick >= hold_time) {
      sentclick = 1;
      return HOLD;
    }
    lastrelease = SDL_GetTicks();
  }
  return NONE;
}

clickTypes get_click () {
  if (!lastvalue[0] && !lastvalue[1] && !lastvalue[2] && !sentclick) {
    if (SDL_GetTicks() - lastrelease > double_click_time) {
      sentclick = 1;
      return CLICK;
    }
  }
  if( SDL_PollEvent( &event ) ) { //If a key was pressed if( event.type == SDL_KEYDOWN ) {
    switch (event.type) {
      case SDL_QUIT:
        exit(1);
      case SDL_VIDEORESIZE:
        break;
      case SDL_MOUSEMOTION:
        break;
      case SDL_MOUSEBUTTONDOWN:
        return downclick(1, MOUSE);
        break;
      case SDL_MOUSEBUTTONUP:
        return upclick (1, MOUSE);
        break;
      case SDL_JOYBUTTONDOWN:
        return downclick (event.jbutton.button, JOYSTICK);
        break;
      case SDL_JOYBUTTONUP:
        return upclick (event.jbutton.button, JOYSTICK);
        break;
      case SDL_KEYDOWN:
//        return downclick (event.key.keysym.sym, KEYBOARD);
        return downclick (1, KEYBOARD);
        break;
      case SDL_KEYUP:
//        return upclick (event.key.keysym.sym, KEYBOARD);
        return upclick (1, KEYBOARD);
        break;
    }
  }
  return NONE;
}

void set_double_click (int ms) {
  double_click_time = ms;
}

void set_hold (int ms) {
  hold_time = ms;
}

int init_input () {
  //if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { return false; }
  lastrelease = SDL_GetTicks();
  lastclick = SDL_GetTicks();
//  joysticks = SDL_NumJoysticks();
//  stick = SDL_JoystickOpen( 0 );
  return 0;
}

int deinit_input() {
//  SDL_JoystickClose( stick ); //Close the joystick
  return 0;
}
