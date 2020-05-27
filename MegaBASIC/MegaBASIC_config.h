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
 *      Author: Mario Pascucci
 */

#ifndef MEGABASIC_CONFIG_H_
#define MEGABASIC_CONFIG_H_


/****************************************
 * System configuration
 ****************************************/

// control codes
#define KEY_CTRLC		3
#define KEY_BACKSPACE	8
#define KEY_TAB			9
#define KEY_LF			10
#define KEY_CTRLL		12
#define KEY_CR			13
#define KEY_ESC			27

#define KEY_RIGHTARROW    0x1E
#define KEY_LEFTARROW     0x1F
#define KEY_UPARROW       0x1C
#define KEY_DOWNARROW     0x1D
#define KEY_DELETE        127
#define KEY_HOME          0x01
#define KEY_INSERT        134
#define KEY_PAGEDOWN      135
#define KEY_PAGEUP        136
#define KEY_END           0x0F

// some system defaults
#define TAB_SIZE	8



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


#if SPI_ILI9341
// size in pixel
#define PIXEL_WIDTH		320
#define PIXEL_HEIGHT	240
// screen size in column/rows using a 5x7 base font, 6x8 character cell
// leave 1 char cell on left for sys icons
#define SCREEN_WIDTH        53
#define SCREEN_HEIGHT       30
// Screen SPI pins (I2C pins are hardware SDA/SCL on Arduino)
// using hardware SPI
#define SCR_SPI_MOSI 51
#define SCR_SPI_MISO 50
#define SCR_SPI_CLK 52
#define SCR_SPI_DC 46
#define SCR_SPI_CS 49
// use -1 for reset if pin is connected to Arduino Reset
#define SCR_SPI_RST -1
// set to backlit pin or 0 if none
#define SCR_BACKLIT 45
#endif


#if SPI_ST7735
// size in pixel
#define PIXEL_WIDTH		160
#define PIXEL_HEIGHT	128
// screen size in column/rows using a 5x7 base font, 6x8 character cell
// leave 1 char cell on left for sys icons
#define SCREEN_WIDTH        26
#define SCREEN_HEIGHT       16
// Screen SPI pins (I2C pins are hardware SDA/SCL on Arduino)
// using hardware SPI
#define SCR_SPI_MOSI 51
#define SCR_SPI_CLK 52
#define SCR_SPI_DC 46
#define SCR_SPI_CS 49
// use -1 for reset if pin is connected to Arduino Reset
#define SCR_SPI_RST -1
// set to backlit pin or 0 if none
#define SCR_BACKLIT 45
#endif


#if I2C_SSD1306
// size in pixel
#define PIXEL_WIDTH		128
#define PIXEL_HEIGHT	64
// screen size in column/rows using a 5x7 base font, 6x8 character cell
// leave 1 char cell on left for sys icons
#define SCREEN_WIDTH        21
#define SCREEN_HEIGHT       8
// no backlit
#define SCR_BACKLIT 0
#endif




//////////////////////////////////////////////////
// SD card
//////////////////////////////////////////////////
#define WITH_SDCARD			1
#define SDCS				47



//////////////////////////////////////////////////
// EXT RAM
//////////////////////////////////////////////////
#define WITH_EXT_RAM		0
#define RAMCS				48

#if WITH_EXT_RAM
#define RAMSIZE				32768
#define RAMADDRBYTES		2
// 2 byte = 16 bit addr
// 3,4 byte = 32 bit addr
typedef uint16_t	xraddr;
#else
#define RAMSIZE 4096
#endif


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
#define WITH_PS2KEY			0
// PS2 pin definition
#define PS2DATA				8
#define PS2IRQ				3



//////////////////////////////////////////////////
// MegaPad (a modified Chatpad)
//////////////////////////////////////////////////
#define WITH_MEGAPAD 		1



//////////////////////////////////////////////////
// buzzer on Arduino pin
//////////////////////////////////////////////////
#define WITH_BUZZER			0
// buzzer pin
#define BUZZER_PIN			5



//////////////////////////////////////////
// Debug serial output
//////////////////////////////////////////

#define MEGA_DEBUG 1

/************ END CONFIGURATION DIRECTIVES ****************/



#endif /* MEGABASIC_CONFIG_H_ */
