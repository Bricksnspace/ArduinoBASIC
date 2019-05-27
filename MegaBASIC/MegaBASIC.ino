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
 * Forked from ArduinoBASIC by Robin Edwards
 * https://github.com/robinhedwards/ArduinoBASIC
 */


#include "Arduino.h"


#include "basic.h"
#include "host.h"


// BASIC
unsigned char mem[MEMORY_SIZE];
#define TOKEN_BUF_SIZE    64
unsigned char tokenBuf[TOKEN_BUF_SIZE];

const char welcomeStr[] PROGMEM = "Arduino MegaBASIC\n";
char autorun = 0;



void setup()
{
	host::init();
    tinybasic::reset();
    host::cls();
    host::outputProgMemString(welcomeStr);
    // show memory size
    host::outputFreeMem(tinybasic::getFreeMem());
    host::startupTone();
}

// The loop function is called in an endless loop
void loop()
{
    int ret = ERROR_NONE;

    if (!autorun) {
        // get a line from the user
        char *input = host::readLine();
        host::newLine();
        // tokenize
        ret = tinybasic::tokenize((unsigned char*)input, tokenBuf, TOKEN_BUF_SIZE);
    }
    else {
        host::loadProgram();
        tokenBuf[0] = TOKEN_RUN;
        tokenBuf[1] = 0;
        autorun = 0;
    }
    //host::newLine();
    // execute the token buffer
    if (ret == ERROR_NONE) {
        ret = tinybasic::processInput(tokenBuf);
    }
    if (ret != ERROR_NONE) {
        if (tinybasic::getLineNumber() !=0) {
            host::outputInt(tinybasic::getLineNumber());
            host::outputChar('-');
        }
        host::outputProgMemString((char *)pgm_read_word(&(errorTable[ret])));
        host::newLine();
    }
}
