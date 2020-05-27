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
 * ST7735Screen.h
 *
 *  Created on: 07 apr 2019
 *      Author: Mario Pascucci
 */

/*
 * Pinout
 * 1 - Vcc
 * 2 - GND
 * 3 - CS			-> mega pin 53
 * 4 - Reset		-> mega pin 48 or Reset
 * 5 - A0 (C/D)		-> mega pin 49
 * 6 - SDA (MOSI)	-> mega pin 51
 * 7 - SCK			-> mega pin 52
 * 8 - Backlight	-> 5V
 *
 */


#ifndef ST7735SCREEN_H_
#define ST7735SCREEN_H_

#include "Adafruit_ST7735.h"


// standard values for 5x7 characters
#define CHAR_WIDTH	6
#define LINE_HEIGHT	8


#endif /* ST7735SCREEN_H_ */
