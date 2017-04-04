#include <NMT_GFX.h>
#include <PS2Keyboard.h>

// An extremely crude Nyan Cat example.  will work on this.


byte cat_args[]={5,17,16,16};
unsigned short cato[91]={
0xaaa,0x0, 0xffff, 0xffff, 0xc000,
0xaaa,0xaaa3, 0xf555, 0x5557, 0xf000,
0xaaa,0xaaa3, 0xd555, 0x5565, 0xf000,
0xaaa,0xaaa3, 0x5565, 0x5555, 0x7000,
0xaaa,0xaaa3, 0x5555, 0x4155, 0x7000,
0xaaa,0xaaa3, 0x5555, 0x3c55, 0x70f0,
0xaaa,0xa3, 0x5655, 0x3f15, 0x73f0,
0xaaa,0x3c23, 0x5555, 0x3fc0, 0xff0,
0xaaa,0xf03, 0x5555, 0x3fff, 0xfff0,
0xaaa,0x83c3, 0x5554, 0xffff, 0xfffc,
0xaaa,0xa0f3, 0x5554, 0xfe3f, 0xfe3c,
0xaaa,0xa803, 0x6554, 0xfc3f, 0xcc3c,
0xaaa,0xaa83, 0x5554, 0xd7ff, 0xffd4,
0xaaa,0xaaa3, 0xd554, 0xd73f, 0x3cd4,
0xaaa,0xaaa3, 0xf556, 0x3f00, 0xf0,
0xaaa,0x0, 0xffff, 0xcfff, 0xffc0,
0xaaa,0x3f, 0x0, 0x0, 0x0,
0xaaa,0x3c, 0x3c0, 0xf0, 0x3c00};
NMT_GFX ngt;
const int IRQpin =  5;

PS2Keyboard keyboard;

void setup() {
  keyboard.begin(8, 2);
  ngt.begin();
  ngt.println("Loading textures...");
  for(byte i=0;i<4;i++)
    ngt.w_vram(i,cat_args[i]);
  for(byte i=0;i<190;i++)
    ngt.w_vram(i+4,255&(cato[i/2]>>(8*(i%2))));
  ngt.println("Setting up colors...");
  for(byte i=0;i<64;i++){
    ngt.block_color(128+i,i); // RAINBOW
    ngt.block_color(64+i,25); // PINK for Nyan Cat
    ngt.block_color(192+i,0b101010); // GREY for Nyan Cat
  }
  unsigned short x=100;
  unsigned short y=100;
  int x_limit=ngt.x_tiles()<<4;  // 16 pixels/tile
  int y_limit=ngt.y_tiles()<<4;  // 16 pixels/tile
  ngt.println("Ready to play...");
  delay(500);
  while(true){
    long ls=millis();
    ngt.clear();
    ngt.sprite(x,y,0,0);
    ngt.sprite(x+40,y,0,0);
    ngt.sprite(x,y-24,0,0);
    ngt.sprite(x,y+24,0,0);
    if (keyboard.available()) {
    
      // read the next key
      char c = keyboard.read();
      if (c == PS2_LEFTARROW&&x>0) {
        x-=5;
      } else if (c == PS2_RIGHTARROW&&x<x_limit) {
        x+=5;
      } else if (c == PS2_UPARROW&&y>0) {
        y-=5;
      } else if (c == PS2_DOWNARROW&&y<y_limit) {
        y+=5;
      }
    }
    while(ls+33>millis());
  }
}
void loop(){
}
