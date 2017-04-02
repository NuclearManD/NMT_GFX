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
//
// Private methods
//
void NMT_GFX::wait_cmd_done(){
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
  _NTI_GFX_.write('=');
  _NTI_GFX_.write(a);
  _NTI_GFX_.write(b<<2);
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
void NMT_GFX::box(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
  line(x1,y1,x2,y1);
  fast(x2,y2);
  fast(x1,y2);
  fast(x1,y1);
}/*
void NMT_GFX::fill_oval(unsigned short x1, unsigned short y1, unsigned short rx, unsigned short ry){
  wait_cmd_done();
  _NTI_GFX_.write(55);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(rx>>8);
  _NTI_GFX_.write(rx&255);
  _NTI_GFX_.write(ry>>8);
  _NTI_GFX_.write(ry&255);
}
void NMT_GFX::oval(unsigned short x1, unsigned short y1, unsigned short rx, unsigned short ry){
  wait_cmd_done();
  _NTI_GFX_.write(56);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(rx>>8);
  _NTI_GFX_.write(rx&255);
  _NTI_GFX_.write(ry>>8);
  _NTI_GFX_.write(ry&255);
}
void NMT_GFX::circle(unsigned short x1, unsigned short y1, unsigned short r){
  wait_cmd_done();
  _NTI_GFX_.write(56);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(r>>8);
  _NTI_GFX_.write(r&255);
  _NTI_GFX_.write(r>>8);
  _NTI_GFX_.write(r&255);
}
void NMT_GFX::fill_circle(unsigned short x1, unsigned short y1, unsigned short r){
  wait_cmd_done();
  _NTI_GFX_.write(55);
  _NTI_GFX_.write(x1>>8);
  _NTI_GFX_.write(x1&255);
  _NTI_GFX_.write(y1>>8);
  _NTI_GFX_.write(y1&255);
  _NTI_GFX_.write(r>>8);
  _NTI_GFX_.write(r&255);
  _NTI_GFX_.write(r>>8);
  _NTI_GFX_.write(r&255);
}*/
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
  _NTI_GFX_.write('1');
  
}
void NMT_GFX::fill(byte color){
  wait_cmd_done();
  _NTI_GFX_.write('5');
  _NTI_GFX_.write(color);
}
void NMT_GFX::set_color(byte color){
  wait_cmd_done();
  _NTI_GFX_.write('9');
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