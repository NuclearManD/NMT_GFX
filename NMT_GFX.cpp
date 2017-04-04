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

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
#include "Arduino.h"
#include "NMT_GFX.h"
SoftwareSerial _NTI_GFX_(6, 9); // RX, TX
unsigned short __LS_POS__=0;
//
// Private methods
//
void __wait_cmd_done(){
  long timer=millis()+300;
  while(!_NTI_GFX_.available()){
    if(timer<millis()){
      _NTI_GFX_.write(1);
      timer=millis()+300;
    }
  }
  while(_NTI_GFX_.available()){
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
  delay(1500);		// wait for reboot
  set_color(1);		// run setup by setting correct color and obtaining card version
  get_card_ver();
}
void NMT_GFX::end(){
	_NTI_GFX_.end();
}
void NMT_GFX::block_color(byte a, byte b){
  wait_cmd_done();
  _NTI_GFX_.write(61);
  _NTI_GFX_.write(a);
  _NTI_GFX_.write(b<<2);
}
byte NMT_GFX::make_color(byte r, byte g, byte b){
  return ((r&3)<<4+(g&3)<<2+b&3);
}
void NMT_GFX::tile_color(unsigned short a, byte b){
  wait_cmd_done();
  _NTI_GFX_.write(66);
  _NTI_GFX_.write(a>>8);
  _NTI_GFX_.write(a&255);
  _NTI_GFX_.write(b);
}
byte NMT_GFX::x_tiles(){
  wait_cmd_done();
  _NTI_GFX_.write(64);
  while(!_NTI_GFX_.available());
  return _NTI_GFX_.read();
}
byte NMT_GFX::y_tiles(){
  wait_cmd_done();
  _NTI_GFX_.write(64);
  while(!_NTI_GFX_.available());
  _NTI_GFX_.read();
  while(!_NTI_GFX_.available());
  return _NTI_GFX_.read();
}
void NMT_GFX::line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
  wait_cmd_done();
  _NTI_GFX_.write(52);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(x2>>8);
  _NTI_GFX_.write(x2&255);
  _NTI_GFX_.write(y2>>8);
  _NTI_GFX_.write(y2&255);
}
void NMT_GFX::fill_box(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
  wait_cmd_done();
  _NTI_GFX_.write(54);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(x2>>8);
  _NTI_GFX_.write(x2&255);
  _NTI_GFX_.write(y2>>8);
  _NTI_GFX_.write(y2&255);
}
void NMT_GFX::fast(unsigned short x1, unsigned short y1){
  wait_cmd_done();
  _NTI_GFX_.write(67);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
}
void NMT_GFX::pixel(unsigned short x1, unsigned short y1){
  wait_cmd_done();
  _NTI_GFX_.write(56);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
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
	for(byte i=4;i<limit;i+=1){
		binary_image[i]=color;
	}
}
void Sprite::pixel(byte x, byte y, byte color){
	byte sid=binary_image[0]*y*2+(x>>2+(1-(x>>2)%2));// byte for this pixel
	byte mask=1<<((x%4)*2);
	binary_image[sid+4]=mask*(color&3);
}
void Sprite::set_size(byte x, byte y){
	binary_image[0]=x>>3+1;
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
	return binary_image[1]*8;
}
void Sprite::upload(){
	int len=binary_image[0]*binary_image[1]+4+__LS_POS__;
	tadr=__LS_POS__;
	if(len>2048)
		return;
	for(int i=__LS_POS__;i<len;i++){
		__wait_cmd_done();
		_NTI_GFX_.write(63);
		_NTI_GFX_.write(i>>8);
		_NTI_GFX_.write(i&255);
		_NTI_GFX_.write(binary_image[i-__LS_POS__]);
	}
	__LS_POS__=len;
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
  wait_cmd_done();
  _NTI_GFX_.write(63);
  _NTI_GFX_.write(adr>>8);
  _NTI_GFX_.write(adr&255);
  _NTI_GFX_.write(dat);
}
void NMT_GFX::sprite(unsigned short x, unsigned short y, byte rot, unsigned short adr){
  wait_cmd_done();
  _NTI_GFX_.write(62);
  _NTI_GFX_.write(x>>8);
  _NTI_GFX_.write(x&255);
  _NTI_GFX_.write(y>>8);
  _NTI_GFX_.write(y&255);
  _NTI_GFX_.write(rot);
  _NTI_GFX_.write(adr>>8);
  _NTI_GFX_.write(adr&255);
}
void NMT_GFX::vec_sprite(unsigned short x, unsigned short y, unsigned short s, byte rot, unsigned short adr){
  wait_cmd_done();
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
}
void NMT_GFX::clear(){
  wait_cmd_done();
  _NTI_GFX_.write(49);
  
}
void NMT_GFX::fill(byte color){
  wait_cmd_done();
  _NTI_GFX_.write(53);
  _NTI_GFX_.write(color);
}
void NMT_GFX::set_color(byte color){
  wait_cmd_done();
  _NTI_GFX_.write(57);
  _NTI_GFX_.write(color);
}
void NMT_GFX::print(char x){
  wait_cmd_done();
  if(x!='\r'||x!='\n'){
    _NTI_GFX_.write('3');
    _NTI_GFX_.write(x);
  }
  _NTI_GFX_.write('\r');
}
void NMT_GFX::print(char* x){
  wait_cmd_done();
  char tmp[256];
  int loc=0;
  for(int i=0;x[i]!=0;i++){
    if(x[i]=='\r' || x[i]=='\n'){
      tmp[loc]=0;
      print(tmp);
      _NTI_GFX_.write('\r');
      wait_cmd_done();
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
}
void NMT_GFX::println(char* x){
  print(x);
  wait_cmd_done();
  _NTI_GFX_.write(13);
}
void NMT_GFX::println(StringSumHelper x){
  print((char*)x.c_str());
  wait_cmd_done();
  _NTI_GFX_.write(13);
}
char* NMT_GFX::get_card_ver(){
  wait_cmd_done();
  _NTI_GFX_.write(32);
  String out="";
  while(true){
    while(!_NTI_GFX_.available());
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