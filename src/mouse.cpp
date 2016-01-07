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
  
}

Mouse::Mouse() {
  this->enabled = false;
  this->reset();
}

void Mouse::emulate(int tstates) {

  uint8_t diffx = (keyboard->mouse_x - this->posx)/2;
  uint8_t diffy = (keyboard->mouse_y - this->posy)/2;
  this->posx = keyboard->mouse_x;
  this->posy = keyboard->mouse_y;
  
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
