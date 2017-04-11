# NMT_GFX
Origionally designed for NGT20 shields, it will be designed for newer versions of the product over time.  The library draws graphics and can be used to make sprite definitions.

Will improve this over time - refer to datasheet for more info: http://datauniversalgame.drewbrophy.com/NGT20.pdf

## Library Functions
void NMT_GFX::print(x);
Print character or string x at the cursor and advance the cursor once.

void NMT_GFX::println(x);
Print the string or char x onto the screen and advance the cursor to the end of the string, then
go to the next line.

void NMT_GFX::write_at(q, int x, int y);
Print the string or char q onto the screen where the upper left of the string is placed at (x, y).
Does not affect cursor position.

void NMT_GFX::set_cursor_pos(int x, int y);
Set cursor position to (x, y). Note that unlike other functions, x and y are measured in
characters, which are 6x13 pixels each.

void NMT_GFX::line(int x1, int y1, int x2, int y2);
Draw line from (x1, y1) to (x2, y2).  (x2, y2) is new endpoint.

void NMT_GFX::box(int x1, int y1, int x2, int y2);
Make a square between (x1, y1) and (x2, y2)

void NMT_GFX::fill_box(int x1, int y1, int x2, int y2);
Make a solid square between (x1, y1) and (x2, y2)

void NMT_GFX::fast(int x, int y);
Makes a line between the endpoint of the last line or the last pixel, whichever occured last, and
(x, y).  New endpoint is (x, y)

void NMT_GFX::clear();
Fill every tile with color zero in its color slot.

void NMT_GFX::fill(byte color);
Fill every tile with color in its color slot.

void NMT_GFX::set_color(byte color);
Make next draws use color.

void NMT_GFX::pixel(int x, int y);
Draw pixel at (x, y)

byte NMT_GFX::x_tiles();
Returns number of x tiles.  Multiply this by 16 to get number of columns.

byte NMT_GFX::y_tiles();
Returns number of y tiles.  Multiply this by 16 to get number of rows.

char* NMT_GFX::get_card_ver();
Returns string for device.  For an NGT20 it should be “NGT20”. Newer or older versions may say
something different, like “NGT35”.

void NMT_GFX::block_color(byte slot, byte color);
Set color (slot>>6) in slot (slot&63) to the argument ‘color’.

void NMT_GFX::tile_color(int tile, byte slot);
Make a tile use a specific color slot.

byte NMT_GFX::make_color(byte r, byte g, byte b);
Returns a color for use in NMT_GFX::block_color() using RGB color input.
