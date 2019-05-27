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
 * PiezoBuzzer.cpp
 *
 *  Created on: 10 apr 2019
 *      Author: Mario Pascucci
 */


#include <Arduino.h>
#include "MegaBASIC_config.h"
#include "PiezoBuzzer.h"


namespace piezobuzzer {



void init(void) {
	pinMode(BUZZER_PIN, OUTPUT);
}


void click(void) {
	digitalWrite(BUZZER_PIN, HIGH);
    delay(1);
    digitalWrite(BUZZER_PIN, LOW);
}


void startupTone(void) {
	for (int i=1; i<=2; i++) {
		for (int j=0; j<50*i; j++) {
			digitalWrite(BUZZER_PIN, HIGH);
			delay(3-i);
			digitalWrite(BUZZER_PIN, LOW);
			delay(3-i);
		}
		delay(100);
	}
}


}	// namespace piezobuzzer


