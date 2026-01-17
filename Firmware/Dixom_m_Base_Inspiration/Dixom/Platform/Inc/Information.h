#ifndef __DixomDevice_H__
#define __DixomDevice_H__

#include   "stdint.h"
#define    DixomC12       0
#define    DixomM         1
#define    DixomDevice    DixomM

void       Initialization_Firmware_Information();
void       Initialization_Device_Information();
void       DIXOM_PCB(uint8_t ControlByte,  uint8_t* data);
void       DIXOM_VERSION();
void       EXHANGE_LOG(uint8_t ControlByte, uint8_t *Received_String);



#endif
