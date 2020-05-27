/*
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
 * adapted by Mario Pascucci
 * for MegaBasic project (c) 2020
 */

#include "Arduino.h"

#include "Chatpad.h"
#include "HardwareSerial.h"

#include <avr/pgmspace.h>

// Masks for modifier bits
static const byte kShiftMask = (1 << 0);
static const byte kGreenSquareMask = (1 << 1);
static const byte kOrangeCircleMask = (1 << 2);
static const byte kPeopleMask = (1 << 3);

// Voodoo protocol messages
static const byte kInitMessage[] = { 0x87, 0x02, 0x8C, 0x1F, 0xCC };
static const byte kAwakeMessage[] = { 0x87, 0x02, 0x8C, 0x1B, 0xD0 };

static volatile bool toggle1 = 0;
static HardwareSerial * isrserial;



// needed to maintain chatpad "awake"
// using poll or checks on milliseconds doesn't work if system is busy
ISR(TIMER1_COMPA_vect)        // interrupt service routine
{
	if (toggle1){
		digitalWrite(LED_BUILTIN,HIGH);
		toggle1 = 0;
	}
	else{
		digitalWrite(LED_BUILTIN,LOW);
		toggle1 = 1;
	}
    isrserial->write(kAwakeMessage, sizeof(kAwakeMessage));

}


void Chatpad::init(HardwareSerial &serial, Chatpad::callback_t callback) {
  _serial = &serial;
  _callback = callback;
  _last_modifiers = 0;
  _last_key0 = 0;
  _last_key1 = 0;
  _last_ping = 0;
  _serial->begin(19200);

  // init sequence for clone chatpad (not original)
  // from https://github.com/KeiTakagi/XboxChatpad
  while (!_serial) delay(100);
  delay(500);
  for (int i=0;i<3;i++)
  {
	  _serial->write(kInitMessage, sizeof(kInitMessage));
	  delay(40);
  }
  // setup timer 1 to 1sec interrupt
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  isrserial = &serial;
  interrupts();             // enable all interrupts
}



void Chatpad::poll() {
  // Only act if a full message is available.
  if (_serial->available() >= 8) {
//#if MEGA_DEBUG
//	  Serial.println("SA");
//#endif // MEGA_DEBUG
	// try to synchronize for first byte
	if (_serial->peek() != 0xB4)
	{
	  _serial->read();
//#if MEGA_DEBUG
//	  Serial.print(b,HEX);
//	  Serial.print('-');
//#endif // MEGA_DEBUG
	  return;
	}
    for (int i = 0; i < 8; i++) {
      _buffer[i] = _serial->read();
//#if MEGA_DEBUG
//      Serial.print(_buffer[i],HEX);
//      Serial.print('-');
//#endif // MEGA_DEBUG
    }

    // We expect "status report" packets beginning with 0xA5, but don't know
    // what to do with them -- so we silently discard them.
    if (_buffer[0] == 0xA5) return;

    // We *do not* expect other types of packets.  If we find one, complain
    // to the user.
    if (_buffer[0] != 0xB4) {
      Serial.print("Unexpected packet type: ");
      Serial.println(_buffer[0], HEX);
      return;
    }
    if (_buffer[1] != 0xC5) {
      Serial.print("Unexpected second byte: ");
      Serial.println(_buffer[1], HEX);
      return;
    }

    // Check the checksum.
    unsigned char checksum = _buffer[0];
    for (int i = 1; i < 7; i++) checksum += _buffer[i];
    checksum = -checksum;
    if (checksum != _buffer[7]) {
      Serial.println("Checksum failure");
      return;
    }

    // Packet looks good!
    // Dissect the parts we care about:
    byte modifiers = _buffer[3];
    byte key0 = _buffer[4];
    byte key1 = _buffer[5];

    // Check for changes in the modifiers.
    byte modifier_changes = modifiers ^ _last_modifiers;
    if (modifier_changes & kShiftMask) {
      dispatch(KeyShift, modifiers & kShiftMask);
    }
    if (modifier_changes & kGreenSquareMask) {
      dispatch(KeyGreenSquare, modifiers & kGreenSquareMask);
    }
    if (modifier_changes & kOrangeCircleMask) {
      dispatch(KeyOrangeCircle, modifiers & kOrangeCircleMask);
    }
    if (modifier_changes & kPeopleMask) {
      dispatch(KeyPeople, modifiers & kPeopleMask);
    }
    _last_modifiers = modifiers;

    // Check for changes in the other keys
    if (key0 && key0 != _last_key0 && key0 != _last_key1) {
      dispatch(key0, Down);
    }
    if (key1 && key1 != _last_key0 && key1 != _last_key1) {
      dispatch(key1, Down);
    }
    if (_last_key0 && _last_key0 != key0 && _last_key0 != key1) {
      dispatch(_last_key0, Up);
    }
    if (_last_key1 && _last_key1 != key0 && _last_key1 != key1) {
      dispatch(_last_key1, Up);
    }
    _last_key0 = key0;
    _last_key1 = key1;
  }

//  uint32_t time = millis();
//  if (time - _last_ping > 1000)
//  {
//    _last_ping = time;
//     _serial->write(kAwakeMessage, sizeof(kAwakeMessage));
//  }
}

bool Chatpad::isShiftDown() const {
  return _last_modifiers & kShiftMask;
}

bool Chatpad::isGreenSquareDown() const {
  return _last_modifiers & kGreenSquareMask;
}

bool Chatpad::isOrangeCircleDown() const {
  return _last_modifiers & kOrangeCircleMask;
}

bool Chatpad::isPeopleDown() const {
  return _last_modifiers & kPeopleMask;
}

void Chatpad::dispatch(uint8_t code, int is_down) {
  _callback(*this, (keycode_t) code, is_down? Down : Up);
}



/**************
 * Translation
 */


char Chatpad::toAscii(keycode_t code) {
	unsigned int index = (((code - 0x11) & 0x70) >> 1) | ((code - 0x11) & 0x7);

	index *= 5;

	if (isShiftDown())
		index += 1;
	else if (isGreenSquareDown())
		index += 2;
	else if (isOrangeCircleDown())
		index += 3;
	else if (isPeopleDown())
		index += 4;
	if (index >= sizeof(kAsciiTable)) return 0;
    return pgm_read_byte_near(kAsciiTable + index);
}



