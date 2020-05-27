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
 * Screen.cpp
 *
 *  Created on: 02 nov 2019
 *      Author: Mario Pascucci
 */


// System configuration
#include "MegaBASIC_config.h"


#include "Screen.h"


char screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
char lineDirty[SCREEN_HEIGHT];
int curX = 0, curY = 0;
int markX = 0, markY = 0;
bool inputMode = false;



#if WITH_EXT_SCREEN
#if SPI_ST7735
#include "ST7735Screen.inc"
#elif SPI_SSD1306
#include "SSD1306Screen.inc"
#elif I2C_SSD1306
#include "SSD1306Screen.inc"
#elif SPI_ILI9341
#include "ILI9341Screen.inc"
#endif
#elif WITH_SERIAL_CONSOLE
#include "TTYScreen.inc"
#else
#error "Please choose a screen type"
#endif 	// WITH_EXT_SCREEN


namespace screen {


void screenOff(void)
{
#if SCR_BACKLIT
	digitalWrite(SCR_BACKLIT,0);
#endif
}


void screenOn(void)
{
#if SCR_BACKLIT
		digitalWrite(SCR_BACKLIT,1);
#endif
}





void cls(void)
{
	memset(screenBuffer, 32, sizeof(screenBuffer));
	memset(lineDirty, 0, SCREEN_HEIGHT);
	//background(0,0,255);			// blue background
	clear();
	moveCursor(0, 0);
	curX = curY = 0;
	lineDirty[0] = 1;
}


void scrollBuffer(void) {
    memcpy(screenBuffer[0], screenBuffer[1], (SCREEN_WIDTH)*(SCREEN_HEIGHT-1));
    memset(screenBuffer[SCREEN_HEIGHT-1], 32, SCREEN_WIDTH);
    memset(lineDirty, 1, SCREEN_HEIGHT);
    if (markY > 0)
    	markY--;
}


void outputString(char const* str)
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







int outputChar(char c)
{
	int num;
	switch (c)
	{
	case KEY_BACKSPACE:
    	if (curX == 0)
    	{
    		if (curY > 0 && curY > markY)
    		{
    			curY--;
    			curX = SCREEN_WIDTH;
    		}
    		else
    			return 0;
    	}
    	curX--;
    	screenBuffer[curY][curX] = ' ';
    	lineDirty[curY] = 1;
    	break;
	case '\t':
		num = TAB_SIZE - (curX % TAB_SIZE);
		if (num == 0)
			num = TAB_SIZE;
        lineDirty[curY] = 1;
		for (int i=0;i<num;i++)
		{
	        screenBuffer[curY][curX++] = ' ';
	        if (curX == SCREEN_WIDTH)
	        {
	        	curX--;
	        	break;
	        }
		}
		break;
	case '\n':
    	if (++curY >= SCREEN_HEIGHT)
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
	default:
        screenBuffer[curY][curX++] = c;
        lineDirty[curY] = 1;
        if (curX >= SCREEN_WIDTH)
        {
        	if (++curY >= SCREEN_HEIGHT)
        	{
        		scrollBuffer();
        		curY = SCREEN_HEIGHT - 1;
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

void startInput(void)
{
	if (curX > 0)
		newLine();
	markX = curX;
	markY = curY;
}



void showCursor(bool show)
{
	inputMode = show;
	lineDirty[curY] = 1;
	showBuffer();
}

char* getBuffer()
{
	screenBuffer[curY][curX] = 0;	// buffer termination
	return &screenBuffer[markY][markX];
}






}


