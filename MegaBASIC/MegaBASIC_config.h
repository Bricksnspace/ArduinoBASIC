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
 * MegaBAISC_config.h
 *
 *  Created on: 08 mag 2019
 *      Author: mario
 */

#ifndef MEGABASIC_CONFIG_H_
#define MEGABASIC_CONFIG_H_


/****************************************
 * System configuration
 ****************************************/

//////////////////////////////////////////////////
// External display
//////////////////////////////////////////////////
#define WITH_EXT_SCREEN		1
// types of LCD/OLED screen
// select only one, then define rows/column and pins

// some monochrome OLED and small TFT screen
// screen size is 128x64 pixel
// 21chr * 8rows or 10chr * 16rows
#define SPI_SSD1306	0		// SPI interface (not implemented yet)
#define I2C_SSD1306	0		// I2C interface

// Like Adafruit 1.8" small RGB TFT without touch
// size 128x160
// 21chr * 20rows or 26chr * 16rows
#define SPI_ST7735	0

// RGB TFT display from 2.2" to 3.2"
// size 240x320
// 40chr * 40rows or 53chr * 30rows
#define SPI_ILI9341	1

// size in pixel
#define PIXEL_WIDTH		240
#define PIXEL_HEIGHT	320
// screen size in column/rows using a 5x7 base font, 6x8 character cell
#define SCREEN_WIDTH        40
#define SCREEN_HEIGHT       40
// Screen SPI pins (I2C pins are hardware SDA/SCL on Arduino)
// using hardware SPI
#define SCR_SPI_MOSI 51
#define SCR_SPI_MISO 50
#define SCR_SPI_CLK 52
#define SCR_SPI_DC 49
#define SCR_SPI_CS 53
// use -1 for reset if pin is connected to Arduino Reset
#define SCR_SPI_RST -1



//////////////////////////////////////////////////
// External EEPROM
//////////////////////////////////////////////////
#define WITH_EXT_EEPROM		0
// External EEPROM address and size
#define EXTERNAL_EEPROM_ADDR    0x50    // I2C address (7 bits)
#define EXTERNAL_EEPROM_SIZE    32768   // only <=32k tested (64k might work?)


//////////////////////////////////////////////////
// Serial console
//////////////////////////////////////////////////
#define WITH_SERIAL_CONSOLE 0
// same for keyboard
#define WITH_SERIAL_INPUT	0
#define SERIAL_SPEED		115200L



//////////////////////////////////////////////////
// PS/2 keyboard
//////////////////////////////////////////////////
#define WITH_PS2KEY			1
// PS2 pin definition
#define PS2DATA				8
#define PS2IRQ				3


//////////////////////////////////////////////////
// buzzer on Arduino pin
//////////////////////////////////////////////////
#define WITH_BUZZER			0
// buzzer pin
#define BUZZER_PIN			5


/************ END CONFIGURATION DIRECTIVES ****************/



#endif /* MEGABASIC_CONFIG_H_ */
