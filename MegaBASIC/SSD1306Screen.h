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
 * SSD1306Screen.h
 *
 *  Created on: 23 mag 2019
 *      Author: mario
 */

#ifndef SSD1306SCREEN_H_
#define SSD1306SCREEN_H_


#include "MegaBASIC_config.h"
#include "Adafruit_SSD1306.h"


// standard values for 5x7 characters
#define CHAR_WIDTH	6
#define LINE_HEIGHT	8


namespace ssd1306screen {


#include "screendefs.h"


} /* namespace ssd1306screen */



#endif /* SSD1306SCREEN_H_ */
