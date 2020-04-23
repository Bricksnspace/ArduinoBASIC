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
 * host.cpp
 *
 *  Created on: 08 mag 2019
 *      Author: Mario Pascucci
 */


#include "Arduino.h"


#include "host.h"

extern unsigned char mem[];

#include <EEPROM.h>
extern EEPROMClass EEPROM;


//bool inputMode = 0;
char inkeyChar = 0;


const char bytesFreeStr[] PROGMEM = "bytes free\n";


#if WITH_EXT_SCREEN
// used for cursor blinking on external I2C/SPI display
#define BLINK_TIME	400			// in milliseconds
//int timer1_counter;
//#define TIMER_VAL	34286

//void initTimer() {
//    noInterrupts();           // disable all interrupts
//    TCCR1A = 0;
//    TCCR1B = 0;
//    //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
//    TCNT1 = TIMER_VAL;   // preload timer
//    TCCR1B |= (1 << CS12);    // 256 prescaler
//    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
//    interrupts();             // enable all interrupts
//}
//
//ISR(TIMER1_OVF_vect)        // interrupt service routine
//{
//    TCNT1 = TIMER_VAL;   // preload timer
//    screen::blink();
//    screen::redraw();
//}
#endif // WITH_EXT_SCREEN

namespace host {


void init()
{
	keyboard::init();
	screen::init();
	buzzer::init();
#if MEGA_DEBUG
	Serial.begin(115200);
#endif
#if WITH_EXT_SCREEN
    //initTimer();
#endif // WITH_EXT_SCREEN
}



void sleep(long ms) {
    delay(ms);
}

void digitalWrite(int pin,int state) {
    ::digitalWrite(pin, state ? HIGH : LOW);
}

int digitalRead(int pin) {
    return ::digitalRead(pin);
}

int analogRead(int pin) {
    return ::analogRead(pin);
}



void pinMode(int pin,int mode) {
	switch (mode)
	{
	case 0:
		::pinMode(pin, INPUT);
		break;
	case 1:
		::pinMode(pin, OUTPUT);
		break;
	case 2:
		::pinMode(pin, INPUT_PULLUP);
	}

}


void click() {
	buzzer::click();
}

void startupTone() {
	buzzer::startupTone();
}


long millisec()
{
	return millis();
}


void color(int r, int g, int b)
{
	screen::color(r,g,b);
}


void plot(int x, int y)
{
	screen::plot(x,y);
}


void cls() {
	screen::cls();
}

void moveCursor(int x, int y) {
	screen::moveCursor(x, y);
}

void showBuffer() {
	screen::showBuffer();
}

void scrollBuffer() {
	screen::scrollBuffer();
}

void outputString(char *str) {
	screen::outputString(str);
}

void outputProgMemString(const char *p) {
	screen::outputProgMemString(p);
}

void outputChar(char c) {
	screen::outputChar(c);
}

int outputInt(long num) {
	return screen::outputInt(num);
}

char *floatToStr(float f, char *buf) {
    // floats have approx 7 sig figs
    float a = fabs(f);
    if (f == 0.0f) {
        buf[0] = '0';
        buf[1] = 0;
    }
    else if (a<0.0001 || a>1000000) {
        // this will output -1.123456E99 = 14 characters max including trailing nul
    	// TODO da controllare cosa esce
        //dtostre(f, buf, 7, 0);	// ATMega
    	dtostrf(f, 1, 7, buf);	// stm32
    }
    else {
    	int decPos = 0;
    	if (f >= 100000.0) decPos = 1;
    	else if (f >= 10000.0) decPos = 2;
		else if (f >= 1000.0) decPos = 3;
		else if (f >= 100.0) decPos = 4;
    	else if (f >= 10.0) decPos = 5;
    	else if (f >= 1.0) decPos = 6;
    	else decPos = 7;
        dtostrf(f, 1, decPos, buf);
        if (decPos) {
            // remove trailing 0s
            char *p = buf;
            while (*p) p++;
            p--;
            while (*p == '0') {
                *p-- = 0;
            }
            if (*p == '.') *p = 0;
        }
    }
    return buf;
}

void outputFloat(float f) {
    char buf[16];
    screen::outputString(floatToStr(f, buf));
}

void newLine() {
	screen::newLine();
}


#if WITH_SERIAL_INPUT

char lineBuffer[128];

char *readLine()
{
	screen::showCursor(true);
	bool done = false;
	int l = 0;
	while (!done) {
		while (keyboard::available()) {
            char c = keyboard::read();
            buzzer::click();
            if (c == 0x0d)
            {
            	done = true;
            	lineBuffer[l] = 0;
            	break;
            }
            else if (c == 0x08 || c == 127)
            {
            	// backspace
            	if (l > 0)
            	{
            		l--;
            		screen::outputChar(0x7f);
            	}
            }
            else
            {
            	if (l == 127)
            	{
            		lineBuffer[l] = 0;
            		done = true;
            		break;
            	}
            	else
            	{
                	screen::outputChar(c);
                	lineBuffer[l++] = c;
            	}
            }
            screen::showBuffer();
		}
	}
	screen::showCursor(false);
	return lineBuffer;
}
#else
char *readLine()
{

	screen::showCursor(true);

    //if (curX == 0) memset(screenBuffer + SCREEN_WIDTH*(curY), 32, SCREEN_WIDTH);
    //else host_newLine();
	screen::freeLine();

	int startPosX = screen::getX();
	int startPosY = screen::getY();

    bool done = false;
    while (!done) {
        while (keyboard::available()) {
            char c = keyboard::read();
            buzzer::click();
            if (c == KEY_CR)
            {
            	done = true;
            	break;
            }
            // scroll if we need to
            if ((screen::getY() == SCREEN_HEIGHT-1) &&
            		screen::getX() == SCREEN_WIDTH-1) {
                if (startPosY > 0) {
                    startPosY--;
                    screen::outputChar(c);
                    screen::showBuffer();
                }
            }
            else
            {
            	screen::outputChar(c);
            	screen::showBuffer();
            }
        }
    }
    // remove the cursor
    screen::showCursor(false);
    screen::showBuffer();
    return screen::getBuffer(startPosX, startPosY);
}
#endif

char getKey() {
    char c = inkeyChar;
    inkeyChar = 0;
//    if (c >= 32 && c <= 126)
        return c;
//    else return 0;
}

bool ESCPressed() {
    while (keyboard::available()) {
        // read the next key
        inkeyChar = keyboard::read();
        if (inkeyChar == 27)	// Esc key
            return true;
    }
    return false;
}

void outputFreeMem(unsigned int val)
{
    //newLine();
    outputInt(val);
    outputChar(' ');
    outputProgMemString(bytesFreeStr);
}



void saveProgram(bool autoexec, int size) {
    EEPROM.write(0, autoexec ? MAGIC_AUTORUN_NUMBER : 0x00);
    EEPROM.write(1, size & 0xFF);
    EEPROM.write(2, (size >> 8) & 0xFF);
    for (int i=0; i<size; i++)
        EEPROM.write(3+i, mem[i]);
}


int loadProgram() {
    // skip the autorun byte
    int size = EEPROM.read(1) | (EEPROM.read(2) << 8);
    for (int i=0; i<size; i++)
        mem[i] = EEPROM.read(i+3);
    return size;
}


#if WITH_SDCARD


int SdDir(void)
{
	File root;
	if (!SD.begin(SDCS)) {
		Serial.println("initialization failed!");
		return -1;
	}
	Serial.println("initialization done.");

	root = SD.open("/");
	for (;;)
	{
		File entry =  root.openNextFile();
		if (! entry) {
			// no more files
			break;
		}
		outputString(entry.name());
		if (entry.isDirectory())
		{
			outputChar('/');
		}
		else
		{
			outputChar('\t');
			outputInt(entry.size()/1024);
			outputChar('k');
			outputChar('\n');
		}
		entry.close();
	}
	root.close();
	return 0;
}


bool SdSave(char* filename)
{

}


#endif



#if EXTERNAL_EEPROM
#include <I2cMaster.h>
extern TwiMaster rtc;

void writeExtEEPROM(unsigned int address, byte data)
{
  if (address % 32 == 0) buzzer.click();
  rtc.start((EXTERNAL_EEPROM_ADDR<<1)|I2C_WRITE);
  rtc.write((int)(address >> 8));   // MSB
  rtc.write((int)(address & 0xFF)); // LSB
  rtc.write(data);
  rtc.stop();
  delay(5);
}

byte readExtEEPROM(unsigned int address)
{
  rtc.start((EXTERNAL_EEPROM_ADDR<<1)|I2C_WRITE);
  rtc.write((int)(address >> 8));   // MSB
  rtc.write((int)(address & 0xFF)); // LSB
  rtc.restart((EXTERNAL_EEPROM_ADDR<<1)|I2C_READ);
  byte b = rtc.read(true);
  rtc.stop();
  return b;
}

// get the EEPROM address of a file, or the end if fileName is null
unsigned int getExtEEPROMAddr(char *fileName) {
    unsigned int addr = 0;
    while (1) {
        unsigned int len = readExtEEPROM(addr) | (readExtEEPROM(addr+1) << 8);
        if (len == 0) break;

        if (fileName) {
            bool found = true;
            for (int i=0; i<=strlen(fileName); i++) {
                if (fileName[i] != readExtEEPROM(addr+2+i)) {
                    found = false;
                    break;
                }
            }
            if (found) return addr;
        }
        addr += len;
    }
    return fileName ? EXTERNAL_EEPROM_SIZE : addr;
}

void host_directoryExtEEPROM() {
    unsigned int addr = 0;
    while (1) {
        unsigned int len = readExtEEPROM(addr) | (readExtEEPROM(addr+1) << 8);
        if (len == 0) break;
        int i = 0;
        while (1) {
            char ch = readExtEEPROM(addr+2+i);
            if (!ch) break;
            host_outputChar(readExtEEPROM(addr+2+i));
            i++;
        }
        addr += len;
        host_outputChar(' ');
    }
    host_outputFreeMem(EXTERNAL_EEPROM_SIZE - addr - 2);
}

bool host_removeExtEEPROM(char *fileName) {
    unsigned int addr = getExtEEPROMAddr(fileName);
    if (addr == EXTERNAL_EEPROM_SIZE) return false;
    unsigned int len = readExtEEPROM(addr) | (readExtEEPROM(addr+1) << 8);
    unsigned int last = getExtEEPROMAddr(NULL);
    unsigned int count = 2 + last - (addr + len);
    while (count--) {
        byte b = readExtEEPROM(addr+len);
        writeExtEEPROM(addr, b);
        addr++;
    }
    return true;
}

bool host_loadExtEEPROM(char *fileName) {
    unsigned int addr = getExtEEPROMAddr(fileName);
    if (addr == EXTERNAL_EEPROM_SIZE) return false;
    // skip filename
    addr += 2;
    while (readExtEEPROM(addr++)) ;
    sysPROGEND = readExtEEPROM(addr) | (readExtEEPROM(addr+1) << 8);
    for (int i=0; i<sysPROGEND; i++)
        mem[i] = readExtEEPROM(addr+2+i);
}

bool host_saveExtEEPROM(char *fileName) {
    unsigned int addr = getExtEEPROMAddr(fileName);
    if (addr != EXTERNAL_EEPROM_SIZE)
        host_removeExtEEPROM(fileName);
    addr = getExtEEPROMAddr(NULL);
    unsigned int fileNameLen = strlen(fileName);
    unsigned int len = 2 + fileNameLen + 1 + 2 + sysPROGEND;
    if ((long)EXTERNAL_EEPROM_SIZE - addr - len - 2 < 0)
        return false;
    // write overall length
    writeExtEEPROM(addr++, len & 0xFF);
    writeExtEEPROM(addr++, (len >> 8) & 0xFF);
    // write filename
    for (int i=0; i<strlen(fileName); i++)
        writeExtEEPROM(addr++, fileName[i]);
    writeExtEEPROM(addr++, 0);
    // write length & program
    writeExtEEPROM(addr++, sysPROGEND & 0xFF);
    writeExtEEPROM(addr++, (sysPROGEND >> 8) & 0xFF);
    for (int i=0; i<sysPROGEND; i++)
        writeExtEEPROM(addr++, mem[i]);
    // 0 length marks end
    writeExtEEPROM(addr++, 0);
    writeExtEEPROM(addr++, 0);
    return true;
}

#endif


}	// namespace host



