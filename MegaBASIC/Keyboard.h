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
 * Keyboard.h
 *
 *  Created on: 02 nov 2019
 *      Author: Mario Pascucci
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

namespace keyboard {

void init(void);
bool available(void);
char read(void);

} /* namespace keyboard */

#endif /* KEYBOARD_H_ */
