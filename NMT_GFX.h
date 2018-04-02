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
#include <SoftwareSerial.h>

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class NMT_GFX
{
private:
  uint16_t line_index;
  SoftwareSerial* _NTI_GFX_;

public:
  unsigned short __LS_POS__;
  // public methods
  void wait_cmd_done();
  void write(byte c);
  char gfxver[16];
  void begin();
  void NMT_GFX::begin(int rx, int tx);
  void end();
  void block_color(byte a, byte b);
  byte make_color(byte r, byte g, byte b);
  void line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  void box(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  //void oval(unsigned short x1, unsigned short y1, unsigned short rx, unsigned short ry);
  //void circle(unsigned short x1, unsigned short y1, unsigned short r);
  void fill_box(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  //void fill_oval(unsigned short x1, unsigned short y1, unsigned short rx, unsigned short ry);
  //void fill_circle(unsigned short x1, unsigned short y1, unsigned short r);
  void fast(unsigned short x1, unsigned short y1);
  void set_cursor_pos(byte x1, byte y1);
  void w_vram(unsigned short adr,uint8_t dat);
  
  uint16_t add_line(long x1,long y1,long z1,long x2,long y2,long z2);
  void del_line(uint16_t id);
  
  void render_3d();
  
  void w_vram_long(unsigned short adr,int32_t dat);
  void w_vram_word(unsigned short adr,int16_t dat);
  void write_at(char* q, unsigned short x, unsigned short y);
  void write_at(char q, unsigned short x, unsigned short y);
  void write_at(StringSumHelper q, unsigned short x, unsigned short y);
  void write_at(const char* q, unsigned short x, unsigned short y);
  void sprite(unsigned short x, unsigned short y, byte rot, unsigned short adr);
  void vec_sprite(unsigned short x, unsigned short y, unsigned short s, byte rot, unsigned short adr);
  void clear();
  void fill(byte color);
  void set_color(byte color);
  void pixel(unsigned short x, unsigned short y);
  void print(char x);
  void print(char* x);
  void println(const char* x);
  void print(const char* x);
  void print(StringSumHelper x);
  void println(char x);
  void println(char* x);
  void println(StringSumHelper x);
  void tile_color(unsigned short a, byte b);
  byte x_tiles();
  byte y_tiles();
  char* get_card_ver();
};
class Sprite{
public:
  // public methods
  byte* binary_image;
  unsigned short tadr;
  void fill(byte color);
  void pixel(byte x, byte y, byte color);
  void set_size(byte x, byte y);
  void set_center(byte x, byte y);
  byte get_size_x();
  byte get_size_y();
  void upload(NMT_GFX* ser);
  void display(unsigned short x, unsigned short y, byte rot,NMT_GFX* ser);
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
