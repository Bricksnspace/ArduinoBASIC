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
 * BasicErrors.h
 *
 *  Created on: 25 apr 2020
 *      Author: mario
 */


#include <avr/pgmspace.h>


#ifndef BASICERRORS_H_
#define BASICERRORS_H_


#define ERROR_NONE							0
// parse errors
#define ERROR_LEXER_BAD_NUM					1
#define ERROR_LEXER_TOO_LONG				2
#define ERROR_LEXER_UNEXPECTED_INPUT	    3
#define ERROR_LEXER_UNTERMINATED_STRING     4
#define ERROR_EXPR_MISSING_BRACKET			5
#define ERROR_UNEXPECTED_TOKEN				6
#define ERROR_EXPR_EXPECTED_NUM				7
#define ERROR_EXPR_EXPECTED_STR				8
#define ERROR_LINE_NUM_TOO_BIG				9
// runtime errors
#define ERROR_OUT_OF_MEMORY					10
#define ERROR_EXPR_DIV_ZERO					11
#define ERROR_VARIABLE_NOT_FOUND			12
#define ERROR_UNEXPECTED_CMD				13
#define ERROR_BAD_LINE_NUM					14
#define ERROR_BREAK_PRESSED					15
#define ERROR_NEXT_WITHOUT_FOR				16
#define ERROR_STOP_STATEMENT				17
#define ERROR_MISSING_THEN					18
#define ERROR_RETURN_WITHOUT_GOSUB			19
#define ERROR_WRONG_ARRAY_DIMENSIONS	    20
#define ERROR_ARRAY_SUBSCRIPT_OUT_RANGE	    21
#define ERROR_STR_SUBSCRIPT_OUT_RANGE	    22
#define ERROR_IN_VAL_INPUT					23
#define ERROR_BAD_PARAMETER                 24
// I/O errors
#define ERROR_SD_NOT_FOUND					25
#define ERROR_IS_DIRECTORY					26
#define ERROR_FILE_EXISTS					27
#define ERROR_FILE_NOT_FOUND				28
#define ERROR_FILE_WRITE					29
#define ERROR_FILE_READ						30
#define ERROR_FILE_DELETE_FAIL				31
#define ERROR_VOLUME_NOT_FOUND				32
#define ERROR_ROOT_UNREADABLE				33








#endif /* BASICERRORS_H_ */
