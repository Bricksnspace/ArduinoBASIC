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
 * ILI9341Screen.cpp
 *
 *  Created on: 03 giu 2019
 *      Author: Mario Pascucci
 */



#include <Arduino.h>
#include "ILI9341Screen.h"



namespace screen {

uint16_t fgColor = ILI9341_YELLOW;
uint16_t bgColor = ILI9341_BLUE;


Adafruit_ILI9341 tft = Adafruit_ILI9341(SCR_SPI_CS, SCR_SPI_DC, SCR_SPI_RST);


void init()
{
	tft.begin();
	tft.setRotation(3);
	tft.setTextColor(fgColor,bgColor);
	tft.setTextWrap(false);
#if SCR_BACKLIT != 0
	{
		pinMode(SCR_BACKLIT, 1);
		digitalWrite(SCR_BACKLIT,1);
	}
#endif
}



void clear(void)
{
	tft.setTextColor(fgColor,bgColor);
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



//void scrollBuffer(void) {
//    memcpy(screenBuffer[0], screenBuffer[1], (SCREEN_WIDTH)*(SCREEN_HEIGHT-1));
//    memset(screenBuffer[SCREEN_HEIGHT-1], 32, SCREEN_WIDTH);
//    memset(lineDirty, 1, SCREEN_HEIGHT);
//    if (markY > 0)
//    	markY--;
//}
//
//
//void outputString(char const* str)
//{
//	while (*str)
//	{
//		outputChar(*str++);
//	}
//}
//
//
//
//void outputProgMemString(const char* p) {
//    while (1) {
//        unsigned char c = pgm_read_byte(p++);
//        if (c == 0) break;
//        outputChar((char)c);
//    }
//}
//
//
//

//int outputInt(long num)
//{
//	char buf[14];
//	sprintf(buf, "%ld",num);
//	int l = strlen(buf);
//	outputString(buf);
//	return l;
//}
//
//void newLine(void)
//{
//	outputChar('\n');
//}
//
//
//
//int getX(void) {
//	return curX;
//}
//
//int getY(void) {
//	return curY;
//}
//
//void startInput(void)
//{
//	if (curX > 0)
//		newLine();
//	markX = curX;
//	markY = curY;
//}
//
//
//
//void showCursor(bool show)
//{
//	inputMode = show;
//	lineDirty[curY] = 1;
//	showBuffer();
//}
//
//char* getBuffer()
//{
////#if MEGA_DEBUG
////	Serial.print("r=");
////	Serial.print(markY);
////	Serial.print(" c=");
////	Serial.print(markX);
////	Serial.print(" cr=");
////	Serial.print(curY);
////	Serial.print(" cc=");
////	Serial.println(curX);
////#endif
//	screenBuffer[curY][curX] = 0;	// buffer termination
//	return &screenBuffer[markY][markX];
//}
//
//
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
