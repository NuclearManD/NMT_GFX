#include <NMT_GFX.h>
NMT_GFX ngt;
Sprite lels;
byte image[100];
void setup(){
  ngt.begin();
  
  for(int i=0;i<64;i++){
    ngt.block_color(i,i);
  }
  int quepie=ngt.x_tiles()*ngt.y_tiles();
  for(int i=0;i<quepie;i++){
    ngt.tile_color(i,i);
  }
}
void loop(){
}
