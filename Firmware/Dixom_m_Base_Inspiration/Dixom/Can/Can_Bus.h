/*
 * Can.h
 *
 *  Created on: Jun 3, 2022
 *      Author: 79825
 */

#ifndef CAN_CAN_BUS_H_
#define CAN_CAN_BUS_H_

#include "stdint.h"
//#include "Constants.h"



uint8_t CAN_Initialization_Final();
uint8_t CAN_Struct_Flash_Read();
uint8_t CAN_Struct_Default();


void CAN_Show_Settings(uint8_t ShowType);
void Can_Settings_Default();
uint16_t CanStructSize();
void CanNotUsed(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
void Receiver_CAN1(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);



typedef struct
{
	uint8_t  Manufacturer;
	uint8_t  Model;
	uint8_t  Generation;
	uint8_t  Year;
	uint8_t  Transmission;
	uint8_t  DriveUnit;
	uint8_t  HandlebarSide;
	void (*Button) (uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
	void (*Climate) (uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
	void (*Dashboard) (uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
} sCar;

typedef struct
{
	uint8_t WorkingMode;
	uint8_t FanSpeed;
	uint8_t DualZone;
	uint8_t DriverTemperature;
	uint8_t PassengerTemperature;
	uint8_t AirDirection;
} sClimate;

typedef struct
{
	uint8_t Speed;
	uint8_t Tachometer;
	uint8_t FuelLevel;
	uint8_t EngineTemperature;
	uint8_t DoorSatate[10];
	uint8_t TurnSignals;
} sDashboard;

typedef enum
{
	CanMsgCount            =0,
	CanSpeed               =1,
	CanTxPermision         =2,
	CanAutoConnect         =3,
	CanDataOutPermision    =4,
	Reserved0              =5,
	Reserved1              =6,
	Reserved2              =7,
	Reserved3              =8,
	Reserved4              =9,
}TDE_CanSettings;

typedef struct{
	uint8_t param;
} sCanSettings;

typedef union
{
    struct
    {
    	uint8_t         FrameIdType :1;
    	uint8_t         Dlc         :4;
    	uint8_t         MsgCount    :3;
    };
    uint8_t Byte; //structure in one byte
} sCanFrameInfo;


typedef struct
{
	uint32_t  Id;
	uint8_t   IdType   :1;
	uint8_t   Dlc      :3;
	uint8_t   Reserved :4;
	uint8_t   Data[8];
} sCanFrame;


#define FILTERSCALE_16BIT  0
#define FILTERSCALE_32BIT  1
#define ID_TYPE_STANDARD   0
#define ID_TYPE_EXTENDED   1
#define FILTER_MODE_MASK   0
#define FILTER_MODE_LISTID 1




typedef struct
{
	uint32_t   IdHigh;
	uint32_t   IdLow;
	uint32_t   MaskIdHigh;
	uint32_t   MaskIdLow;
	uint8_t    IdType;
	uint8_t    Scale;
	uint8_t    Mode; //Mask listID
	uint8_t    Activation;
} sCanFilter;


typedef struct
{
	sCanFrameInfo   FrameInfo;
	uint8_t         Buffer[40][60];
	uint8_t         Len;
} sCanMsgOut;




#define CAN_ACTION_QUEUE_COUNT 50

typedef struct
{
	uint16_t   ButtonID;
	uint8_t    Pressed;
} sCanButton;


typedef struct
{
	sCanButton Button[CAN_ACTION_QUEUE_COUNT];
	uint8_t    TxPointer;
	uint8_t    RxPointer;
} sCanActionQueue;

#define CAN_FILTER_MAX_ID   536870911

#define NUM_ELEM_SETTINGS  24
#define NUM_ELEM_FILER     14
#define NUM_ELEM_ACTION    40

typedef struct
{
	uint32_t        CheckSum;
	sCanSettings    Settings[NUM_ELEM_SETTINGS];
	sCanFilter      Filter[NUM_ELEM_FILER];
	sCar            Car;
	sCanActionQueue ActionQueue;
	sClimate        Climate;
	sDashboard      Dashboard;
	sCanMsgOut      MsgOut;
	uint8_t         Status;
} sCan;

#endif /* CAN_CAN_BUS_H_ */
