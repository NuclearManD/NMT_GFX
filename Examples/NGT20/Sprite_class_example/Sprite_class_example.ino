#include <NMT_GFX.h>
NMT_GFX ngt;
Sprite sprite;
byte image[52];
void setup(){
  ngt.begin();
  ngt.set_color(1);
  ngt.println("test...");
  // tell sprite where to store it's data
  sprite.binary_image=(byte*)image;
  // tell sprite how big it is
  sprite.set_size(8,8);
  // set the center at 4,4 pixels
  sprite.set_center(4,4);
  // fill sprite
  sprite.fill(2);
  // draw some pixels
  sprite.pixel(3,3,0);
  sprite.pixel(4,3,1);
  sprite.pixel(3,4,2);
  sprite.pixel(4,4,3);
  sprite.upload(&ngt);
  // Display sprite at 50,60 at a 0 degree angle
  sprite.display(50,60,0,&ngt);
}
void loop(){
}