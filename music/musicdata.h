#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "notationdefines.h"
#include "../notes.h"

extern const uint8_t PROGMEM voiceOne[];
extern const uint8_t PROGMEM voiceTwo[];
extern const uint8_t PROGMEM voiceThree[];
extern const uint16_t PROGMEM voiceOneLength;
extern const uint16_t PROGMEM voiceTwoLength;
extern const uint16_t PROGMEM voiceThreeLength;

#endif
