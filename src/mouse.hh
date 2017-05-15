#ifndef SRC_MOUSE_HH_
#define SRC_MOUSE_HH_

#include "z80free/Z80free.h"
#include <inttypes.h>
#include <SDL/SDL.h>

class Mouse {

  int posx;
  int posy;

public:

  byte x;
  byte y;
  byte button;
  bool enabled;

  Mouse();
  void reset();
  void emulate(int);
  byte in(word);

};

extern class Mouse *mouse;
#endif
