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
#include <avr/wdt.h>

#include "host.h"
#include "BasicErrors.h"


#include <EEPROM.h>
extern EEPROMClass EEPROM;


//bool inputMode = 0;
char inkeyChar = 0;


const char bytesFreeStr[] PROGMEM = "bytes free\n";




#if !WITH_EXT_RAM
	unsigned char mem[RAMSIZE];
#endif // ! WITH_EXT_RAM


namespace host {

#if WITH_EXT_RAM
	Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI();  // use hardware SPI
#endif  // WITH_EXT_RAM

void init()
{
#if WITH_EXT_RAM
	fram.begin(RAMCS, RAMADDRBYTES);
#endif  // WITH_EXT_RAM
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

#if WITH_EXT_RAM

// read size bytes from address addr in buf
void xrread(xraddr addr, byte *buf, xraddr size)
{
	fram.read(addr, buf, size);
}



// read size bytes from address addr in buf
byte xrread8(xraddr addr)
{
	fram.read8(addr);
}



// external RAM version of memmove(dest,src,size)
void xrmove(xraddr dest, xraddr src, xraddr num)
{
	if (dest == src || num == 0)
		return;
	if (dest < src)
	{
		fram.writeEnable(true);
//		if (src-dest < 8)
//		{
//			for (xraddr i=0;i<num;i++)
//			{
//				fram.write8(dest+i,fram.read8(src+i));
//			}
//		}
//		else
//		{
			byte b[8];
			while (num > 8)
			{
				fram.read(src,b,8);
				fram.write(dest,b,8);
				num -= 8;
				src += 8;
				dest += 8;
			}
			fram.read(src,(byte*)&b,num);
			fram.write(dest, (byte*)&b, num);
//		}
		fram.writeEnable(false);
		return;
	}
	else
	{
		fram.writeEnable(true);
//		if (dest-src < 8)
//		{
//			// it is an unsigned, can't be less than 0
//			for (xraddr i=num;i<1;i--)
//			{
//				fram.write8(dest+i-1,fram.read8(src+i-1));
//			}
//		}
//		else
//		{
			byte b[8];
			while (num > 8)
			{
				num -= 8;
				fram.read(src+num,b,8);
				fram.write(dest+num,b,8);
			}
			fram.read(src,(byte*)&b,num);
			fram.write(dest, (byte*)&b, num);
//		}
		fram.writeEnable(false);
		return;
	}
}



// write size bytes to address addr
void xrwrite(xraddr addr, byte *buf, xraddr size)
{
	fram.writeEnable(true);
	fram.write(addr, buf, size);
	fram.writeEnable(false);
}


void xrwrite8(xraddr addr, byte b)
{
	fram.writeEnable(true);
	fram.write8(addr, b);
	fram.writeEnable(false);
}




#endif // WITH_EXT_RAM

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


void line(int x, int y, int x1, int y1)
{
	screen::line(x,y,x1,y1);
}


void circle(int x, int y, int r)
{
	screen::circle(x, y, r);
}



void rect(int x, int y, int w, int h)
{
	screen::rect(x, y, w, h);
}



void cls() {
	screen::cls();
}


void bgr(int r, int g, int b)
{
	screen::background(r,g,b);
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
        dtostre(f, buf, 7, 0);	// ATMega
    	//dtostrf(f, 1, 7, buf);	// stm32
    }
    else {
    	int decPos = 0;
    	if (a >= 100000.0) decPos = 1;
    	else if (a >= 10000.0) decPos = 2;
		else if (a >= 1000.0) decPos = 3;
		else if (a >= 100.0) decPos = 4;
    	else if (a >= 10.0) decPos = 5;
    	else if (a >= 1.0) decPos = 6;
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
    char buf[20];
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
	screen::startInput();

    bool done = false;
    while (!done) {
        while (keyboard::available()) {
            char c = keyboard::read();
            buzzer::click();
            if (c == KEY_CR)
            {
            	done = true;
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
    return screen::getBuffer();
}
#endif

char getKey() {
    char c = inkeyChar;
    inkeyChar = 0;
    return c;
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


void reset(void)
{
	wdt_disable();
	wdt_enable(WDTO_15MS);
	while (1) {};
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


//int initSD(SdVolume *vol)
//{
//	bool ret;
//	Sd2Card card;
//	ret = card.init(SPI_HALF_SPEED, SDCS);
//	if (!ret)
//		return ERROR_SD_NOT_FOUND;
//	ret = vol->init(card);
//	if (!ret)
//		return ERROR_VOLUME_NOT_FOUND;
//	return ERROR_NONE;
//}


int SdDir(void)
{
	File root;

//	ret = initSD(&v);
//	if (ret != ERROR_NONE)
//		return ret;
//	if (!root.openRoot(&v))
//		return ERROR_ROOT_UNREADABLE;
	if (!SD.begin(SPI_HALF_SPEED,SDCS)) {
		return ERROR_SD_NOT_FOUND;
	}
	root = SD.open("/");
	if (!root)
		return ERROR_ROOT_UNREADABLE;
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
		}
		outputChar('\n');
		entry.close();
	}
	root.close();
	SD.end();
	return ERROR_NONE;
}


int SdSave(char* filename, byte* buf, uint16_t size)
{
	File s;
	if (strlen(filename) > MAX_FILENAME)
		return ERROR_BAD_PARAMETER;
	if (!SD.begin(SPI_HALF_SPEED,SDCS)) {
		return ERROR_SD_NOT_FOUND;
	}
	char *e = filename + strlen(filename) - 4;
	if (strcasecmp(e,".bas") != 0)
		strcat(filename,".bas");
	if (SD.exists(filename))
	{
		SD.end();
		return ERROR_FILE_EXISTS;
	}
	s = SD.open(filename, FILE_WRITE);
    //for (uint16_t i=0; i<size; i++)
    s.write(buf, size);
    s.close();
    if (s.getWriteError())
	{
		SD.end();
    	return ERROR_FILE_WRITE;
	}
    SD.end();
    return ERROR_NONE;
}


int SdLoad(char* filename, byte* buf, int *len)
{
	File s;
	if (strlen(filename) > MAX_FILENAME)
		return ERROR_BAD_PARAMETER;
	if (!SD.begin(SPI_HALF_SPEED,SDCS)) {
		return ERROR_SD_NOT_FOUND;
	}
	char *e = filename + strlen(filename) - 4;
	if (strcasecmp(e,".bas") != 0)
		strcat(filename,".bas");
	if (!SD.exists(filename))
	{
		SD.end();
		return ERROR_FILE_NOT_FOUND;
	}
	s = SD.open(filename);
	if (s.isDirectory())
	{
		SD.end();
		return ERROR_IS_DIRECTORY;
	}
	uint16_t size = s.size();
	if (size > RAMSIZE)
	{
		SD.end();
		return ERROR_OUT_OF_MEMORY;
	}
	uint16_t l = s.read(buf, size);
	SD.end();
	if (l < size)	// short read, error?
		return ERROR_FILE_READ;
	*len = size;
	return ERROR_NONE;
}


int SdDel(char * filename)
{
	if (strlen(filename) > MAX_FILENAME)
		return ERROR_BAD_PARAMETER;
	if (!SD.begin(SPI_HALF_SPEED,SDCS)) {
		return ERROR_SD_NOT_FOUND;
	}
	char *e = filename + strlen(filename) - 4;
	if (strcasecmp(e,".bas") != 0)
		strcat(filename,".BAS");
	if (!SD.exists(filename))
	{
		SD.end();
		return ERROR_FILE_NOT_FOUND;
	}
	if (!SD.remove(filename))
	{
		SD.end();
		return ERROR_FILE_DELETE_FAIL;
	}
	SD.end();
	return ERROR_NONE;
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



