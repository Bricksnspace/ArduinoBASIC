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
 * host.h
 *
 *  Created on: 08 mag 2019
 *      Author: mario
 */

#ifndef HOST_H_
#define HOST_H_

#include "Arduino.h"

// System configuration
#include "MegaBASIC_config.h"

// keyboard
#if WITH_PS2KEY
#include <PS2Key.h>
namespace keyboard = ps2key;
#elif WITH_SERIAL_INPUT
#include "TTYKey.h"
namespace keyboard = ttykey;
#else
#error "Please choose a keyboard type"
#endif	// WITH_PS2KEY

// screen
#if WITH_EXT_SCREEN
#if SPI_ST7735
#include "ST7735Screen.h"
namespace screen = st7735screen;
#elif SPI_SSD1306
#include "SSD1306Screen.h"
namespace screen = ssd1306screen;
#elif I2C_SSD1306
#include "SSD1306Screen.h"
namespace screen = ssd1306screen;
#elif SPI_ILI9341
#include "ILI9341Screen.h"
namespace screen = ili9341screen;
#endif
#elif WITH_SERIAL_CONSOLE
#include "TTYScreen.h"
namespace screen = ttyscreen;
#else
#error "Please choose a screen type"
#endif 	// WITH_EXT_SCREEN

// buzzer
#if WITH_BUZZER
#include "PiezoBuzzer.h"
namespace buzzer = piezobuzzer;
#else
#include "Buzzer.h"
namespace buzzer = nullbuzzer;
#endif // WITH_BUZZER


// program memory
#define WITH_INTERNAL_RAM 	1

#define WITH_EXTERNAL_RAM	0

#if WITH_INTERNAL_RAM
#define MEMORY_SIZE	512
#elif WITH_EXTERNAL_RAM
#define MEMORY_SIZE 32768
#else
#error "Please choose RAM type"
#endif	// RAM



#define MAGIC_AUTORUN_NUMBER    0xFC

namespace host {

void init(void);
void click(void);
void startupTone(void);
void sleep(long ms);
void digitalWrite(int pin,int state);
int digitalRead(int pin);
int analogRead(int pin);
void pinMode(int pin, int mode);
void cls();
void showBuffer(void);
//void scrollBuffer(void);
void moveCursor(int x, int y);
void outputString(char *str);
void outputProgMemString(const char *str);
void outputChar(char c);
void outputFloat(float f);
char *floatToStr(float f, char *buf);
int outputInt(long val);
void newLine();
char *readLine();
char getKey();
bool ESCPressed();
void outputFreeMem(unsigned int val);
void saveProgram(bool autoexec, int size);
int loadProgram();
// graphic functions
void color(int,int,int);
void plot(int, int);


#if WITH_EXT_EEPROM
#include <I2cMaster.h>
void writeExtEEPROM(unsigned int address, byte data);
void host_directoryExtEEPROM();
bool host_saveExtEEPROM(char *fileName);
bool host_loadExtEEPROM(char *fileName);
bool host_removeExtEEPROM(char *fileName);
#endif // WITH_EXT_EEPROM

};  // namespace host



#endif /* HOST_H_ */
