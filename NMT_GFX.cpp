/*
NMT_GFX.cpp - graphics support for NGT20+ Nuclaer Graphics Devices
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

#include "Arduino.h"
#include "NMT_GFX.h"


#if defined(ARDUINO_SAM_DUE) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560) // We need to be a shield, Softwareserial compatible
	#include <SoftwareSerial.h>
	#ifndef GFX_RX
		#define GFX_RX 6
	#endif
	#ifndef GFX_TX
		#define GFX_TX 9
	#endif
	#if defined(__AVR_ATmega328P__)
		#define AVAIL _NTI_GFX_.available()
		#define CHK_WT 300
	#else
		#define AVAIL !digitalRead(GFX_RX)
		#define CHK_WT 50
	#endif
	SoftwareSerial _NTI_GFX_(GFX_RX, GFX_TX); // RX, TX
#else
	#if !(defined(GFX_TX)|defined(GFX_RX))                        // NOT a shield!
		#define _NTI_GFX_ Serial1
		#define AVAIL Serial1.available()
		#warning "Using Serial1 for NGT20 I/O! Beware!"
	#else
		#error "This board currently does not support special UART pins for NTI_GFX lib. Sorry."
	#endif
	#define GFX_RX 0
	#define GFX_TX 1
	#define CHK_WT 300
#endif

unsigned short __LS_POS__=0;
//
// Private methods
//
void __wait_cmd_done(){
  long timer=millis()+CHK_WT;
  while(!AVAIL){
    if(timer<millis()){
      _NTI_GFX_.write(1);
      timer=millis()+CHK_WT;
    }
  }
  while(AVAIL){
    _NTI_GFX_.read();
    delayMicroseconds(200); // For 57600+ baud, to read every last byte
  }
}
void NMT_GFX::wait_cmd_done(){
  __wait_cmd_done();
}
// 
// Public methods
//
void NMT_GFX::begin(){
  _NTI_GFX_.begin(115200);
  delay(600);
  _NTI_GFX_.write('\r');            // If _NTI_GFX_ card is executing command 51 then terminate it
  for(byte i=0;i<8;i++)           // make sure all commands exit on _NTI_GFX_ card by sending a ton of junk data
    _NTI_GFX_.write('0');           // Reset _NTI_GFX_ card
  delay(2000);		// wait for reboot
  set_color(1);		// run setup by setting correct color and obtaining card version
}
void NMT_GFX::end(){
	//_NTI_GFX_.end();
}
void NMT_GFX::block_color(byte a, byte b){
  _NTI_GFX_.write(61);
  _NTI_GFX_.write(a);
  _NTI_GFX_.write(b<<2);
  wait_cmd_done();
}
byte NMT_GFX::make_color(byte r, byte g, byte b){
  return (((r&3)*16)+((g&3)*4)+(b&3));
}
void NMT_GFX::tile_color(unsigned short a, byte b){
  _NTI_GFX_.write(66);
  _NTI_GFX_.write(a>>8);
  _NTI_GFX_.write(a&255);
  _NTI_GFX_.write(b);
  wait_cmd_done();
}
byte NMT_GFX::x_tiles(){
	#ifdef ARDUINO_AVR_MEGA2560
		return 16;
	#else
  _NTI_GFX_.write(64);
  while(!_NTI_GFX_.available());
  return _NTI_GFX_.read();
  wait_cmd_done();
  #endif
}
byte NMT_GFX::y_tiles(){
	#ifdef ARDUINO_AVR_MEGA2560
		return 12;
	#else
  _NTI_GFX_.write(64);
  while(!_NTI_GFX_.available());
  _NTI_GFX_.read();
  while(!_NTI_GFX_.available());
  return _NTI_GFX_.read();
  #endif
  //wait_cmd_done();
}
void NMT_GFX::line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
  _NTI_GFX_.write(52);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(x2>>8);
  _NTI_GFX_.write(x2&255);
  _NTI_GFX_.write(y2>>8);
  _NTI_GFX_.write(y2&255);
  wait_cmd_done();
}
void NMT_GFX::fill_box(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
  _NTI_GFX_.write(54);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(x2>>8);
  _NTI_GFX_.write(x2&255);
  _NTI_GFX_.write(y2>>8);
  _NTI_GFX_.write(y2&255);
  wait_cmd_done();
}
void NMT_GFX::fast(unsigned short x1, unsigned short y1){
  _NTI_GFX_.write(67);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  wait_cmd_done();
}
void NMT_GFX::set_cursor_pos(byte x1, byte y1){
  _NTI_GFX_.write(50);
  _NTI_GFX_.write(x1);
  _NTI_GFX_.write(y1);
  wait_cmd_done();
}
void NMT_GFX::pixel(unsigned short x1, unsigned short y1){
  _NTI_GFX_.write(56);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  wait_cmd_done();
}
void NMT_GFX::box(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
  line(x1,y1,x2,y1);
  fast(x2,y2);
  fast(x1,y2);
  fast(x1,y1);
}
void Sprite::fill(byte color){
	byte limit=binary_image[0]*binary_image[1]*2+4;// calculate the last byte in the sprite
	color=(color&3)*0x55;     // solve for # in a byte to make desired color
	for(byte i=4;i<limit;i++){
		binary_image[i]=color;
	}
}
void Sprite::pixel(byte x, byte y, byte color){
	byte sid=binary_image[0]*y*2+(((x>>2)&0x3E)|((~x>>2)&1));// byte for this pixel
	byte mask=~(3<<(6-(x%4)*2));
	binary_image[sid+4]&=mask;
	binary_image[sid+4]|=(color&3)<<(6-(x%4)*2);
}
void Sprite::set_size(byte x, byte y){
	binary_image[0]=((x-1)>>3)+1;
	binary_image[1]=y;
}
void Sprite::set_center(byte x, byte y){
	binary_image[2]=x;
	binary_image[3]=y;
}
byte Sprite::get_size_x(){
	return binary_image[0]*8;
}
byte Sprite::get_size_y(){
	return binary_image[1];
}
void Sprite::upload(){
	unsigned short len=binary_image[0]*binary_image[1]*2+4;
	tadr=__LS_POS__;
	if(len+__LS_POS__>2048)
		return;
	for(unsigned short i=0;i<len;i++){
		__wait_cmd_done();
		_NTI_GFX_.write(63);
		_NTI_GFX_.write((i+__LS_POS__)>>8);
		_NTI_GFX_.write((i+__LS_POS__)&255);
		_NTI_GFX_.write(binary_image[i]);
	}
	__LS_POS__+=len;
}
void Sprite::display(unsigned short x, unsigned short y, byte rot){
	__wait_cmd_done();
	_NTI_GFX_.write(62);
	_NTI_GFX_.write(x>>8);
	_NTI_GFX_.write(x&255);
	_NTI_GFX_.write(y>>8);
	_NTI_GFX_.write(y&255);
	_NTI_GFX_.write(rot);
	_NTI_GFX_.write(tadr>>8);
	_NTI_GFX_.write(tadr&255);
}
void NMT_GFX::w_vram(unsigned short adr,byte dat){
  _NTI_GFX_.write(63);
  _NTI_GFX_.write(adr>>8);
  _NTI_GFX_.write(adr&255);
  _NTI_GFX_.write(dat);
  wait_cmd_done();
}
void NMT_GFX::sprite(unsigned short x, unsigned short y, byte rot, unsigned short adr){
  _NTI_GFX_.write(62);
  _NTI_GFX_.write(x>>8);
  _NTI_GFX_.write(x&255);
  _NTI_GFX_.write(y>>8);
  _NTI_GFX_.write(y&255);
  _NTI_GFX_.write(rot);
  _NTI_GFX_.write(adr>>8);
  _NTI_GFX_.write(adr&255);
  wait_cmd_done();
}
void NMT_GFX::vec_sprite(unsigned short x, unsigned short y, unsigned short s, byte rot, unsigned short adr){
  _NTI_GFX_.write(65);
  _NTI_GFX_.write(x>>8);
  _NTI_GFX_.write(x&255);
  _NTI_GFX_.write(y>>8);
  _NTI_GFX_.write(y&255);
  _NTI_GFX_.write(s>>8);
  _NTI_GFX_.write(s&255);
  _NTI_GFX_.write(rot);
  _NTI_GFX_.write(adr>>8);
  _NTI_GFX_.write(adr&255);
  wait_cmd_done();
}
void NMT_GFX::clear(){
  _NTI_GFX_.write(49);
  wait_cmd_done();
  
}
void NMT_GFX::fill(byte color){
  _NTI_GFX_.write(53);
  _NTI_GFX_.write(color);
  wait_cmd_done();
}
void NMT_GFX::set_color(byte color){
  _NTI_GFX_.write(57);
  _NTI_GFX_.write(color);
  wait_cmd_done();
}
void NMT_GFX::print(char x){
  if(x!='\r'||x!='\n'){
    _NTI_GFX_.write('3');
    _NTI_GFX_.write(x);
  }
  _NTI_GFX_.write('\r');
  wait_cmd_done();
}
void NMT_GFX::print(char* x){
  char tmp[256];
  int loc=0;
  for(int i=0;x[i]!=0;i++){
    if(x[i]=='\r' || x[i]=='\n'){
      tmp[loc]=0;
      println(tmp);
      loc=0;
    }else{
      tmp[loc]=x[i];
      loc++;
    }
  }
  tmp[loc]=0;
  _NTI_GFX_.write('3');
  _NTI_GFX_.print(tmp);
  _NTI_GFX_.write(0x0D);  // Terminate command
  wait_cmd_done();
}
void NMT_GFX::println(char* x){
  print(x);
  _NTI_GFX_.write(13);
  wait_cmd_done();
}
void NMT_GFX::println(char x){
  print(x);
  _NTI_GFX_.write(13);
  wait_cmd_done();
}
void NMT_GFX::println(StringSumHelper x){
  println((char*)x.c_str());
}
char* NMT_GFX::get_card_ver(){
	#ifndef __AVR_ATmega328P__
	return (char*)"NGT20";
	#endif
  _NTI_GFX_.write(32);
  String out="";
  while(true){
    while(!AVAIL);
    char c=_NTI_GFX_.read();
    if(c=='\r')
      break;
    out+=c;
  }
  for(byte i=0;i<out.length();i++){
    gfxver[i]=out.charAt(i);
  }
  return gfxver;
}
void NMT_GFX::println(const char* x){
	println((char*)x);
}
void NMT_GFX::print(const char* x){
	print((char*)x);
}
void NMT_GFX::print(StringSumHelper x){
	print((char*)x.c_str());
}
void NMT_GFX::write_at(char* q, unsigned short x, unsigned short y){
  _NTI_GFX_.write(55);
  _NTI_GFX_.write(x>>8);
  _NTI_GFX_.write(x&255);
  _NTI_GFX_.write(y>>8);
  _NTI_GFX_.write(y&255);
  for(int i=0;q[i]!=0;i++){
    _NTI_GFX_.write(q[i]);
  }
  _NTI_GFX_.write(0x0D);  // Terminate command
  wait_cmd_done();
}
void NMT_GFX::write_at(char q, unsigned short x, unsigned short y){
  _NTI_GFX_.write(55);
  _NTI_GFX_.write(x>>8);
  _NTI_GFX_.write(x&255);
  _NTI_GFX_.write(y>>8);
  _NTI_GFX_.write(y&255);
  _NTI_GFX_.write(q);
  _NTI_GFX_.write(0x0D);  // Terminate command
  wait_cmd_done();
}
void NMT_GFX::write_at(StringSumHelper q, unsigned short x, unsigned short y){
	write_at((char*)q.c_str(),x,y);
}
void NMT_GFX::write_at(const char* q, unsigned short x, unsigned short y){
	write_at((char*)q,x,y);
}