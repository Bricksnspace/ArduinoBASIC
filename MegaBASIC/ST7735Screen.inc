/*
 Copyright 2019 Mario Pascucci <mpascucci@gmail.com>
 Copyright 2014 Robin Edwards

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
 * ST7735Sreen.cpp
 *
 *  Created on: 07 apr 2019
 *      Author: Mario Pascucci
 */


#include "ST7735Screen.h"
#include <Arduino.h>



namespace screen {

uint16_t fgColor = ST7735_YELLOW;
uint16_t bgColor = ST7735_BLUE;

Adafruit_ST7735 tft = Adafruit_ST7735(SCR_SPI_CS, SCR_SPI_DC, SCR_SPI_RST);


void init()
{
	tft.initR(INITR_BLACKTAB);
	tft.setTextColor(fgColor, bgColor);
	tft.setTextWrap(false);
#if (SCR_BACKLIT != 0)
	{
		pinMode(SCR_BACKLIT, 1);
		digitalWrite(SCR_BACKLIT,1);
	}
#endif
}




void clear(void)
{
	tft.setTextColor(fgColor, bgColor);
	tft.fillScreen(bgColor);
}



void moveCursor(int x, int y)
{
    tft.setCursor(x*CHAR_WIDTH, y*LINE_HEIGHT);
}


void showBuffer(void)
{
    for (int y=0; y<SCREEN_HEIGHT; y++) {
        if (lineDirty[y] || (inputMode && y==curY)) {
            moveCursor(0,y);
            for (int x=0; x<SCREEN_WIDTH; x++) 
            {
                tft.write(screenBuffer[y][x]);
            }
            lineDirty[y] = 0;
        }
    }
    if (inputMode)
    {
    	moveCursor(curX, curY);
    	tft.write('\x16');
    }
}





char* getBuffer(int , int ) {
//	return screenBuffer+y*SCREEN_WIDTH+x;
	return nullptr;
}


void color(unsigned char r, unsigned char g, unsigned char b)
{
	fgColor = tft.color565(r, g, b);
}


void background(unsigned char r, unsigned char g, unsigned char b)
{
	bgColor = tft.color565(r, g, b);
}





void plot(int x, int y)
{
	tft.startWrite();
	tft.writePixel(x, y, fgColor);
	tft.endWrite();
}



void line(int x, int y, int x1, int y1)
{
	tft.drawLine(x, y, x1, y1, fgColor);
}



void circle(int x, int y, int r)
{
	tft.drawCircle(x, y, r, fgColor);
}


void rect(int x, int y, int w, int h)
{
	tft.drawRect(x, y, w, h, fgColor);
}


} /* namespace screen */
