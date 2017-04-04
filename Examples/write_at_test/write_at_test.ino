#include <NMT_GFX.h>
NMT_GFX ngt;
void setup(){
  ngt.begin();
  ngt.write_at("_-_-_-", 36,36);
  ngt.write_at("@@@@@@", 0,0);
  ngt.set_color(2);
  ngt.write_at("This example tests", 25,60);
  ngt.write_at("the NGT20's write_at() function.", 25,73);
}
void loop(){
}
