#ifndef ARDUINO_H_
#define ARDUINO_H_
#include "stdint.h"

void             Arduino_Data_Transmit(uint8_t *Recive_data, uint8_t len);
void             Arduino_Data_Resive(uint8_t *data, uint8_t from);
void             ArduinoUpWritePointer();
void             ArduinoDownWritePointer(void);
void             ArduinoSignature();
void             Arduino_Loop(void);

#endif /* RTC_H_ */
