/*
ngt.cpp - graphics support for NGT20+ Nuclaer Graphics Devices
Copyright (c) 2017 Dylan Brophy.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef NMT_GFX_h
#define NMT_GFX_h

#include <inttypes.h>

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class NMT_GFX
{
private:
  // per object data
  // private methods
  void wait_cmd_done();

public:
  // public methods
  char gfxver[16];
  void begin();
  void end();
  void block_color(byte a, byte b);
  void line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  void w_vram(unsigned short adr,byte dat);
  void sprite(unsigned short x, unsigned short y, byte rot, unsigned short adr);
  void vec_sprite(unsigned short x, unsigned short y, unsigned short s, byte rot, unsigned short adr);
  void clear();
  void fill(byte color);
  void set_color(byte color);
  void print(char x);
  void print(char* x);
  void println(char* x);
  void println(StringSumHelper x);
  char* get_card_ver();
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif
