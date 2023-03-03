#include <stdint.h>
#include <Arduino.h>

#define TOUCH_SENSE_DONE  ((TSI0_GENCS & TSI_GENCS_SCNIP) == 0)
#define FAIL    (-1)

#define CURRENT   2  // 0 to 15 - current to use, value is 2*(current+1)
#define NSCAN     9  // number of times to scan, 0 to 31, value is nscan+1
#define PRESCALE  2  // prescaler, 0 to 7 - value is 2^(prescaler+1)

static const uint8_t pin2tsi[] = {
    //0    1    2    3    4    5    6    7    8    9
    9,  10, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255,  13,   0,   6,   8,   7,
    255, 255,  14,  15, 255, 255, 255, 255, 255,  11,
    12, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

uint32_t ch;

int  touchSenseInit(uint8_t pin){

  ch = pin2tsi[pin];
  if (ch > 15) return FAIL;

  *portConfigRegister(pin) = PORT_PCR_MUX(0);

  SIM_SCGC5 |= SIM_SCGC5_TSI;
  TSI0_GENCS = TSI_GENCS_REFCHRG(4) | TSI_GENCS_EXTCHRG(3) | TSI_GENCS_PS(PRESCALE) | TSI_GENCS_DVOLT(0) 
  | TSI_GENCS_NSCN(NSCAN) | TSI_GENCS_TSIEN | TSI_GENCS_EOSF;
  TSI0_DATA = TSI_DATA_TSICH(ch) | TSI_DATA_SWTS;
  delayMicroseconds(10);  // todo: Is this needed?
  return 0;
}

bool  touchSenseDone(){
  if (TSI0_GENCS & TSI_GENCS_SCNIP) {
    return false;
  } else {
    return true;
  }
}

int  touchSenseRead(){
  return TSI0_DATA & 0xFFFF;
}
