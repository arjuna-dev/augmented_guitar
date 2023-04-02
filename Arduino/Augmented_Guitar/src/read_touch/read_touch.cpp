#include "core_pins.h"
#include "Arduino.h"
#include "../mux/mux.h"

#define NSCAN     9
#define PRESCALE  2
static const uint8_t pin2tsi[] = {
  //0    1    2    3    4    5    6    7    8    9
  9,  10, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,  13,   0,   6,   8,   7,
  255, 255,  14,  15, 255, 255, 255, 255, 255,  11,
  12, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

int teensyTouchReturn() {
  delayMicroseconds(1);
  return TSI0_DATA & 0xFFFF;
}

int teensyTouchInit(uint8_t pin) {
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

int teensyTouchDone() {
  if (TSI0_GENCS & TSI_GENCS_SCNIP) return false;
  else return true;
}

void teensyTouchRead(int touch_array[], int touch_array_size, int*& ptr_touch_array, int pin_array[],
                     int pin_array_size, int*& ptr_pin_array, int*& ptr_mux_ch_index) {

  if (teensyTouchDone()) {
    
    int* touch_array_end = touch_array + touch_array_size;
    int* pin_array_end = pin_array + pin_array_size;

    *ptr_touch_array = teensyTouchReturn();
    ptr_touch_array++;

    if (ptr_touch_array == touch_array_end) {
      ptr_touch_array = touch_array;
    }

    ptr_pin_array++;
    if (ptr_pin_array == pin_array_end) {
      ptr_pin_array = pin_array;
      (*ptr_mux_ch_index)++;
      if (*ptr_mux_ch_index == 16) {
        *ptr_mux_ch_index = 0;
      }
      selectMuxChannel(*ptr_mux_ch_index);
    }
    int current_pin = *ptr_pin_array;
    teensyTouchInit(current_pin);
  }
}
