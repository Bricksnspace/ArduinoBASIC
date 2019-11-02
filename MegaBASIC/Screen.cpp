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


