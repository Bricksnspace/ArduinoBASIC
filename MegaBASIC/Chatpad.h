/*
 * Chatpad.h - Use an Xbox360 mini-keyboard with your Arduino.
 *
 * Copyright (C) 2011 Cliff L. Biffle, all rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Adapted by Mario Pascucci
 * for MegaBasic project (c) 2020
 */


#ifndef CHATPAD_H_
#define CHATPAD_H_

#include <stdint.h>


#include "MegaBASIC_config.h"


// Declaration of Arduino's internal serial port type.
class HardwareSerial;


/*
 * A serial interface to the Xbox 360 chatpad.  Usage:
 *
 *  Chatpad pad;
 *
 *  void my_callback(Chatpad &pad, Chatpad::keycode_t key,
 *      Chatpad::eventtype_t event) {
 *    ...
 *  }
 *
 *  void setup() {
 *    ...
 *    pad.init(Serial3, my_callback);
 *  }
 *
 *  void loop() {
 *    ...
 *    pad.poll();
 *  }
 */



  // These tables have been compacted and must be accessed using this formula:
  //  index = (((keycode & 0xF0) - 0x10) >> 1) | ((keycode & 0x0F) - 1)

  // Normal, Shift  , Ctrl , Alt    , Win
  // Normal, Shift  , Green, Orange , People
  static const uint8_t kAsciiTable[] PROGMEM = {
    '7', '\'', 0 ,  0 ,  0 ,      /* 11 Key7 */
    '6', '&',  0 ,  0 ,  0 ,      /* 12 Key6 */
    '5', '%',  0 ,  0 ,  0 ,      /* 13 Key5 */
    '4', '$',  0 ,  0 ,  0 ,      /* 14 Key4 */
    '3', '#',  0 ,  0 ,  0 ,      /* 15 Key3 */
    '2', '"',  0 ,  0 ,  0 ,      /* 16 Key2 */
    '1', '!',  0 ,  0 ,  0 ,      /* 17 Key1 */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 18 Unused */

    'u', 'U', '&',  0 ,  0 ,      /* 21 KeyU */
    'y', 'Y', '`',  0 ,  0 ,      /* 22 KeyY */
    't', 'T', '%',  0 ,  0 ,      /* 23 KeyT */
    'r', 'R', '#', '$',  0 ,      /* 24 KeyR */
    'e', 'E',  0 ,  0 , KEY_ESC,  /* 25 KeyE */
    'w', 'W', '@',  0 ,  0 ,      /* 26 KeyW */
    'q', 'Q', '!',  0 ,  0 ,      /* 27 KeyQ */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 28 Unused */

    'j', 'J', '\'', '"',  0 ,     /* 31 KeyJ */
    'h', 'H', '/', '\\',  0 ,     /* 32 KeyH */
    'g', 'G',  0 ,  0 ,  0 ,      /* 33 KeyG */
    'f', 'F', '}', '?',  0 ,      /* 34 KeyF */
    'd', 'D', '{',  0 ,  0 ,      /* 35 KeyD */
    's', 'S',  0 ,  0 ,  0 ,      /* 36 KeyS */
    'a', 'A', '~',  0 ,  0 ,      /* 37 KeyA */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 38 Unused */

    'n', 'N', '<',  0 ,  0 ,      /* 41 KeyN */
    'b', 'B', '|', '+',  0 ,      /* 42 KeyB */
    'v', 'V', '-', '_',  0 ,      /* 43 KeyV */
    'c', 'C', KEY_ESC,  0 ,  0 ,  /* 44 KeyC */
    'x', 'X',  0 ,  0 ,  0 ,      /* 45 KeyX */
    'z', 'Z', '`',  0 ,  0 ,      /* 46 KeyZ */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 47 Unused */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 48 Unused */

    KEY_RIGHTARROW,  0 , KEY_UPARROW,  0 ,  0 , /* 51 KeyRight */
    'm', 'M', '>',  0 ,  0 ,      /* 52 KeyM */
    '.', '.', '?',  0 ,  0 ,      /* 53 KeyPeriod */
    ' ', ' ', ' ', ' ',  0 ,      /* 54 KeySpace  */
    KEY_LEFTARROW,  0 , KEY_DOWNARROW,  0 ,  0 , /* 55 KeyLeft */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 56 Unused */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 57 Unused */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 58 Unused */

     0 ,  0 ,  0 ,  0 ,  0 ,      /* 61 Unused */
    ',', ',', ':', ';',  0 ,      /* 62 KeyComma */
    KEY_CR, KEY_LF ,  0 ,  0 ,  0 , /* 63 KeyEnter */
    'p', 'P', ')', '=',  0 ,      /* 64 KeyP */
    '0',  0 ,  0 ,  0 ,  0 ,      /* 65 Key0 */
    '9', ')',  0 ,  0 ,  0 ,      /* 66 Key9 */
    '8', '(',  0 ,  0 ,  0 ,      /* 67 Key8 */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 68 Unused */

    KEY_BACKSPACE, KEY_DELETE,  0 ,  0 ,  0 , /* 71 KeyBackspace */
    'l', 'L', ']',  0 , KEY_CTRLL,/* 72 KeyL */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 73 Unused */
     0 ,  0 ,  0 ,  0 ,  0 ,      /* 74 Unused */
    'o', 'O', '(',  0 ,  0 ,      /* 75 KeyO */
    'i', 'I', '*',  0 ,  0 ,      /* 76 KeyI */
    'k', 'K', '[',  0 ,  0 ,      /* 77 KeyK */
     0 ,  0 ,  0 ,  0 ,  0        /* 78 Unused */
  };




class Chatpad {
public:
  // Provides names for the numeric keycodes.
  enum keycode_t {
    Key1 = 0x17,
    Key2 = 0x16,
    Key3 = 0x15,
    Key4 = 0x14,
    Key5 = 0x13,
    Key6 = 0x12,
    Key7 = 0x11,
    Key8 = 0x67,
    Key9 = 0x66,
    Key0 = 0x65,

    KeyQ = 0x27,
    KeyW = 0x26,
    KeyE = 0x25,
    KeyR = 0x24,
    KeyT = 0x23,
    KeyY = 0x22,
    KeyU = 0x21,
    KeyI = 0x76,
    KeyO = 0x75,
    KeyP = 0x64,

    KeyA = 0x37,
    KeyS = 0x36,
    KeyD = 0x35,
    KeyF = 0x34,
    KeyG = 0x33,
    KeyH = 0x32,
    KeyJ = 0x31,
    KeyK = 0x77,
    KeyL = 0x72,
    KeyComma = 0x62,

    KeyZ = 0x46,
    KeyX = 0x45,
    KeyC = 0x44,
    KeyV = 0x43,
    KeyB = 0x42,
    KeyN = 0x41,
    KeyM = 0x52,
    KeyPeriod = 0x53,
    KeyEnter = 0x63,

    KeyLeft = 0x55,
    KeySpace = 0x54,
    KeyRight = 0x51,
    KeyBackspace = 0x71,

    KeyShift = 0x81,
    KeyGreenSquare = 0x82,
    KeyPeople = 0x83,
    KeyOrangeCircle = 0x84,
  };



  enum eventtype_t {
    Up = 0,
    Down = 1,
  };

  typedef void (*callback_t)(Chatpad &, keycode_t, eventtype_t);

  /*
   * Sets up communications with the chatpad, including initializing the
   * serial port.
   */
  void init(HardwareSerial &, callback_t);

  /*
   * Processes any pending messages from the chatpad.
   */
  void poll();

  /*
   * Checks if Shift is down.  Use this during a callback.
   */
  bool isShiftDown() const;
  /*
   * Checks if Green Square is down.  Use this during a callback.
   */
  bool isGreenSquareDown() const;
  /*
   * Checks if Orange Circle is down.  Use this during a callback.
   */
  bool isOrangeCircleDown() const;
  /*
   * Checks if People is down.  Use this during a callback.
   */
  bool isPeopleDown() const;

  /*
   * Converts a key to ASCII, given the current status of the Shift
   * key.  If the key doesn't map to an ASCII character (example:
   * the left arrow key), returns 0.
   */
  char toAscii(keycode_t);

private:
  HardwareSerial *_serial;
  callback_t _callback;
  uint8_t _buffer[8];

  uint8_t _last_modifiers;
  uint8_t _last_key0;
  uint8_t _last_key1;

  uint32_t _last_ping;

  void dispatch(uint8_t, int is_down);
};



#endif /* CHATPAD_H_ */
