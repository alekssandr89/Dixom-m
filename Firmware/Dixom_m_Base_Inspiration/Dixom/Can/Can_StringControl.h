/*
 * Can_StringControl.h
 *
 *  Created on: 8 θών. 2022 γ.
 *      Author: 79825
 */

#ifndef CAN_CAN_STRINGCONTROL_H_
#define CAN_CAN_STRINGCONTROL_H_

void CAN_STRING_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String);
void CAN_SHOW_PARAMETERS(uint8_t ShowType);

void CAN_SendMessage(uint8_t ControlByte, uint8_t *Received_String);
void CAN_Settings(uint8_t ControlByte, uint8_t *Received_String);
void CAN_Settings_Speed();
void CAN_Speed(uint8_t ControlByte, uint8_t *Received_String);
void CAN_Status(uint8_t ControlByte, uint8_t *Received_String);
void GetCanStatus();
void CAN_Car(uint8_t ControlByte, uint8_t *Received_String);
void CAN_Filter(uint8_t ControlByte, uint8_t *Received_String);

void CAN_Flash_NeedToSave();

#endif /* CAN_CAN_STRINGCONTROL_H_ */
