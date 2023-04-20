#include "Arduino.h"
#include "core_pins.h"
#include "teensy_tsi.h"

#define NSCAN     9
#define PRESCALE  2
static const uint8_t pin2tsi[] = {
  //0    1    2    3    4    5    6    7    8    9
  9,  10, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,  13,   0,   6,   8,   7,
  255, 255,  14,  15, 255, 255, 255, 255, 255,  11,
  12, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

TeensyTSI::TeensyTSI(){}

int TeensyTSI::teensyTouchReturn() {
  delayMicroseconds(1);
  return TSI0_DATA & 0xFFFF;
}

int TeensyTSI::teensyTouchInit(int pin) {
  uint32_t ch;

  if (pin >= NUM_DIGITAL_PINS) return -1;
  ch = pin2tsi[pin];
  if (ch == 255) return -1;

  *portConfigRegister(pin) = PORT_PCR_MUX(0);
  SIM_SCGC5 |= SIM_SCGC5_TSI;

  TSI0_GENCS = TSI_GENCS_REFCHRG(4) | TSI_GENCS_EXTCHRG(3) | TSI_GENCS_PS(PRESCALE)
               | TSI_GENCS_NSCN(NSCAN) | TSI_GENCS_TSIEN | TSI_GENCS_EOSF;
  TSI0_DATA = TSI_DATA_TSICH(ch) | TSI_DATA_SWTS;
  delayMicroseconds(10);
  return 0;
}

int TeensyTSI::teensyTouchDone() {
  if (TSI0_GENCS & TSI_GENCS_SCNIP) return false;
  else return true;
}