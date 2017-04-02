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
NMT_GFX ngt;
unsigned short vec[]={
  0x4000+0x2000/3*0, 50,
  0x8000+0x2000/3*1+1, 50,
  0x8000+0x2000/3*2-1, 50,
  0x8000+0x2000/3*0, 50,
  0,0
};
void setup(){
  Serial.begin(115200);
  ngt.begin();
  ngt.print("Hello!\nThis is an arduino writing\non screen with a ");
  ngt.println(ngt.gfxver);
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
  delay(750);
  ngt.println("Now WATCH!");
  ngt.line(0,50,255,50);
  for(byte i=4;i<50;i+=5){
    ngt.block_color(64+i,0b000100);
    ngt.block_color(128+i,0b011000);
    ngt.block_color(192+i,0b010001);
  }
  ngt.sprite(4,20,0,0);
  ngt.sprite(13,20,0,0);
  ngt.sprite(48,20,2,0);
  ngt.sprite(68,20,3,0);
  ngt.sprite(28,20,1,0);
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
  ngt.println("\rNow I'll show you a vector sprite.");
  delay(1750);
  ngt.clear();
  for(byte i=0;i<50;i++)
    ngt.w_vram(i,255&(vec[i]>>8*(1-(i%2))));
  for(byte i=0;i<64;i++)
    ngt.block_color(128+i,i);
  byte rot=0;
  while(true){
    long ls=millis();
    ngt.set_color(1);
    ngt.println("U see dis?");
    ngt.set_color(2);
    ngt.vec_sprite(100,100,100,rot,0);
    ngt.vec_sprite(150,0,100,rot,0);
    ngt.vec_sprite(125,125,100,255-rot,0);
    rot++;
    while(ls+33>millis());
    ngt.clear();
  }
}
void loop(){
}
