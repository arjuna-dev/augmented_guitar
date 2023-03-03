#ifndef READ_TOUCH_H
#define READ_TOUCH_H

int touchSenseInit(uint8_t pin);
bool touchSenseDone();
int touchSenseRead();

#endif
