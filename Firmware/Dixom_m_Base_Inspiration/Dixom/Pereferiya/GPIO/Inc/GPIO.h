#ifndef __GPIO_H__
#define __GPIO_H__

#include "stdint.h"
#include "GPIO_Constants.h"

void             Launch_Initialization_Gpio();
void             Configured_Initialization_Gpio();
void             Power_DCDC_33V_5V(uint8_t state);
void             Up_Modules_Reset(uint8_t state);
void             Power_LED(uint8_t state);
void             Power_USB1(uint8_t state);
void             Power_USB2(uint8_t state);
void             Power_VBUS(uint8_t state);
void             Switch_Trigger1(uint8_t state);
void             Switch_Trigger2(uint8_t state);
void             Switch_Trigger3(uint8_t state);
void             Switch_Dac_Reset(uint8_t state);
void             Switch_USB_Otg(uint8_t state);
void             Power_CINV_12v(uint8_t state);
void             Power_Remout(uint8_t state);
void             Power_Camera(uint8_t state);
void             Switch_CS_DSP(uint8_t state);
void             Switch_CS_FLASH(uint8_t state);

void             SetTrigger(uint8_t triggerNum, uint8_t pressed);

void             Trigger_State_Show(int8_t triggerNum, uint8_t state);


void             SET_STATE_USB(uint8_t ControlByte, uint8_t *Recive_USB);
void             GET_STATE_USB(uint8_t *Recive_USB);



void             RearPinState();
void             OnOffRearCam();
void             ActionRearState();
void             RearCamVolDwon();
void             ReadCamVolUp();
void             GPIO_Loop();



#endif
