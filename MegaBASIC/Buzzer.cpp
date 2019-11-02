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
 * Buzzer.cpp
 *
 *  Created on: 06 apr 2019
 *      Author: Mario Pascucci
 */


// System configuration
#include "MegaBASIC_config.h"

#include "Buzzer.h"



#if WITH_BUZZER
#include "PiezoBuzzer.inc"
#else
namespace buzzer {
void init(void) {};
void click(void) {};
void startupTone(void) {};
} /* namespace buzzer */
#endif


