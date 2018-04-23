#include <NMT_GFX.h>
byte tree[]={1,8,0,0,
0b01010100,0b00010101,
0b01010101,0b01010101,
0b01010101,0b01010101,
0b01010101,0b01010101,
0b10100011,0b00001010,
0b10100000,0b00001010,
0b10100000,0b00001010,
0b10101000,0b00101010};
byte cat_args[]={5,17,16,16};
unsigned short cato[91]={
0xaaa,0x0, 0xffff, 0xffff, 0xc000,
0xaaa,0xaaa3, 0xf555, 0x5557, 0xf000,
0xaaa,0xaaa3, 0xd555, 0x5555, 0xf000,
0xaaa,0xaaa3, 0x5555, 0x5555, 0x7000,
0xaaa,0xaaa3, 0x5555, 0x4155, 0x7000,
0xaaa,0xaaa3, 0x5555, 0x3c55, 0x70f0,
0xaaa,0xa3, 0x5555, 0x3f15, 0x73f0,
0xaaa,0x3c23, 0x5555, 0x3fc0, 0xff0,
0xaaa,0xf03, 0x5555, 0x3fff, 0xfff0,
0xaaa,0x83c3, 0x5554, 0xffff, 0xfffc,
0xaaa,0xa0f3, 0x5554, 0xfe3f, 0xfe3c,
0xaaa,0xa803, 0x5554, 0xfc3f, 0xcc3c,
0xaaa,0xaa83, 0x5554, 0xd7ff, 0xffd4,
0xaaa,0xaaa3, 0xd554, 0xd73f, 0x3cd4,
0xaaa,0xaaa3, 0xf555, 0x3f00, 0xf0,
0xaaa,0x0, 0xffff, 0xcfff, 0xffc0,
0xaaa,0x3f, 0x0, 0x0, 0x0,
0xaaa,0x3c, 0x3c0, 0xf0, 0x3c00};
NMT_GFX ngt;
void setup(){
  ngt.begin();
  ngt.print("Hello!\nThis is an arduino writing\non screen with a ");
  ngt.println(ngt.get_card_ver());
  delay(1200);
  ngt.println("Look, I can even do colors!");
  ngt.set_color(3);
  ngt.print("green ");
  ngt.set_color(2);
  ngt.print("red ");
  ngt.set_color(1);
  ngt.println("and white");
  ngt.println("(plus much more)");
  delay(1400);
  ngt.println("Okay, now I'll clear the screen.");
  delay(1700);
  ngt.clear();
  ngt.println("I dont only do text though, so");
  ngt.println("let me load my graphics");
  for(byte i=0;i<20;i++)
    ngt.w_vram(i,tree[i]);
  for(byte i=0;i<4;i++)
    ngt.w_vram(i+20,cat_args[i]);
  for(byte i=0;i<182;i++)
    ngt.w_vram(i+24,255&(cato[i/2]>>(8*(i%2))));
  delay(750);
  ngt.println("Now WATCH!");
  ngt.line(0,50,255,50);
  byte x_tiles=ngt.x_tiles();              // Get tiles for x and y
  byte y_tiles=ngt.y_tiles();
  ngt.block_color(64+63,0b001100);
  ngt.block_color(128+63,0b011000);        // define tree colors
  ngt.block_color(192+63,0b010001);
  for(byte i=0;i<50;i++){    // make all tiles use text coloring
    ngt.tile_color(i,0);
  }
  for(byte i=x_tiles-1;i<50;i+=x_tiles){   // Make bottom tiles use tree coloring
    ngt.tile_color(i,63);
  }
  ngt.sprite(4,180,0,0);
  ngt.sprite(13,180,0,0);
  ngt.sprite(48,180,2,0);
  ngt.sprite(68,180,0,20);
  ngt.sprite(28,180,1,0);
  ngt.line(0,150,30,130);
  ngt.line(30,130,60,150);
  ngt.line(60,150,90,130);
  ngt.line(90,130,120,150);
  ngt.line(120,150,150,130);
  delay(1000);
  ngt.println(' ');
  ngt.set_color(1);
  ngt.println("The trees are drawn as sprites: an image of one is stored and copied to the screen using only one command, instead of resending the entire tree over serial.");
  delay(6000);
  ngt.println("\rNow I'll show you a moving rainbow sprite.");
  delay(1750);
  ngt.clear();
  for(byte i=0;i<64;i++){
    ngt.block_color(128+i,i); // RAINBOW
    ngt.block_color(64+i,25); // PINK for Nyan Cat
    ngt.block_color(192+i,0b101010); // GREY for Nyan Cat
  }
  byte rot=0;
    ngt.sprite(50,50,0,20);
   // while(true);
  while(true){
    long ls=millis();
    ngt.clear();
    ngt.set_color(1);
    ngt.println("U see dis?");
    ngt.sprite(50,50+rot%64,0,0);
    ngt.sprite(100-rot%96,50+rot%64,0,20);
    rot++;
    while(ls+33>millis());
  }// */
}
void loop(){
}