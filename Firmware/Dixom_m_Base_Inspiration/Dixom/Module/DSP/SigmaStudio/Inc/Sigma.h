
#ifndef __SIGMATRANSFER_H__
#define __SIGMATRANSFER_H__

//#include "Exchange.h"
typedef const unsigned char       ADI_REG_TYPE;
/*
void             Transmit_Sigma( uint16_t devAddress, uint16_t podAddress, uint16_t dataLen, ADI_REG_TYPE *data);
void             Transmit_Sigma1701( uint16_t devAddress, uint16_t podAddress, uint16_t dataLen, ADI_REG_TYPE *data);



void             Transmit_Sigma1701( uint16_t devAddress, uint16_t podAddress, uint16_t dataLen, ADI_REG_TYPE *data);*/


//void             SIGMA_WRITE_REGISTER_BLOCK( uint8_t devAddress, int podAddress, int Length_Sigma_Data, ADI_REG_TYPE *SigmaData);
//void             InitSoundDevice(void);

void             Initialization_DSP_Audio_Processor(void);

void             InitDspSerialInput(void);
void             Init_DSP_ADAU1452(void);
void             DSP_Clock();
void             Init_DSP_ADAU1701(void);
void             LoopLevelDetector();





#endif
