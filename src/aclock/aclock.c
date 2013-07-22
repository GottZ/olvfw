
#include "ch.h"
#include "hal.h"
#include "gdisp.h"
#include "gwin.h"

#include "pictures.h"
#include "aclock.h"

// algorithm for reading a certain amount of bits from a buffer of aligned bits.
unsigned char bit_read (unsigned int pos, unsigned char bits, unsigned char mask, unsigned char* data) {
	unsigned char* c = data +pos *bits /8;
	unsigned char shift = 16 -bits -(pos *bits %8);
	if (shift < 8) return (c[0] << 8 | c[1]) >> shift & mask;
	else return c[0] >> (shift -8) & mask;
};

// algorithm that draws analog clock parts onto the screenbuffer
void aclock_print(unsigned char pic, unsigned char time) {
	unsigned int i;
	
	unsigned char line;
	unsigned char offy, startx, endx;
	
	unsigned int pos = 0;
	color_t color;
  unsigned char colorpart;
	unsigned char x, y;
	
	unsigned char pid = time %15;
	pid = pid < 8 ? pid : 15 -pid;
	pid += pic *8;

  if (pic == 2) {
    for (x = 24; x < 105; x++) {
      for (y = 24; y < 105; y++) {
        gdispDrawPixel(x, y, Black);
      }
    }
  }
	
  // y offset of current image
  //offy = aclock_info_starty[pid];
	for (line = 0; line < aclock_info_lines[pid]; line++) {
    // y offset line increasion
    //offy++;
    offy = aclock_info_starty[pid] + line;
    // x start offset
		startx = bit_read(line*2, 6, 0x3f, aclock_info[pid]) + aclock_info_offset[pid];
    // x end offset
		endx = bit_read(line*2+1, 6, 0x3f, aclock_info[pid]) + aclock_info_offset[pid];

    // if image is a lever
		if (pic < 2) {
			for (i = startx; i <= endx; i++) {
        // get pixel position
				if (time < 8) {
					x = offy;
					y = 128 -i;
				} else if (time < 15) {
					x = i;
					y = 128 -offy;
				} else if (time < 23) {
					x = i;
					y = offy;
				} else if (time < 30) {
					x = offy;
					y = i;
				} else if (time < 38) {
					x = 128 -offy;
					y = i;
				} else if (time < 45) {
					x = 128 -i;
					y = offy;
				} else if (time < 53) {
					x = 128 -i;
					y = 128 -offy;
				} else {
					x = 128 -offy;
					y = 128 -i;
				}
				
        // get clock pixel alpha
				colorpart = bit_read(pos++, 5, 0x1f, aclock_data[pid]) << 3;
        color = RGB2COLOR(colorpart, colorpart, colorpart);
        // set clock pixel as alpha overlay
        if (pic) gdispDrawPixel(x, y, color | gdispGetPixelColor(x, y));
        // set clock pixel
        else gdispDrawPixel(x, y, color);
			}
    // if image is the background
		} else {
			for (i = offy > 23 ? startx : 64; i <= endx; i++) {
        if (i < startx) {
          color = Black;
        }
        else {
          // get background pixel alpha
          colorpart = bit_read(pos++, 5, 0x1f, aclock_data[pid]) << 3;
          color = RGB2COLOR(colorpart, colorpart, colorpart);
        }
        // set background pixel
				gdispDrawPixel(        i,      offy, color);
				gdispDrawPixel(128    -i,      offy, color);
				gdispDrawPixel(        i, 128 -offy, color);
				gdispDrawPixel(128    -i, 128 -offy, color);
				gdispDrawPixel(     offy,         i, color);
				gdispDrawPixel(128 -offy,         i, color);
				gdispDrawPixel(     offy, 128    -i, color);
				gdispDrawPixel(128 -offy, 128    -i, color);
			}
		}
	}
};

