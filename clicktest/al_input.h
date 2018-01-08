#ifndef AL_INPUT_H_INCLUDED
#define AL_INPUT_H_INCLUDED

enum clickTypes {NONE, CLICK, HOLD, DCLICK};

clickTypes get_click ();

void set_double_click (int ms) ;

void set_hold (int ms);

int init_input ();

int deinit_input();

#endif // AL_INPUT_H_INCLUDED
