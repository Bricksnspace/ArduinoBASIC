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
 * Keyboard.cpp
 *
 *  Created on: 02 nov 2019
 *      Author: Mario Pascucci
 */


// System configuration
#include "MegaBASIC_config.h"


#include "Keyboard.h"


#if WITH_PS2KEY
#include "PS2Key.inc"
#elif WITH_SERIAL_INPUT
#include "TTYKey.inc"
#elif WITH_MEGAPAD
#include "MegaPad.inc"
#else
#error "Please choose a keyboard type"
#endif	// WITH_PS2KEY


