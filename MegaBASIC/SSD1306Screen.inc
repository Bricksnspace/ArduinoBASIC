/*
	Copyright 2019 Mario Pascucci <mpascucci@gmail.com>

	This is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this software.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * SSD1306Screen.cpp
 *
 *  Created on: 23 mag 2019
 *      Author: Mario Pascucci
 */


#include "SSD1306Screen.h"
#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_GFX.h"



namespace screen {

uint16_t fgColor = WHITE;
uint16_t bgColor = BLACK;


// TODO: this is for I2C display, add SPI when available

#if I2C_SSD1306
// last parameter is Reset pin, -1 if none or linked with Arduino Reset
Adafruit_SSD1306 display(PIXEL_WIDTH, PIXEL_HEIGHT, &Wire, -1);
#else
#error "SPI SSD1306 not implemented yet"
#endif

void init()
{
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.setTextColor(fgColor,bgColor);
	display.setTextWrap(false);
}


void clear(void)
{
	display.setTextColor(fgColor,bgColor);
	display.clearDisplay();
	display.display();
}



void moveCursor(int x, int y)
{
    display.setCursor(x*CHAR_WIDTH, y*LINE_HEIGHT);
}


void showBuffer(void)
{
    for (int y=0; y<SCREEN_HEIGHT; y++) {
        if (lineDirty[y] || (inputMode && y==curY)) {
            moveCursor(0,y);
            for (int x=0; x<SCREEN_WIDTH; x++)
            {
                display.write(screenBuffer[y][x]);
            }
            lineDirty[y] = 0;
        }
    }
    if (inputMode)
    {
    	moveCursor(curX, curY);
    	display.write('\x16');
    }
    display.display();
}




// graphic functions


void color(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 0 && g == 0 && b == 0)
		fgColor = BLACK;
	else
		fgColor = WHITE;
}

void background(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 0 && g == 0 && b == 0)
		bgColor = BLACK;
	else
		bgColor = WHITE;
}



void plot(int x, int y)
{
	display.writePixel(x,y,(unsigned)fgColor);
	display.display();
}



void line(int x, int y, int x1, int y1)
{
	display.drawLine(x, y, x1, y1, fgColor);
	display.display();
}


void circle(int x, int y, int r)
{
	display.drawCircle(x, y, r, fgColor);
	display.display();
}


void rect(int x, int y, int w, int h)
{
	display.drawRect(x, y, w, h, fgColor);
	display.display();
}





} /* namespace screen */


