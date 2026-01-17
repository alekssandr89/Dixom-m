#include <Description.h>
#include <Description.h>
#include <DSP_InternalCalls.h>
#include <DSP_StringControl.h>
#include <Information.h>
#include "Bluetooth.h"
#include "GPIO.h"
#include "licens.h"
#include "Exchange.h"
#include "main_Action.h"
#include "main_Menu.h"
#include "Exchange.h"
#include "Functions.h"
#include <string.h>
#include "RTC.h"
#include "Test.h"
#include "Can_StringControl.h"
#include "ADC.h"
#include <FmRadio.h>
#include "Nextion_LCD.h"
#include "DisplayMain.h"
#include "Arduino.h"
#include <Structures.h>
#include <Memory_Spi.h>
#include <UpdateFirmware.h>

extern sDixom Dixom;

/*
 *                   ----===ControlByte===----
 *
 * Выполняет необходимое действие в функции выбранного пакета.     *
 * DATA_SET               = '=', //устанавливает новое значение
 * DATA_GET               = ' ', //возвращает значение
 * DATA_GET_ALL           = '*', //возвращает все значения
 * DATA_GET_MODIFIED      = '#', //возвращает изменённые значения
 * DATA_DEFAULT           = '~', //устанавливает стандартные параметры
 * DATA_DELETE            = '-', //удаляет свойства
 */

void main_Menu(uint8_t *Received_String){//MENU

	uint8_t  Offset = 2,  Bias = 3, Package = 0, Function = 0, ControlByte = 0;
	Package = strtoul((char *)Received_String, NULL, 10);
	if(Package > 9) Offset++;
	Function = strtoul((char *)Received_String + Offset, NULL, 10);
	if(Package>9)Bias++;
	if(Package>99)Bias++;
	if(Function>9)Bias++;
	if(Function>99)Bias++;
	ControlByte = Received_String[Bias];
	Bias++;

	      if(Package == PACKED_SYSTEM)            /*0*/  {  SYSTEM(ControlByte, Function, Received_String+Bias);                      } // 0
	 else if(Package == PACKED_GENERAL)           /*1*/  {  GENERAL (ControlByte, Function, Received_String+Bias);                    } // 1
	 else if(Package == PACKED_GATEWAY)           /*2*/  {  GATEWAY(ControlByte, Function, Received_String+Bias);                 	          } // 2
	 else if(Package == PACKED_CAN)               /*3*/  {  CAN_STRING_CONTROL (ControlByte, Function, Received_String+Bias);         } // 3
	 else if(Package == PACKED_SOUND)             /*4*/  {  SOUND_STRING_CONTROL (ControlByte, Function, Received_String+Bias);       } // 4
	 else if(Package == PACKED_BLUETOOTH)         /*5*/  {  BLUETOOTH_Menu(ControlByte, Function, Received_String+Bias);              } // 5
	 else if(Package == PACKED_EXTERNAL_CONTROL)  /*6*/  {  SET_EXTERNAL_CONTROL(ControlByte, Function, Received_String+Bias);        } // 6
	 else if(Package == PACKED_FM_RADIO)          /*7*/  {  RADIO_SET_STRING_CONTROL (ControlByte, Function, Received_String+Bias);   } // 7
	 else if(Package == PACKED_EXPERIMENT)        /*20*/ {  TEST(ControlByte, Function, Received_String+Bias);                        } // 20
     else{
 	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoSuchPackage, AutoLenght,  false,  true, allAvailable);
	 }
}

void GENERAL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String){//BASIS 5 1 -

	     if(Function == GENERAL_SETTINGS)               /*2*/   {  SETTINGS (ControlByte, Received_String);                                  } //1 2
	else if(Function == GENERAL_SHOW_MODIFIED)          /*4*/   {  SHOW_ALL_MODIFIED_PARAMETERS();                                           } //1 4
	else if(Function == GENERAL_DEFAULT)                /*6*/   {  SET_DEFAULT_STRUCTURES(ControlByte, Received_String);                     } //1 5
	else if(Function == GENERAL_TIMER)                  /*10*/  {  TIMER(ControlByte, Received_String);                                      } //1 6
	else if(Function == GENERAL_DATA_TIME)              /*11*/  {  RTC_TIME(ControlByte, Received_String);                                   } //1 7
	else if(Function == GENERAL_TEMPERATURE)            /*20*/  {  RTC_TEMPERATURE();                                                        } //1 7
	else if(Function == GENERAL_BIT_CHIP_CONTROL)       /*9*/   {  BIT_CHIP_CONTROL(ControlByte, Received_String);                           } //1 9
	else if(Function == GENERAL_LOG)                    /*8*/   {  EXHANGE_LOG(ControlByte, Received_String);                                } //1 10
	else if(Function == GENERAL_SAVE)                   /*5*/   {  Structures_Write_To_Flash();     Structures_Write_To_EEPROM();            } //1 11
	else if(Function == GENERAL_I2C1_SCAN)              /*12*/  {  Scanning_I2C1();                                                   	     } //1 13
	else if(Function == GENERAL_I2C2_SCAN)              /*13*/  {  Scanning_I2C2();                                                          } //1 14
	else if(Function == GENERAL_IWDG)                   /*14*/  {  CMD_IWDG_SET(ControlByte, Received_String);                               } //1 15
	else if(Function == GENERAL_SPI)                    /*15*/  {  SetStateSPI(ControlByte, Received_String);                                } //1 16
	else if(Function == GENERAL_DAC_RESTART)            /*7*/   {  DAC_Restart();                                                            } //1 17
	else if(Function == GENERAL_ACTION)                 /*16*/  {  ActionSend(ControlByte, Received_String);                                 } //1 18
	else if(Function == GENERAL_USB_PWR_CONTROL)        /*17*/  {  SET_STATE_USB(ControlByte, Received_String);                              } //1 19
	else if(Function == GENERAL_RECIVE_DATA_LOSS_TEST)  /*19*/  {  RECIVE_DATA_LOSS_TEST(Received_String);                                    } //1 19
	else{
	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoPackageFunction, AutoLenght,  false,  true, allAvailable);
	}
}


void SHOW_ALL_MODIFIED_PARAMETERS(){
	CAN_SHOW_PARAMETERS(Modified);
	TIMER_SHOW_PARAMETERS(Modified);
	SETTINGS_SHOW_PARAMETERS(Modified);
	SHOW_CONTROL_PARAMETERS(Modified);
	DSP_SHOW_PARAMETERS(Modified);
	FM_RADIO_SHOW_PARAMETERS(Modified);
 	TransmitDataOutputs((uint8_t *)Basis_Notification_1_1_5_0, AutoLenght,  false,  true, allAvailable);

}




void RECIVE_DATA_LOSS_TEST (uint8_t *InputData){
	uint32_t   count = 0;
	uint8_t   response = 0;

	sscanf((char *)InputData, "%lu %hhu", &count, &response);
	setTestCount(count, response);
}





