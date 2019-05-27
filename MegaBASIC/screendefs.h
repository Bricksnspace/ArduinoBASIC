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
 * screendefs.h
 *
 *  Created on: 05 mag 2019
 *      Author: Mario Pascucci
 */

#ifndef SCREENDEFS_H_
#define SCREENDEFS_H_


void init(void);
void cls(void);
void moveCursor(int, int);
void showBuffer(void);
void scrollBuffer(void);
int getX(void);
int getY(void);
void showCursor(bool );
void freeLine(void);
void blink(void);
void redraw(void);
void outputString(char const *s);
void outputProgMemString(const char *s);
int outputChar(char);
int outputInt(long num);
void newLine(void);
char *getBuffer(int , int );
// graphic functions
void color(unsigned char,unsigned char, unsigned char);
void color(unsigned int);
void plot(int,int);



#endif /* SCREENDEFS_H_ */
