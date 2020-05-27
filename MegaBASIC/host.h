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
 * host.h
 *
 *  Created on: 08 mag 2019
 *      Author: Mario Pascucci
 */

#ifndef HOST_H_
#define HOST_H_

#include "Arduino.h"

// System configuration
#include "MegaBASIC_config.h"

// keyboard
#include "Keyboard.h"

// screen
#include "Screen.h"

// buzzer
#include "Buzzer.h"

#if WITH_EXT_RAM
#include <Adafruit_FRAM_SPI.h>
#endif

#if WITH_SDCARD
#include <SD.h>
#define MAX_FILENAME	8
#endif

#define MAGIC_AUTORUN_NUMBER    0xFC

namespace host {

void init(void);
void click(void);
void startupTone(void);
void sleep(long ms);
void digitalWrite(int pin,int state);
#if WITH_EXT_RAM
xraddr xrmove(xraddr dest, xraddr src, xraddr size);
void xrread(xraddr addr, byte * buf, xraddr size);
byte xrread8(xraddr addr);
void xrwrite(xraddr, byte * buf, xraddr size);
void xrwrite8(xraddr addr, byte b);
#endif
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
void line(int, int, int, int);
void bgr(int, int, int);
void circle(int, int, int);
void rect(int, int, int, int);
// special functions
void reset(void);


#if WITH_SDCARD
int SdDir(void);
int SdSave(char* filename, byte* buf, uint16_t size);
int SdLoad(char* filename, byte* buf, int *len);
int SdDel(char* filename);
#endif

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
