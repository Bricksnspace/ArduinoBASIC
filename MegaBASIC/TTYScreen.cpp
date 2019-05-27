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
 * TTYScreen.cpp
 *
 *  Created on: 09 apr 2019
 *      Author: Mario Pascucci
 */

#include <Arduino.h>
#include "MegaBASIC_config.h"
#include "TTYScreen.h"

namespace ttyscreen {

void init(void) {
	Serial.begin(SERIAL_SPEED);
	while (!Serial) {
	 // wait for serial port to connect. Needed for native USB port only
	}
}

void outputString(char const * s) {
	Serial.print(s);
}

void outputProgMemString(const char* s) {
	unsigned char c;
    while ((c = pgm_read_byte(s++)) != 0) {
        outputChar(c);
    }
}

int outputChar(char c) {
	Serial.print(c);
	return 0;
}

int outputInt(long num) {
	return Serial.print(num);
}

void newLine(void) {
	Serial.print('\n');
}

void cls(void) {};
void moveCursor(int, int) {};
void showBuffer(void) {};
void scrollBuffer(void) {};
int getX(void) { return 0; }
int getY(void) {return 0; }
void showCursor(bool ) {};
void freeLine(void) {};
void blink(void) {};
void redraw(void) {};
char *getBuffer(int , int ) { return nullptr; };


} /* namespace ttyscreen */
