/*
	Copyright 2020 Mario Pascucci <mpascucci@gmail.com>

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
 * Chatpad.inc
 *
 *  Created on: 18 apr 2020
 *      Author: Mario Pascucci
 */


#include "Arduino.h"
#include "Chatpad.h"

#define KEYBUFFERSIZE 16


namespace keyboard {

Chatpad pad;

char keyBuffer[KEYBUFFERSIZE];

int head = 0, tail = 0;


int keyCount(void)
{
	int num;
	if (tail > head)
		num = head + KEYBUFFERSIZE - tail;
	else
		num = head - tail;
	return num;
}



void keyCallback(Chatpad &pad, Chatpad::keycode_t key, Chatpad::eventtype_t event)
{
	if (keyCount() >= KEYBUFFERSIZE)
		return;
	if (event == Chatpad::Down)
	{
		char c = pad.toAscii(key);
		if (c != 0)
		{
			keyBuffer[head] = c;
			head++;
			head %= KEYBUFFERSIZE;
		}
	}
}




void init(void)
{
	pad.init(Serial1,keyCallback);
}



bool available(void)
{
	pad.poll();
	return keyCount() > 0;
}



char read(void)
{
	if (keyCount() > 0) {
		char c = keyBuffer[tail];
		tail++;
		tail %= KEYBUFFERSIZE;
		return c;
	}
	return 0;
}



} /* namespace keyboard */
