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


// TODO: to optimize text see scroll functions

namespace screen {

char screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH+1];
char lineDirty[SCREEN_HEIGHT];
int curX = 0, curY = 0;
bool inputMode = false;
volatile bool flash = 1, needRedraw = 0;
int curColor = WHITE;


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
	display.setTextColor(WHITE,BLACK);
	display.setTextWrap(false);
}

void cls(void)
{
	memset(screenBuffer, 32, sizeof(screenBuffer));
	display.clearDisplay();
	display.setCursor(0, 0);
	display.display();
	curX = curY = 0;
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
            screenBuffer[y][SCREEN_WIDTH] = 0;	// make sure string is terminated
            display.print(screenBuffer[y]);
//            for (int x=0; x<SCREEN_WIDTH; x++) {
//                char c = screenBuffer[y][x];
                // XXX move to buffer writing if (c=='\r') c = ' ';
//                tft.write(c);
//            }
            lineDirty[y] = 0;
        }
    }
    if (inputMode && flash)
    {
    	display.setCursor(curX*CHAR_WIDTH, curY*LINE_HEIGHT);
    	display.print('\x16');
    }
    display.display();
}


void blink(void) {
	flash = !flash;
}


//void redraw(void)
//{
//	needRedraw = true;
//}


void scrollBuffer(void) {
    memcpy(screenBuffer[0], screenBuffer[1], (SCREEN_WIDTH+1)*(SCREEN_HEIGHT-1));
    memset(screenBuffer[SCREEN_HEIGHT-1], 32, SCREEN_WIDTH);
    memset(lineDirty, 1, SCREEN_HEIGHT);
}


int outputChar(char c)
{
	//tft.print(c);
//    int pos = curY*SCREEN_WIDTH+curX;
	switch (c)
	{
	case 0x7f:
	//if (c==0x7f)		// ASCII Del as Backspace
    //{
    	if (curX == 0)
    	{
    		if (curY > 0)
    		{
    			curY--;
    			curX = SCREEN_WIDTH;
    		}
    		else
    			return 0;
    	}
    	curX--;
    	screenBuffer[curY][curX] = 32;
    	lineDirty[curY] = 1;
    	break;
    //}
	case '\n':
		// do a "newline" with implicit "CR"
    //else if (c == '\n')
    //{
    	if (++curY == SCREEN_HEIGHT)
    	{
    		scrollBuffer();
    		curY--;
    	}
    	curX = 0;
    	break;
	case '\r':
		// do a "CR" without a newline
		curX = 0;
		lineDirty[curY] = 1;
		break;
    //}
	default:
        screenBuffer[curY][curX++] = c;
        lineDirty[curY] = 1;
        if (curX >= SCREEN_WIDTH)
        {
        	if (++curY >= SCREEN_HEIGHT)
        	{
        		scrollBuffer();
        		curY--;
        	}
        	curX = 0;
        }
    }
    return 0;
}


int outputInt(long num)
{
	char buf[14];
	sprintf(buf, "%ld",num);
	int l = strlen(buf);
	outputString(buf);
	return l;
}



void outputString(char * str)
{
	while (*str)
	{
		outputChar(*str++);
	}
}



void outputProgMemString(const char* p) {
    while (1) {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0) break;
        outputChar((char)c);
    }
}





void newLine(void)
{
	outputChar('\n');
}



int getX(void) {
	return curX;
}


int getY(void) {
	return curY;
}


void freeLine(void) {
//    if (curX == 0) memset(screenBuffer + SCREEN_WIDTH*(curY), 32, SCREEN_WIDTH);
//    else newLine();
}



void showCursor(bool show)
{
	inputMode = show;
	lineDirty[curY] = 1;
	showBuffer();
}


char* getBuffer(int , int ) {
//	return screenBuffer+y*SCREEN_WIDTH+x;
	return nullptr;
}


// graphic functions


void color(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 0 && g == 0 && b == 0)
		curColor = BLACK;
	else
		curColor = WHITE;
}


void color(unsigned int c)
{
	// INVERSE doesn't work as expected, but leave here the code, just in case...
	if (c == 2) c = INVERSE;
	else if (c > 2) c = WHITE;
	curColor = c;
}


void plot(int x, int y)
{
	display.writePixel(x,y,(unsigned)curColor);
	display.display();
}


} /* namespace screen */


