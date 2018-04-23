#include <NMT_GFX.h>
#include <PS2Keyboard.h>

NMT_GFX ngt;

PS2Keyboard keyboard;

void setup() {
  keyboard.begin(4, 2);
  ngt.begin(11,10);
  ngt.println("Loading world...");
  ngt.add_line(-1000,-100,-1000,1000,-100,-1000,2);        
  ngt.add_line(1000,-100,-1000,1000,100,-1000,2);               
  ngt.add_line(1000,100,-1000,1000,100,1000,2);                 
  ngt.add_line(1000,100,1000,-1000,100,1000,2);              
  ngt.add_line(-1000,100,1000,-1000,-100,1000,2);              
  ngt.add_line(-1000,-100,1000,-1000,-100,-1000,2);
  
  ngt.add_line(-1000,-100,1000,1000,-100,1000,2);            
  ngt.add_line(1000,-100,-1000,1000,-100,1000,2);
  
  ngt.add_line(-1000,-100,-1000,-1000,100,-1000,2);
  ngt.add_line(1000,-100,1000,1000,100,1000,2);
                         
  ngt.add_line(-1000,100,-1000,1000,100,-1000,2);
  ngt.add_line(-1000,100,-1000,-1000,100,1000,2);
  
  ngt.add_line(10,10,10,30,10,10,3);        
  ngt.add_line(30,10,10,30,30,10,3);               
  ngt.add_line(30,30,10,30,30,30,3);                 
  ngt.add_line(30,30,30,10,30,30,3);              
  ngt.add_line(10,30,30,10,10,30,3);              
  ngt.add_line(10,10,30,10,10,10,3);
  
  ngt.add_line(10,10,30,30,10,30,3);            
  ngt.add_line(30,10,10,30,10,30,3);
  
  ngt.add_line(10,10,10,10,30,10,3);
  ngt.add_line(30,10,30,30,30,30,3);
                         
  ngt.add_line(10,30,10,30,30,10,3);
  ngt.add_line(10,30,10,10,30,30,3);
   
  ngt.println("Setting up colors...");
  for(byte i=0;i<64;i++){
    ngt.block_color(128+i,i); // RAINBOW
    ngt.block_color(64+i,25); // PINK for Nyan Cat
    ngt.block_color(192+i,0b101010); // GREY for Nyan Cat
  }
  ngt.println("Ready to play...");
  delay(500);
  double x=0,z=0;
  int rot=0;
  while(true){
    int32_t time=millis();
    if (keyboard.available()) {
    
      // read the next key
      char c = keyboard.read();
      if (c == PS2_LEFTARROW) {
        rot+=3;
      } else if (c == PS2_RIGHTARROW) {
        rot-=3;
      } else if (c == PS2_UPARROW) {
        x-=cos(rot*PI/180);
        z+=sin(rot*PI/180);
      } else if (c == PS2_DOWNARROW) {
        x+=cos(rot*PI/180);
        z-=sin(rot*PI/180);
      }
      ngt.rotatef(rot,0);
      ngt.translatef((int32_t)x,0,(int32_t)z);
    }
    ngt.frame_3d();
    delay(max(1,(16+time)-(int32_t)millis()));
  }
}
void loop(){
}
