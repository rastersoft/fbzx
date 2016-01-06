#include "mouse.hh"
#include "computer.hh"
#include "emulator.hh"
#include "keyboard.hh"
#include "osd.hh"

#include <stdio.h>

class Mouse *mouse;

void Mouse::reset() {
  
  this->posx = keyboard->mouse_x;
  this->posy = keyboard->mouse_y;
  this->x = 0;
  this->y = 0;
  this->button = 255;
  this->enabled = true;
  
}

Mouse::Mouse() {
  this->reset();
}

void Mouse::emulate(int tstates) {

  unsigned int diffx = keyboard->mouse_x - this->posx;
  unsigned int diffy = keyboard->mouse_y - this->posy;
  this->posx = keyboard->mouse_x;
  this->posy = keyboard->mouse_y;
  
  if (diffx < 2) {
    diffx=0;
  }
  if (diffy < 2) {
    diffy=0;
  }
  this->x += diffx;
  this->y -= diffy;
  this->button = 255;
  if (keyboard->mouse_right) {
    this->button -= 1;
  }
  if (keyboard->mouse_left) {
    this->button -= 2;
  }
  if (keyboard->mouse_center) {
    this->button -= 4;
  }
}
